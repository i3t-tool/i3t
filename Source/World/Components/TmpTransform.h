#pragma once
#include "../Component.h"

class TmpTransform:public Component
{
public:
	TmpTransform();
	// void start();
	void update();
	static const char* componentType() { return TmpTransform::s_type; };
	const char* getComponentType() { return TmpTransform::s_type; };

private:
	static const char* s_type;

	glm::mat4 m_scal=glm::mat4(1.0f),m_rotx=glm::mat4(1.0f), m_roty = glm::mat4(1.0f), m_rotz = glm::mat4(1.0f), m_translate=glm::mat4(1.0f),m_free=glm::mat4(1.0f);
};

