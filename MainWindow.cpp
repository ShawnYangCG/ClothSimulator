//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Main GUI of the software
//
//  December, 2021
//
//  ------------------------
//  MainWindow.cpp
//  ------------------------
//  
//  The main GUI of the software with widgets and buttons .etc
//  
//////////////////////////////////////////////////////////////////////
#include "MainWindow.h"
#include <QFile>
#include <sstream>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    clearOpenGLWidget();
    init();
    showGround();
}

void MainWindow::clearOpenGLWidget(){
    // set openGL widget empty
    renderWidget = new RenderWidget();
    ui.openGLLayout->addWidget(renderWidget);
    lightRotator = new ArcBallWidget(NULL);
    modelRotator = new ArcBallWidget(NULL);
}

void MainWindow::showGround() {
    QTextCodec* code = QTextCodec::codecForName("GB2312");
    // add ball object
    QString ballfileName = ".\\testObj\\environment\\ball.obj";
    if (ballfileName == NULL) {
        return;
    }
    // transform QString to char*
    std::string name = code->fromUnicode(ballfileName).data();
    const char* c2 = name.data();
    std::ifstream ballFile(c2);

    ballObject = new TexturedObject();
    if (!ballObject->ReadObjectStream(ballFile))
    { // object read failed 
        std::cout << "Read failed for object " << c2 << std::endl;
        return;
    } // object read failed

    // add ground object
    QString groundfileName = ".\\testObj\\environment\\ground.obj";
    if (groundfileName == NULL) {
        return;
    }
    // transform QString to char*
    name = code->fromUnicode(groundfileName).data();
    c2 = name.data();
    std::ifstream groundFile(c2);

    groundObject = new TexturedObject();
    if (!groundObject->ReadObjectStream(groundFile))
    { // object read failed 
        std::cout << "Read failed for object " << c2 << std::endl;
        return;
    } // object read failed
    groundObject->IsQUADS = true;
    renderWidget->renderParameters = &this->renderParameters;
    this->InitParameters();
    renderWidget->this_ui = this;
    renderController = new RenderController(NULL, &renderParameters, this);
}

void MainWindow::init() {
    // initialize all UI
    ui.frameSlider->setMaximum(100);
    ui.frameSlider->setMinimum(1);
    ui.verticalSlider_value->setMaximum(20);
    ui.verticalSlider_value->setMinimum(0);
    ui.verticalSlider_speed->setMaximum(720);
    ui.verticalSlider_speed->setMinimum(0);
    ui.frameSlider->setValue(1);
    ui.frameNumTextEdit->setText("1");
    ui.textBrowser_value->setText("0");
    ui.textBrowser_speed->setText("0");
    
    ui.checkBox_gravity->setChecked(false);
    ui.checkBox_ground->setChecked(true);
    ui.checkBox_ball->setChecked(false);
    ui.checkBox_wind->setChecked(false);
    ui.checkBox_rotation->setChecked(false);

    ui.checkBox_gravity->setEnabled(false);
    ui.checkBox_ground->setEnabled(false);
    ui.checkBox_ball->setEnabled(false);
    ui.checkBox_wind->setEnabled(false);
    ui.checkBox_rotation->setEnabled(false);
    ui.checkBox_lock->setEnabled(false);
    ui.checkBox_record->setEnabled(false);

    ui.verticalSlider_value->setEnabled(false);
    ui.textBrowser_value->setEnabled(false);
    ui.verticalSlider_speed->setEnabled(false);
    ui.textBrowser_speed->setEnabled(false);
    ui.checkBox_rotation->setEnabled(false);
    
    // connect slots and signals
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(Menu_Open_clicked()), Qt::UniqueConnection);
    connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(Menu_Save_clicked()), Qt::UniqueConnection);
    connect(ui.actionExport, SIGNAL(triggered()), this, SLOT(Menu_Export_clicked()), Qt::UniqueConnection);
    connect(ui.playButton, SIGNAL(clicked()), this, SLOT(playButtonClicked()), Qt::UniqueConnection);
    connect(ui.frameSlider, SIGNAL(valueChanged(int)), this, SLOT(timeSliderChange()), Qt::UniqueConnection);
    connect(ui.checkBox_gravity, SIGNAL(clicked()), this, SLOT(checkBox_gravityChecked()), Qt::UniqueConnection);
    connect(ui.checkBox_ground, SIGNAL(clicked()), this, SLOT(checkBox_groundChecked()), Qt::UniqueConnection);
    connect(ui.checkBox_ball, SIGNAL(clicked()), this, SLOT(checkBox_ballChecked()), Qt::UniqueConnection);
    connect(ui.checkBox_wind, SIGNAL(clicked()), this, SLOT(checkBox_windChecked()), Qt::UniqueConnection);
    connect(ui.checkBox_rotation, SIGNAL(clicked()), this, SLOT(checkBox_rotationChecked()), Qt::UniqueConnection);
    connect(ui.verticalSlider_speed, SIGNAL(valueChanged(int)), this, SLOT(verticalSlider_speedValueChanged()), Qt::UniqueConnection);
    connect(ui.verticalSlider_value, SIGNAL(valueChanged(int)), this, SLOT(verticalSlider_valueValueChanged()), Qt::UniqueConnection);
    connect(ui.checkBox_lock, SIGNAL(clicked()), this, SLOT(checkBox_lockChecked()), Qt::UniqueConnection);
    connect(ui.checkBox_record, SIGNAL(clicked()), this, SLOT(checkBox_recordChecked()), Qt::UniqueConnection);

    ResetInterface();
}

void MainWindow::ResetInterface() {
    renderWidget->update();
}

void MainWindow::InitParameters() {
    renderWidget->renderParameters->useLighting = true;
    renderWidget->renderParameters->texturedRendering = false;
    renderWidget->renderParameters->textureModulation = false;
    renderWidget->renderParameters->depthTestOn = true;
    renderWidget->renderParameters->showAxes = true;
    renderWidget->renderParameters->showObject = true;
    renderWidget->renderParameters->centreObject = false;
    renderWidget->renderParameters->scaleObject = false;
    renderWidget->renderParameters->mapUVWToRGB = true;
}

void MainWindow::Menu_Open_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open cloth", ".\\testObj", "OBJ (*.obj)");
    if (fileName.isEmpty()) return;
    QString error;
    if (!loadCloth(fileName, &error))
        QMessageBox::warning(this, "Cannot load cloth", error);
}

bool MainWindow::loadCloth(const QString& fileName, QString* error) {
    auto fail = [&](const QString& message) { if (error) *error = message; return false; };
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return fail("Cannot open file: " + fileName + "\n" + file.errorString());
    const QByteArray data = file.readAll();
    if (file.error() != QFileDevice::NoError) return fail(file.errorString());
    std::istringstream geometryFile(std::string(data.constData(), data.size()));
    std::unique_ptr<TexturedObject> candidate(new TexturedObject());
    if (!candidate->ReadObjectStream(geometryFile))
        return fail("Invalid OBJ: expected vertices, texture coordinates, normals and v/vt/vn faces.");
    const size_t count = candidate->vertices.size();
    const size_t row = static_cast<size_t>(sqrt(static_cast<double>(count)));
    if (row < 2 || row * row != count)
        return fail("Cloth requires a square grid. Please use testObj/plane10x10.obj.");

    renderWidget->IsPlaying = false;
    ui.openGLLayout->removeWidget(renderWidget);
    renderWidget->hide();
    delete renderController;
    renderWidget->deleteLater();
    delete simulationEnvironment;
    delete texturedObject;
    texturedObject = candidate.release();
    gravityON = windON = ballON = rotationON = islocked = isrecord = false;
    groundON = true;
    windValue = rotationDegree = 0;
    renderWidget = new RenderWidget(texturedObject, &renderParameters, this);
    ui.openGLLayout->addWidget(renderWidget);
    InitParameters();
    renderController = new RenderController(texturedObject, &renderParameters, this);
    simulationEnvironment = new SimulationEnvironment(this);
    init();
    ui.playButton->setText("play");
    ui.frameSlider->setEnabled(true);
    ui.nextFrameButton->setEnabled(true);
    ui.preFrameButton->setEnabled(true);
    ui.checkBox_lock->setChecked(false);
    ui.checkBox_record->setChecked(false);
    ui.checkBox_gravity->setEnabled(true);
    ui.checkBox_ground->setEnabled(true);
    ui.checkBox_ball->setEnabled(true);
    ui.checkBox_wind->setEnabled(true);
    ui.checkBox_lock->setEnabled(true);
    ui.checkBox_record->setEnabled(true);
    ResetInterface();
    return true;
}

void MainWindow::Menu_Save_clicked() {
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, QString(QStringLiteral("File")),
        ".\\testObj", QString(QStringLiteral("OBJ(*.obj)")));//obj file only
    if (fileName == NULL) {
        return;
    }

    // transform QString to char*
    QTextCodec* code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(fileName).data();
    const char* c2 = name.data();
    std::ofstream geometryFile(c2);
    texturedObject->WriteObjectStream(geometryFile);
}

void MainWindow::Menu_Export_clicked() {
    std::cout << this->renderWidget->graphs.size() << std::endl;
}

void MainWindow::playButtonClicked() {
    if (!simulationEnvironment) return;
    if (ui.playButton->text() == "play") {
        ui.nextFrameButton->setDisabled(true);
        ui.preFrameButton->setDisabled(true);
        ui.frameSlider->setDisabled(true);
        ui.playButton->setText("pause");
        renderWidget->setStartFrame(renderWidget->getCurrentFrame());
        renderWidget->timerStart();
        renderWidget->IsPlaying = true;
        renderWidget->update();
    }
    else {// enable all buttons
        ui.nextFrameButton->setEnabled(true);
        ui.preFrameButton->setEnabled(true);
        ui.frameSlider->setEnabled(true);
        ui.playButton->setText("play");
        renderWidget->timerStop();
        renderWidget->IsPlaying = false;
        renderWidget->update();
    }
}

void MainWindow::setTimeSlider(int value) {
    if (value <= ui.frameSlider->maximum() && value >= 1) {
        ui.frameSlider->setValue(value);
        timeSliderChange();
    }
}

int MainWindow::getTimeSliderMax() {
    return this->ui.frameSlider->maximum();
}

void MainWindow::setTimeSliderMax(int value) {
    this->ui.frameSlider->setMaximum(value);
}

void MainWindow::timeSliderChange() {
    ui.frameNumTextEdit->setText(QString::number(ui.frameSlider->value()));
    renderWidget->changeFrame(ui.frameSlider->value());
}

void MainWindow::checkBox_gravityChecked() {
    if(this->gravityON == false)
        this->gravityON = true;
    else
        this->gravityON = false;
    this->renderWidget->update();
}

void MainWindow::checkBox_groundChecked() {
    if (this->groundON == false)
        this->groundON = true;
    else
        this->groundON = false; 
    this->renderWidget->update();
}

void MainWindow::checkBox_ballChecked() {
    if (this->ballON == false) {
        this->ballON = true;
        ui.checkBox_rotation->setEnabled(true);
    } 
    else {
        this->ballON = false;
        ui.checkBox_rotation->setEnabled(false);
    }
    ui.checkBox_rotation->setChecked(false);
    ui.textBrowser_speed->setEnabled(false);
    ui.verticalSlider_speed->setEnabled(false);
    this->rotationON = false;
    this->renderWidget->update();
}

void MainWindow::checkBox_windChecked() {
    if (this->windON == false) {
        this->windON = true;
        ui.verticalSlider_value->setEnabled(true);
        ui.textBrowser_value->setEnabled(true);
    }
    else {
        this->windON = false;
        ui.verticalSlider_value->setEnabled(false);
        ui.textBrowser_value->setEnabled(false);
    }
    this->renderWidget->update();
}

void MainWindow::checkBox_rotationChecked() {
    if (this->rotationON == false) {
        this->rotationON = true;
        ui.verticalSlider_speed->setEnabled(true);
        ui.textBrowser_speed->setEnabled(true);
    }
    else {
        this->rotationON = false;
        ui.verticalSlider_speed->setEnabled(false);
        ui.textBrowser_speed->setEnabled(false);
    }
    this->renderWidget->update();
}

void MainWindow::verticalSlider_speedValueChanged() {
    ui.textBrowser_speed->setText(QString::number(ui.verticalSlider_speed->value()));
    this->rotationDegree = ui.verticalSlider_speed->value();
    this->simulationEnvironment->simulationObjects.back().rotationSpeed = ui.verticalSlider_speed->value();
    this->renderWidget->update();
}

void MainWindow::verticalSlider_valueValueChanged() {
    ui.textBrowser_value->setText(QString::number(ui.verticalSlider_value->value()));
    this->windValue = ui.verticalSlider_value->value();
    this->renderWidget->update();
}

void MainWindow::checkBox_lockChecked() {
    if (this->islocked == false)
        this->islocked = true;
    else
        this->islocked = false;
    this->renderWidget->update();
}

void MainWindow::checkBox_recordChecked() {
    if (this->isrecord == false)
        this->isrecord = true;
    else
        this->isrecord = false;
    this->renderWidget->update();
}