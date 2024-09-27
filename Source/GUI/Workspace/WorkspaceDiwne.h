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

#include "DIWNE/diwne_include.h"

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "glm/glm.hpp"

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
typedef std::vector<Ptr<CoreNode>>::iterator coreNodeIter;

enum WorkspaceDiwneAction
{
	None,
	CreateAndPlugTypeConstructor,
	NOTunselectAllNodes
};

extern WorkspaceDiwne* g_diwne;

class WorkspaceDiwne : public DIWNE::Diwne
{
	friend void Sequence::moveNodeToWorkspace(Ptr<CoreNode> node);

public:
	WorkspaceDiwne(DIWNE::SettingsDiwne* settingsDiwne);
	~WorkspaceDiwne() override;

	ImDrawListSplitter m_channelSplitter;
	Memento* copiedNodes = nullptr;
	Core::MatrixTracker* tracking;
	float timeUntilNextTrack = 0;
	bool smoothTracking;

	ViewportHighlightResolver m_viewportHighlightResolver;

	Vp::Entity* m_viewportLastSelectedEntity{nullptr};
	bool m_viewportSelectionChanged{false};

	void clear() override;

	void popupContent() override;

	bool beforeBegin() override;
	bool beforeContent() override;
	bool content() override;
	bool afterContent() override;
	bool afterEnd() override;
	bool finalize() override;

	void setWorkAreaZoom(float val = 1) override;

	WorkspaceDiwneAction m_workspaceDiwneAction, m_workspaceDiwneActionPreviousFrame;
	void setWorkspaceDiwneAction(WorkspaceDiwneAction wda)
	{
		m_workspaceDiwneAction = wda;
	}
	WorkspaceDiwneAction getWorkspaceDiwneAction()
	{
		return m_workspaceDiwneAction;
	}
	WorkspaceDiwneAction getWorkspaceDiwneAction_previousFrame()
	{
		return m_workspaceDiwneActionPreviousFrame;
	}
	WorkspaceDiwneAction getWorkspaceDiwneActionActive() const
	{
		return m_workspaceDiwneAction == WorkspaceDiwneAction::None ? m_workspaceDiwneActionPreviousFrame
		                                                            : m_workspaceDiwneAction;
	}

	/**
	 * \brief All WorkspaceNodes
	 * \note Nodes inside Sequences are not directly in this vector (they are in Sequence)
	 **/
	std::vector<Ptr<CoreNode>> m_workspaceCoreNodes;

	/**
	 * A map connecting Core node id's with equivalent gui nodes.
	 * Contains raw pointers as values so it can be populated in constructors.
	 */
	std::unordered_map<Core::ID, CoreNode*> m_coreIdMap;

	const std::vector<Ptr<CoreNode>>& getAllNodes() const
	{
		return m_workspaceCoreNodes;
	};

	std::vector<Ptr<CoreNode>> getSelectedNodes();

	std::vector<CoreLink*> m_linksToDraw;
	std::vector<CoreLink> m_linksCameraToSequence;

	/**
	 * \brief For a given input, create appropriate constructor box and plug it to this input
	 * \return true if successful (input was not a Ptr)
	 */
	bool processCreateAndPlugTypeConstructor();

	/**
	 * O(N) where N is workspace nodes count.
	 *
	 * @tparam T
	 * @param id
	 * @return
	 */
	template <typename T>
	Result<Ptr<T>, Error> getNode(Core::ID id) const;

	template <typename T>
	void addTypeConstructorNode()
	{
		CoreInPin* pin = getLastActivePin<CoreInPin>().get();
		auto newNode = addNodeToPosition<T>(pin->getLinkConnectionPointDiwne(), true);
		pin->plug(std::static_pointer_cast<CoreNodeWithPins>(newNode)
		              ->getOutputs()
		              .at(0)
		              .get()); /* \todo JH \todo MH always 0 with type constructor? */
	}

	template <class T>
	auto inline addNodeToPosition(ImVec2 const position = ImVec2(0, 0), bool shiftToLeftByNodeWidth = false)
	{
		// Nodes should be created in the diwne zoom scaling environment (so ImGui calls return scaled values like font
		// size, padding etc.)
		// Hence scaling is applied here if not active, and then restored to its original state at the end of this
		// method
		bool zoomScalingWasActive = diwne.ensureZoomScaling(true);

		auto node = std::make_shared<T>(*this);

		node->setNodePositionDiwne(position);

		if (shiftToLeftByNodeWidth)
		{
			node->drawDiwne(); /* to obtain size */
			node->translateNodePositionDiwne(
			    ImVec2(-node->getNodeRectSizeDiwne().x - I3T::getSize(ESizeVec2::NewNode_positionShift).x,
			           I3T::getSize(ESizeVec2::NewNode_positionShift).y));
		}

		m_workspaceCoreNodes.push_back(node);
		m_takeSnap = true; /* JH maybe better in place where this function is called*/
		detectRotationTransformAndSetFloatMode(node);

		diwne.ensureZoomScaling(zoomScalingWasActive); // Restore zoom scaling to original state

		return node;
	}

	void detectRotationTransformAndSetFloatMode(auto node)
	{
		if (std::dynamic_pointer_cast<Transformation<Core::ETransformType::EulerX>>(node) != nullptr ||
		    std::dynamic_pointer_cast<Transformation<Core::ETransformType::EulerY>>(node) != nullptr ||
		    std::dynamic_pointer_cast<Transformation<Core::ETransformType::EulerZ>>(node) != nullptr ||
		    std::dynamic_pointer_cast<Transformation<Core::ETransformType::Quat>>(node) != nullptr ||
		    std::dynamic_pointer_cast<Transformation<Core::ETransformType::AxisAngle>>(node) != nullptr)
		{
			std::dynamic_pointer_cast<CoreNode>(node).get()->setFloatPopupMode(FloatPopupMode::Angle);
		}
	}

	template <class T>
	auto inline addNodeToPositionOfPopup()
	{
		auto result = addNodeToPosition<T>(screen2diwne(getPopupPosition()));
		return result;
	}

	void manipulatorStartCheck3D();

	void shiftNodesToBegin(std::vector<Ptr<CoreNode>> const& nodesToShift);
	void shiftNodesToEnd(std::vector<Ptr<CoreNode>> const& nodesToShift);
	void shiftInteractingNodeToEnd();

	void processDragAllSelectedNodes();

	void selectAll();
	void invertSelection();
	void zoomToAll();
	void zoomToSelected();
	ImRect getOverNodesRectangleDiwne(std::vector<Ptr<CoreNode>> nodes);
	void zoomToRectangle(ImRect const& rect);

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
	std::vector<Ptr<Model>> getSequenceModels(Ptr<Sequence> seq);

	void processTrackingMove();

	void deleteCallback();
	void copySelectedNodes();
	void pasteSelectedNodes();
	void cutSelectedNodes();
	void duplicateClickedNode();
	void duplicateSelectedNodes();
	void deselectNodes();
	void deselectWorkspaceNode(Ptr<CoreNode> transform);
	bool isNodeLabelBeingEdited();

	// bool allowInteraction();

	std::vector<Ptr<CoreNode>> getSelectedNodesInnerIncluded();
	std::vector<Ptr<CoreNode>> getAllNodesInnerIncluded();

	std::vector<Ptr<CoreNode>> getAllCameras();
	std::vector<Ptr<Model>> getAllModels();
	std::vector<Ptr<CoreNode>> getAllInputFreeSequence();
	std::vector<Ptr<CoreNode>> getAllInputFreeModel();

	bool bypassZoomAction() override;
	bool bypassDragAction() override;
	bool bypassHoldAction() override;
	bool bypassUnholdAction() override;

	bool processZoom() override;

	bool bypassSelectionRectangleAction() override;
	ImVec2 bypassDiwneGetSelectionRectangleStartPosition() override;
	ImVec2 bypassDiwneGetSelectionRectangleSize() override;

	bool m_updateDataItemsWidth; ///< Indicates a change in zoom level this frame
	bool m_reconnectCameraToSequence;

	bool m_trackingFromLeft;
};

template <typename T>
Result<Ptr<T>, Error> WorkspaceDiwne::getNode(Core::ID id) const
{
	Ptr<GuiNode> node{};
	for (const auto& n : getAllNodes())
	{
		if (n->getNodebase()->getId() == id)
		{
			node = n;
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

/// This function takes snapshot of current state.
template <typename T>
auto inline addNodeToNodeEditor(ImVec2 const position = ImVec2(0, 0))
{
	auto result = g_diwne->addNodeToPosition<T>(position);

	App::getModule<StateManager>().takeSnapshot();

	return result;
}

template <typename T>
auto inline addNodeToNodeEditorNoSave(ImVec2 const position = ImVec2(0, 0))
{
	return g_diwne->addNodeToPosition<T>(position);
}

//

bool connectNodesNoSave(Ptr<CoreNode> lhs, Ptr<CoreNode> rhs, int lhsPin, int rhsPin);

bool connectNodes(Ptr<CoreNode> lhs, Ptr<CoreNode> rhs, int lhsPin, int rhsPin);

} // namespace Workspace