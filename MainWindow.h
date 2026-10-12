//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Main GUI of the software
//
//  December, 2021
//
//  ------------------------
//  MainWindow.h
//  ------------------------
//  
//  The main GUI of the software with widgets and buttons .etc
//  
//////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"
#include "ui_ClothSimulator_version_1.h"
#include "renderWindow/RenderWidget.h"
#include "renderWindow/ArcBallWidget.h"
#include "renderWindow/RenderController.h"
#include "SimulationEnvironment.h"
#include "renderWindow/TexturedObject.h"
class RenderWidget;
class RenderController;
class SimulationEnvironment;
class TexturedObject;
class MainWindow : public QMainWindow
{// MainWindow
    Q_OBJECT

public:
    // pointers of  widget and button
    ArcBallWidget* modelRotator;
    ArcBallWidget* lightRotator;
    RenderWidget* renderWidget;
    TexturedObject *texturedObject = nullptr;
    TexturedObject* ballObject;
    TexturedObject* groundObject;
    RenderParameters renderParameters;
    RenderController *renderController;
    SimulationEnvironment* simulationEnvironment = nullptr;
    
    // whether a button was clicked/checked or not 
    bool gravityON = false;
    bool groundON = true;
    bool ballON = false;
    bool windON = false;
    bool rotationON = false;
    bool islocked = false;
    bool isrecord = false;
    float rotationDegree = 1;
    float windValue = 0;

    // functions
public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    bool loadCloth(const QString& fileName, QString* error = nullptr);
    void init();
    void clearOpenGLWidget();
    void showGround();
    void ResetInterface();
    void InitParameters();
    void setTimeSlider(int value);   
    int getTimeSliderMax();
    void setTimeSliderMax(int value);

private:
    Ui::ClothSimulator_version_1Class ui;

    // slots
private slots:
    void Menu_Open_clicked();
    void Menu_Save_clicked();
    void Menu_Export_clicked();
    void playButtonClicked();
    void timeSliderChange();
    void checkBox_gravityChecked();
    void checkBox_groundChecked();
    void checkBox_ballChecked();
    void checkBox_windChecked();
    void checkBox_rotationChecked();
    void verticalSlider_speedValueChanged();
    void verticalSlider_valueValueChanged();
    void checkBox_lockChecked();
    void checkBox_recordChecked();
};// MainWindow
