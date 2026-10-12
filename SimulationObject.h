//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Simulation object module of cloth simulation
//
//  December, 2021
//
//  ------------------------
//  SimulationObject.h
//  ------------------------
//  
//  Put SimulationObject into SimulationEnvironment and then doing 
//  simulation.
//  
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _SIMULATION_OBJECT_H
#define _SIMULATION_OBJECT_H
#include "stdafx.h"
#include "renderWindow/Cartesian3.h"
#include "FieldOfForce.h"
#include "SimulationEnvironment.h"
#include "renderWindow/TexturedObject.h"
class springPair;
class FieldOfForce;
class SimulationEnvironment;
class TexturedObject;
class SimulationVertice {// SimulationVertice
public:
	Cartesian3 coords;
	float mass = 0.05;	// 10x10: 0.1; 20x20: 0.05
	float velocity;
	Cartesian3 velocity_direction;
	bool simulationEnable;
	std::vector<springPair> springs;
	FieldOfForce readyToCompute;
	FieldOfForce EnvironmentField;
	float minimalCollisionDistence = 1;
	float energylose = 2 / 100.0;

public:
	Cartesian3 getSpringForce(springPair);
	FieldOfForce getSpringField();
	void getSimulatingSolution(float timeStep);
	float getEnergyEk();
	bool IsInface(std::vector<Cartesian3> faceVertices, Cartesian3 faceNormal);
	void setCollisionVelocity(Cartesian3 faceNormal);
};// SimulationVertice

class springPair {// springPair
public:
	SimulationVertice *targetVertice;
	float coefficientK = 100.0;	// 20x20: 200; 10x10: 100
	float damperK = 6;
	float initLength;
public:
	springPair(SimulationVertice *source, SimulationVertice *target);
	springPair(SimulationVertice *source, SimulationVertice *target, float coefficientK);
};// springPair

class SimulationObject { //SimulationObject
public:
	// vector of vertices
	SimulationEnvironment* Environment;
	std::vector<SimulationVertice> vertices;
	TexturedObject* textureObject;
	float EnvironmentTimeStep;	// deltaT = EnvironmentTimeStep / timeStep
	int timeStep = 20; // more large value means smaller timestep (larger than 0)
	float energylose = 2 / 100.0;	// 20x20: 15; 10x10: 10;
	float rotationSpeed = 0;	// degree/s
	float frictionCoefficient = 0;	// between 0 and 1
	
public:
	SimulationObject(SimulationEnvironment*);
	SimulationObject(TexturedObject* ,SimulationEnvironment*);
	void computeWithField(FieldOfForce field);
	void writeValueBack();
	Cartesian3 getRotationVelocity(Cartesian3 coods);
	void setRotationVelocity(SimulationVertice *vertice, Cartesian3 velocity);
	void lockCorner();
	void unlockCorner();
};//SimulationObject
#endif