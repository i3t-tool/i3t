#pragma once
/**
 * \file RotationManipulator.h
 * \author Daniel Gruncl
 */
#include "../Component.h"
#include "../GameObject.h"

#include "Core/Nodes/Node.h"

class RotationManipulator:public Component
{
public:
	RotationManipulator();
	void update();
	void render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return RotationManipulator::s_type; };
	const char* getComponentType() { return RotationManipulator::s_type; };

	Ptr<Core::NodeBase> m_editednode;
	Ptr<Core::Sequence> m_parent;
private:
	static const char* s_type;

	static const int s_x=1<<0,s_y=1<<1,s_z=1<<2;
	GameObject*m_circleh,*m_threeaxis;
	char m_stencilx,m_stencily,m_stencilz;
	int m_activehandle=-1,m_hoverhandle=-1,m_axisnum=-1,m_allowedaxis=0;
	glm::mat4 m_handlespace=glm::mat4(1.0f);///<transformation of handles - handles are not parent of any object
	glm::mat4 m_edited=glm::mat4(1.0f);
	glm::vec2 m_dirbkp=glm::vec2(0.0f);
};
