#include "CameraControl.h"
#include "Core/Input/InputManager.h"
#include "../Component.h"
#include "../Transforms.h"
#include <iostream>
#include <typeinfo>

const char* CameraControl::typeStatic=NULL;
	
CameraControl::CameraControl(){
	CameraControl::typeStatic=typeid(CameraControl).name();
	this->type=CameraControl::typeStatic;
		
}

void CameraControl::update(){
	float x = InputManager::m_mouseXDelta / 8.0f;
	float y = InputManager::m_mouseYDelta / 8.0f;
		
	if (InputManager::isKeyPressed(Keys::mouseMiddle)) {
		glm::vec4 move = (getRotation(owner->transformation,2) *glm::vec4(x,0.0f,0.0f,0.0f)+ glm::vec4(0.0f,y,0.0f,0.0f))*0.25f;
		if(InputManager::isKeyPressed(Keys::shiftr)){move *= 3.0f;}
		owner->translate(glm::vec3(-move.x, move.y, -move.z));
	}
	else if (InputManager::isKeyPressed(Keys::mouseRight)){
		owner->rotateAround((glm::vec3)owner->transformation[0],-y,(glm::vec3)owner->transformation[3]);
		owner->rotateAround(glm::vec3(0.0f,1.0f,0.0f),-x, (glm::vec3)owner->transformation[3]);
	}
		
	glm::vec4 move = glm::vec4(0.0f);
	bool moved=false;
	if (InputManager::isKeyPressed(Keys::ctrlr)){
		if (InputManager::isKeyPressed(Keys::up)){	move[1] += 0.2f;	moved = true;}
		if (InputManager::isKeyPressed(Keys::down)){	move[1] += -0.2f;	moved = true;}
	}
	else {
		if (InputManager::isKeyPressed(Keys::up)){	move[2] +=-0.2f;	moved = true;}
		if (InputManager::isKeyPressed(Keys::down)){	move[2] += 0.2f;	moved = true;}
	}
	if (InputManager::isKeyPressed(Keys::left)){		move[0]+= -0.2f;	moved = true;}
	if (InputManager::isKeyPressed(Keys::right)){	move[0]+= 0.2f;		moved = true;}
	if (InputManager::isKeyPressed(Keys::shiftr)){	move *= 3.0f;}

	if(moved){
		move=getRotation(owner->transformation,2)*move;
		owner->translate((glm::vec3)move);
	}
}
	
void CameraControl::setRotation(float x,float y){
	/*this->rotyNeg=-y;
	this->rotxNeg=-x;
				
	glm::mat4 roty = glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotx = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
		
	owner->rot=rotx*roty;
	owner->translate(glm::vec3(0.0f));	*/	
}

