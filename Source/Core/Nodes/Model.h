#pragma once

#include "sceneGraph/MeshGeometry.h"

#include "Core/Nodes/Model.h"
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

	// TODO: (DR) Model needs to hold a reference to a new Viewport object,
	// ideally somehow abstracted so that layer separation is preserved (unlikely)
	// MeshNode* mesh() const { return m_mesh; }
	// void setMesh(MeshNode* m) { m_mesh = m; }

private:
	// MeshNode* m_mesh = nullptr;
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
