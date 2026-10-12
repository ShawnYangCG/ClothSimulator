//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Frame record module of cloth simulation
//
//  December, 2021
//
//  ------------------------
//  FrameRecord.h
//  ------------------------
//  
//  Used to store a frame of the simulation
//  
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _FRAME_RECORD_H
#define _FRAME_RECORD_H
#include <vector>
#include "renderWindow/Cartesian3.h"
#include "renderWindow/TexturedObject.h"
class FrameRecord {//FrameRecord
public:
	std::vector<Cartesian3> vertices;
	int FrameNumber;
public:
	FrameRecord(int frame);
	void readVertices(TexturedObject* texturedObject);
	void writeVertices(TexturedObject* texturedObject);
};//FrameRecord
#endif
