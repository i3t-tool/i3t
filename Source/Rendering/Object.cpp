#include "Object.h"

#include <iostream>
#include <sstream>

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Rendering/Shaper.h"
#include "Rendering/ShaperElements.h"
#include "Transformation.h"

Object::Object(Geometry* _geometry, Material* _material, Transformation* _transformation)
		: ModelInstance(_geometry, _material), transformation(_transformation)
{
}

Transformation* Object::getTransformation() const
{
	return transformation;
}

void Object::debug(Shaper* shaper)
{
	ShaperElements::setShaper(shaper);
	ShaperElements::drawOrientation(getTrans(), 1.0f);
}

void Object::update()
{
	if (transformation != NULL)
	{
		// setTrans(transformation->getTransformationOut());
		// glm::mat4 trans = WorkspaceWindow::DrawInScene();
		setTrans(trans);
	}
}

std::string ObjectDefinition::getObjectDefinitionTreePath() const
{
	std::string path = node->name;
	path.append(" ");
	path.append(name);

	ObjectDefinitionTree* an = node->parent;

	while (an != NULL)
	{
		std::string ts = an->name;
		ts.append(" ");
		ts.append(path);
		path = ts;

		an = an->parent;
	}

	return path;
}

ObjectDefinition* ObjectDefinitionTree::getObjectDefinitionFromTreePath(const std::string& path)
{
	std::stringstream ss(path);

	std::string input;
	ss >> input;
	ss >> input;

	ObjectDefinitionTree* an = ObjectDefinitionTree::tree;

	while (ss.good())
	{
		ObjectDefinitionTree* t = an->getNode(input); // getSubmenu

		if (t == NULL)
		{
			return an->getDefiniton(input); // getNIde - getMenuItem
		}
		else
			an = t;

		ss >> input;
	}

	return an->getDefiniton(input);
}

// statics
// std::map<std::string, Object*> ObjectMap::objects;
ObjectDefinitionTree* ObjectDefinitionTree::tree;
