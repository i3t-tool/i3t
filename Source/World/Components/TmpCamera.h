#pragma once
#include "../Component.h"
#include "Camera.h"

class TmpCamera :public Component
{
public:
	TmpCamera();
	void start();
	void update();
	static const char* componentType() { return TmpCamera::s_type; };
	const char* getComponentType() { return TmpCamera::s_type; };

private:
	static const char* s_type;

	Camera*m_cam;
	//glm::mat4 m_proj=glm::mat4(1.0f),m_view=glm::mat4(1.0f);
};