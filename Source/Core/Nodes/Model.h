#pragma once

#include "sceneGraph/MeshGeometry.h"

#include "Core/Nodes/Model.h"
#include "Core/Nodes/Node.h"
#include "Core/Resources/Mesh.h"

namespace Core
{
class Model : public Node
{
public:
	Model() : Node(&g_modelProperties){};

	void updateValues(int inputIndex = 0) override;
	Ptr<Node> clone() override;

	MeshNode* mesh() const { return m_mesh; }
	void setMesh(MeshNode* m) { m_mesh = m; }

private:
	MeshNode* m_mesh = nullptr;
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
