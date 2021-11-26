#pragma once

#include <string>
#include <vector>

#include "State/NodeVisitor.h"
#include "State/SceneData.h"

class DumpVisitor : public NodeVisitor
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
	std::string dump(const std::vector<Ptr<NodeClass>>& nodes);

private:
	/**
	 * Visit node and save its data to m_visitedNodesData member variable.
	 * \param node
	 */
	void visit(const Ptr<GuiCamera>& node) override;
	void visit(const Ptr<GuiCycle>& node) override;
	void visit(const Ptr<GuiOperator>& node) override;
	void visit(const Ptr<GuiSequence>& node) override;
	void visit(const Ptr<GuiTransform>& node) override;

	/// Stores last scene representation.
	SceneRawData m_sceneData;

	static bool m_isInitialized;
};

/// \todo MH move somewhere else.
/// \todo MH make function internal.
SceneData loadScene(const std::string& rawScene);

/**
 * Loads scene to the Workspace Window.
 * @param rawScene
 * @return
 */
SceneData loadSceneFromFile(const std::string& sceneFile);

bool saveScene(const std::string& filename, const SceneData& scene);
