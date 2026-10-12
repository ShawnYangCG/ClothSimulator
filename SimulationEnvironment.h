//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Simulation environment module of cloth simulation
//
//  December, 2021
//
//  ------------------------
//  SimulationEnvironment.h
//  ------------------------
//  
//  Used to setup an environment for simulating
//  
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _SIMULATION_ENVIRONMENT_H
#define _SIMULATION_ENVIRONMENT_H
#include "stdafx.h"
#include "FieldOfForce.h"
#include "SimulationObject.h"
#include "MainWindow.h"
#include "renderWindow/TexturedObject.h"
class FieldOfForce;
class SimulationObject;
class MainWindow;
class TexturedObject;
class SimulationEnvironment {// SimulationEnvironment
public:
	// store all simulation fields
	std::vector<FieldOfForce> simulationFields;
	// store all simulation objects
	std::vector<SimulationObject> simulationObjects;
	// store all collision objects
	std::vector<TexturedObject> collisionObjects;

	// mainWindow pointer
	MainWindow* this_ui;

	float timeStep = 0.01;// 1/0.04 = 25 frame/s

public:
	SimulationEnvironment(MainWindow*);
	void StartSimulation();
	void collisionProcess();
};// SimulationEnvironment

#endif