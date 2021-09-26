#pragma once

#include <string>
#include <vector>

#include "Core/Nodes/NodeVisitor.h"

struct NodeData
{
	std::string node;
	std::vector<std::string> edges;
};

class SceneData
{
public:
	void addOperator(const NodeData& data)
	{
		operators.push_back(data.node);

		for (const auto& edge : data.edges)
		{
			edges.push_back(edge);
		}
	}

	void clear()
	{
		operators.clear();
	}

	std::string toString() const;

private:
	std::vector<std::string> operators;
	// std::vector<NodeData> transforms;
	std::vector<std::string> edges;
};

class DumpVisitor : public Core::NodeVisitor
{
public:
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
	std::string dump(const std::vector<Core::NodePtr>& nodes);

private:
	/**
	 * Visit node and save its data to m_visitedNodesData member variable.
	 * \param node
	 */
	void visit(const Core::NodePtr& node) override;

	/// Stores last scene representation.
	SceneData m_sceneData;
};
