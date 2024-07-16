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

#include "WorkspaceWindow.h"

#include <ranges>

#include "GUI/Elements/Nodes/Tools.h"
#include "GUI/WindowManager.h"
#include "Logger/Logger.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"

#ifdef DIWNE_DEBUG
bool s_diwneDebug_on;
#endif // DIWNE_DEBUG

static DIWNE::SettingsDiwne settingsDiwne;

WorkspaceDiwne* g_workspaceDiwne = nullptr;

// std::vector<Ptr<WorkspaceNodeWithCoreData>>
// WorkspaceWindow::m_workspaceDiwne::m_workspaceCoreNodes;

// using namespace Core;

/* ======================================== */
/* ===== W o r k s p a c e  D i w n e ===== */
/* ======================================== */
WorkspaceDiwne::WorkspaceDiwne(DIWNE::SettingsDiwne* settingsDiwne)
    : Diwne(settingsDiwne), m_workspaceDiwneAction(WorkspaceDiwneAction::None),
      m_workspaceDiwneActionPreviousFrame(WorkspaceDiwneAction::None), m_updateDataItemsWidth(false),
      m_trackingFromLeft(false), tracking(nullptr), smoothTracking(true), m_viewportHighlightResolver(this)

{}

WorkspaceDiwne::~WorkspaceDiwne()
{
	clear();
}

void WorkspaceDiwne::clear()
{
	Diwne::clear();
	m_workspaceCoreNodes.clear();
}

void WorkspaceDiwne::selectAll()
{
	for (auto&& workspaceCoreNode : getAllNodesInnerIncluded())
	{
		if (workspaceCoreNode->getIsLabelBeingEdited())
			return;
	}
	for (auto&& workspaceCoreNode : getAllNodesInnerIncluded())
	{
		if (!workspaceCoreNode->getSelected())
		{
			workspaceCoreNode->setSelected(true);
		}
	}
}

void WorkspaceDiwne::invertSelection()
{
	for (auto&& workspaceCoreNode : getAllNodesInnerIncluded())
	{
		bool selected = !workspaceCoreNode->getSelected();
		if (workspaceCoreNode->getSelected() != selected)
		{
			workspaceCoreNode->setSelected(selected);
		}
	}
}

void WorkspaceDiwne::zoomToAll()
{
	zoomToRectangle(getOverNodesRectangleDiwne(getAllNodesInnerIncluded()));
}

void WorkspaceDiwne::zoomToSelected()
{
	zoomToRectangle(getOverNodesRectangleDiwne(getSelectedNodesInnerIncluded()));
}

void WorkspaceDiwne::toggleSelectedNodesVisibility()
{
	auto selected = getSelectedNodesInnerIncluded();
	if (selected.empty())
	{
		for (auto node : getAllNodesInnerIncluded())
		{
			node->setRender(true);
		}
	}
	else
	{
		for (auto node : selected)
		{
			node->setRender(false);
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

void WorkspaceDiwne::trackingSwitchOn(Ptr<WorkspaceSequence> sequence, bool isRightToLeft)
{
	if (sequence == nullptr)
	{
		for (auto&& node : getSelectedNodesInnerIncluded())
		{
			sequence = std::dynamic_pointer_cast<WorkspaceSequence>(node);
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

void WorkspaceDiwne::trackingInit(Ptr<WorkspaceSequence> sequence, std::vector<Ptr<WorkspaceModel>> models,
                                  bool isRightToLeft)
{
	LOG_INFO("TRACKING ON");

	std::vector<UPtr<Core::IModelProxy>> proxy(models.size());
	std::transform(models.begin(), models.end(), proxy.begin(), [](Ptr<WorkspaceModel> model) {
		return std::make_unique<WorkspaceModelProxy>(model);
	});

	sequence->setTint(I3T::getColor(EColor::TrackingSequenceTint));
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
		auto seq = findNodeById(getAllNodesInnerIncluded(), tracking->getSequence()->getId()).value();
		std::dynamic_pointer_cast<WorkspaceSequence>(seq)->setTint(ImVec4(1, 1, 1, 1));
		tracking->getSequence()->stopTracking();
	}
}

ImRect WorkspaceDiwne::getOverNodesRectangleDiwne(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodes)
{
	ImRect rect = ImRect(0, 0, 0, 0);
	if (nodes.size() == 0)
	{
		return rect;
	}
	if (nodes.size() == 1)
	{
		return nodes.at(0)->getRectDiwne();
	}

	rect = nodes.at(0)->getRectDiwne();
	for (auto&& workspaceCoreNode : nodes)
	{
		rect.Add(workspaceCoreNode->getRectDiwne());
	}
	return rect;
}

void WorkspaceDiwne::zoomToRectangle(ImRect const& rect)
{
	if (rect.Min.x == 0 && rect.Min.y == 0 && rect.Max.x == 0 && rect.Max.y == 0)
		return;

	ImRect waScreen = getWorkAreaScreen();
	float heightZoom = waScreen.GetHeight() / rect.GetHeight(), widthZoom = waScreen.GetWidth() / rect.GetWidth();
	setWorkAreaZoom(std::min(heightZoom, widthZoom));

	translateWorkAreaDiwne(rect.Min - getWorkAreaDiwne().Min);
}

void WorkspaceDiwne::deleteCallback()
{
	LOG_DEBUG("Deleting");
	if (isNodeLabelBeingEdited())
		return;

	for (auto&& node : getSelectedNodesInnerIncluded())
	{
		node->deleteActionDiwne();

		Ptr<WorkspaceTransformation> trn = std::dynamic_pointer_cast<WorkspaceTransformation>(node);
		if (trn != nullptr)
		{
			trn->deleteActionDiwne();
		}
	}
}

bool WorkspaceDiwne::isNodeLabelBeingEdited()
{
	for (auto&& workspaceCoreNode : getAllNodesInnerIncluded())
	{
		if (workspaceCoreNode->getIsLabelBeingEdited())
			return true;
	}
	return false;
}

void WorkspaceDiwne::copySelectedNodes()
{
	LOG_INFO("Copying nodes");
	if (isNodeLabelBeingEdited())
		return;
	// Preventing double duplication of selected transformations in a sequence
	for (auto node : getSelectedNodesInnerIncluded())
	{
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
		if (seq)
		{
			for (auto transform : seq->getInnerWorkspaceNodes())
			{
				deselectWorkspaceNode(transform);
			}
		}
		else
		{
			Ptr<WorkspaceCamera> cam = std::dynamic_pointer_cast<WorkspaceCamera>(node);
			if (cam)
			{
				Ptr<WorkspaceSequence> proj = cam->getProjection();
				Ptr<WorkspaceSequence> view = cam->getView();
				for (auto transform : proj->getInnerWorkspaceNodes())
				{
					deselectWorkspaceNode(transform);
				}
				for (auto transform : view->getInnerWorkspaceNodes())
				{
					deselectWorkspaceNode(transform);
				}
			}
		}
	}
	copiedNodes =
	    copyNodes(getSelectedNodesInnerIncluded(), I3T::getUI()->getTheme().get(ESize::Workspace_CopyPasteOffset));
}

void WorkspaceDiwne::deselectWorkspaceNode(Ptr<WorkspaceNodeWithCoreData> transform)
{
	if (transform->getSelected())
	{
		transform->setSelected(false);
	}
}

void WorkspaceDiwne::pasteSelectedNodes()
{
	LOG_INFO("Pasting nodes");
	if (copiedNodes == nullptr)
		return;
	if (isNodeLabelBeingEdited())
		return;
	deselectNodes();
	pasteNodes(*copiedNodes);
}

void WorkspaceDiwne::cutSelectedNodes()
{
	// Prevent double duplication of inner nodes
	if (isNodeLabelBeingEdited())
		return;
	for (auto node : getSelectedNodesInnerIncluded())
	{
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
		if (seq)
		{
			for (auto transform : seq->getInnerWorkspaceNodes())
			{
				deselectWorkspaceNode(transform);
			}
		}
	}

	auto nodes = getSelectedNodesInnerIncluded();
	copiedNodes = copyNodes(nodes);

	// Delete copied nodes
	for (auto node : nodes)
	{
		node->deleteActionDiwne();
	}
}

void WorkspaceDiwne::duplicateClickedNode()
{
	LOG_INFO("Duplicating")
	// Preventing double duplication of selected transformations in a sequence
	for (auto node : getSelectedNodesInnerIncluded())
	{
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
		if (seq)
		{
			for (auto transform : seq->getInnerWorkspaceNodes())
			{
				deselectWorkspaceNode(transform);
			}
		}
	}

	// Duplicating either focused node or all selected nodes
	auto selectedNodes = getSelectedNodesInnerIncluded();

	for (const Ptr<GuiNode>& node : getAllNodesInnerIncluded())
	{
		if (node->m_focusedForInteraction)
		{
			if (node->m_selected)
			{
				deselectNodes();
				// copy and paste to ensure connections
				for (auto node : selectedNodes)
				{
					node->setDuplicateNode(true);
				}
				// pasteNodes(copyNodes(selectedNodes, 5));
			}
			else
			{
				deselectNodes();
				node->setDuplicateNode(true);
				// duplicateNode(node, 5);
			}
		}
	}
}

void WorkspaceDiwne::duplicateSelectedNodes()
{
	LOG_INFO("Duplicating")
	// Preventing double duplication of selected transformations in a sequence
	for (auto node : getSelectedNodesInnerIncluded())
	{
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
		if (seq)
		{
			for (auto transform : seq->getInnerWorkspaceNodes())
			{
				deselectWorkspaceNode(transform);
			}
		}
	}

	auto selectedNodes = getSelectedNodesInnerIncluded();

	// copy and paste to ensure connections
	pasteNodes(*copyNodes(selectedNodes, I3T::getUI()->getTheme().get(ESize::Workspace_CopyPasteOffset)));

	for (auto node : selectedNodes)
	{
		deselectWorkspaceNode(node);
	}
}

void WorkspaceDiwne::deselectNodes()
{
	for (auto node : getAllNodesInnerIncluded())
	{
		deselectWorkspaceNode(node);
	}
}

void WorkspaceDiwne::popupContent()
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
			addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Free>>();
		}
		if (ImGui::MenuItem("translate"))
		{
			addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Translation>>();
		}

		if (ImGui::BeginMenu("rotate"))
		{

			if (ImGui::MenuItem("eulerAngleX"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::EulerX>>();
			}
			if (ImGui::MenuItem("eulerAngleY"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::EulerY>>();
			}
			if (ImGui::MenuItem("eulerAngleZ"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::EulerZ>>();
			}
			if (ImGui::MenuItem("axisAngle"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::AxisAngle>>();
			}
			if (ImGui::MenuItem("quat"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Quat>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("scale"))
		{
			addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Scale>>();
		}
		//            if (ImGui::MenuItem("non-uniform scale"))
		//            {
		//                addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Scale>>();
		//                m_workspaceCoreNodes.back()->getNodebase()->as<Core::Transformation>()->disableSynergies();
		//            }
		if (ImGui::MenuItem("lookAt"))
		{
			addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::LookAt>>();
		}
		if (ImGui::BeginMenu("projection"))
		{
			if (ImGui::MenuItem("ortho"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Ortho>>();
			}
			if (ImGui::MenuItem("perspective"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Perspective>>();
			}
			if (ImGui::MenuItem("frustum"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<Core::ETransformType::Frustum>>();
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
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeTranslation>>();
			}
			if (ImGui::MenuItem("eulerAngleX"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeEulerX>>();
			}
			if (ImGui::MenuItem("eulerAngleY"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeEulerY>>();
			}
			if (ImGui::MenuItem("eulerAngleZ"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeEulerZ>>();
			}
			if (ImGui::MenuItem("rotate"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeAxisAngle>>();
			}
			if (ImGui::MenuItem("scale"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeScale>>();
			}
			if (ImGui::MenuItem("ortho"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeOrtho>>();
			}
			if (ImGui::MenuItem("perspective"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakePerspective>>();
			}
			if (ImGui::MenuItem("frustum"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeFrustum>>();
			}
			if (ImGui::MenuItem("lookAt"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MakeLookAt>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("matrix"))
		{
			if (ImGui::MenuItem("matrix"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixToMatrix>>();
			}
			/*
			    if (ImGui::MenuItem("trackball"))
			    {
			        addNodeToPositionOfPopup<WorkspaceTrackball>();
			    }
			     */
			if (ImGui::MenuItem("inversion"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Inversion>>();
			}
			if (ImGui::MenuItem("transpose"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Transpose>>();
			}
			if (ImGui::MenuItem("determinant"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Determinant>>();
			}
			if (ImGui::MenuItem("mat * mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixMulMatrix>>();
			}
			if (ImGui::MenuItem("mat + mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixAddMatrix>>();
			}
			if (ImGui::MenuItem("mat * vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixMulVector>>();
			}
			if (ImGui::MenuItem("vec4 * mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorMulMatrix>>();
			}
			if (ImGui::MenuItem("float * mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixMulFloat>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("vec3"))
		{
			if (ImGui::MenuItem("vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3ToVector3>>();
			}
			if (ImGui::MenuItem("show vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::ShowVector3>>();
			}
			if (ImGui::MenuItem("vec3 x vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3CrossVector3>>();
			}
			if (ImGui::MenuItem("vec3 . vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3DotVector3>>();
			}
			if (ImGui::MenuItem("vec3 + vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3AddVector3>>();
			}
			if (ImGui::MenuItem("vec3 - vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3SubVector3>>();
			}
			if (ImGui::MenuItem("float * vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3MulFloat>>();
			}
			if (ImGui::MenuItem("normalize vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::NormalizeVector3>>();
			}
			if (ImGui::MenuItem("length(vec3)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3Length>>();
			}
			if (ImGui::MenuItem("mix vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MixVector3>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("vec4"))
		{
			if (ImGui::MenuItem("vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector4ToVector4>>();
			}
			if (ImGui::MenuItem("vec4 . vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorDotVector>>();
			}
			if (ImGui::MenuItem("vec4 + vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorAddVector>>();
			}
			if (ImGui::MenuItem("vec4 - vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorSubVector>>();
			}
			if (ImGui::MenuItem("float * vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorMulFloat>>();
			}
			if (ImGui::MenuItem("normalize vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::NormalizeVector>>();
			}
			if (ImGui::MenuItem("perspective division"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorPerspectiveDivision>>();
			}
			if (ImGui::MenuItem("mix vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MixVector>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("quat"))
		{
			if (ImGui::MenuItem("quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatToQuat>>();
			}
			if (ImGui::MenuItem("quat(float, vec3)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatVecToQuat>>();
			}
			if (ImGui::MenuItem("quat(angle, axis)"))
			{
				// addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::AngleAxisToQuat>>();
				addNodeToPositionOfPopup<WorkspaceAngleAxisToQuat>();
			}
			if (ImGui::MenuItem("quat(vec3, vec3)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VecVecToQuat>>();
			}
			if (ImGui::MenuItem("quat -> float, vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatToFloatVec>>();
			}
			if (ImGui::MenuItem("quat -> angle, axis"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatToAngleAxis>>();
			}
			if (ImGui::MenuItem("float * quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatMulQuat>>();
			}
			if (ImGui::MenuItem("quat * quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatMulQuat>>();
			}
			if (ImGui::MenuItem("quat -> euler"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatToEuler>>();
			}
			if (ImGui::MenuItem("euler -> quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::EulerToQuat>>();
			}
			if (ImGui::MenuItem("slerp"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatSlerp>>();
			}
			if (ImGui::MenuItem("long way slerp"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatLongWaySlerp>>();
			}
			if (ImGui::MenuItem("lerp"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatLerp>>();
			}
			if (ImGui::MenuItem("quat conjugate"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::ConjQuat>>();
			}
			if (ImGui::MenuItem("qvq*"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatVecConjQuat>>();
			}
			if (ImGui::MenuItem("inverse quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatInverse>>();
			}
			if (ImGui::MenuItem("normalize quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::NormalizeQuat>>();
			}
			if (ImGui::MenuItem("length(quat)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatLength>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("float"))
		{
			if (ImGui::MenuItem("float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatToFloat>>();
			}
			if (ImGui::MenuItem("clamp float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::ClampFloat>>();
			}
			if (ImGui::MenuItem("float * float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatMulFloat>>();
			}
			if (ImGui::MenuItem("float / float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatDivFloat>>();
			}
			if (ImGui::MenuItem("float + float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatAddFloat>>();
			}
			if (ImGui::MenuItem("float ^ float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatPowFloat>>();
			}
			if (ImGui::MenuItem("mix float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MixFloat>>();
			}
			if (ImGui::MenuItem("sin & cos"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatSinCos>>();
			}
			if (ImGui::MenuItem("asin & acos"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::ASinACos>>();
			}
			if (ImGui::MenuItem("signum"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Signum>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("conversion"))
		{
			if (ImGui::MenuItem("mat -> TR"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixToTR>>();
			}
			if (ImGui::MenuItem("TR -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::TRToMatrix>>();
			}
			if (ImGui::MenuItem("mat -> vecs4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixToVectors>>();
			}
			if (ImGui::MenuItem("mat -> quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixToQuat>>();
			}
			if (ImGui::MenuItem("mat -> floats"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::MatrixToFloats>>();
			}
			if (ImGui::MenuItem("vecs4 -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorsToMatrix>>();
			}
			if (ImGui::MenuItem("vec4 -> vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorToVector3>>();
			}
			if (ImGui::MenuItem("vec4 -> floats"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::VectorToFloats>>();
			}
			if (ImGui::MenuItem("vecs3 -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vectors3ToMatrix>>();
			}
			if (ImGui::MenuItem("vec3 -> vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3ToVector>>();
			}
			if (ImGui::MenuItem("vec3 -> floats"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::Vector3ToFloats>>();
			}
			if (ImGui::MenuItem("quat -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatToMatrix>>();
			}
			if (ImGui::MenuItem("quat -> floats"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::QuatToFloats>>();
			}
			if (ImGui::MenuItem("floats -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatsToMatrix>>();
			}
			if (ImGui::MenuItem("floats -> vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatsToVector>>();
			}
			if (ImGui::MenuItem("floats -> vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatsToVector3>>();
			}
			if (ImGui::MenuItem("floats -> quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::FloatsToQuat>>();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("sequence"))
	{
		addNodeToPositionOfPopup<WorkspaceSequence>();
	}
	if (ImGui::MenuItem("camera"))
	{
		addNodeToPositionOfPopup<WorkspaceCamera>();
	}
	if (ImGui::MenuItem("model"))
	{
		addNodeToPositionOfPopup<WorkspaceModel>();
	}
	if (ImGui::MenuItem("pulse"))
	{
		addNodeToPositionOfPopup<WorkspaceOperator<Core::EOperatorType::PulseToPulse>>();
	}
	if (ImGui::MenuItem("screen"))
	{
		addNodeToPositionOfPopup<WorkspaceScreen>();
	}
	if (ImGui::MenuItem("cycle"))
	{
		addNodeToPositionOfPopup<WorkspaceCycle>();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Delete"))
	{
		if (ImGui::MenuItem("Selected nodes"))
		{
			for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
			{
				if (workspaceCoreNode->getSelected())
				{
					workspaceCoreNode->deleteActionDiwne();
				}
				else
				{
					Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(workspaceCoreNode);
					if (seq != nullptr)
					{
						for (auto&& nodeInSequence : seq->getInnerWorkspaceNodes())
						{
							if (nodeInSequence->getSelected())
							{
								std::dynamic_pointer_cast<WorkspaceTransformation>(nodeInSequence)->deleteActionDiwne();
							}
						}
					}
				}
			}
		}

		if (ImGui::MenuItem("Selected links"))
		{
			for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
			{
				Ptr<WorkspaceNodeWithCoreDataWithPins> node =
				    std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(workspaceCoreNode);
				if (node != nullptr)
				{
					for (auto&& pin : node->getInputs())
					{
						if (pin->getLink().getSelected())
						{
							pin->unplug();
						}
					}
				}
			}
		}

		if (ImGui::MenuItem("All nodes"))
		{
			clear();
		}

		if (ImGui::MenuItem("All links"))
		{
			for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
			{
				Ptr<WorkspaceNodeWithCoreDataWithPins> node =
				    std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(workspaceCoreNode);
				if (node != nullptr)
				{
					for (auto&& pin : node->getInputs())
					{
						pin->unplug();
					}
				}
			}
		}
		ImGui::EndMenu();
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Selection"))
	{
		if (ImGui::MenuItem("select all", "Ctrl+A"))
		{
			selectAll();
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

bool WorkspaceDiwne::beforeBegin()
{
	diwne.mp_settingsDiwne->fontColor = I3T::getColor(EColor::NodeFont);

	m_workspaceDiwneAction = WorkspaceDiwneAction::None;
	m_linksToDraw.clear();
	m_allowUnselectingNodes = !InputManager::isAxisActive("NOTunselectAll");
	m_reconnectCameraToSequence = false;
	if (m_updateDataItemsWidth)
	{
		m_updateDataItemsWidth = false;
		for (auto node : getAllNodesInnerIncluded())
		{
			node->updateDataItemsWidth();
		}
	}

	return false;
}

bool WorkspaceDiwne::beforeContent()
{
#ifdef DIWNE_DEBUG
	DIWNE_DEBUG((*this), {
		if (m_workspaceDiwneActionPreviousFrame == WorkspaceDiwneAction::None)
			ImGui::Text("WorkspaceWindowAction::None");
		if (m_workspaceDiwneActionPreviousFrame == WorkspaceDiwneAction::CreateAndPlugTypeConstructor)
			ImGui::Text("WorkspaceWindowPrevAction::CreateAndPlugTypeConstructor");

		ImGui::TextUnformatted(fmt::format("WorkspaceNodes: {}", m_workspaceCoreNodes.size()).c_str());
	});
#endif // DIWNE_DEBUG
	return false;
}

bool WorkspaceDiwne::content()
{
	if (m_workspaceCoreNodes.empty())
	{
		/// \todo Cannot draw with empty workspace ->
		/// `m_channelSplitter.Split(ImGui::GetWindowDrawList(), node_count
		/// );` exception.
		return false;
	}

	// deletion of blocks
	bool interaction_happen = false;
	m_workspaceCoreNodes.erase(std::remove_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
	                                          [](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {
		                                          return node->getRemoveFromWorkspace();
	                                          }),
	                           m_workspaceCoreNodes.end());

	// duplication of blocks
	std::vector<Ptr<WorkspaceNodeWithCoreData>> duplicatedNodes;
	bool shouldDuplicate = false;
	for (auto node : getAllNodesInnerIncluded())
	{
		if (node->getDuplicateNode())
		{
			duplicatedNodes.push_back(node);
			node->setDuplicateNode(false);
			shouldDuplicate = true;
		}
	}
	if (shouldDuplicate)
	{
		pasteNodes(*copyNodes(duplicatedNodes, I3T::getUI()->getTheme().get(ESize::Workspace_CopyPasteOffset)));
	}

	int number_of_nodes = m_workspaceCoreNodes.size();
	int node_count = number_of_nodes - 1; /* -1 for space for top node drawn above links */
	if (number_of_nodes > 0)
	{
		m_channelSplitter.Split(ImGui::GetWindowDrawList(), number_of_nodes + 1 /*+1 for links channel on top */);

		/* draw nodes from back to begin (front to back) to catch interactions in
		 * correct order */
		int prev_size = m_workspaceCoreNodes.size();
		bool takeSnap = false;
		for (auto it = m_workspaceCoreNodes.rbegin(); it != m_workspaceCoreNodes.rend(); ++it)
		{
			if (it == m_workspaceCoreNodes.rbegin()) /* node on top */
			{
				m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(),
				                                    number_of_nodes); /* top node is above links */
			}
			else
			{
				m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), --node_count);
			}

			if ((*it) != nullptr)
			{
				interaction_happen |= (*it)->drawNodeDiwne<WorkspaceNodeWithCoreData>(
				    DIWNE::DrawModeNodePosition::OnItsPosition, DIWNE::DrawMode::Interacting);
			}
			if (prev_size != m_workspaceCoreNodes.size())
				break; /* when push/pop to/from Sequence size of m_workspaceCoreNodes is
				          affected and iterator is invalidated (at least with MVSC) */
		}

		/* draw links under last (on top) node */
		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), number_of_nodes - 1);
		for (auto link : m_linksToDraw)
		{
			interaction_happen |= link->drawDiwne();
		}

		/* Cameras To Sequences links */
		std::vector<Ptr<WorkspaceNodeWithCoreData>> all_cameras = getAllCameras();
		if (all_cameras.size() > 0)
		{
			Ptr<DIWNE::Link> link = std::make_shared<DIWNE::Link>(diwne, INT_MAX, "TemporalLink");
			Ptr<WorkspaceNodeWithCoreDataWithPins> cameraWithPins;

			std::vector<Ptr<WorkspaceNodeWithCoreData>> all_inputFree_Sequence = getAllInputFreeSequence();
			if (all_inputFree_Sequence.size() > 0)
			{
				for (auto const& camera : all_cameras)
				{
					cameraWithPins = std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(camera);
					for (auto const& sequence : all_inputFree_Sequence)
					{
						link->setLinkEndpointsDiwne(
						    cameraWithPins->getOutputs()[Core::I3T_CAMERA_OUT_MUL]->getLinkConnectionPointDiwne(),
						    std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(sequence)
						        ->getInputs()[Core::I3T_SEQ_IN_MUL]
						        ->getLinkConnectionPointDiwne());
						link->drawDiwne(DIWNE::DrawMode::JustDraw);
					}
				}
			}

			std::vector<Ptr<WorkspaceNodeWithCoreData>> all_inputFree_Model = getAllInputFreeModel();
			if (all_inputFree_Model.size() > 0)
			{
				for (auto const& camera : all_cameras)
				{
					cameraWithPins = std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(camera);
					for (auto const& model : all_inputFree_Model)
					{
						link->setLinkEndpointsDiwne(
						    cameraWithPins->getOutputs()[Core::I3T_CAMERA_OUT_MUL]->getLinkConnectionPointDiwne(),
						    std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(model)
						        ->getInputs()[0 /*\todo JH  \todo MH Some constant from core here*/]
						        ->getLinkConnectionPointDiwne());
						link->drawDiwne(DIWNE::DrawMode::JustDraw);
					}
				}
			}
		}

		m_channelSplitter.Merge(ImGui::GetWindowDrawList());
	}

	/// \todo see #111, wrongly computed m_takeSnap value.
	/*
	if (m_diwneAction == DIWNE::DiwneAction::InteractingContent)
	{
	    m_takeSnap |= interaction_happen;
	}
	 */

	return interaction_happen;
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceDiwne::getAllCameras()
{
	std::vector<Ptr<WorkspaceNodeWithCoreData>> cameras;
	for (auto const& node : m_workspaceCoreNodes)
	{
		if (std::dynamic_pointer_cast<WorkspaceCamera>(node))
		{
			cameras.push_back(node);
		};
	}
	return cameras;
}

std::vector<Ptr<WorkspaceModel>> WorkspaceDiwne::getAllModels()
{
	std::vector<Ptr<WorkspaceModel>> models;
	for (auto const& node : m_workspaceCoreNodes)
	{
		if (auto model = std::dynamic_pointer_cast<WorkspaceModel>(node))
		{
			models.push_back(model);
		};
	}
	return models;
}

std::vector<Ptr<WorkspaceModel>> WorkspaceDiwne::getSequenceModels(Ptr<WorkspaceSequence> seq)
{
	auto filtered = m_workspaceCoreNodes | std::views::filter([&seq](Ptr<WorkspaceNodeWithCoreData>& node) {
		                const auto isModel = std::dynamic_pointer_cast<WorkspaceModel>(node);
		                const auto isSequenceChild =
		                    Core::GraphManager::getParent(node->getNodebase()) == seq->getNodebase();

		                return isModel && isSequenceChild;
	                }) |
	                std::views::transform([](Ptr<WorkspaceNodeWithCoreData>& node) {
		                return std::dynamic_pointer_cast<WorkspaceModel>(node);
	                });

	return {filtered.begin(), filtered.end()};
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceDiwne::getAllInputFreeSequence()
{
	std::vector<Ptr<WorkspaceNodeWithCoreData>> sequences;
	for (auto const& node : m_workspaceCoreNodes)
	{
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
		if (seq && !seq->getInputs()[0]->isConnected())
		{
			sequences.push_back(node);
		}; /* \todo JH  \todo MH Always 0? */
	}
	return sequences;
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceDiwne::getAllInputFreeModel()
{
	std::vector<Ptr<WorkspaceNodeWithCoreData>> models;
	for (auto const& node : m_workspaceCoreNodes)
	{
		Ptr<WorkspaceModel> model = std::dynamic_pointer_cast<WorkspaceModel>(node);
		if (model && !model->getInputs()[0]->isConnected())
		{
			models.push_back(node);
		}; /* \todo JH  \todo MH Always 0? */
	}
	return models;
}

bool WorkspaceDiwne::afterContent()
{
	bool interaction_happen = false;
	interaction_happen |= processCreateAndPlugTypeConstructor();

	processTrackingMove();

	/* selection will be active in next frame */
	if (InputManager::isAxisActive("NOTunselectAll"))
	{
		setWorkspaceDiwneAction(WorkspaceDiwneAction::NOTunselectAllNodes);
	}
	if (getNodesSelectionChanged())
	{
		shiftNodesToEnd(getSelectedNodes());

		if (getWorkspaceDiwneActionActive() != WorkspaceDiwneAction::NOTunselectAllNodes &&
		    getDiwneActionActive() != DIWNE::DiwneAction::SelectionRectFull &&
		    getDiwneActionActive() != DIWNE::DiwneAction::SelectionRectTouch)
		{
			for (auto node : getAllNodesInnerIncluded())
			{
				if (node != getLastActiveNode<WorkspaceNodeWithCoreData>())
				{
					if (node->getSelected())
					{
						node->setSelected(false);
					}
				}
			}
		}
	}

	/* hold or drag or interacting or new_link  */
	if ((m_diwneAction == DIWNE::DiwneAction::DragNode || m_diwneAction == DIWNE::DiwneAction::HoldNode ||
	     m_diwneAction == DIWNE::DiwneAction::InteractingContent || m_diwneAction == DIWNE::DiwneAction::NewLink ||
	     m_diwneAction == DIWNE::DiwneAction::TouchNode || m_diwneAction == DIWNE::DiwneAction::TouchPin))
	{
		shiftInteractingNodeToEnd();
	}

	if (m_diwneAction == DIWNE::DiwneAction::DragNode && getLastActiveNode<WorkspaceNodeWithCoreData>()->getSelected())
	{
		processDragAllSelectedNodes();
	}

	// Handle reaction to selection in viewport
	if (m_viewportSelectionChanged)
	{
		bool validSelection = false;
		if (m_viewportLastSelectedEntity != nullptr)
		{
			if (Vp::SceneModel* sceneObject = dynamic_cast<Vp::SceneModel*>(m_viewportLastSelectedEntity))
			{
				auto nodeOpt = findNodeById(g_workspaceDiwne->getAllNodesInnerIncluded(), sceneObject->m_guiNodeId);
				if (nodeOpt)
				{
					Ptr<GuiNode>& node = nodeOpt.value();
					bool selected = node->getSelected();
					g_workspaceDiwne->deselectNodes();
					if (!selected)
					{
						// TODO: (DR) These 3 lines and if statement should basically always be together, the
						// setSelected method
						//   should handle this on its own! This is the case across all uses of Node::setSelected.
						//   A boolean flag should be used to trigger processSelect and snapshot or not.
						//   The goal here is that process(Un)Select() gets always called but ONLY once for each state.
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
			g_workspaceDiwne->deselectNodes();
		}
	}
	m_viewportSelectionChanged = false;

	// Handle manipulators
	manipulatorStartCheck3D();

	return interaction_happen;
}

bool WorkspaceDiwne::afterEnd()
{
	m_workspaceDiwneActionPreviousFrame = m_workspaceDiwneAction;
	m_viewportHighlightResolver.resolve();
	return false;
}

bool WorkspaceDiwne::finalize()
{
	if (m_takeSnap)
	{
		App::getModule<StateManager>().takeSnapshot();
	}
	return false;
}

void WorkspaceDiwne::processDragAllSelectedNodes()
{
	for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
	{
		if (workspaceCoreNode->getSelected() && workspaceCoreNode != getLastActiveNode<WorkspaceNodeWithCoreData>())
			workspaceCoreNode->processDrag();
	}
}

bool WorkspaceDiwne::processCreateAndPlugTypeConstructor()
{
	if (m_workspaceDiwneAction == WorkspaceDiwneAction::CreateAndPlugTypeConstructor)
	{
		switch (getLastActivePin<WorkspaceCoreInputPin>()->getType())
		{
		case Core::EValueType::Pulse:
			addTypeConstructorNode<WorkspaceOperator<Core::EOperatorType::PulseToPulse>>();
			break;
		case Core::EValueType::Float:
			addTypeConstructorNode<WorkspaceOperator<Core::EOperatorType::FloatToFloat>>();
			break;
		case Core::EValueType::Vec3:
			addTypeConstructorNode<WorkspaceOperator<Core::EOperatorType::Vector3ToVector3>>();
			break;
		case Core::EValueType::Vec4:
			addTypeConstructorNode<WorkspaceOperator<Core::EOperatorType::Vector4ToVector4>>();
			break;
		case Core::EValueType::Matrix:
			addTypeConstructorNode<WorkspaceOperator<Core::EOperatorType::MatrixToMatrix>>();
			break;
		case Core::EValueType::Quat:
			addTypeConstructorNode<WorkspaceOperator<Core::EOperatorType::QuatToQuat>>();
			break;
		case Core::EValueType::MatrixMul:
			addTypeConstructorNode<WorkspaceSequence>();
			break;
		case Core::EValueType::Screen:
			addTypeConstructorNode<WorkspaceCamera>();
			break;
			// case Core::EValueType::Ptr:
			//	addTypeConstructorNode<XXXXXX>();
			//	break;
		}
		return true;
	}
	return false; // for Ptr and for a possible non-existent wire type in EValueType
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceDiwne::getSelectedNodesInnerIncluded()
{
	std::vector<Ptr<WorkspaceNodeWithCoreData>> selected;
	for (auto&& workspaceCoreNode : getAllNodesInnerIncluded())
	{
		if (workspaceCoreNode->getSelected())
		{
			selected.push_back(workspaceCoreNode);
		}
	}
	return selected;
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceDiwne::getAllNodesInnerIncluded()
{
	std::vector<Ptr<WorkspaceNodeWithCoreData>> allNodes;
	for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
	{
		allNodes.push_back(workspaceCoreNode);

		/* inner of Sequence */
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(workspaceCoreNode);
		if (seq != nullptr)
		{
			for (auto&& nodeInSequence : seq->getInnerWorkspaceNodes())
			{
				allNodes.push_back(nodeInSequence);
			}
		}

		/* inner of Sequences in Camera */
		// TODO: (DR) What about the sequence nodes inside the Camera? Those are omitted, that caused problems with for
		//   example these inner sequences not gettinng the proper zoom changed events. Overall its just an
		//   inconsistency that I elected to not fix yet as to not distrub other systems which may not account for this.
		//   Should be changed after a discussion. Namely I'm not sure how node duplication would handle this.
		//   WorkspaceCamera currently overrides the updateDataItemsWidth() to overcome this but that should be reverted
		//   if the above is fixed.
		Ptr<WorkspaceCamera> cam = std::dynamic_pointer_cast<WorkspaceCamera>(workspaceCoreNode);
		if (cam != nullptr)
		{
			for (auto&& nodeInSequence : cam->getProjection()->getInnerWorkspaceNodes())
			{
				allNodes.push_back(nodeInSequence);
			}
			for (auto&& nodeInSequence : cam->getView()->getInnerWorkspaceNodes())
			{
				allNodes.push_back(nodeInSequence);
			}
		}
	}

	return allNodes;
}

void WorkspaceDiwne::shiftNodesToBegin(std::vector<Ptr<WorkspaceNodeWithCoreData>> const& nodesToShift)
{

	for (int i = 0; i < nodesToShift.size(); i++)
	{
		coreNodeIter ith_selected_node =
		    std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
		                 [nodesToShift, i](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {
			                 return node->getId() == nodesToShift.at(i)->getId();
		                 });

		if (ith_selected_node != m_workspaceCoreNodes.end())
		{
			std::iter_swap(m_workspaceCoreNodes.begin() + i, ith_selected_node);
		}
	}
}

void WorkspaceDiwne::shiftNodesToEnd(std::vector<Ptr<WorkspaceNodeWithCoreData>> const& nodesToShift)
{
	int node_num = nodesToShift.size();
	//    str2.erase(std::remove_if(str2.begin(),
	//                              str2.end(),
	//                              [](unsigned char x){return std::isspace(x);})
	//    a.erase(std::remove_if(a.begin(), a.end(), predicate), a.end());
	for (int i = 0; i < node_num; i++)
	{
		coreNodeIter ith_selected_node =
		    std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
		                 [nodesToShift, i](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {
			                 return node->getId() == nodesToShift.at(i)->getId();
		                 });
		if (ith_selected_node != m_workspaceCoreNodes.end())
		{
			std::iter_swap(m_workspaceCoreNodes.end() - node_num + i, ith_selected_node);
		}
	}
}

void WorkspaceDiwne::shiftInteractingNodeToEnd()
{
	if (mp_lastActiveNode != nullptr && mp_lastActiveNode.get() != m_workspaceCoreNodes.back().get())
	{
		coreNodeIter draged_node_it = std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
		                                           [this](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {
			                                           return node.get() == this->mp_lastActiveNode.get();
		                                           });

		if (draged_node_it != m_workspaceCoreNodes.end() && draged_node_it != m_workspaceCoreNodes.end() - 1)
		{
			std::rotate(draged_node_it, draged_node_it + 1, m_workspaceCoreNodes.end());
		}
	}
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceDiwne::getSelectedNodes()
{
	std::vector<Ptr<WorkspaceNodeWithCoreData>> selected;
	for (auto const& node : m_workspaceCoreNodes)
	{
		if (node->getSelected())
		{
			selected.push_back(node);
		};
	}
	return selected;
}

void WorkspaceDiwne::manipulatorStartCheck3D()
{
	if (getNodesSelectionChanged())
	{
		I3T::getViewport()->getManipulators().clearManipulators();

		std::vector<Ptr<WorkspaceNodeWithCoreData>> selectedNodes = getSelectedNodesInnerIncluded();
		for (const auto& node : selectedNodes)
		{
			Ptr<WorkspaceTransformation> selected_transformation =
			    std::dynamic_pointer_cast<WorkspaceTransformation>(node);
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
			g_workspaceDiwne->trackingSmoothLeft();
		}
		if (InputManager::isAxisActive("trackingSmoothRight") != 0)
		{
			g_workspaceDiwne->trackingSmoothRight();
		}
	}
}

bool WorkspaceDiwne::bypassZoomAction()
{
	return InputManager::isAxisActive("scroll") != 0;
}
bool WorkspaceDiwne::bypassDragAction()
{
	return InputManager::isAxisActive("pan") != 0;
}
bool WorkspaceDiwne::bypassHoldAction()
{
	return InputManager::isAxisActive("pan") != 0;
}
bool WorkspaceDiwne::bypassUnholdAction()
{
	return InputManager::isAxisActive("pan") == 0;
}

bool WorkspaceDiwne::bypassSelectionRectangleAction()
{
	return InputManager::isAxisActive("selectionRectangle") != 0 /* && (InputManager::m_mouseXDragDelta >
	                                                                ImGui::GetIO().MouseDragThreshold ||F
	                                                                InputManager::m_mouseYDragDelta >
	                                                                ImGui::GetIO().MouseDragThreshold ||
	                                                                -InputManager::m_mouseXDragDelta >
	                                                                ImGui::GetIO().MouseDragThreshold ||
	                                                                -InputManager::m_mouseYDragDelta >
	                                                                ImGui::GetIO().MouseDragThreshold)*/
	    ;
} /* \todo JH I suspect bug if dragging start outside of WorkspaceWindow... */
ImVec2 WorkspaceDiwne::bypassDiwneGetSelectionRectangleStartPosition()
{
	return screen2diwne(bypassMouseClickedPos0());
} /* \todo JH I suspect bug if dragging start outside of WorkspaceWindow... */
ImVec2 WorkspaceDiwne::bypassDiwneGetSelectionRectangleSize()
{
	return bypassGetMouseDragDelta0() / getWorkAreaZoom();
} /* \todo JH I suspect bug if dragging start outside of WorkspaceWindow... */

bool WorkspaceDiwne::processZoom()
{
	m_updateDataItemsWidth = true;
	return Diwne::processZoom();
}

void WorkspaceDiwne::setWorkAreaZoom(float val)
{
	float old = m_workAreaZoom;
	Diwne::setWorkAreaZoom(val);
	if (old != val)
	{
		m_updateDataItemsWidth = true;
	}
}

/* ========================================== */
/* ===== W o r k s p a c e  W i n d o w ===== */
/* ========================================== */
WorkspaceWindow::WorkspaceWindow(bool show) : IWindow("Workspace", show), m_wholeApplication(App::get())
{
	m_autoFocus = true;
	initDiwneFromTheme();
	g_workspaceDiwne = new WorkspaceDiwne(&settingsDiwne);
	// Input actions for workspace window.
	m_input->bindAction("selectAll", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->selectAll();
	});
	m_input->bindAction("invertSelection", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->invertSelection();
	});
	m_input->bindAction("zoomToAll", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->zoomToAll();
	});
	m_input->bindAction("zoomToSelected", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->zoomToSelected();
	});
	m_input->bindAction("delete", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->deleteCallback();
	});
	m_input->bindAction("copy", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->copySelectedNodes();
	});
	m_input->bindAction("paste", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->pasteSelectedNodes();
	});
	m_input->bindAction("cut", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->cutSelectedNodes();
	});
	m_input->bindAction("duplicate", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->duplicateClickedNode();
	});
	m_input->bindAction("duplicateSelected", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->duplicateSelectedNodes();
	});
	m_input->bindAction("trackingEscOff", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingSwitchOff();
	});
	m_input->bindAction("trackingSmoothLeft", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingSmoothLeft();
	});
	m_input->bindAction("trackingSmoothRight", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingSmoothRight();
	});
	m_input->bindAction("trackingJaggedLeft", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingJaggedLeft();
	});
	m_input->bindAction("trackingJaggedRight", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingJaggedRight();
	});
	m_input->bindAction("trackingModeSwitch", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingModeSwitch();
	});
	m_input->bindAction("trackingSwitch", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingSwitch();
	});
	m_input->bindAction("trackingSwitchOn", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingSwitchOn();
	});
	m_input->bindAction("trackingSwitchOff", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->trackingSwitchOff();
	});
	m_input->bindAction("toggleNodeWorkspaceVisibility", EKeyState::Pressed, [&]() {
		g_workspaceDiwne->toggleSelectedNodesVisibility();
	});

	App::getModule<StateManager>().addOriginator(this);

	// Setup viewport selection callback
	I3T::getViewport()->getMainScene().lock()->addSelectionCallback([](Vp::Entity* newlySelectedEntity) {
		// Save information about this callback and perform actions based on it later while in workspace window context.
		// This is a workaround due to viewport selection occurring in unknown order at unknown time.
		g_workspaceDiwne->m_viewportSelectionChanged = true;
		g_workspaceDiwne->m_viewportLastSelectedEntity = newlySelectedEntity;
	});
}

WorkspaceWindow::~WorkspaceWindow()
{
	delete g_workspaceDiwne;
}

// TODO - Make diwne change settings on theme switch (when Theme::apply() is called)
void WorkspaceWindow::initDiwneFromTheme()
{
	settingsDiwne.selectionRounding = I3T::getUI()->getTheme().get(ESize::Nodes_Rounding);
	settingsDiwne.itemSelectedBorderColor = I3T::getUI()->getTheme().get(EColor::Workspace_SelectedBorder);
	settingsDiwne.itemSelectedBorderThicknessDiwne =
	    I3T::getUI()->getTheme().get(ESize::Workspace_SelectedBorderThickness);
	settingsDiwne.objectFocusBorderColor = I3T::getUI()->getTheme().get(EColor::Workspace_FocusBorder);
	settingsDiwne.objectFocusBorderThicknessDiwne = I3T::getUI()->getTheme().get(ESize::Workspace_FocusBorderThickness);
	settingsDiwne.objectFocusForInteractionBorderColor =
	    I3T::getUI()->getTheme().get(EColor::Workspace_InteractionFocusBorder);
	settingsDiwne.objectFocusForInteractionBorderThicknessDiwne =
	    I3T::getUI()->getTheme().get(ESize::Workspace_InteractionFocusBorderThickness);
}

//
// Node builder functions.
//

WorkspaceDiwne& WorkspaceWindow::getNodeEditor()
{
	return *g_workspaceDiwne;
}

void WorkspaceWindow::render()
{
	ImGui::PushStyleColor(ImGuiCol_TabActive, I3T::getUI()->getTheme().get(EColor::DockTabActive));
	/* Draw to window only if is visible - call ImGui::End() everytime */
	if (ImGui::Begin(getName(), getShowPtr(),
	                 g_WindowFlags | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar |
	                     ImGuiWindowFlags_NoScrollWithMouse))
	{
		this->updateWindowInfo();

		if (ImGui::BeginMenuBar())
		{
			showEditMenu();
			ImGui::EndMenuBar();
		}

		DIWNE::DrawMode drawMode = DIWNE::DrawMode::JustDraw;
		// TODO: (DR) Make this consistent with ViewportWindow (check for active input rather than focus)
		if (I3T::getUI()->getWindowManager().isFocused<WorkspaceWindow>())
		{
			drawMode = DIWNE::DrawMode::Interacting;
		}
		g_workspaceDiwne->drawDiwne(drawMode);
	}
	ImGui::End();
	ImGui::PopStyleColor();
}

void WorkspaceWindow::showEditMenu()
{
	if (ImGui::BeginMenu("Edit"))
	{
		/*
		if (ImGui::MenuItem("Undo"))
		{
		    // B
		    /// \todo Undo.
		    // UndoRedo::undo();
		}

		if (ImGui::MenuItem("Redo"))
		{
		    // N
		    /// \todo Redo.
		    // UndoRedo::redo();
		}
		 */

		if (ImGui::MenuItem("Select all"))
		{
			g_workspaceDiwne->selectAll();
		}

		if (ImGui::MenuItem("Zoom all"))
		{
			g_workspaceDiwne->zoomToAll();
		}

		ImGui::EndMenu();
	}
}

//

bool connectNodesNoSave(Ptr<WorkspaceNodeWithCoreData> lhs, Ptr<WorkspaceNodeWithCoreData> rhs, int lhsPin, int rhsPin)
{
	bool success = std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(rhs)->getInputs().at(rhsPin)->plug(
	    std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(lhs)->getOutputs().at(lhsPin).get(), false);
	if (!success)
	{
		LOG_INFO("Cannot connect pin{} to pin{} of nodes {} and {}", lhs->getNodebase()->getSignature(),
		         rhs->getNodebase()->getSignature(), lhsPin, rhsPin);
	}
	rhs->updateDataItemsWidth();
	lhs->updateDataItemsWidth();
	return success;
}

/////////////////////////////////////////
// State save/load
/////////////////////////////////////////

Memento WorkspaceWindow::saveScene(Scene* scene)
{
	Memento memento;
	rapidjson::Value::AllocatorType& a = memento.GetAllocator();

	SerializationVisitor visitor(memento);
	visitor.dump(getNodeEditor().m_workspaceCoreNodes);

	JSON::addFloat(memento["workspace"], "zoom", g_workspaceDiwne->getWorkAreaZoom(), a);
	JSON::addRect(memento["workspace"], "workArea", g_workspaceDiwne->getWorkAreaDiwne(), a);

	return memento;
}

void WorkspaceWindow::loadScene(const Memento& memento, Scene* scene)
{
	clearScene();

	if (!memento.HasMember("workspace"))
	{
		LOG_ERROR("Failed to load workspace! No 'workspace' member found.");
		return;
	}

	NodeDeserializer::createFrom(memento);

	if (memento["workspace"].HasMember("zoom"))
		g_workspaceDiwne->setWorkAreaZoom(memento["workspace"]["zoom"].GetFloat());

	if (memento["workspace"].HasMember("workArea"))
		g_workspaceDiwne->setWorkAreaDiwne(JSON::getRect(memento["workspace"]["workArea"]));
}

void WorkspaceWindow::clearScene()
{
	for (auto& node : getNodeEditor().m_workspaceCoreNodes)
	{
		node->deleteActionDiwne();
	}
	getNodeEditor().clear();
}

Memento WorkspaceWindow::saveGlobal()
{
	return emptyMemento();
}

void WorkspaceWindow::loadGlobal(const Memento& memento) {}

void WorkspaceWindow::clearGlobal() {}
