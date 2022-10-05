#pragma once

#include "sceneGraph/MeshNode.h"

namespace Core
{
class MeshNode final : public pgr::sg::MeshNode
{
public:
	explicit MeshNode(pgr::sg::SceneNode* parent = nullptr)
	    : pgr::sg::MeshNode("<MeshNode>", parent)
	{
	}

	~MeshNode() override;

	void destroy();

	void transform(const glm::mat4& mat);

	void loadProgram() override;
};
} // namespace Core
