#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include <QCheckBox>
#include <cmath>
#include <iostream>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    if (argc != 2 || !QDir::setCurrent(QString::fromLocal8Bit(argv[1]))) return 2;
    MainWindow window;
    QString error;
    if (!window.loadCloth("testObj/plane10x10.obj", &error)) return 3;
    auto ball = window.findChild<QCheckBox*>("checkBox_ball");
    auto rotation = window.findChild<QCheckBox*>("checkBox_rotation");
    ball->click();

    // Isolate rotation from integration, gravity, and spring forces.
    // Each case starts with a stationary point inside the sphere.
    auto contactSpeed = [&]() {
        SimulationObject object(window.simulationEnvironment);
        object.timeStep = 1;
        object.rotationSpeed = 180;
        object.vertices[0].coords = Cartesian3(0.1f, 0.6f, 0);
        object.vertices[0].simulationEnable = false;
        object.computeWithField(FieldOfForce());
        return object.vertices[0].velocity;
    };
    if (contactSpeed() != 0) return 4;
    rotation->click();
    if (!(contactSpeed() > 0.1f)) return 5;
    rotation->click();
    if (contactSpeed() != 0) return 6;
    rotation->click();
    ball->click();
    if (window.rotationON || contactSpeed() != 0) return 7;
    ball->click();
    if (window.rotationON || contactSpeed() != 0) return 8;
    std::cout << "PASS: rotation off/on/off and sphere off/on respect UI state\n";
    return 0;
}
