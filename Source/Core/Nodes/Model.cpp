/**
 * \file
 * \brief
 * \authors Martin Herich, Dan Rakušan
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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
