#pragma once

#include "GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"
#include "GUI/Elements/Nodes/WorkspaceTransformation.h"

/// Node data as string
struct NodeData
{
	std::string              node;
	std::vector<std::string> edges;
};

/// Whole scene in text format.
class SceneRawData
{
	using RawData = std::vector<std::string>;

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

	void addScreen(const NodeData& data)
	{
		screens.push_back(data.node);
		addEdges(data.edges);
	}

	void clear() { operators.clear(); }

	[[nodiscard]] std::string toString() const;

private:
	void addEdges(const std::vector<std::string>& aEdges)
	{
		for (const auto& edge : aEdges) { edges.push_back(edge); }
	}

	RawData operators;
	RawData transforms;
	RawData sequences;
	RawData cameras;
	RawData screens;
	RawData edges;
};

struct SceneData
{
	std::vector<Ptr<GuiNode>> nodes;

	Ptr<GuiNode> findNode(Core::ID id)
	{
		for (auto& node : nodes)
			if (node->getId() == id) return node;

		return nullptr;
	}
};
