#pragma once
#include "../Component.h"
#include "../GameObject.h"

#include "Source/Core/Nodes/Node.h"

class FrustumManipulator :public Component
{
public:
	FrustumManipulator();
	void update();
	void start();
	void render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return FrustumManipulator::s_type; };
	const char* getComponentType() { return FrustumManipulator::s_type; };

	const Ptr<Core::NodeBase>* m_editednode = nullptr;
	const Ptr<Core::Sequence>* m_parent = nullptr;
private:
	static const char* s_type;

	GameObject* m_frustrum, * m_frustruml, * m_cameraico, * m_handle;
	union Stencils { struct N { unsigned char f, n, hu, hb, wr, wl, a; }names; unsigned char arr[7]; }m_stencils;
	glm::vec4 m_hposs[6];
	int m_activehandle=-1,m_hoverhandle=-1, m_axisnum=0;
	glm::mat4 m_edited = glm::mat4(1.0f);
	//Renderer*icorend;
	float m_near=1.0f,m_far=50.0f,m_left=-1.0f,m_right=1.0f,m_top=1.0f,m_bottom=-1.0f;
};

