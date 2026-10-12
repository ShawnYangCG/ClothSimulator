//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5823M Animation and Simulation
//  Frame record module of cloth simulation
//
//  December, 2021
//
//  ------------------------
//  FrameRecord.cpp
//  ------------------------
//  
//  Used to store a frame of the simulation
//  
//////////////////////////////////////////////////////////////////////
#include"FrameRecord.h"
FrameRecord::FrameRecord(int frame) {
	this->FrameNumber = frame;
}

void FrameRecord::readVertices(TexturedObject* texturedObject) {
	for (int i = 0; i < texturedObject->vertices.size(); i++) {
		Cartesian3 vertice(texturedObject->vertices[i]);
		this->vertices.push_back(vertice);
	}
}

void FrameRecord::writeVertices(TexturedObject* texturedObject) {
	for (int i = 0; i < texturedObject->vertices.size(); i++) {
		texturedObject->vertices[i] = this->vertices[i];
	}
}