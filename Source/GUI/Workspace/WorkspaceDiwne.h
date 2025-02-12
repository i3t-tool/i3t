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

#include "DIWNE/diwne_include.h"
#include "diwne_config.h" // Some DIWNE trickery to use I3T logger

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
extern WorkspaceDiwne* g_diwne;

class WorkspaceDiwne : public DIWNE::NodeEditor
{
	friend void Sequence::moveNodeToWorkspace(Ptr<CoreNode> node); // TODO: Friend stuff would be nice to avoid
public:
	ImDrawListSplitter m_channelSplitter;
	Memento* copiedNodes = nullptr;
	Core::MatrixTracker* tracking;
	float timeUntilNextTrack = 0;
	bool smoothTracking;

	Vp::Entity* m_viewportLastSelectedEntity{nullptr};
	bool m_viewportSelectionChanged{false};

	ViewportHighlightResolver m_viewportHighlightResolver;

	bool m_updateDataItemsWidth;      ///< Indicates a change in zoom level this frame
	bool m_reconnectCameraToSequence; // TODO: Unused probably

	bool m_trackingFromLeft;

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

	// Interaction
	// =============================================================================================================
	void onZoom() override;

	// TODO: Restore functionality
	/**
	 * \brief For a given input, create appropriate constructor box and plug it to this input
	 * \return true if successful (input was not a Ptr)
	 */
	//	bool processCreateAndPlugTypeConstructor();

	// Node/Object management
	// =============================================================================================================
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
		// TODO: Rework <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		//		CorePin* pin = getLastActivePin<CorePin>().get();
		//		auto newNode = addNodeToPosition<T>(pin->getConnectionPoint(), true);
		//		pin->plug(std::static_pointer_cast<CoreNodeWithPins>(newNode)
		//		              ->getOutputs()
		//		              .at(0)
		//		              .get()); /* \todo JH \todo MH always 0 with type constructor? */
	}

	// TODO: Replace with DIWNE::NodeEditor functionality
	template <class T>
	auto inline addNodeToPosition(ImVec2 const position = ImVec2(0, 0), bool shiftToLeftByNodeWidth = false)
	{
		auto node = NodeEditor::createNode<T>(position, shiftToLeftByNodeWidth);

		m_takeSnap = true; /* JH maybe better in place where this function is called*/

		//TODO: This call makes no sense here, there should be another way
		// I'm thinking this should be handled by the respective Transformation subclasses
		detectRotationTransformAndSetFloatMode(node);

		return node;
	}

	// TODO: Replace with DIWNE::NodeEditor functionality
	template <class T>
	auto inline addNodeToPositionOfPopup()
	{
		auto result = addNodeToPosition<T>(canvas().screen2diwne(diwne.getPopupPosition()));
		return result;
	}

	void processInteractions(DIWNE::DrawInfo& context) override;

	void manipulatorStartCheck3D();

	void processDragAllSelectedNodes();

	// TODO: Move to DIWNE::NodeEditor
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
	bool isNodeLabelBeingEdited();

	std::vector<Ptr<CoreNode>> getSelectedNodesInnerIncluded() override;
	std::vector<Ptr<CoreNode>> getAllNodesInnerIncluded() override;

	// TODO: (DR) Move to DIWNE::NodeEditor
	std::vector<Ptr<CoreNode>> getAllCameras();
	std::vector<Ptr<Model>> getAllModels();
	std::vector<Ptr<CoreNode>> getAllInputFreeSequence();
	std::vector<Ptr<CoreNode>> getAllInputFreeModel();

//	bool isZoomingDiwne() override;

	bool processZoom() override;

	// TODO: (DR) Move somewhere else
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