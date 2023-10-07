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
		auto& targetStorage = m_inputs[0].data();
		m_modelMatrix = targetStorage.getMat4();
	}

	Node::updateValues(inputIndex); // Callback
}

void Model::onUnplugInput(size_t index)
{
	m_modelMatrix = glm::mat4(1.0f);
	Node::updateValues(index);
}

void Model::resetModelPosition()
{
	m_modelMatrix = glm::mat4(1.0f);
	Node::updateValues(0);
}
} // namespace Core
