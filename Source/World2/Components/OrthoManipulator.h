#pragma once
#include "../Component.h"
#include "../GameObject.h"

#include "Source/Core/Nodes/Node.h"

class OrthoManipulator:public Component
{
public:
	OrthoManipulator();
	void update();
	void start();
	void render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return OrthoManipulator::typeStatic; };
	const char* getComponentType() { return OrthoManipulator::typeStatic; };

	const Ptr<Core::NodeBase> *m_editednode=nullptr;
	const Ptr<Core::Sequence> *m_parent=nullptr;
private:
	static const char* typeStatic;

	GameObject*m_frustrum,*m_frustruml,*m_cameraico,*m_handle;
	union Stencils{struct N{unsigned char f,n,hu,hb,wr,wl,a;}names;unsigned char arr[7];}m_stencils;
	glm::vec4 m_hposs[6];
	int m_activehandle,m_axisnum;
	glm::mat4 m_edited=glm::mat4(1.0f);
	//Renderer*icorend;
	float m_near=-0.0f,m_far=20.0f,m_left=-10.0f,m_right=10.0f,m_top=10.0f,m_bottom=-10.0f;
};

