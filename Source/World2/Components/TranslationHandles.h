#pragma once
#include "../Component.h"
#include "../GameObject.h"

#include "Source/Core/Nodes/Node.h"


class TranslationHandles : public Component {
public:
	TranslationHandles();
	void start();
	void update();
	void render(glm::mat4* parent, bool renderTransparent);
	static const char* componentType() { return TranslationHandles::typeStatic; };
	const char* getComponentType() { return TranslationHandles::typeStatic; };

	Core::NodeBase *editedobj;
private:
	static const char* typeStatic;


};

