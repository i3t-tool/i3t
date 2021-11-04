#pragma once

#include "GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"

/// \todo MH Rename this file.
using NodeClass = std::shared_ptr<WorkspaceNodeWithCoreData>;

struct NodeData
{
	std::string node;
	std::vector<std::string> edges;
};

class SceneRawData
{
public:
	void addOperator(const NodeData& data)
	{
		operators.push_back(data.node);
		addEdges(data.edges);
	}

	void addTransform(const NodeData& data) { transforms.push_back(data.node); }

	void addSequence(const NodeData& data)
	{
		sequences.push_back(data.node);
		addEdges(data.edges);
	}

	void addCamera(const NodeData& data)
	{
		cameras.push_back(data.node);
		addEdges(data.edges);
	}

	void clear()
	{
		operators.clear();
	}

	[[nodiscard]] std::string toString() const;

private:
	void addEdges(const std::vector<std::string>& aEdges)
	{
		for (const auto& edge : aEdges)
		{
			edges.push_back(edge);
		}
	}

	std::vector<std::string> operators;
	std::vector<std::string> transforms;
	std::vector<std::string> sequences;
	std::vector<std::string> cameras;
	std::vector<std::string> edges;
};

struct SceneData
{
	std::vector<Core::NodePtr> operators;
	std::vector<Core::TransformPtr> transforms;

	Core::NodePtr findNode(Core::ID id)
	{
		for (auto& node : operators)
		{
			if (node->getId() == id)
			{
				return node;
			}
		}
		return nullptr;
	}
};
