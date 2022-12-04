#include "Tools.h"

#include <map>
#include <queue>
#include <set>

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

void copyNodes(const std::vector<Ptr<GuiNode>>& nodes)
{
	/*
	if (nodes.empty()) {
	  return;
	}

	std::set<Core::ID>           nodesToCopy;
	std::map<Core::ID, Core::ID> links;

	for (const auto& node : nodes) {
	  const auto& coreNode = node->getNodebase();
	  nodesToCopy.insert(coreNode->getId());
	}

	std::queue<Ptr<Core::Node>> queue;
	std::set<Core::ID>          visited;
	const auto& startNode = Core::GraphManager::getParent(nodes.front()->getNodebase());

	queue.push(startNode);
	visited.insert(startNode->getId());

	while (!queue.empty())
	{
	  const auto leftNode = queue.front(); queue.pop();
	  const auto outputs = leftNode->getOutputPins();
	  for (const auto& outputPin : outputs)
	  {
	    for (const auto& inputPin : outputPin.getOutComponents()) {
	      const auto rightNode = inputPin->getOwner();
	      if (nodesToCopy.contains(rightNode->getId()) && !visited.contains(rightNode->getId())) {
	        visited.insert(rightNode->getId());
	        queue.push(rightNode);
	        links.insert({leftNode->getId(), rightNode->getId()});
	      }
	    }
	  }
	}
	*/

	Memento memento;

	SerializationVisitor serializer(memento, true);
	serializer.dump(nodes);

	std::cerr << JSON::toString(memento) << std::endl;

	NodeDeserializer::createFrom(memento);
}
