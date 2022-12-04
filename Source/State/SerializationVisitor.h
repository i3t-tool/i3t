#pragma once

#include <array>
#include <string>
#include <unordered_set>
#include <vector>

#include "imgui.h"

#include "Core/Nodes/Node.h"
#include "Core/Nodes/NodeData.h"
#include "State/NodeVisitor.h"
#include "State/Stateful.h"

class SerializationVisitor : public NodeVisitor
{
public:
	SerializationVisitor(Memento& memento, bool assignNewIds = false) : m_memento(memento), m_assignNewIds(assignNewIds)
	{
	}

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
	void dump(const std::vector<Ptr<GuiNode>>& nodes);

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
	void visit(const Ptr<GuiScreen>& node) override;
	void visit(const Ptr<GuiModel>& node) override;

	/// id and position
	///
	/// \param target
	/// \param node
	///
	/// \pre target is JSON object
	void dumpCommon(rapidjson::Value& target, const Ptr<GuiNode>& node);

	void dumpSequence(rapidjson::Value& target, const Ptr<GuiSequence>& node);
	void dumpTransform(rapidjson::Value& target, const Ptr<GuiTransform>& node);

	void addBool(rapidjson::Value& target, const char* key, bool value);
	void addString(rapidjson::Value& target, const char* key, const std::string& value);
	void addVector(rapidjson::Value& target, const char* key, const ImVec2& vec);
	void addVector(rapidjson::Value& target, const char* key, const glm::vec3& vec);
	void addVector(rapidjson::Value& target, const char* key, const glm::vec4& vec);
	void addMatrix(rapidjson::Value& target, const char* key, const glm::mat4& mat);
	void addData(rapidjson::Value& target, const char* key, const DataStore& data);

	/// \param target document["edges"] or any JSON array.
	void addEdges(rapidjson::Value& target, const Ptr<Core::Node>& node);

	Memento& m_memento;
	bool m_assignNewIds;

	std::unordered_set<Core::ID> m_ids;
	std::unordered_map<Core::ID, Core::ID> m_oldToNewId;
};

/**
 * Creates all nodes in the workspace.
 */
// SceneData buildScene(const std::string& rawScene, GuiNodes& workspaceNodes);
