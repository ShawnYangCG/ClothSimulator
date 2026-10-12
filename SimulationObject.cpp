//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Simulation object module of cloth simulation
//
//  December, 2021
//
//  ------------------------
//  SimulationObject.cpp
//  ------------------------
//  
//  Put SimulationObject into SimulationEnvironment and then doing 
//  simulation.
//  
//////////////////////////////////////////////////////////////////////
#include "SimulationObject.h"

SimulationObject::SimulationObject(SimulationEnvironment* simulationEnvironment) {
	this->Environment = simulationEnvironment;
	SimulationVertice vertice;
	vertice.coords.x = 0.0;
	vertice.coords.y = 0.0;
	vertice.coords.z = 0.0;
	vertice.simulationEnable = true;
	vertice.velocity = 0.0;
	vertice.velocity_direction.x = 0.0;
	vertice.velocity_direction.y = 0.0;
	vertice.velocity_direction.z = 0.0;
	this->vertices.push_back(vertice);
	this->EnvironmentTimeStep = simulationEnvironment->timeStep;
}

SimulationObject::SimulationObject(TexturedObject* texturedObject,SimulationEnvironment* simulationEnvironment) {
	this->Environment = simulationEnvironment;
	this->textureObject = texturedObject;
	for (int i = 0; i < texturedObject->vertices.size(); i++) {
		SimulationVertice vertice;
		vertice.coords.x = texturedObject->vertices[i].x;
		vertice.coords.y = texturedObject->vertices[i].y;
		vertice.coords.z = texturedObject->vertices[i].z;
		vertice.simulationEnable = true;
		vertice.velocity = 0.0;
		vertice.velocity_direction.x = 0.0;
		vertice.velocity_direction.y = 0.0;
		vertice.velocity_direction.z = 0.0;
		this->vertices.push_back(vertice);
	}
	this->EnvironmentTimeStep = simulationEnvironment->timeStep;
}

void SimulationObject::computeWithField(FieldOfForce field) {
	for (int j = 0; j < this->timeStep; j++) {
		for (int i = 0; i < this->vertices.size(); i++) {
			// combine the field with spring force
			this->vertices[i].EnvironmentField = field;
			FieldOfForce springField = this->vertices[i].getSpringField();
			this->vertices[i].readyToCompute = field + springField;
		}

		// collision detect
		if(this->Environment->this_ui->ballON || this->Environment->this_ui->groundON)
			for (int i = 0; i < this->vertices.size(); i++) {
				// for ball
				if (this->Environment->this_ui->ballON && 
					(this->vertices[i].coords - Cartesian3(0,0.5,0)).length() <= 0.207) {
					this->vertices[i].setCollisionVelocity((this->vertices[i].coords - Cartesian3(0, 0.5, 0)).unit());
					if (this->IsRotation = true) {
						this->setRotationVelocity(&this->vertices[i], 
							this->getRotationVelocity(this->vertices[i].coords - Cartesian3(0, 0.5, 0)));
					}
				}
				//for ground
				if (this->Environment->this_ui->groundON && this->vertices[i].coords.y <= 0) {
					this->vertices[i].setCollisionVelocity(Cartesian3(0, 1, 0));
				}
			}

		// use the final combined field to simulate all vertices
		for (int i = 0; i < this->vertices.size(); i++) {
			if (this->vertices[i].simulationEnable == true) {
				this->vertices[i].getSimulatingSolution(this->EnvironmentTimeStep/this->timeStep);
			}
		}
	}
	// set energy lose
	for (int i = 0; i < this->vertices.size(); i++) {
		if (this->vertices[i].simulationEnable == true) {
			this->vertices[i].velocity = this->vertices[i].velocity * (1 - this->energylose);
		}
	}
}

void SimulationObject::writeValueBack() {
	for (int i = 0; i < this->vertices.size(); i++) {
		this->textureObject->vertices[i]= this->vertices[i].coords;
	}
}

Cartesian3 SimulationVertice::getSpringForce(springPair spring) {
	Cartesian3 springDirection = spring.targetVertice->coords - this->coords;	//p2-p1
	float springLength = springDirection.length();
	Cartesian3 springForceDirection = springDirection.unit(); // p2-p1/|p2-p1|
	float deltaLength = springLength - spring.initLength;	// Lc-Lr
	Cartesian3 deltaVelocity = spring.targetVertice->velocity * spring.targetVertice->velocity_direction
		- this->velocity * this->velocity_direction;
	Cartesian3 springForce = (spring.coefficientK * deltaLength
		+ spring.damperK * deltaVelocity.dot(springForceDirection)) * springForceDirection;
	return springForce;
}

springPair::springPair(SimulationVertice *source, SimulationVertice *target) {
	this->targetVertice = target;
	this->initLength = (target->coords - source->coords).length();
}

springPair::springPair(SimulationVertice *source, SimulationVertice *target, float coefficientK) {
	this->targetVertice = target;
	this->initLength = (target->coords - source->coords).length();
	this->coefficientK = coefficientK;
}

float SimulationVertice::getEnergyEk() {
	// Ek = 0.5 * m * V^2
	return 0.5 * this->mass * this->velocity * this->velocity;
}

void SimulationVertice::getSimulatingSolution(float timeStep) {
	Cartesian3 Coods0 = this->coords;
	Cartesian3 V0 = this->velocity * this->velocity_direction;
	Cartesian3 acceleration = this->readyToCompute.value * this->readyToCompute.direction;
	Cartesian3 deltaV = acceleration * timeStep;
	Cartesian3 Vt = V0 + deltaV;
	this->velocity = Vt.length();
	this->velocity_direction = Vt.unit();
	Cartesian3 deltaCoods = V0 * timeStep + 0.5 * (acceleration * timeStep * timeStep);
	this->coords = Coods0 + deltaCoods;
}

FieldOfForce SimulationVertice::getSpringField() {
	FieldOfForce fieldWithSpring;
	for (int i = 0; i < this->springs.size(); i++) {
		Cartesian3 springForce = this->getSpringForce(this->springs[i]);
		Cartesian3 acceleration = springForce / this->mass;
		FieldOfForce springForceField(acceleration.length(), acceleration.unit());
		fieldWithSpring = fieldWithSpring + springForceField;
	}
	return fieldWithSpring;
}

bool SimulationVertice::IsInface(std::vector<Cartesian3> faceVertices, Cartesian3 faceNormal) {
	bool IsInface = false;
	for (int i = 0; i < faceVertices.size(); i++) {
		Cartesian3 directEdge1;
		Cartesian3 directEdge2;
		Cartesian3 directEdgeVertice(this->coords);
		Cartesian3 source(faceVertices[i]);
		if (i == faceVertices.size() - 1) {	
			Cartesian3 target(faceVertices[0]);
			directEdge1 = target - source;	
		}
		else if(i + 1 < faceVertices.size()){
			Cartesian3 target(faceVertices[i+1]);
			directEdge1 = target - source;
		}
		directEdge2 = directEdgeVertice - source;
		Cartesian3 crossProduct = directEdge1.cross(directEdge2);
		if ((faceNormal.unit() - crossProduct.unit()).length() < this->minimalCollisionDistence) {
			IsInface = true;
		}
		else {
			return false;
		}
	}
	return IsInface;
}

void SimulationVertice::setCollisionVelocity(Cartesian3 faceNormal) {
	if (this->velocity_direction.dot(faceNormal) < 0) {
		this->velocity_direction = this->velocity_direction - 2 *
									this->velocity_direction.dot(faceNormal) *
									faceNormal;
		Cartesian3 collisionField = this->readyToCompute.dot(faceNormal) * faceNormal;
		FieldOfForce newAccelerate(collisionField.length(), collisionField.unit());
		this->readyToCompute = this->readyToCompute - newAccelerate;
		this->readyToCompute.value = (1 - this->energylose) * this->readyToCompute.value ;
	}
}

void SimulationObject::setRotationVelocity(SimulationVertice *vertice, Cartesian3 velocity) {
	Cartesian3 newVelocity = vertice->velocity * vertice->velocity_direction;
	newVelocity = newVelocity + (1- frictionCoefficient) * velocity;
	vertice->velocity = newVelocity.length();
	vertice->velocity_direction = newVelocity.unit();
}

Cartesian3 SimulationObject::getRotationVelocity(Cartesian3 coods) {
	float degreeInArc = this->rotationSpeed / 57.29578;
	if (degreeInArc == 0)
		return Cartesian3(0, 0, 0);
	Cartesian3 omiga(0, degreeInArc, 0);
	Cartesian3 velocity = (coods - coods.Project(omiga)).cross(omiga);
	return velocity;
}

void SimulationObject::lockCorner() {
	int row = sqrt(this->vertices.size());
	this->vertices[0].simulationEnable = false;
	this->vertices[row - 1].simulationEnable = false;
}

void SimulationObject::unlockCorner() {
	int row = sqrt(this->vertices.size());
	this->vertices[0].simulationEnable = true;
	this->vertices[row - 1].simulationEnable = true;
}