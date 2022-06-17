#include <iostream>

#include "Component.h"

const char* Component::s_type = NULL;

const char* Component::getComponentType()
{
	printf("Default type\n");
	return Component::s_type;
}

Component::Component()
{
	// printf("component\n");
	Component::s_type  = typeid(Component).name();
	this->m_type       = Component::s_type;
	this->m_gameObject = NULL;
	this->m_isActive   = true;
}
void Component::GUI()
{ /*printf("Default GUI\n");*/
}
void Component::start()
{ /*printf("Default start\n");*/
}
void Component::update()
{ /*printf("Default update\n");*/
}
void Component::render(glm::mat4* parent, bool renderTransparent)
{ /*printf("Default render\n");*/
}
