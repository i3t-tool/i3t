/**
 * \file
 * \brief I3T Node Editor, specialization of the DIWNE object.
 * \authors Jaroslav Holecek, Sofie Sasorina
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "glm/glm.hpp"

#include "DIWNE/Core/NodeEditor.h"

#include "Config.h"
#include "Core/Result.h"
#include "I3T.h"
#include "Logger/Logger.h"
#include "State/StateManager.h"

#include "GUI/Workspace/Nodes/Basic/CoreLink.h"
#include "GUI/Workspace/Nodes/Camera.h"
#include "GUI/Workspace/Nodes/Cycle.h"
#include "GUI/Workspace/Nodes/Model.h"
#include "GUI/Workspace/Nodes/Operator.h"
#include "GUI/Workspace/Nodes/Screen.h"
#include "GUI/Workspace/Nodes/Sequence.h"
#include "GUI/Workspace/Nodes/Transformation.h"
#include "Tools.h"
#include "ViewportHighlightResolver.h"

namespace Workspace
{
class ScriptingNode;

class WorkspaceDiwne : public DIWNE::NodeEditor
{
	friend void Sequence::moveNodeToWorkspace(Ptr<CoreNode> node); // TODO: Friend stuff would be nice to avoid
public:
	ImDrawListSplitter m_channelSplitter;
	Memento* copiedNodes = nullptr;
	Core::MatrixTracker* tracking{nullptr};
	float timeUntilNextTrack = 0;
	bool smoothTracking{true};

	Vp::Entity* m_viewportLastSelectedEntity{nullptr};
	bool m_viewportSelectionChanged{false};

	ViewportHighlightResolver m_viewportHighlightResolver;

	bool m_updateDataItemsWidth{false}; ///< Indicates a change in zoom level this frame

	bool m_trackingFromLeft{false};

	// TODO: The nodes should be mainly stored in the DIWNE::NodeEditor not in a subclass
	//  Or a system where the storage can be specified by subclasses should be created
	//  However this turns out to be very tricky since vectors of derived classes cannot be easily converted
	//  into vectors of base class. There are hacky workarounds like reinterpret_cast but generally at least a shallow
	//  O(n) copy and cast of pointers is required. That may be insignificant but I'm not sure (large node counts?)
	//  - The simplest solution is to use a single base node vector in NodeEditor. Issue is that in subclasses everytime
	//    a non base functionality is required the fetched node would need to be (pseudo-safely) static-casted.
	//    That is a lot of boilerplate code though.
	//  - More complicated but still clean solution is to create a generic getNodes method that subclasses override,
	//  however its not clear what this method would return, it can't return a base class vector, it also can't return
	//  a generic iterator since it is type specific. It could return a pair of pointers for read only access.
	//
	// TODO: I've moved the CoreNode vector to DIWNE::NodeEditor for the time being.
	//  This way the code can be rewritten in DIWNE but still compile and be tested in Workspace
	//  Will see how we go from there.
	//	/**
	//	 * \brief All WorkspaceNodes
	//	 * \note Nodes inside Sequences are not directly in this vector (they are in Sequence)
	//	 **/
	//	std::vector<Ptr<CoreNode>> m_workspaceCoreNodes;

	/**
	 * A map connecting Core node id's with equivalent gui nodes.
	 * Contains raw pointers as values so it can be populated in constructors.
	 */
	std::unordered_map<Core::ID, CoreNode*> m_coreIdMap;

	WorkspaceDiwne(DIWNE::SettingsDiwne* settingsDiwne);
	~WorkspaceDiwne() override;

	// Lifecycle
	// =============================================================================================================
	//	void initialize(DIWNE::DrawInfo& context) override;
	void begin(DIWNE::DrawInfo& context) override;
	void content(DIWNE::DrawInfo& context) override;
	void end(DIWNE::DrawInfo& context) override;
	void finalize(DIWNE::DrawInfo& context) override;

	void popupContent(DIWNE::DrawInfo& context) override;

	// Object management
	// =============================================================================================================
	template <class T>
	auto inline addNodeToPositionOfPopup()
	{
		auto result = createNode<T>(canvas().screen2diwne(diwne.getPopupPosition()));
		return result;
	}

	/**
	 * Duplicates nodes marked for duplication using Workspace::Node::setDuplicateNode()
	 */
	void performLazyDuplication();

	// Interaction
	// =============================================================================================================
	void processInteractions(DIWNE::DrawInfo& context) override;

	void onZoom() override;
	// TODO: (DR) Reimplement
	//    bool isZoomingDiwne() override;
	bool processZoom() override;

	void manipulatorStartCheck3D();

	void trackingSmoothLeft();
	void trackingSmoothRight();
	void trackingJaggedLeft();
	void trackingJaggedRight();
	void trackingModeSwitch();
	void trackingSwitch();
	void trackingSwitchOn(Ptr<Sequence> sequence = nullptr, bool isRightToLeft = true);

	/**
	 * @pre @p sequence is not null.
	 */
	void trackingInit(Ptr<Sequence> sequence, std::vector<Ptr<Model>> models, bool isRightToLeft);
	void trackingSwitchOff();
	void toggleSelectedNodesVisibility();

	void processTrackingMove();

	void zoomToAll();
	void zoomToSelected();
	ImRect getOverNodesRectangleDiwne(std::vector<Ptr<DIWNE::Node>> nodes);
	void zoomToRectangle(ImRect const& rect);

	void copySelectedNodes();
	void pasteSelectedNodes();
	void cutSelectedNodes();
	void duplicateSelectedNodes();

	// Node getters
	// =============================================================================================================

	// TODO: Rename to getCoreNode()
	/**
	 * O(N) where N is workspace nodes count.
	 * @tparam T
	 * @param id
	 * @return
	 */
	template <typename T>
	Result<Ptr<T>, Error> getNode(Core::ID id) const
	{
		Ptr<GuiNode> node{};
		auto allCoreNodes = getAllCoreNodes();
		for (auto it = allCoreNodes.begin(); it != allCoreNodes.end(); ++it)
		{
			std::shared_ptr<CoreNode> n = it.sharedPtr();
			if (n->getNodebase()->getId() == id)
			{
				node = std::move(n);
			}
		}

		if (node == nullptr)
		{
			return Err("cannot find node #" + std::to_string(id));
		}

		auto result = std::dynamic_pointer_cast<T>(node);

		if (result == nullptr)
		{
			return Err("node #" + std::to_string(id) + " is not of given type");
		}

		return result;
	}

	/**
	 * Replaces existing old node with a specified new node.
	 * Old node is destroyed and its link connections are replugged into the new node if possible.
	 * Attempts to reconnect input and output pins at corresponsing indices.
	 * @return True on complete sucess, false on failure or when some pins couldn't be reconnected.
	 */
	bool replaceAndReplugNode(Ptr<CoreNodeWithPins> oldNode, Ptr<CoreNodeWithPins> newNode);

private:
	void fetchConnectionStateForPins(const std::vector<Ptr<CorePin>>& pins, std::vector<DIWNE::Pin*>& connections);
	bool restoreConnectionStateForPins(const std::vector<Ptr<CorePin>>& pins,
	                                   const std::vector<DIWNE::Pin*>& connections);

public:
	DIWNE::FilteredNodeRange<CoreNode> getCoreNodes() const
	{
		return DIWNE::FilteredNodeRange<CoreNode>(
		    [](const DIWNE::Node* node) -> bool {
			    return node->getFlag(CoreNode::CORE_NODE_FLAG);
		    },
		    &m_nodes);
	}

	/// Gets all I3T Core nodes
	DIWNE::FilteredRecursiveNodeRange<CoreNode> getAllCoreNodes() const
	{
		return DIWNE::FilteredRecursiveNodeRange<CoreNode>(
		    [](const DIWNE::Node* node) -> bool {
			    return node->getFlag(CoreNode::CORE_NODE_FLAG);
		    },
		    &m_nodes);
	}

	/// Gets all I3T Core nodes that are selected
	DIWNE::FilteredRecursiveNodeRange<CoreNode> getAllSelectedCoreNodes() const
	{
		return DIWNE::FilteredRecursiveNodeRange<CoreNode>(
		    [](const DIWNE::Node* node) -> bool {
			    return node->getFlag(CoreNode::CORE_NODE_FLAG) && node->getSelected();
		    },
		    &m_nodes);
	}

	// Gets all I3T Core nodes that are selected and aren't a child of an already selected node
	DIWNE::FilteredRecursiveNodeRange<CoreNode> getAllSelectedCoreNodesWithoutNesting() const
	{
		return DIWNE::FilteredRecursiveNodeRange<CoreNode>(
		    [](const DIWNE::Node* node) -> bool {
			    return node->getFlag(CoreNode::CORE_NODE_FLAG) && node->getSelected();
		    },
		    [](const DIWNE::Node* node) -> bool {
			    return !node->getSelected();
		    },
		    &m_nodes);
	}

	DIWNE::FilteredNodeRange<Camera> getAllCameras();
	DIWNE::FilteredNodeRange<Model> getAllModels();
	std::vector<Ptr<Model>> getSequenceModels(Ptr<Sequence> seq);
	DIWNE::FilteredNodeRange<Sequence> getAllInputFreeSequence();
	DIWNE::FilteredNodeRange<Model> getAllInputFreeModel();
	DIWNE::FilteredRecursiveNodeRange<ScriptingNode> getAllScriptingNodes();

	// =============================================================================================================
};

class WorkspaceEditorInputAdapter : public DIWNE::NodeEditorInputAdapter
{
public:
	WorkspaceEditorInputAdapter(DIWNE::NodeEditor& editor) : NodeEditorInputAdapter(editor) {}

	bool selectAllNodes() override;
	bool invertSelection() override;
	bool deleteSelectedNodes() override;
};
} // namespace Workspace