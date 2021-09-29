#pragma once

#include <string>
#include <vector>

#include "Core/Nodes/NodeVisitor.h"
#include "GUI/Elements/Nodes/WorkspaceNodeWithCoreData.h"

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

class DumpVisitor : public Core::NodeVisitor
{
public:
	DumpVisitor();

	/**
	 * Get string representation of current scene.
	 *
	 * Example output for two operator nodes connected with each other.
	 * \code
	 * operators:
	 *   - id: 1
	 *     type: FloatToFloat
	 *     position: [0.0, 0.0]
	 *     value: 1.0
	 *   - id: 4
	 *     type: FloatToFloat
	 *  	 position: [0.0, 0.0]
	 *  	 value: -1.0
	 * edges:
	 *   - [1, 0, 4, 0]
	 * \endcode
	 */
	std::string dump(const std::vector<NodeClass>& nodes);

private:
	/**
	 * Visit node and save its data to m_visitedNodesData member variable.
	 * \param node
	 */
	void visit(const Core::NodePtr& node) override;

	/// Stores last scene representation.
	SceneRawData m_sceneData;

	static bool m_isInitialized;
};

SceneData load(const std::string& rawScene);
