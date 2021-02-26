#pragma once
#include <iostream>
#include "../Component.h"

class CameraControl: public Component{
public:
	CameraControl();
	void update();
	static const char* componentType(){return CameraControl::typeStatic;};
	const char* getComponentType(){return CameraControl::typeStatic;};
		
	void setRotation(float x,float y);
private:
	static const char* typeStatic;
		
	float rotyNeg=0.0f,rotxNeg=0.0f;
};
	

