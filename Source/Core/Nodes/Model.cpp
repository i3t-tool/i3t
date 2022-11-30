#include "Model.h"

#include "Core/Nodes/GraphManager.h"

namespace Core
{
void Model::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn())
	{
		// Models m_modelMatrix value is updated and then can be read from the
		// Node's update callback
		auto& targetStorage = m_inputs[0].getStorage();
		m_modelMatrix = targetStorage.getMat4();
		Node::updateValues(inputIndex); // Callback
	}
}

Ptr<Node> Model::clone() { return GraphManager::createModel(); }
} // namespace Core
