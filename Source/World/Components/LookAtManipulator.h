#pragma once
/**
 * \file LookAtManipulator.h
 * \author Daniel Gruncl
 */
#include "../Component.h"
#include "../GameObject.h"

#include "Core/Nodes/Node.h"

class LookAtManipulator:public Component
{
public:
	LookAtManipulator();
	void GUI();
	void update();
	void render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return LookAtManipulator::s_type; };
	const char* getComponentType() { return LookAtManipulator::s_type; };

	static const int EDIT_CENTER=1,EDIT_EYE=2;
	Ptr<Core::NodeBase> m_editednode;
	Ptr<Core::Sequence> m_parent;
private:
	static const char* s_type;

	int m_editmode=EDIT_CENTER;
	GameObject*m_planeh,*m_arrowh,*m_threeaxis,*m_cameraico;
	char m_stencilx,m_stencily,m_stencilz,m_stencilzx,m_stencilzy,m_stencilyx;
	int m_activehandle=-1,m_hoverhandle=-1,m_axisnum=-1,m_axisnum2=-1;
	glm::mat4 m_handlespace=glm::mat4(1.0f);///<transformation of handles - handles are not parent of any object
	glm::mat4 m_edited=glm::mat4(1.0f);
};

