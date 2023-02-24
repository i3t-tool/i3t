#pragma once

#include "sceneGraph/MeshGeometry.h"

#include "Core/Nodes/Node.h"

namespace Core
{
class Model : public Node
{
public:
	Model() : Node(&g_modelProperties){};

	/// Latest model matrix value
	glm::mat4 m_modelMatrix{};

	void updateValues(int inputIndex = 0) override;
	Ptr<Node> clone() override;

	void onUnplugInput(size_t index) override;

	/// \todo
	void resetModelPosition();
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
