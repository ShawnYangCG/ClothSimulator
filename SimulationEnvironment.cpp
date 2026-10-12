//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Simulation environment module of cloth simulation
//
//  December, 2021
//
//  ------------------------
//  SimulationEnvironment.cpp
//  ------------------------
//  
//  Used to setup an environment for simulating
//  
//////////////////////////////////////////////////////////////////////
#include "SimulationEnvironment.h"

SimulationEnvironment::SimulationEnvironment(MainWindow* uiPointer) {
	this->this_ui = uiPointer;
	// just for testing
	// create an object with one vertice
	SimulationObject objectA(uiPointer->texturedObject,this);
	
	int row = sqrt(objectA.vertices.size());

	this->simulationObjects.push_back(objectA);
	//this->collisionObjects.push_back(*this->this_ui->ballObject);

	// add spring (only for squire cloth)
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < row; j++) {
			if (j - 2 >= 0) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[i * row + j - 2]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (j - 1 >= 0) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[i * row + j - 1]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (j + 2 < row) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[i * row + j + 2]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (j + 1 < row) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[i * row + j + 1]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (i - 2 >= 0) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[(i - 2) * row + j]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (i - 1 >= 0) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[(i - 1) * row + j]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (i + 2 < row) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[(i + 2) * row + j]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (i + 1 < row) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[(i + 1) * row + j]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (i - 1 >= 0 && j - 1 >= 0) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[(i - 1) * row + (j - 1)]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (i + 1 < row  && j - 1 >= 0) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[(i + 1) * row + (j - 1)]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (i - 1 >= 0 && j + 1 < row) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[(i - 1) * row + (j + 1)]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
			if (i + 1 < row && j + 1 < row) {
				springPair spring(
					&this->simulationObjects.back().vertices[i * row + j],
					&this->simulationObjects.back().vertices[(i + 1) * row + (j + 1)]);
				this->simulationObjects.back().vertices[i * row + j].springs.push_back(spring);
			}
		}
	}
	FieldOfForce GRAVITY("GRAVITY");
	FieldOfForce WIND(0, Cartesian3(0, 0, 1));
	this->simulationFields.push_back(GRAVITY);
	this->simulationFields.push_back(WIND);
}

void SimulationEnvironment::StartSimulation() {
	// combine all fields into one field
	FieldOfForce combineField;
	if (this->this_ui->windON) {// if wind switch is ON
		this->simulationFields.back().IsEnable = true;
		this->simulationFields.back().value = this->this_ui->windValue;
	}
	else
		this->simulationFields.back().IsEnable = false;
	if (this->this_ui->gravityON)
		this->simulationFields.front().IsEnable = true;
	else
		this->simulationFields.front().IsEnable = false;
	for (int i = 0; i < this->simulationFields.size(); i++) {
		if(this->simulationFields[i].IsEnable)
			combineField = combineField + this->simulationFields[i];
	}

	// use this field to compute all simulation object with all vertices
	for (int i = 0; i < this->simulationObjects.size(); i++) {
		if (this->this_ui->islocked)
			this->simulationObjects[i].lockCorner();
		else
			this->simulationObjects[i].unlockCorner();
		this->simulationObjects[i].computeWithField(combineField);
	}

	// write the value back to the TexturedObject class
	for (int i = 0; i < this->simulationObjects.size(); i++) {
		this->simulationObjects[i].writeValueBack();
	}
}

void SimulationEnvironment::collisionProcess() {
	for (int i = 0; i < this->simulationObjects.size(); i++) { // for each simulation object
		for (int j = 0; j < this->simulationObjects[i].vertices.size(); j++) { // for each vertice in simulation object 
			if (this->simulationObjects[i].vertices[j].coords.length() <= 0.2) {
				this->simulationObjects[i].vertices[j].setCollisionVelocity(
					Cartesian3(this->simulationObjects[i].vertices[j].coords.unit()));
			}
		}
	}
}