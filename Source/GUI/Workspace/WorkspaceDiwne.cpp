/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
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

#include <ranges>

#include "DIWNE/Core/diwne_actions.h"

#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"

using namespace Workspace;

/* ======================================== */
/* ===== W o r k s p a c e  D i w n e ===== */
/* ======================================== */
WorkspaceDiwne::WorkspaceDiwne(DIWNE::SettingsDiwne* settingsDiwne)
    : NodeEditor(settingsDiwne), m_viewportHighlightResolver(this)
{
	setInputAdapter<WorkspaceEditorInputAdapter>();
}

WorkspaceDiwne::~WorkspaceDiwne()
{
	clear();
}

void WorkspaceDiwne::begin(DIWNE::DrawInfo& context)
{
	diwne.mp_settingsDiwne->fontColor = I3T::getColor(EColor::NodeFont);

	// TODO: This system is tbh kinda stupid, why can't the nodes update themselves before drawing?
	//  Why iterate over everything again? What am I missing?
	//  On another node I think data items width could be updated every frame, its not that deep
	//  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	if (m_updateDataItemsWidth)
	{
		m_updateDataItemsWidth = false;
		for (auto& node : getAllCoreNodes())
		{
			node.updateDataItemsWidth();
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
		App::getModule<StateManager>().takeSnapshot();
	}
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
		if (InputManager::isActionTriggered("trackingEscOff", EKeyState::Pressed))
			this->trackingSwitchOff();
		if (InputManager::isActionTriggered("trackingSmoothLeft", EKeyState::Pressed))
			this->trackingSmoothLeft();
		if (InputManager::isActionTriggered("trackingSmoothRight", EKeyState::Pressed))
			this->trackingSmoothRight();
		if (InputManager::isActionTriggered("trackingJaggedLeft", EKeyState::Pressed))
			this->trackingJaggedLeft();
		if (InputManager::isActionTriggered("trackingJaggedRight", EKeyState::Pressed))
			this->trackingJaggedRight();
		if (InputManager::isActionTriggered("trackingModeSwitch", EKeyState::Pressed))
			this->trackingModeSwitch();
		if (InputManager::isActionTriggered("trackingSwitch", EKeyState::Pressed))
			this->trackingSwitch();
		if (InputManager::isActionTriggered("trackingSwitchOn", EKeyState::Pressed))
			this->trackingSwitchOn();
		if (InputManager::isActionTriggered("trackingSwitchOff", EKeyState::Pressed))
			this->trackingSwitchOff();
		if (InputManager::isActionTriggered("toggleNodeWorkspaceVisibility", EKeyState::Pressed))
			this->toggleSelectedNodesVisibility();
		processTrackingMove();
	}
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

void WorkspaceDiwne::processTrackingMove()
{
	if (Core::GraphManager::isTrackingEnabled())
	{
		if (InputManager::isAxisActive("trackingSmoothLeft") != 0)
		{
			this->trackingSmoothLeft();
		}
		if (InputManager::isAxisActive("trackingSmoothRight") != 0)
		{
			this->trackingSmoothRight();
		}
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
		    *Tools::copyNodes(duplicatedNodes, I3T::getUI()->getTheme().get(ESize::Workspace_CopyPasteOffset)));
	}
}

void WorkspaceDiwne::toggleSelectedNodesVisibility()
{
	auto selected = getSelectedNodesInnerIncluded().collectRaw();
	if (selected.empty())
	{
		for (auto node : getAllNodesInnerIncluded())
		{
			node.setRendered(true);
		}
	}
	else
	{
		for (auto node : selected)
		{
			node->setRendered(false);
			node->setSelected(false);
		}
	}
}
void WorkspaceDiwne::trackingSmoothLeft()
{
	if (Core::GraphManager::isTrackingEnabled() && smoothTracking)
	{
		if (timeUntilNextTrack > 0)
		{
			timeUntilNextTrack -= ImGui::GetIO().DeltaTime;
			return;
		}
		timeUntilNextTrack = I3T::getSize(ESize::Workspace_TrackingTimeBetweenTracks);

		float step = I3T::getSize(ESize::Tracking_SmoothScrollSpeed) / tracking->getTrackingProgress().size();
		if (m_trackingFromLeft)
			tracking->setParam(tracking->getParam() - step);
		else
			tracking->setParam(tracking->getParam() + step);
	}
}

void WorkspaceDiwne::trackingSmoothRight()
{
	if (Core::GraphManager::isTrackingEnabled() && smoothTracking)
	{
		if (timeUntilNextTrack > 0)
		{
			timeUntilNextTrack -= ImGui::GetIO().DeltaTime;
			return;
		}
		timeUntilNextTrack = I3T::getSize(ESize::Workspace_TrackingTimeBetweenTracks);

		float step = I3T::getSize(ESize::Tracking_SmoothScrollSpeed) / tracking->getTrackingProgress().size();
		if (m_trackingFromLeft)
			tracking->setParam(tracking->getParam() + step);
		else
			tracking->setParam(tracking->getParam() - step);
	}
}

void WorkspaceDiwne::trackingJaggedLeft()
{
	if (Core::GraphManager::isTrackingEnabled() && !smoothTracking)
	{
		float step = I3T::getSize(ESize::Tracking_JaggedScrollSpeed) / tracking->getTrackingProgress().size();
		if (m_trackingFromLeft)
			tracking->setParam(tracking->getParam() - step);
		else
			tracking->setParam(tracking->getParam() + step);
	}
}

void WorkspaceDiwne::trackingJaggedRight()
{
	if (Core::GraphManager::isTrackingEnabled() && !smoothTracking)
	{
		float step = I3T::getSize(ESize::Tracking_JaggedScrollSpeed) / tracking->getTrackingProgress().size();

		if (m_trackingFromLeft)
			tracking->setParam(tracking->getParam() + step);
		else
			tracking->setParam(tracking->getParam() - step);
	}
}

void WorkspaceDiwne::trackingModeSwitch()
{
	smoothTracking = !smoothTracking;
}

void WorkspaceDiwne::trackingSwitch()
{
	LOG_INFO("TRACKING CALLED");
	if (Core::GraphManager::isTrackingEnabled())
		trackingSwitchOff();
	else
		trackingSwitchOn();
}

void WorkspaceDiwne::trackingSwitchOn(Ptr<Sequence> sequence, bool isRightToLeft)
{
	if (sequence == nullptr)
	{
		auto selectedNodes = getSelectedNodesInnerIncluded();
		for (auto node = selectedNodes.begin(); node != selectedNodes.end(); ++node)
		{
			sequence = std::dynamic_pointer_cast<Sequence>(node.sharedPtr());
			if (sequence)
			{
				const auto models = getSequenceModels(sequence);
				if (models.empty())
					continue;

				trackingInit(sequence, models, isRightToLeft);

				break;
			}
		}
	}
	else
	{
		const auto models = getSequenceModels(sequence);
		if (models.empty())
			return;

		trackingInit(sequence, models, isRightToLeft);
	}
}

void WorkspaceDiwne::trackingInit(Ptr<Sequence> sequence, std::vector<Ptr<Model>> models, bool isRightToLeft)
{
	LOG_INFO("TRACKING ON");

	std::vector<UPtr<Core::IModelProxy>> proxy(models.size());
	std::transform(models.begin(), models.end(), proxy.begin(), [](Ptr<Model> model) {
		return std::make_unique<ModelProxy>(model);
	});

	//	sequence->setTint(I3T::getColor(EColor::TrackingSequenceTint)); // TODO: (DR) This wasn't implemented
	const auto coreSeq = sequence->getNodebase()->as<Core::Sequence>();
	if (isRightToLeft)
	{
		tracking = coreSeq->startTracking(Core::TrackingDirection::RightToLeft, std::move(proxy));
	}
	else
	{
		tracking = coreSeq->startTracking(Core::TrackingDirection::LeftToRight, std::move(proxy));
	}
	m_trackingFromLeft = !isRightToLeft;
}

void WorkspaceDiwne::trackingSwitchOff()
{
	LOG_INFO("TRACKING OFF CALLED");
	if (Core::GraphManager::isTrackingEnabled())
	{
		// TODO: (DR) This seems poorly done, couldn't we use the Core <-> GUI id map?
		auto seq = Tools::findNodeById(getAllCoreNodes().collect(), tracking->getSequence()->getId()).value();
		// TODO: (DR) This was never implemented
		// std::dynamic_pointer_cast<Sequence>(seq)->setTint(ImVec4(1, 1, 1, 1));
		tracking->getSequence()->stopTracking();
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
	                               I3T::getUI()->getTheme().get(ESize::Workspace_CopyPasteOffset));
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
	for (auto node : nodes)
	{
		node->destroy();
	}
}

void WorkspaceDiwne::duplicateSelectedNodes()
{
	LOG_INFO("Duplicating nodes")
	for (auto& node : getAllSelectedCoreNodesWithoutNesting())
	{
		node.setDuplicateNode(true);
	}
	deselectAllNodes();
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

// TODO: Rewrite with filtered iterator
//  This cannot really be done as seq passed here would need a capturing lambda as a predicate
//  I could add a void user pointer or similar to the predicate function to allow passing additional data
std::vector<Ptr<Model>> WorkspaceDiwne::getSequenceModels(Ptr<Sequence> seq)
{
	std::vector<Ptr<Model>> models;
	for (const auto& node : m_nodes)
	{
		Ptr<Model> model = std::dynamic_pointer_cast<Model>(node);
		if (!model)
			continue;
		if (Core::GraphManager::getParent(model->getNodebase()) != seq->getNodebase())
			continue;
		models.push_back(model);
	}
	return models;
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

void WorkspaceDiwne::onZoom()
{
	m_updateDataItemsWidth = true;
}

// TODO: This is inconsistent with the getZoomDelta method!
// bool WorkspaceDiwne::isZoomingDiwne()
//{
//	return InputManager::isAxisActive("scroll") != 0;
//}

bool WorkspaceDiwne::processZoom()
{
	m_updateDataItemsWidth = true;
	return NodeEditor::processZoom();
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

void WorkspaceDiwne::popupContent(DIWNE::DrawInfo& context)
{
	ImGui::PushFont(I3T::getFont(EFont::TutorialHint));
	ImGui::PushStyleColor(ImGuiCol_Text, I3T::getColor(EColor::AddMenuHeader));

	ImGui::Text("Add...");

	ImGui::PopFont();
	ImGui::PopStyleColor();

	ImGui::Separator();
	if (ImGui::BeginMenu("transformation"))
	{
		/* \todo JH  \todo MH can be done by for-cycle if somewhere is list of types
		 * with group (transformation, operator, ...) and label*/
		if (ImGui::MenuItem("free"))
		{
			addNodeToPositionOfPopup<Transformation<Core::ETransformType::Free>>();
		}
		if (ImGui::MenuItem("translate"))
		{
			addNodeToPositionOfPopup<Transformation<Core::ETransformType::Translation>>();
		}

		if (ImGui::BeginMenu("rotate"))
		{

			if (ImGui::MenuItem("eulerAngleX"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::EulerX>>();
			}
			if (ImGui::MenuItem("eulerAngleY"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::EulerY>>();
			}
			if (ImGui::MenuItem("eulerAngleZ"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::EulerZ>>();
			}
			if (ImGui::MenuItem("axisAngle"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::AxisAngle>>();
			}
			if (ImGui::MenuItem("quat"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::Quat>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("scale"))
		{
			addNodeToPositionOfPopup<Transformation<Core::ETransformType::Scale>>();
		}
		//            if (ImGui::MenuItem("non-uniform scale"))
		//            {
		//                addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Scale>>();
		//                m_workspaceCoreNodes.back()->getNodebase()->as<Core::Transformation>()->disableSynergies();
		//            }
		if (ImGui::MenuItem("lookAt"))
		{
			addNodeToPositionOfPopup<Transformation<Core::ETransformType::LookAt>>();
		}
		if (ImGui::BeginMenu("projection"))
		{
			if (ImGui::MenuItem("ortho"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::Ortho>>();
			}
			if (ImGui::MenuItem("perspective"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::Perspective>>();
			}
			if (ImGui::MenuItem("frustum"))
			{
				addNodeToPositionOfPopup<Transformation<Core::ETransformType::Frustum>>();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("operator"))
	{
		if (ImGui::BeginMenu("transformation"))
		{
			if (ImGui::MenuItem("translate"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeTranslation>>();
			}
			if (ImGui::MenuItem("eulerAngleX"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeEulerX>>();
			}
			if (ImGui::MenuItem("eulerAngleY"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeEulerY>>();
			}
			if (ImGui::MenuItem("eulerAngleZ"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeEulerZ>>();
			}
			if (ImGui::MenuItem("rotate"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeAxisAngle>>();
			}
			if (ImGui::MenuItem("scale"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeScale>>();
			}
			if (ImGui::MenuItem("ortho"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeOrtho>>();
			}
			if (ImGui::MenuItem("perspective"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakePerspective>>();
			}
			if (ImGui::MenuItem("frustum"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeFrustum>>();
			}
			if (ImGui::MenuItem("lookAt"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MakeLookAt>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("matrix"))
		{
			if (ImGui::MenuItem("matrix"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToMatrix>>();
			}
			/*
			    if (ImGui::MenuItem("trackball"))
			    {
			        addNodeToPositionOfPopup<WorkspaceTrackball>();
			    }
			     */
			if (ImGui::MenuItem("inversion"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Inversion>>();
			}
			if (ImGui::MenuItem("transpose"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Transpose>>();
			}
			if (ImGui::MenuItem("determinant"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Determinant>>();
			}
			if (ImGui::MenuItem("mat * mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixMulMatrix>>();
			}
			if (ImGui::MenuItem("mat + mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixAddMatrix>>();
			}
			if (ImGui::MenuItem("mat * vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixMulVector>>();
			}
			if (ImGui::MenuItem("vec4 * mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorMulMatrix>>();
			}
			if (ImGui::MenuItem("float * mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixMulFloat>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("vec3"))
		{
			if (ImGui::MenuItem("vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3ToVector3>>();
			}
			if (ImGui::MenuItem("show vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::ShowVector3>>();
			}
			if (ImGui::MenuItem("vec3 x vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3CrossVector3>>();
			}
			if (ImGui::MenuItem("vec3 . vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3DotVector3>>();
			}
			if (ImGui::MenuItem("vec3 + vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3AddVector3>>();
			}
			if (ImGui::MenuItem("vec3 - vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3SubVector3>>();
			}
			if (ImGui::MenuItem("float * vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3MulFloat>>();
			}
			if (ImGui::MenuItem("normalize vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::NormalizeVector3>>();
			}
			if (ImGui::MenuItem("length(vec3)"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3Length>>();
			}
			if (ImGui::MenuItem("mix vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MixVector3>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("vec4"))
		{
			if (ImGui::MenuItem("vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector4ToVector4>>();
			}
			if (ImGui::MenuItem("vec4 . vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorDotVector>>();
			}
			if (ImGui::MenuItem("vec4 + vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorAddVector>>();
			}
			if (ImGui::MenuItem("vec4 - vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorSubVector>>();
			}
			if (ImGui::MenuItem("float * vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorMulFloat>>();
			}
			if (ImGui::MenuItem("normalize vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::NormalizeVector>>();
			}
			if (ImGui::MenuItem("perspective division"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorPerspectiveDivision>>();
			}
			if (ImGui::MenuItem("mix vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MixVector>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("quat"))
		{
			if (ImGui::MenuItem("quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToQuat>>();
			}
			if (ImGui::MenuItem("quat(float, vec3)"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatVecToQuat>>();
			}
			if (ImGui::MenuItem("quat(angle, axis)"))
			{
				// addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::AngleAxisToQuat>>();
				addNodeToPositionOfPopup<AngleAxisToQuatOperator>();
			}
			if (ImGui::MenuItem("quat(vec3, vec3)"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VecVecToQuat>>();
			}
			if (ImGui::MenuItem("quat -> float, vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToFloatVec>>();
			}
			if (ImGui::MenuItem("quat -> angle, axis"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToAngleAxis>>();
			}
			if (ImGui::MenuItem("float * quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatMulQuat>>();
			}
			if (ImGui::MenuItem("quat * quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatMulQuat>>();
			}
			if (ImGui::MenuItem("quat -> euler"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToEuler>>();
			}
			if (ImGui::MenuItem("euler -> quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::EulerToQuat>>();
			}
			if (ImGui::MenuItem("slerp"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatSlerp>>();
			}
			if (ImGui::MenuItem("long way slerp"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatLongWaySlerp>>();
			}
			if (ImGui::MenuItem("lerp"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatLerp>>();
			}
			if (ImGui::MenuItem("quat conjugate"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::ConjQuat>>();
			}
			if (ImGui::MenuItem("qvq*"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatVecConjQuat>>();
			}
			if (ImGui::MenuItem("inverse quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatInverse>>();
			}
			if (ImGui::MenuItem("normalize quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::NormalizeQuat>>();
			}
			if (ImGui::MenuItem("length(quat)"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatLength>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("float"))
		{
			if (ImGui::MenuItem("float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatToFloat>>();
			}
			if (ImGui::MenuItem("clamp float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::ClampFloat>>();
			}
			if (ImGui::MenuItem("float * float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatMulFloat>>();
			}
			if (ImGui::MenuItem("float / float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatDivFloat>>();
			}
			if (ImGui::MenuItem("float + float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatAddFloat>>();
			}
			if (ImGui::MenuItem("float ^ float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatPowFloat>>();
			}
			if (ImGui::MenuItem("mix float"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MixFloat>>();
			}
			if (ImGui::MenuItem("sin & cos"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatSinCos>>();
			}
			if (ImGui::MenuItem("asin & acos"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::ASinACos>>();
			}
			if (ImGui::MenuItem("signum"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Signum>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("conversion"))
		{
			if (ImGui::MenuItem("mat -> TR"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToTR>>();
			}
			if (ImGui::MenuItem("TR -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::TRToMatrix>>();
			}
			if (ImGui::MenuItem("mat -> vecs4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToVectors>>();
			}
			if (ImGui::MenuItem("mat -> quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToQuat>>();
			}
			if (ImGui::MenuItem("mat -> floats"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::MatrixToFloats>>();
			}
			if (ImGui::MenuItem("vecs4 -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorsToMatrix>>();
			}
			if (ImGui::MenuItem("vec4 -> vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorToVector3>>();
			}
			if (ImGui::MenuItem("vec4 -> floats"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::VectorToFloats>>();
			}
			if (ImGui::MenuItem("vecs3 -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vectors3ToMatrix>>();
			}
			if (ImGui::MenuItem("vec3 -> vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3ToVector>>();
			}
			if (ImGui::MenuItem("vec3 -> floats"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::Vector3ToFloats>>();
			}
			if (ImGui::MenuItem("quat -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToMatrix>>();
			}
			if (ImGui::MenuItem("quat -> floats"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::QuatToFloats>>();
			}
			if (ImGui::MenuItem("floats -> mat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatsToMatrix>>();
			}
			if (ImGui::MenuItem("floats -> vec4"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatsToVector>>();
			}
			if (ImGui::MenuItem("floats -> vec3"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatsToVector3>>();
			}
			if (ImGui::MenuItem("floats -> quat"))
			{
				addNodeToPositionOfPopup<Operator<Core::EOperatorType::FloatsToQuat>>();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("sequence"))
	{
		addNodeToPositionOfPopup<Sequence>();
	}
	if (ImGui::MenuItem("camera"))
	{
		addNodeToPositionOfPopup<Camera>();
	}
	if (ImGui::MenuItem("model"))
	{
		addNodeToPositionOfPopup<Model>();
	}
	if (ImGui::MenuItem("pulse"))
	{
		addNodeToPositionOfPopup<Operator<Core::EOperatorType::PulseToPulse>>();
	}
	if (ImGui::MenuItem("screen"))
	{
		addNodeToPositionOfPopup<Screen>();
	}
	if (ImGui::MenuItem("cycle"))
	{
		addNodeToPositionOfPopup<Cycle>();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Delete"))
	{
		if (ImGui::MenuItem("Selected nodes"))
		{
			deleteSelectedNodes();
		}

		if (ImGui::MenuItem("Selected links"))
		{
			for (auto& link : m_links)
			{
				if (link->getSelected())
				{
					link->destroy();
				}
			}
		}

		if (ImGui::MenuItem("All nodes"))
		{
			clear();
		}

		if (ImGui::MenuItem("All links"))
		{
			for (auto& link : m_links)
			{
				link->destroy();
			}
		}
		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Selection"))
	{
		if (ImGui::MenuItem("select all", "Ctrl+A"))
		{
			selectAllNodes();
		}
		if (ImGui::MenuItem("invert", "Ctrl+I"))
		{
			invertSelection();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Zoom"))
	{
		if (ImGui::MenuItem("to all", "Ctrl+Alt+A"))
		{
			zoomToAll();
		}
		if (ImGui::MenuItem("to selection", "Ctrl+Alt+S"))
		{
			zoomToSelected();
		}
		ImGui::EndMenu();
	}
}