//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Physical simulation module of cloth simulation
//
//  December, 2021
//
//  ------------------------
//  FieldOfForce.h
//  ------------------------
//  
//  A force field with the value and directon ready to compute
//  
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _FIELD_OF_FORCE_H
#define _FIELD_OF_FORCE_H
#include "stdafx.h"
#include "renderWindow/Cartesian3.h"
class FieldOfForce {// class FieldOfForce
public:
	float value = 0;	// the value of force field
	Cartesian3 direction;	// the direction of force field
	bool IsEnable = true;	// whether this field is enable

private:
	float gravity = 9.8;	// the constance of gravity

public:
	// constructors
	FieldOfForce();
	FieldOfForce(std::string keyword);
	FieldOfForce(float value, Cartesian3 direction);
	FieldOfForce(const FieldOfForce& other);
	// functions
	void setMode(std::string keyword);
	void setField(float value, Cartesian3 direction);
	// addition operator
	FieldOfForce operator +(const FieldOfForce& other) const;
	FieldOfForce operator -(const FieldOfForce& other) const;
	float dot(const FieldOfForce& other);
	float dot(Cartesian3 other);
};// class FieldOfForce
#endif
