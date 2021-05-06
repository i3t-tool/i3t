#pragma once
//----------------
/**
 * \file CameraControl.h
 * \author Daniel Gruncl
 * \brief control scene camera
 * mouse right button, mouse middle button,
 * arrows,ctrl,shift
 */
//---------------
#include <iostream>
#include "../Component.h"

class CameraControl: public Component{
public:
	CameraControl();
	void update();
	static const char* componentType(){return CameraControl::s_type;};
	const char* getComponentType(){return CameraControl::s_type;};
		
	void setRotation(float x,float y);
private:
	static const char* s_type;
};
	

