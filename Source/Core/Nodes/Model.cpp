#include "Model.h"

#include "Core/Nodes/GraphManager.h"

namespace Core
{
void Model::updateValues(int inputIndex)
{
	/// \todo New approach
	/*
	if (m_inputs[0].isPluggedIn())
	{
	  auto& targetStorage = m_inputs[0].getStorage();
	  m_mesh->transform(targetStorage.getMat4());
	}
	 */

	if (m_inputs[0].isPluggedIn())
	{
		auto& targetStorage = m_inputs[0].getStorage();
		static_cast<GameObject*>(m_internalData[0].getPointer())->transformation =
		    targetStorage.getMat4();
	}
}

Ptr<Node> Model::clone() { return GraphManager::createModel(); }
} // namespace Core
