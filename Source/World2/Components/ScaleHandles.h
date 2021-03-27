#pragma once
#include "../Component.h"
#include "../GameObject.h"

#include "Source/Core/Nodes/Node.h"


class ScaleHandles : public Component {
public:
	ScaleHandles();
	void start();
	void update();
	void render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return ScaleHandles::typeStatic; };
	const char* getComponentType() { return ScaleHandles::typeStatic; };

	Core::NodeBase* m_editedobj2;
private:
	static const char* typeStatic;

	GameObject*m_editedobj,*m_planeh,*m_scaleh,*m_uniscaleh,*m_threeaxis;
	char m_stencilx,m_stencily,m_stencilz,m_stencilzx,m_stencilzy,m_stencilyx,m_stencilxyz;
	int m_activehandle=-1,m_hoverhandle=-1,m_axisnum=-1,m_axisnum2=-1;
	glm::mat4 m_handlespace;///<transformation of handles - position+rotation - handles are not parent of any object
	glm::mat4 m_bkp=glm::mat4(1.0f);
};

