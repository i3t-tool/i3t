#pragma once

#include <string>
#include <vector>

#include "Core/Nodes/NodeVisitor.h"

#include "State/SceneData.h"

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
