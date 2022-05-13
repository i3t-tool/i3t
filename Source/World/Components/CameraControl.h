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
		
	void setRotation(glm::vec3 dir,bool moveToCenter);
	void setScroll(float val);

	bool m_rotateAroundCenter = true;
private:
	static const char* s_type;

	void rotate();
	glm::vec3 m_dir=glm::vec3(1.0f);
	glm::vec3 m_pos=glm::vec3(1.0f);
	glm::vec3 m_dirbkp=glm::vec3(1.0f);
	glm::vec3 m_posbkp=glm::vec3(1.0f);
	float m_alpha=1.0f;

	bool m_moveToCenter=false;

	float m_scroll=0.0f;
};
	

