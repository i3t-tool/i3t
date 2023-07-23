/**
 * \file	  GUI/Elements/Windows/WorkspaceWindow.h
 * \authors Sofie Sasorina, Jaroslav Holecek
 *
 *  A Workspace window elements declaration.
 */
#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

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

#include "Core/API.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"
#include "Core/Result.h"

#include "GUI/Elements/IWindow.h"
#include "GUI/Elements/Nodes/Tools.h"
#include "GUI/Elements/Nodes/WorkspaceSingleInclude.h"
#include "GUI/ViewportHighlightResolver.h"
#include "Logger/Logger.h"
#include "State/StateManager.h"

typedef std::vector<Ptr<WorkspaceNodeWithCoreData>>::iterator coreNodeIter;

enum WorkspaceDiwneAction
{
	None,
	CreateAndPlugTypeConstructor,
	NOTunselectAllNodes
};

extern class WorkspaceDiwne* g_workspaceDiwne;

class WorkspaceDiwne : public DIWNE::Diwne
{
	friend void WorkspaceSequence::moveNodeToWorkspace(Ptr<WorkspaceNodeWithCoreData> node);

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

	void popupContent();

	bool beforeBegin();
	bool beforeContent();
	bool content();
	bool afterContent();
	bool afterEnd();
	bool finalize();

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
	 * \note Nodes inside Sequentions are not directly in this vector (they are in Sequence)
	 **/
	std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceCoreNodes;

	/**
	 * A map connecting Core node id's with equivalent gui nodes.
	 * Contains raw pointers as values so it can be populated in constructors.
	 */
	std::unordered_map<Core::ID, WorkspaceNodeWithCoreData*> m_coreIdMap;

	const std::vector<Ptr<WorkspaceNodeWithCoreData>>& getAllNodes() const
	{
		return m_workspaceCoreNodes;
	};

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedNodes();

	std::vector<WorkspaceCoreLink*> m_linksToDraw;
	std::vector<WorkspaceCoreLink> m_linksCameraToSequence;

	bool processCreateAndPlugTypeConstructor();

	/**
	 * O(N) where N is workspace nodes count.
	 *
	 * @tparam T
	 * @param id
	 * @return
	 */
	template <typename T> Result<Ptr<T>, Error> getNode(Core::ID id) const;

	template <typename T> void addTypeConstructorNode()
	{
		WorkspaceCoreInputPin* pin = getLastActivePin<WorkspaceCoreInputPin>().get();
		auto newNode = addNodeToPosition<T>(pin->getLinkConnectionPointDiwne(), true);
		pin->plug(std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(newNode)
		              ->getOutputs()
		              .at(0)
		              .get()); /* \todo JH \todo MH always 0 with type constructor? */
	}

	template <class T>
	auto inline addNodeToPosition(ImVec2 const position = ImVec2(0, 0), bool shiftToLeftByNodeWidth = false)
	{
		auto node = std::make_shared<T>(*this);

		node->setNodePositionDiwne(position);

		if (shiftToLeftByNodeWidth)
		{
			node->drawDiwne(); /* for obtain size */
			node->translateNodePositionDiwne(
			    ImVec2(-node->getNodeRectSizeDiwne().x - I3T::getSize(ESizeVec2::NewNode_positionShift).x,
			           I3T::getSize(ESizeVec2::NewNode_positionShift).y));
		}

		m_workspaceCoreNodes.push_back(node);
		m_takeSnap = true; /* JH maybe better in place where this function is called*/
		detectRotationTransformAndSetFloatMode(node);
		return node;
	}

	void detectRotationTransformAndSetFloatMode(auto node)
	{
		if (std::dynamic_pointer_cast<WorkspaceTransformation_s<Core::ETransformType::EulerX>>(node) != nullptr ||
		    std::dynamic_pointer_cast<WorkspaceTransformation_s<Core::ETransformType::EulerY>>(node) != nullptr ||
		    std::dynamic_pointer_cast<WorkspaceTransformation_s<Core::ETransformType::EulerZ>>(node) != nullptr ||
		    std::dynamic_pointer_cast<WorkspaceTransformation_s<Core::ETransformType::Quat>>(node) != nullptr ||
		    std::dynamic_pointer_cast<WorkspaceTransformation_s<Core::ETransformType::AxisAngle>>(node) != nullptr)
		{
			std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(node).get()->setFloatPopupMode(FloatPopupMode::Angle);
		}
	}

	template <class T> auto inline addNodeToPositionOfPopup()
	{
		auto result = addNodeToPosition<T>(screen2diwne(getPopupPosition()));
		return result;
	}

	void manipulatorStartCheck3D();

	void shiftNodesToBegin(std::vector<Ptr<WorkspaceNodeWithCoreData>> const& nodesToShift);
	void shiftNodesToEnd(std::vector<Ptr<WorkspaceNodeWithCoreData>> const& nodesToShift);
	void shiftInteractingNodeToEnd();

	void processDragAllSelectedNodes();

	void selectAll();
	void invertSelection();
	void zoomToAll();
	void zoomToSelected();
	ImRect getOverNodesRectangleDiwne(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodes);
	void zoomToRectangle(ImRect const& rect);

	void trackingSmoothLeft();
	void trackingSmoothRight();
	void trackingJaggedLeft();
	void trackingJaggedRight();
	void trackingModeSwitch();
	void trackingSwitch();
	void trackingSwitchOn(Ptr<WorkspaceSequence> sequence = nullptr);
	void trackingSwitchOff();
	void toggleSelectedNodesVisibility();
	Ptr<WorkspaceModel> getSequenceModel(Ptr<WorkspaceSequence> seq);
	std::vector<Ptr<WorkspaceModel>> getSequenceModels(Ptr<WorkspaceSequence> seq);

	void processTrackingMove();

	void deleteCallback();
	void copySelectedNodes();
	void pasteSelectedNodes();
	void cutSelectedNodes();
	void duplicateClickedNode();
	void duplicateSelectedNodes();
	void deselectNodes();
	void deselectWorkspaceNode(Ptr<WorkspaceNodeWithCoreData> transform);
	bool isNodeLabelBeingEdited();

	// bool allowInteraction();

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getSelectedNodesInnerIncluded();
	std::vector<Ptr<WorkspaceNodeWithCoreData>> getAllNodesInnerIncluded();

	std::vector<Ptr<WorkspaceNodeWithCoreData>> getAllCameras();
	std::vector<Ptr<WorkspaceModel>> getAllModels();
	std::vector<Ptr<WorkspaceNodeWithCoreData>> getAllInputFreeSequence();
	std::vector<Ptr<WorkspaceNodeWithCoreData>> getAllInputFreeModel();

	bool bypassZoomAction();
	bool bypassDragAction();
	bool bypassHoldAction();
	bool bypassUnholdAction();

	bool processZoom() override;

	bool bypassSelectionRectangleAction() override;
	ImVec2 bypassDiwneGetSelectionRectangleStartPosition() override;
	ImVec2 bypassDiwneGetSelectionRectangleSize() override;

	bool m_resizeDataWidth;
	bool m_reconnectCameraToSequence;

	bool m_trackingFromLeft;
};

/*! \class class for Workspace window object
        \brief Store everything what Workspace window need
*/
class WorkspaceWindow : public IWindow, public IStateful
{
public:
	I3T_WINDOW(WorkspaceWindow)

	explicit WorkspaceWindow(bool show);
	~WorkspaceWindow() override;

	//

private:
	Memento getState() override;

	void setState(const Memento& memento, bool newSceneLoaded) override;

	void clear() override;

	void initDiwneFromTheme();
	//

public:
	WorkspaceDiwne& getNodeEditor();

	Application& m_wholeApplication;

	void render();

private:
	void showEditMenu();
};

/* >>>>> STATIC FUNCTIONS <<<<< */

/// This function takes snapshot of current state.
template <typename T> auto inline addNodeToNodeEditor(ImVec2 const position = ImVec2(0, 0))
{
	auto result = g_workspaceDiwne->addNodeToPosition<T>(position);

	App::getModule<StateManager>().takeSnapshot();

	return result;
}

template <typename T> auto inline addNodeToNodeEditorNoSave(ImVec2 const position = ImVec2(0, 0))
{
	return g_workspaceDiwne->addNodeToPosition<T>(position);
}

//

bool connectNodesNoSave(Ptr<WorkspaceNodeWithCoreData> lhs, Ptr<WorkspaceNodeWithCoreData> rhs, int lhsPin, int rhsPin);

inline bool connectNodes(Ptr<WorkspaceNodeWithCoreData> lhs, Ptr<WorkspaceNodeWithCoreData> rhs, int lhsPin, int rhsPin)
{
	const auto result = connectNodesNoSave(lhs, rhs, lhsPin, rhsPin);
	if (result)
	{
		App::getModule<StateManager>().takeSnapshot();
	}

	return result;
}

//

template <typename T> Result<Ptr<T>, Error> WorkspaceDiwne::getNode(Core::ID id) const
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
