/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "sceneGraph/MeshGeometry.h"

#include "Core/Nodes/Node.h"

namespace Core
{
class Model : public Node
{
public:
	Model() : Node(g_modelProperties){};

	/// Latest model matrix value
	glm::mat4 m_modelMatrix{};

	void updateValues(int inputIndex = 0) override;
	void resetModelPosition();

protected:
	void onUnplug(Node* fromNode, Node* toNode, size_t fromIndex, size_t toIndex) override;
};

namespace Builder
{
inline Ptr<Model> createModelNode()
{
	auto ret = std::make_shared<::Core::Model>();
	ret->init();
	ret->updateValues(0);

	return ret;
}
} // namespace Builder
} // namespace Core
