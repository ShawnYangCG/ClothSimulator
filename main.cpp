#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QOpenGLContext>
#include <QCheckBox>
#include <QElapsedTimer>
#include <sstream>
#include <cmath>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QDir().mkpath("output");
    MainWindow w;
    w.show();
    if (a.arguments().contains("--import-test")) {
        QFile report("import-check.txt");
        if (!report.open(QIODevice::WriteOnly | QIODevice::Text)) return 3;
        QTextStream out(&report);
        QString path = a.arguments().last();
        QByteArray oldPath = QTextCodec::codecForName("GB2312")->fromUnicode(path);
        std::ifstream oldFile(oldPath.constData());
        out << "LegacyPathOpen=" << oldFile.is_open() << "\n";
        QElapsedTimer elapsed; elapsed.start();
        QString error;
        if (!w.loadCloth(path, &error)) { out << "FAIL: " << error << "\n"; return 4; }
        out << "ImportMilliseconds=" << elapsed.elapsed() << "\nVertices=" << w.texturedObject->vertices.size() << "\n";
        auto previous = w.texturedObject;
        bool missingRejected = !w.loadCloth(path + ".missing", &error) && w.texturedObject == previous;
        std::istringstream invalid("v bad 0 0\n");
        TexturedObject bad;
        bool malformedRejected = !bad.ReadObjectStream(invalid);
        std::istringstream badFace("v 0 0 0\nvt 0 0 0\nvn 0 1 0\nf 1//1 1//1 1//1\n");
        TexturedObject bad2;
        malformedRejected = malformedRejected && !bad2.ReadObjectStream(badFace);
        out << "MissingFileRejected=" << missingRejected << "\nMalformedOBJRejected=" << malformedRejected << "\n";
        report.close();
        if (!missingRejected || !malformedRejected) return 5;
        w.findChild<QCheckBox*>("checkBox_gravity")->click();
        w.findChild<QCheckBox*>("checkBox_ball")->click();
        QTimer* renderPump = new QTimer(&w);
        QObject::connect(renderPump, &QTimer::timeout, &w, [&]() { if (w.renderWidget->IsPlaying) w.renderWidget->grabFramebuffer(); });
        renderPump->start(20);
        QTimer::singleShot(200, [&]() { QMetaObject::invokeMethod(&w, "playButtonClicked", Qt::DirectConnection); });
        QTimer::singleShot(1800, [&]() {
            QMetaObject::invokeMethod(&w, "playButtonClicked", Qt::DirectConnection);
            bool finite = true; double meanY = 0;
            for (const auto& v : w.texturedObject->vertices) {
                finite = finite && std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
                meanY += v.y;
            }
            meanY /= w.texturedObject->vertices.size();
            const bool valid = w.renderWidget->isValid();
            const int frame = w.renderWidget->getCurrentFrame();
            w.grab().save("import-check.png");
            QFile result("import-check.txt"); result.open(QIODevice::Append | QIODevice::Text);
            QTextStream log(&result);
            log << "OpenGLValid=" << valid << "\nFrame=" << frame << "\nFiniteVertices=" << finite << "\nMeanClothHeight=" << meanY << "\n";
            a.exit(valid && frame > 1 && finite && meanY < 0.999 ? 0 : 6);
        });
    }
    if (a.arguments().contains("--smoke-test")) {
        QTimer::singleShot(2000, [&]() {
            bool ok = w.renderWidget && w.renderWidget->isValid();
            QImage frame = w.renderWidget->grabFramebuffer();
            ok = ok && !frame.isNull();
            w.grab().save("startup-check.png");
            QFile report("startup-check.txt");
            if (report.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&report);
                out << "Qt=" << qVersion() << "\nOpenGLWidgetValid=" << ok
                    << "\nFramebuffer=" << frame.width() << "x" << frame.height()
                    << "\nEnvironmentAssets=" << (QFile::exists("testObj/environment/ball.obj") && QFile::exists("testObj/environment/ground.obj")) << "\n";
            }
            a.exit(ok ? 0 : 2);
        });
    }
    return a.exec();
}
