#pragma once
#include "../Component.h"
#include "../GameObject.h"

#include "Source/Core/Nodes/Node.h"

class FreeManipulator : public Component{
public:
	FreeManipulator();
	void update();
	void start();
	void render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return FreeManipulator::s_type; };
	const char* getComponentType() { return FreeManipulator::s_type; };

	static const int EDIT_ROTATION=7,EDIT_SCALE=8,EDIT_POSITION=9;
	const Ptr<Core::NodeBase> *m_editednode=nullptr;
	const Ptr<Core::Sequence> *m_parent=nullptr;
private:
	static const char* s_type;

	int m_editmode=EDIT_ROTATION;
	int m_editaxis=0;
	GameObject*m_circleh,*m_arrowh,*m_planeh,*m_scaleh,*m_uniscaleh,*m_lineh;
	char m_stencilx,m_stencily,m_stencilz,m_stencilzx,m_stencilzy,m_stencilyx,stencilxyz,m_stencilaxisx,m_stencilaxisy,m_stencilaxisz,m_stencilaxisw;
	int m_activehandle=-1,m_hoverhandle=-1,m_axisnum=-1,m_axisnum2=-1;
	glm::mat4 m_edited=glm::mat4(1.0f);
	glm::mat4 m_handlespace=glm::mat4(1.0f);///<transformation of handles - position+rotation - handles are not parent of any object
	glm::mat4 m_bkp=glm::mat4(1.0f);
	glm::mat4 m_rotfreebkp=glm::mat4(1.0f);
	glm::vec2 m_dirbkp=glm::vec2(0.0f);
};