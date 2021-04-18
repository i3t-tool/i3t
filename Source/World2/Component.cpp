#include <iostream>

#include "Component.h"

const char* Component::typeStatic = NULL;

const char* Component::getComponentType(){
	printf("Default type\n");
	return Component::typeStatic;
}

Component::Component(){
	// printf("component\n");
	Component::typeStatic = typeid(Component).name();
	this->type = Component::typeStatic;
	this->owner = NULL;
	this->isActive = true;
}
void Component::start(){ /*printf("Default start\n");*/}
void Component::update(){ /*printf("Default update\n");*/}
void Component::render(glm::mat4* parent, bool renderTransparent){ /*printf("Default render\n");*/}
