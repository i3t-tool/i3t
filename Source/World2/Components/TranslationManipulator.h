#pragma once
#include "../Component.h"
#include "../GameObject.h"

#include "Source/Core/Nodes/Node.h"


class TranslationManipulator : public Component {
public:
	TranslationManipulator();
	void update();
	void render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return TranslationManipulator::s_type; };
	const char* getComponentType() { return TranslationManipulator::s_type; };

	const Ptr<Core::NodeBase> *m_editednode=nullptr;
	const Ptr<Core::Sequence> *m_parent=nullptr;
private:
	static const char* s_type;

	GameObject*m_planeh,*m_arrowh,*m_threeaxis;
	char m_stencilx,m_stencily,m_stencilz,m_stencilzx,m_stencilzy,m_stencilyx;
	int m_activehandle=-1,m_hoverhandle=-1,m_axisnum=-1,m_axisnum2=-1;
	glm::mat4 m_handlespace;///<transformation of handles - handles are not parent of any object
	glm::mat4 m_edited=glm::mat4(1.0f);
};

