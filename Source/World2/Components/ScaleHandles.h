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

	Core::NodeBase* editedobj;
private:
	static const char* typeStatic;


};

