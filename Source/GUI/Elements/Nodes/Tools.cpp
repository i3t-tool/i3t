#include "Tools.h"

#include <queue>

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"

#include "GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "Utils/JSON.h"

GuiNodePtr findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id)
{
	for (const auto& node : nodes)
		if (node->getNodebase()->getId() == id)
			return node;

	return nullptr;
}

Memento copyNodes(const std::vector<Ptr<GuiNode>>& nodes)
{
	Memento memento;

	SerializationVisitor serializer(memento, true);
	serializer.dump(nodes);

	return memento;
}

void pasteNodes(const Memento& memento)
{
	NodeDeserializer::createFrom(memento);
}
