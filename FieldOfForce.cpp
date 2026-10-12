//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Physical simulation module of cloth simulation
//
//  December, 2021
//
//  ------------------------
//  FieldOfForce.cpp
//  ------------------------
//  
//  A force field with the value and directon ready to compute
//  
//////////////////////////////////////////////////////////////////////
#include "FieldOfForce.h"

FieldOfForce::FieldOfForce(){
	this->value = 0;
	this->direction.x = 0.0;
	this->direction.y = 0.0;
	this->direction.z = 0.0;
}

FieldOfForce::FieldOfForce(const FieldOfForce& other) {
	this->value = other.value;
	this->direction.x = other.direction.x;
	this->direction.y = other.direction.y;
	this->direction.z = other.direction.z;
}

FieldOfForce::FieldOfForce(std::string keyword) {
	if (keyword == "GRAVITY") {
		this->value = this->gravity;
		this->direction.x = 0.0;
		this->direction.y = -1.0;
		this->direction.z = 0.0;
		this->direction = this->direction.unit();
	}
}

FieldOfForce::FieldOfForce(float value, Cartesian3 direction) {
		this->value = value;
		this->direction.x = direction.x;
		this->direction.y = direction.y;
		this->direction.z = direction.z;
		this->direction = this->direction.unit();
}

void FieldOfForce::setMode(std::string keyword) {
	if (keyword == "GRAVITY") {
		this->value = this->gravity;
		this->direction.x = 0.0;
		this->direction.y = -1.0;
		this->direction.z = 0.0;
		this->direction = this->direction.unit();
	}
}

void FieldOfForce::setField(float value, Cartesian3 direction) {
	this->value = value;
	this->direction.x = direction.x;
	this->direction.y = direction.y;
	this->direction.z = direction.z;
	this->direction = this->direction.unit();
}

FieldOfForce FieldOfForce::operator + (const FieldOfForce& other) const
{ // FieldOfForce::operator +()
	FieldOfForce returnVal(this->value, this->direction);
	returnVal.direction.x = returnVal.value * returnVal.direction.x + other.value * other.direction.x;
	returnVal.direction.y = returnVal.value * returnVal.direction.y + other.value * other.direction.y;
	returnVal.direction.z = returnVal.value * returnVal.direction.z + other.value * other.direction.z;
	returnVal.value = returnVal.direction.length();
	returnVal.direction = returnVal.direction.unit();
	return returnVal;
} // FieldOfForce::operator +()

FieldOfForce FieldOfForce::operator - (const FieldOfForce& other) const
{ // FieldOfForce::operator -()
	FieldOfForce returnVal(this->value, this->direction);
	returnVal.direction.x = returnVal.value * returnVal.direction.x - other.value * other.direction.x;
	returnVal.direction.y = returnVal.value * returnVal.direction.y - other.value * other.direction.y;
	returnVal.direction.z = returnVal.value * returnVal.direction.z - other.value * other.direction.z;
	returnVal.value = returnVal.direction.length();
	returnVal.direction = returnVal.direction.unit();
	return returnVal;
} // FieldOfForce::operator -()

float FieldOfForce::dot(const FieldOfForce& other) {
	return (this->value * this->direction).dot(other.value * other.direction);
}

float FieldOfForce::dot(Cartesian3 other) {
	return (this->value * this->direction).dot(other);
}