#include <iostream>
#include <typeinfo>
#include "../Component.h"
#include "CameraControl.h"
#include "../Transforms.h"
#include "../../Core/InputController.h"

const char* CameraControl::typeStatic=NULL;
	
CameraControl::CameraControl(){
	CameraControl::typeStatic=typeid(CameraControl).name();
	this->type=CameraControl::typeStatic;
		
}

void CameraControl::update(){
	float x = InputController::m_mouseXDelta / 8.0f;
	float y = InputController::m_mouseYDelta / 8.0f;
		
	if (InputController::isKeyPressed(Keys::mouseMiddle)) {
		glm::vec4 move = (getRotation(owner->transformation,2) *glm::vec4(x,0.0f,0.0f,0.0f)+ glm::vec4(0.0f,y,0.0f,0.0f))*0.25f;
		if(InputController::isKeyPressed(Keys::shiftr)){move *= 3.0f;}
		owner->translate(glm::vec3(-move.x, move.y, -move.z));
	}
	else if (InputController::isKeyPressed(Keys::mouseRight)){
		owner->rotate((glm::vec3)owner->transformation[0],-y);	
		owner->rotate(glm::vec3(0.0f,1.0f,0.0f),-x);	
	}
		
	glm::vec4 move = glm::vec4(0.0f);
	bool moved=false;
	if (InputController::isKeyPressed(Keys::ctrlr)){
		if (InputController::isKeyPressed(Keys::up)){	move += glm::vec4(0.0f, 0.2f, 0.0f, 0.0f);	moved = true;}
		if (InputController::isKeyPressed(Keys::down)){	move += glm::vec4(0.0f, -0.2f, 0.0f, 0.0f);	moved = true;}
	}
	else {
		if (InputController::isKeyPressed(Keys::up)){	move += glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);	moved = true;}
		if (InputController::isKeyPressed(Keys::down)){	move += glm::vec4(0.0f, 0.0f, 0.2f, 0.0f);	moved = true;}
	}
	if (InputController::isKeyPressed(Keys::left)){		move += glm::vec4(-0.2f, 0.0f, 0.0f, 0.0f);	moved = true;}
	if (InputController::isKeyPressed(Keys::right)){	move += glm::vec4(0.2f, 0.0f, 0.0f, 0.0f);	moved = true;}
	if (InputController::isKeyPressed(Keys::shiftr)){	move *= 3.0f;}

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

