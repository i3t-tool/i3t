/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "WorkspaceDiwne.h"

#include "WorkspaceModule.h"

#include <ranges>

#include "DIWNE/Core/diwne_actions.h"

#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"
#include "GUI/Elements/Dialogs/ImportedModelsDialog.h"
#include "GUI/I3TGui.h"
#include "GUI/Workspace/Nodes/ScriptingNode.h"
#include "Localization/Localization.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"

using namespace Workspace;

WorkspaceDiwne::WorkspaceDiwne(const char* label, DIWNE::SettingsDiwne* settingsDiwne)
    : NodeEditor(label, settingsDiwne), m_viewportHighlightResolver(this)
{
	setInputAdapter<WorkspaceEditorInputAdapter>();
}

WorkspaceDiwne::~WorkspaceDiwne()
{
	clear();
}

void WorkspaceDiwne::begin(DIWNE::DrawInfo& context)
{
	// NOTE: Could data items width be updated every frame? Is it premature op that they don't?
	if (m_updateDataItemsWidth)
	{
		m_updateDataItemsWidth = false;
		for (auto& node : getAllCoreNodes())
		{
			node.queueUpdateDataItemsWidth();
		}
	}
	DIWNE::NodeEditor::begin(context);
}

void WorkspaceDiwne::content(DIWNE::DrawInfo& context)
{
	performLazyDuplication();

	NodeEditor::content(context);

	// TODO: Figure out what to do about all this
	//  I feel like we are on the verge of removing this anyway so its not a priority
	//  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//		/* Cameras To Sequences links */
	//		if (m_cameraLink == nullptr)
	//			m_cameraLink = std::make_shared<DIWNE::Link>(diwne, "TemporalLink");
	//		std::vector<Ptr<CoreNode>> all_cameras = getAllCameras();
	//		if (all_cameras.size() > 0)
	//		{
	//			Ptr<CoreNodeWithPins> cameraWithPins;
	//
	//			std::vector<Ptr<CoreNode>> all_inputFree_Sequence = getAllInputFreeSequence();
	//			if (all_inputFree_Sequence.size() > 0)
	//			{
	//				for (auto const& camera : all_cameras)
	//				{
	//					cameraWithPins = std::dynamic_pointer_cast<CoreNodeWithPins>(camera);
	//					for (auto const& sequence : all_inputFree_Sequence)
	//					{
	//						m_cameraLink->setLinkEndpointsDiwne(
	//						    cameraWithPins->getOutputs()[Core::I3T_CAMERA_OUT_MUL]->getConnectionPoint(),
	//						    std::dynamic_pointer_cast<CoreNodeWithPins>(sequence)
	//						        ->getInputs()[Core::I3T_SEQ_IN_MUL]
	//						        ->getConnectionPoint());
	//						m_cameraLink->drawDiwne(context, DIWNE::DrawMode_JustDraw);
	//					}
	//				}
	//			}
	//
	//			std::vector<Ptr<CoreNode>> all_inputFree_Model = getAllInputFreeModel();
	//			if (all_inputFree_Model.size() > 0)
	//			{
	//				for (auto const& camera : all_cameras)
	//				{
	//					cameraWithPins = std::dynamic_pointer_cast<CoreNodeWithPins>(camera);
	//					for (auto const& model : all_inputFree_Model)
	//					{
	//						m_cameraLink->setLinkEndpointsDiwne(
	//						    cameraWithPins->getOutputs()[Core::I3T_CAMERA_OUT_MUL]->getConnectionPoint(),
	//						    std::dynamic_pointer_cast<CoreNodeWithPins>(model)
	//						        ->getInputs()[0 /*\todo JH  \todo MH Some constant from core here*/]
	//						        ->getConnectionPoint());
	//						m_cameraLink->drawDiwne(context, DIWNE::DrawMode_JustDraw);
	//					}
	//				}
	//			}
	//		}
}

void WorkspaceDiwne::end(DIWNE::DrawInfo& context)
{
	NodeEditor::end(context);
	m_viewportHighlightResolver.resolve();

	DIWNE_DEBUG_CUSTOM(diwne, {
		ImVec2 originPos = ImVec2(getRect().Min.x, getRect().Min.y);
		std::string str;
		if (Core::GraphManager::isTracking())
		{
			str = Core::GraphManager::getTracker()->getDebugString();
		}
		if (!str.empty())
		{
			ImGui::GetForegroundDrawList()->AddText(m_canvas->diwne2screen(originPos), IM_COL32_WHITE, str.c_str());
		}
	});
}

void WorkspaceDiwne::finalize(DIWNE::DrawInfo& context)
{
	// Handle reaction to selection in viewport
	if (m_viewportSelectionChanged)
	{
		bool validSelection = false;
		if (m_viewportLastSelectedEntity != nullptr)
		{
			if (Vp::SceneModel* sceneObject = dynamic_cast<Vp::SceneModel*>(m_viewportLastSelectedEntity))
			{
				auto nodeOpt = Tools::findNodeById(this->getAllCoreNodes().collect(), sceneObject->m_guiNodeId);
				if (nodeOpt)
				{
					Ptr<GuiNode>& node = nodeOpt.value();
					bool selected = node->getSelected();
					this->deselectAllNodes();
					if (!selected)
					{
						if (!node->getSelected())
						{
							node->setSelected(true);
						}
					}
					validSelection = true;
				}
			}
		}
		if (!validSelection)
		{
			this->deselectAllNodes();
		}
	}
	m_viewportSelectionChanged = false;

	// Handle manipulators
	manipulatorStartCheck3D();

	if (m_takeSnap)
	{
		App::getModule<StateManager>().requestSnapshot();
	}
}

void WorkspaceDiwne::onPopup()
{
	NodeEditor::onPopup();
	LOG_EVENT_OPEN_POP_UP(m_labelDiwne);
}

void WorkspaceDiwne::onDrag(DIWNE::DrawInfo& context, bool dragStart, bool dragEnd)
{
	NodeEditor::onDrag(context, dragStart, dragEnd);

	if (dragEnd && !context.state.isActionActive(DIWNE::Actions::selectionRect, this))
	{
		App::getModule<StateManager>().requestRewritableSnapshot();
	}
}

// TODO: This is inconsistent with the getZoomDelta method!
// bool WorkspaceDiwne::isZoomingDiwne()
//{
//	return InputManager::isAxisActive("scroll") != 0;
//}

void WorkspaceDiwne::onZoom()
{
	m_updateDataItemsWidth = true;
	App::getModule<StateManager>().requestRewritableSnapshot();
	NodeEditor::onZoom();
}

void WorkspaceDiwne::processInteractions(DIWNE::DrawInfo& context)
{
	NodeEditor::processInteractions(context);

	if (!context.logicalUpdates && !context.inputConsumed)
	{
		if (InputManager::isActionTriggered("zoomToAll", EKeyState::Pressed))
			this->zoomToAll();
		if (InputManager::isActionTriggered("zoomToSelected", EKeyState::Pressed))
			this->zoomToSelected();
		if (InputManager::isActionTriggered("copy", EKeyState::Pressed))
			this->copySelectedNodes();
		if (InputManager::isActionTriggered("paste", EKeyState::Pressed))
			this->pasteSelectedNodes();
		if (InputManager::isActionTriggered("cut", EKeyState::Pressed))
			this->cutSelectedNodes();
		if (InputManager::isActionTriggered("duplicateSelected", EKeyState::Pressed))
			this->duplicateSelectedNodes();
		if (InputManager::isActionTriggered("toggleNodeWorkspaceVisibility", EKeyState::Pressed))
			this->toggleSelectedNodesVisibility();
	}
}

void WorkspaceDiwne::zoomToAll()
{
	auto allNodes = getAllNodesInnerIncluded();
	zoomToRectangle(getOverNodesRectangleDiwne(allNodes.collect()));
}

void WorkspaceDiwne::zoomToSelected()
{
	auto allSelectedNodes = getSelectedNodesInnerIncluded();
	zoomToRectangle(getOverNodesRectangleDiwne(allSelectedNodes.collect()));
}

void WorkspaceDiwne::performLazyDuplication()
{
	std::vector<Ptr<CoreNode>> duplicatedNodes;
	bool shouldDuplicate = false;
	// TODO: Only core nodes can be duplicated? Why?
	auto allCoreNodes = getAllCoreNodes();
	for (auto node = allCoreNodes.begin(); node != allCoreNodes.end(); ++node)
	{
		if (node->getDuplicateNode())
		{
			duplicatedNodes.push_back(node.sharedPtr());
			node->setDuplicateNode(false);
			shouldDuplicate = true;
		}
	}
	if (shouldDuplicate)
	{
		Tools::pasteNodes(
		    *Tools::copyNodes(duplicatedNodes, I3T::getUI()->getTheme().get(ESizeVec2::Nodes_NewPositionShift).x));
	}
}

void WorkspaceDiwne::toggleSelectedNodesVisibility()
{
	auto selected = getSelectedNodesInnerIncluded().collectRaw();
	if (selected.empty())
	{
		for (auto& node : getAllNodesInnerIncluded())
		{
			node.setRendered(true);
		}
	}
	else
	{
		for (auto& node : selected)
		{
			node->setRendered(false);
			node->setSelected(false);
		}
	}
}

ImRect WorkspaceDiwne::getOverNodesRectangleDiwne(std::vector<Ptr<DIWNE::Node>> nodes)
{
	ImRect rect = ImRect(0, 0, 0, 0);
	if (nodes.size() == 0)
	{
		return rect;
	}
	if (nodes.size() == 1)
	{
		return nodes.at(0)->getRect();
	}

	rect = nodes.at(0)->getRect();
	for (auto& workspaceCoreNode : nodes)
	{
		rect.Add(workspaceCoreNode->getRect());
	}
	return rect;
}

void WorkspaceDiwne::zoomToRectangle(ImRect const& rect)
{
	if (rect.Min.x == 0 && rect.Min.y == 0 && rect.Max.x == 0 && rect.Max.y == 0)
		return;

	ImRect waScreen = canvas().getViewportRectScreen();
	float heightZoom = waScreen.GetHeight() / rect.GetHeight(), widthZoom = waScreen.GetWidth() / rect.GetWidth();
	setZoom(std::min(heightZoom, widthZoom));

	canvas().moveViewport(rect.Min - canvas().getViewportRectDiwne().Min);
}

void WorkspaceDiwne::copySelectedNodes()
{
	LOG_INFO("Copying nodes");
	copiedNodes = Tools::copyNodes(getAllSelectedCoreNodesWithoutNesting().collect(),
	                               I3T::getUI()->getTheme().get(ESizeVec2::Nodes_NewPositionShift).x);
}

void WorkspaceDiwne::pasteSelectedNodes()
{
	LOG_INFO("Pasting nodes");
	if (copiedNodes == nullptr)
		return;
	deselectAllNodes();
	Tools::pasteNodes(*copiedNodes);
}

void WorkspaceDiwne::cutSelectedNodes()
{
	LOG_INFO("Cutting nodes");
	auto nodes = getAllSelectedCoreNodesWithoutNesting().collect();
	copiedNodes = Tools::copyNodes(nodes);

	// Delete copied nodes
	for (auto& node : nodes)
	{
		node->destroy();
	}
}

void WorkspaceDiwne::duplicateSelectedNodes()
{
	LOG_INFO("Duplicating nodes");
	for (auto& node : getAllSelectedCoreNodesWithoutNesting())
	{
		node.setDuplicateNode(true);
	}
	deselectAllNodes();
}

bool WorkspaceDiwne::replaceAndReplugNode(Ptr<CoreNodeWithPins> oldNode, Ptr<CoreNodeWithPins> newNode)
{
	bool success = true;

	std::vector<DIWNE::Pin*> inputConnections;
	std::vector<DIWNE::Pin*> outputConnections;

	// Fetch old connection state (associate external pins with local pin indices)
	fetchConnectionStateForPins(oldNode->getInputs(), inputConnections);
	fetchConnectionStateForPins(oldNode->getOutputs(), outputConnections);

	// Unplug old node
	for (auto& input : oldNode->getInputs())
		input->unplug(false);
	for (auto& input : oldNode->getOutputs())
		input->unplug(false);

	// Replug new node from old state
	success &= restoreConnectionStateForPins(newNode->getInputs(), inputConnections);
	success &= restoreConnectionStateForPins(newNode->getOutputs(), outputConnections);

	return success & replaceNode(oldNode, newNode);
}

void WorkspaceDiwne::fetchConnectionStateForPins(const std::vector<Ptr<CorePin>>& pins,
                                                 std::vector<DIWNE::Pin*>& connections)
{
	for (int i = 0; i < pins.size(); i++)
	{
		auto& pin = pins.at(i);
		if (pin->isPlugged())
		{
			DIWNE::Pin* output = pin->getLink()->getOtherPin(pin.get());
			if (output)
			{
				connections.push_back(output);
				continue;
			}
			else
			{
				LOG_ERROR(
				    "Encountered a pin whose link isn't connected to it while replacing a node! Pin: '{}', Link: '{}'",
				    pin->m_labelDiwne, pin->getLink()->m_labelDiwne);
				assert(false);
			}
		}
		connections.push_back(nullptr);
	}
}

bool WorkspaceDiwne::restoreConnectionStateForPins(const std::vector<Ptr<CorePin>>& pins,
                                                   const std::vector<DIWNE::Pin*>& connections)
{
	bool success = true;
	for (int i = 0; i < pins.size(); i++)
	{
		if (i >= connections.size())
			break;
		auto& pin = pins.at(i);
		if (auto otherPin = connections[i])
		{
			success &= pin->plug(otherPin, false);
		}
	}
	return success;
}

// TODO: Could avoid dynamic_cast using node flags
DIWNE::FilteredNodeRange<Camera> WorkspaceDiwne::getAllCameras()
{
	return DIWNE::FilteredNodeRange<Camera>(
	    [](const DIWNE::Node* node) -> bool {
		    return dynamic_cast<const Camera*>(node);
	    },
	    &m_nodes);
}

// TODO: Could avoid dynamic_cast using node flags
DIWNE::FilteredNodeRange<Model> WorkspaceDiwne::getAllModels()
{
	return DIWNE::FilteredNodeRange<Model>(
	    [](const DIWNE::Node* node) -> bool {
		    return dynamic_cast<const Model*>(node);
	    },
	    &m_nodes);
}

// TODO: Could avoid dynamic_cast using node flags
DIWNE::FilteredNodeRange<Sequence> WorkspaceDiwne::getAllInputFreeSequence()
{
	return DIWNE::FilteredNodeRange<Sequence>(
	    [](const DIWNE::Node* node) -> bool {
		    const Sequence* seq = dynamic_cast<const Sequence*>(node);
		    return seq && !seq->getInputs()[0]->isConnected();
		    /* \todo JH  \todo MH Always 0? */
	    },
	    &m_nodes);
}

// TODO: Could avoid dynamic_cast using node flags
DIWNE::FilteredNodeRange<Model> WorkspaceDiwne::getAllInputFreeModel()
{
	return DIWNE::FilteredNodeRange<Model>(
	    [](const DIWNE::Node* node) -> bool {
		    const Model* model = dynamic_cast<const Model*>(node);
		    return model && !model->getInputs()[0]->isConnected();
		    /* \todo JH  \todo MH Always 0? */
	    },
	    &m_nodes);
}

DIWNE::FilteredRecursiveNodeRange<ScriptingNode> WorkspaceDiwne::getAllScriptingNodes()
{
	return DIWNE::FilteredRecursiveNodeRange<ScriptingNode>(
	    [](const DIWNE::Node* node) -> bool {
		    return dynamic_cast<const Workspace::ScriptingNode*>(node);
	    },
	    &m_nodes);
}

bool WorkspaceEditorInputAdapter::selectAllNodes()
{
	return InputManager::isActionTriggered("selectAll", EKeyState::Pressed);
}
bool WorkspaceEditorInputAdapter::invertSelection()
{
	return InputManager::isActionTriggered("invertSelection", EKeyState::Pressed);
}
bool WorkspaceEditorInputAdapter::deleteSelectedNodes()
{
	return InputManager::isActionTriggered("delete", EKeyState::Pressed);
}

void WorkspaceDiwne::manipulatorStartCheck3D()
{
	if (getNodesSelectionChanged())
	{
		I3T::getViewport()->getManipulators().clearManipulators();

		auto selectedCoreNodes = getAllSelectedCoreNodes();
		for (auto node = selectedCoreNodes.begin(); node != selectedCoreNodes.end(); ++node)
		{
			Ptr<TransformationBase> selected_transformation =
			    std::dynamic_pointer_cast<TransformationBase>(node.sharedPtr());
			if (selected_transformation != nullptr)
			{
				I3T::getViewport()->getManipulators().addManipulator(selected_transformation->getNodebase());
			}
		}
	}
}

void WorkspaceDiwne::startTracking(Sequence* sequence, bool trackFromLeft)
{
	const auto coreSeq = sequence->getNodebase()->as<Core::Sequence>();
	Ptr<Core::Camera> coreCam;
	if (sequence->isCameraSequence())
	{
		assert(dynamic_cast<Camera*>(sequence->getParentObject()) != nullptr);
		coreCam = static_cast<Sequence*>(sequence->getParentObject())->getNodebase()->as<Core::Camera>();
	}
	Core::GraphManager::startTracking(
	    coreSeq, coreCam, trackFromLeft ? Core::TrackingDirection::LeftToRight : Core::TrackingDirection::RightToLeft);
}

void WorkspaceDiwne::stopTracking()
{
	Core::GraphManager::stopTracking();
}

Core::MatrixTracker* WorkspaceDiwne::getTracker() const
{
	return Core::GraphManager::getTracker();
}

bool WorkspaceDiwne::isTracking() const
{
	return Core::GraphManager::isTracking();
}
bool WorkspaceDiwne::isTrackingFromLeft() const
{
	return Core::GraphManager::isTrackingFromLeft();
}

void WorkspaceDiwne::trackingSmoothLeft()
{
	if (isTracking() && smoothTracking)
	{
		if (timeUntilNextTrack > 0)
		{
			timeUntilNextTrack -= ImGui::GetIO().DeltaTime;
			return;
		}
		timeUntilNextTrack = WorkspaceModule::g_settings.tracking_timeBetweenTracks;

		Core::MatrixTracker* tracking = getTracker();
		float step = WorkspaceModule::g_settings.tracking_smoothScrollSpeed / tracking->getTransformCount();
		if (isTrackingFromLeft())
			tracking->setProgress(tracking->getProgress() - step);
		else
			tracking->setProgress(tracking->getProgress() + step);
	}
}

void WorkspaceDiwne::trackingSmoothRight()
{
	if (isTracking() && smoothTracking)
	{
		if (timeUntilNextTrack > 0)
		{
			timeUntilNextTrack -= ImGui::GetIO().DeltaTime;
			return;
		}
		timeUntilNextTrack = WorkspaceModule::g_settings.tracking_timeBetweenTracks;

		Core::MatrixTracker* tracking = getTracker();
		float step = WorkspaceModule::g_settings.tracking_smoothScrollSpeed / tracking->getTransformCount();
		if (isTrackingFromLeft())
			tracking->setProgress(tracking->getProgress() + step);
		else
			tracking->setProgress(tracking->getProgress() - step);
	}
}

void WorkspaceDiwne::trackingJaggedLeft()
{
	Core::MatrixTracker* tracking = getTracker();
	if (tracking->isTracking() && !smoothTracking)
	{
		float step = WorkspaceModule::g_settings.tracking_jaggedScrollSpeed / tracking->getTransformCount();
		if (isTrackingFromLeft())
			tracking->setProgress(tracking->getProgress() - step);
		else
			tracking->setProgress(tracking->getProgress() + step);
	}
}

void WorkspaceDiwne::trackingJaggedRight()
{
	if (isTracking() && !smoothTracking)
	{
		Core::MatrixTracker* tracking = getTracker();
		float step = WorkspaceModule::g_settings.tracking_jaggedScrollSpeed / tracking->getTransformCount();

		if (isTrackingFromLeft())
			tracking->setProgress(tracking->getProgress() + step);
		else
			tracking->setProgress(tracking->getProgress() - step);
	}
}

void WorkspaceDiwne::trackingModeSwitch()
{
	smoothTracking = !smoothTracking;
}

void WorkspaceDiwne::popupContent(DIWNE::DrawInfo& context)
{
	ImGui::PushFont(I3T::getFont(EFont::TutorialHint));
	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getColor(EColor::TextDisabled));

	ImGui::Text(_t("Add..."));

	ImGui::PopFont();
	ImGui::PopStyleColor();

	ImGui::Separator();

	addMenu();
}

void WorkspaceDiwne::addMenu()
{
	if (I3TGui::BeginMenuWithLog(_t("Model")))
	{
		for (const auto& resource : RMI.getDefaultResources(Core::ResourceType::Model))
		{
			if (I3TGui::MenuItemWithLog(resource.alias.c_str(), nullptr, false, true, [&]() {
				    LOG_EVENT_OBJECT_ADDED(resource.alias, m_labelDiwne);
			    }))
			{
				auto model = addNodeToPositionOfPopup<Model>();
				model->m_viewportModel.lock()->setModel(resource.alias);
			}
		}
		std::vector<std::string> importedResources = RMI.getImportedResourceAliases();
		if (!importedResources.empty())
		{
			ImGui::Separator();
			for (const auto& alias : importedResources)
			{
				Core::Mesh* importedModel = RMI.meshByAlias(alias);
				if (importedModel)
				{
					if (I3TGui::MenuItemWithLog(alias.c_str(), nullptr, false, true, [&]() {
						    LOG_EVENT_OBJECT_ADDED(alias, m_labelDiwne);
					    }))
					{
						auto model = addNodeToPositionOfPopup<Model>();
						model->m_viewportModel.lock()->setModel(alias);
					}
				}
				else
				{
					I3TGui::MenuItemWithLog(alias.c_str(), nullptr, false, false, [&]() {
						LOG_EVENT_OBJECT_ADDED(alias, m_labelDiwne);
					});
				}
			}
		}
		ImGui::Separator();
		if (I3TGui::MenuItemWithLog(_t("Import models...")))
		{
			App::getModule<UIModule>().getWindowManager().showUniqueWindow<ImportedModelsDialog>();
		}

		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (I3TGui::MenuItemWithLog("Matrix Sequence"))
	{
		addNodeToPositionOfPopup<Sequence>();
	}
	if (I3TGui::BeginMenuWithLog("Transformation"))
	{
		/* \todo JH  \todo MH can be done by for-cycle if somewhere is list of types
		 * with group (transformation, operator, ...) and label*/
		if (I3TGui::MenuItemWithLog("free"))
		{
			addNodeToPositionOfPopup<Transformation<Core::ETransformType::Free>>();
		}
		if (I3TGui::MenuItemWithLog("translate"))
		{
			addNodeToPositionOfPopup<Transformation<Core::ETransformType::Translation>>();
		}

		if (I3TGui::BeginMenuWithLog("rotate"))
		{

			if (I3TGui::MenuItemWithLog("eulerAngleX"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::EulerX>>();
			}
			if (I3TGui::MenuItemWithLog("eulerAngleY"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::EulerY>>();
			}
			if (I3TGui::MenuItemWithLog("eulerAngleZ"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::EulerZ>>();
			}
			if (I3TGui::MenuItemWithLog("axisAngle"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::AxisAngle>>();
			}
			if (I3TGui::MenuItemWithLog("quat"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::Quat>>();
			}
			ImGui::EndMenu();
		}
		if (I3TGui::MenuItemWithLog("scale"))
		{
			addNodeToPositionOfPopup<Transformation<Core::ETransformType::Scale>>();
		}
		//            if (I3TGui::MenuItemWithLog("non-uniform scale"))
		//            {
		//                addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Scale>>();
		//                m_workspaceCoreNodes.back()->getNodebase()->as<Core::Transformation>()->disableSynergies();
		//            }
		if (I3TGui::MenuItemWithLog("lookAt"))
		{
			addNodeToPositionOfPopup<Transformation<Core::ETransformType::LookAt>>();
		}
		if (I3TGui::BeginMenuWithLog("projection"))
		{
			if (I3TGui::MenuItemWithLog("ortho"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::Ortho>>();
			}
			if (I3TGui::MenuItemWithLog("perspective"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::Perspective>>();
			}
			if (I3TGui::MenuItemWithLog("frustum"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::Frustum>>();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (I3TGui::MenuItemWithLog("Camera"))
	{
		addNodeToPositionOfPopup<Camera>();
	}
	if (I3TGui::MenuItemWithLog("Screen"))
	{
		addNodeToPositionOfPopup<Screen>();
	}

	ImGui::Separator();

	if (I3TGui::BeginMenuWithLog("Animate"))
	{
		if (I3TGui::MenuItemWithLog("Pulse"))
		{
			addNodeToPositionOfPopup<Operator<Core::EOperatorType::PulseToPulse>>();
		}
		if (I3TGui::MenuItemWithLog("Cycle"))
		{
			addNodeToPositionOfPopup<Cycle>();
		}
		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (I3TGui::BeginMenuWithLog("Operator"))
	{
		if (I3TGui::BeginMenuWithLog("transformation"))
		{
			if (I3TGui::MenuItemWithLog("translate"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeTranslation>>();
			}
			if (I3TGui::MenuItemWithLog("eulerAngleX"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeEulerX>>();
			}
			if (I3TGui::MenuItemWithLog("eulerAngleY"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeEulerY>>();
			}
			if (I3TGui::MenuItemWithLog("eulerAngleZ"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeEulerZ>>();
			}
			if (I3TGui::MenuItemWithLog("rotate"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeAxisAngle>>();
			}
			if (I3TGui::MenuItemWithLog("scale"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeScale>>();
			}
			if (I3TGui::MenuItemWithLog("ortho"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeOrtho>>();
			}
			if (I3TGui::MenuItemWithLog("perspective"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakePerspective>>();
			}
			if (I3TGui::MenuItemWithLog("frustum"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeFrustum>>();
			}
			if (I3TGui::MenuItemWithLog("lookAt"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeLookAt>>();
			}
			ImGui::EndMenu();
		}
		if (I3TGui::BeginMenuWithLog("matrix"))
		{
			if (I3TGui::MenuItemWithLog("matrix"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToMatrix>>();
			}
			/*
			    if (I3TGui::MenuItemWithLog("trackball"))
			    {
			        addNodeToPositionOfPopup<WorkspaceTrackball>();
			    }
			     */
			if (I3TGui::MenuItemWithLog("inversion"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Inversion>>();
			}
			if (I3TGui::MenuItemWithLog("transpose"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Transpose>>();
			}
			if (I3TGui::MenuItemWithLog("determinant"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Determinant>>();
			}
			if (I3TGui::MenuItemWithLog("mat * mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixMulMatrix>>();
			}
			if (I3TGui::MenuItemWithLog("mat + mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixAddMatrix>>();
			}
			if (I3TGui::MenuItemWithLog("mat * vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixMulVector>>();
			}
			if (I3TGui::MenuItemWithLog("vec4 * mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorMulMatrix>>();
			}
			if (I3TGui::MenuItemWithLog("float * mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixMulFloat>>();
			}
			ImGui::EndMenu();
		}
		if (I3TGui::BeginMenuWithLog("vec3"))
		{
			if (I3TGui::MenuItemWithLog("vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3ToVector3>>();
			}
			if (I3TGui::MenuItemWithLog("show vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::ShowVector3>>();
			}
			if (I3TGui::MenuItemWithLog("vec3 x vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3CrossVector3>>();
			}
			if (I3TGui::MenuItemWithLog("vec3 . vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3DotVector3>>();
			}
			if (I3TGui::MenuItemWithLog("vec3 + vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3AddVector3>>();
			}
			if (I3TGui::MenuItemWithLog("vec3 - vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3SubVector3>>();
			}
			if (I3TGui::MenuItemWithLog("float * vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3MulFloat>>();
			}
			if (I3TGui::MenuItemWithLog("normalize vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::NormalizeVector3>>();
			}
			if (I3TGui::MenuItemWithLog("length(vec3)"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3Length>>();
			}
			if (I3TGui::MenuItemWithLog("mix vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MixVector3>>();
			}
			ImGui::EndMenu();
		}
		if (I3TGui::BeginMenuWithLog("vec4"))
		{
			if (I3TGui::MenuItemWithLog("vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector4ToVector4>>();
			}
			if (I3TGui::MenuItemWithLog("vec4 . vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorDotVector>>();
			}
			if (I3TGui::MenuItemWithLog("vec4 + vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorAddVector>>();
			}
			if (I3TGui::MenuItemWithLog("vec4 - vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorSubVector>>();
			}
			if (I3TGui::MenuItemWithLog("float * vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorMulFloat>>();
			}
			if (I3TGui::MenuItemWithLog("normalize vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::NormalizeVector>>();
			}
			if (I3TGui::MenuItemWithLog("perspective division"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorPerspectiveDivision>>();
			}
			if (I3TGui::MenuItemWithLog("mix vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MixVector>>();
			}
			ImGui::EndMenu();
		}
		if (I3TGui::BeginMenuWithLog("quat"))
		{
			if (I3TGui::MenuItemWithLog("quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToQuat>>();
			}
			if (I3TGui::MenuItemWithLog("quat(float, vec3)"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatVecToQuat>>();
			}
			if (I3TGui::MenuItemWithLog("quat(angle, axis)"))
			{
				// addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::AngleAxisToQuat>>();
				addNodeToPositionOfPopup<AngleAxisToQuatOperator>();
			}
			if (I3TGui::MenuItemWithLog("quat(vec3, vec3)"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VecVecToQuat>>();
			}
			if (I3TGui::MenuItemWithLog("quat -> float, vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToFloatVec>>();
			}
			if (I3TGui::MenuItemWithLog("quat -> angle, axis"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToAngleAxis>>();
			}
			if (I3TGui::MenuItemWithLog("float * quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatMulQuat>>();
			}
			if (I3TGui::MenuItemWithLog("quat * quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatMulQuat>>();
			}
			if (I3TGui::MenuItemWithLog("quat -> euler"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToEuler>>();
			}
			if (I3TGui::MenuItemWithLog("euler -> quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::EulerToQuat>>();
			}
			if (I3TGui::MenuItemWithLog("slerp"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatSlerp>>();
			}
			if (I3TGui::MenuItemWithLog("long way slerp"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatLongWaySlerp>>();
			}
			if (I3TGui::MenuItemWithLog("lerp"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatLerp>>();
			}
			if (I3TGui::MenuItemWithLog("quat conjugate"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::ConjQuat>>();
			}
			if (I3TGui::MenuItemWithLog("qvq*"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatVecConjQuat>>();
			}
			if (I3TGui::MenuItemWithLog("inverse quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatInverse>>();
			}
			if (I3TGui::MenuItemWithLog("normalize quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::NormalizeQuat>>();
			}
			if (I3TGui::MenuItemWithLog("length(quat)"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatLength>>();
			}
			ImGui::EndMenu();
		}
		if (I3TGui::BeginMenuWithLog("float"))
		{
			if (I3TGui::MenuItemWithLog("float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatToFloat>>();
			}
			if (I3TGui::MenuItemWithLog("clamp float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::ClampFloat>>();
			}
			if (I3TGui::MenuItemWithLog("float * float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatMulFloat>>();
			}
			if (I3TGui::MenuItemWithLog("float / float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatDivFloat>>();
			}
			if (I3TGui::MenuItemWithLog("float + float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatAddFloat>>();
			}
			if (I3TGui::MenuItemWithLog("float ^ float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatPowFloat>>();
			}
			if (I3TGui::MenuItemWithLog("mix float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MixFloat>>();
			}
			if (I3TGui::MenuItemWithLog("sin & cos"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatSinCos>>();
			}
			if (I3TGui::MenuItemWithLog("asin & acos"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::ASinACos>>();
			}
			if (I3TGui::MenuItemWithLog("signum"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Signum>>();
			}
			ImGui::EndMenu();
		}
		if (I3TGui::BeginMenuWithLog("conversion"))
		{
			if (I3TGui::MenuItemWithLog("mat -> TR"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToTR>>();
			}
			if (I3TGui::MenuItemWithLog("TR -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::TRToMatrix>>();
			}
			if (I3TGui::MenuItemWithLog("mat -> vecs4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToVectors>>();
			}
			if (I3TGui::MenuItemWithLog("mat -> quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToQuat>>();
			}
			if (I3TGui::MenuItemWithLog("mat -> floats"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToFloats>>();
			}
			if (I3TGui::MenuItemWithLog("vecs4 -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorsToMatrix>>();
			}
			if (I3TGui::MenuItemWithLog("vec4 -> vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorToVector3>>();
			}
			if (I3TGui::MenuItemWithLog("vec4 -> floats"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorToFloats>>();
			}
			if (I3TGui::MenuItemWithLog("vecs3 -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vectors3ToMatrix>>();
			}
			if (I3TGui::MenuItemWithLog("vec3 -> vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3ToVector>>();
			}
			if (I3TGui::MenuItemWithLog("vec3 -> floats"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3ToFloats>>();
			}
			if (I3TGui::MenuItemWithLog("quat -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToMatrix>>();
			}
			if (I3TGui::MenuItemWithLog("quat -> floats"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToFloats>>();
			}
			if (I3TGui::MenuItemWithLog("floats -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatsToMatrix>>();
			}
			if (I3TGui::MenuItemWithLog("floats -> vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatsToVector>>();
			}
			if (I3TGui::MenuItemWithLog("floats -> vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatsToVector3>>();
			}
			if (I3TGui::MenuItemWithLog("floats -> quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatsToQuat>>();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (I3TGui::MenuItemWithLog("Scripting node"))
	{
		addNodeToPositionOfPopup<ScriptingNode>();
	}
}
