/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.
*/
#include "WorkspaceWindow.h"

#include "GUI/Elements/Nodes/Builder.h"
#include "GUI/Elements/Nodes/Tools.h"
#include "GUI/WindowManager.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "Utils/JSON.h"
#include "Logger/Logger.h"

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
      m_workspaceDiwneActionPreviousFrame(WorkspaceDiwneAction::None), m_resizeDataWidth(false),
      m_trackingFromLeft(false), tracking(nullptr), smoothTracking(true)

{
}

WorkspaceDiwne::~WorkspaceDiwne() { m_workspaceCoreNodes.clear(); }

void WorkspaceDiwne::selectAll()
{
	for (auto&& workspaceCoreNode : getAllNodesInnerIncluded())
	{
		workspaceCoreNode->setSelected(true);
	}
}

void WorkspaceDiwne::invertSelection()
{
	for (auto&& workspaceCoreNode : getAllNodesInnerIncluded())
	{
		workspaceCoreNode->setSelected(!workspaceCoreNode->getSelected());
	}
}

void WorkspaceDiwne::zoomToAll() { zoomToRectangle(getOverNodesRectangleDiwne(getAllNodesInnerIncluded())); }

void WorkspaceDiwne::zoomToSelected() { zoomToRectangle(getOverNodesRectangleDiwne(getSelectedNodesInnerIncluded())); }

void WorkspaceDiwne::trackingSmoothLeft()
{
	if (Core::GraphManager::isTrackingEnabled() && smoothTracking)
	{
		float step = I3T::getSize(ESize::Tracking_SmoothScrollSpeed)/tracking->getTrackingProgress().size();
		tracking->setParam(tracking->getParam()+step);
	}
}

void WorkspaceDiwne::trackingSmoothRight()
{
	if (Core::GraphManager::isTrackingEnabled() && smoothTracking)
	{
		float step = I3T::getSize(ESize::Tracking_SmoothScrollSpeed) / tracking->getTrackingProgress().size();

		tracking->setParam(tracking->getParam()-step);
	}
}
void WorkspaceDiwne::trackingJaggedLeft()
{
	if (Core::GraphManager::isTrackingEnabled() && !smoothTracking)
	{
		float step = I3T::getSize(ESize::Tracking_JaggedScrollSpeed)/tracking->getTrackingProgress().size();
		tracking->setParam(tracking->getParam()+step);
	}
}

void WorkspaceDiwne::trackingJaggedRight()
{
	if (Core::GraphManager::isTrackingEnabled() && !smoothTracking)
	{
		float step = I3T::getSize(ESize::Tracking_JaggedScrollSpeed) / tracking->getTrackingProgress().size();

		tracking->setParam(tracking->getParam()-step);
	}
}

void WorkspaceDiwne::trackingModeSwitch(){
	smoothTracking = !smoothTracking;
}

void WorkspaceDiwne::trackingSwitch()
{
	LOG_INFO("TRACKING CALLED");
	if(Core::GraphManager::isTrackingEnabled()) trackingSwitchOff();
	else trackingSwitchOn();
}

void WorkspaceDiwne::trackingSwitchOn(Ptr<WorkspaceSequence> sequence)
{
	if(sequence == nullptr)
	{
		for (auto&& node : getSelectedNodesInnerIncluded())
		{
			Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
			if (seq)
			{
				// m_trackingFirstTransformation = std::dynamic_pointer_cast<WorkspaceTransformation>(seq->getInnerWorkspaceNodes().back());
				const auto model = getSequenceModel(seq);
				if (model == nullptr)
					continue;
				LOG_INFO("TRACKING ON");
				seq->setTint(I3T::getColor(EColor::TrackingSequenceTint));
				tracking = seq->getNodebase()->as<Core::Sequence>()->startTracking(
				    std::make_unique<WorkspaceModelProxy>(model));
				break;
			}
		}
	}
	else
	{
			const auto model = getSequenceModel(sequence);
			if (model == nullptr) return;
			LOG_INFO("TRACKING ON");
		  sequence->setTint(I3T::getColor(EColor::TrackingSequenceTint));
			tracking = sequence->getNodebase()->as<Core::Sequence>()->startTracking(
			    std::make_unique<WorkspaceModelProxy>(model));
	}


}

void WorkspaceDiwne::trackingSwitchOff()
{
			LOG_INFO("TRACKING OFF");
	    auto seq = findNodeById(getAllNodesInnerIncluded(), tracking->getSequence()->getId()).value();
	    std::dynamic_pointer_cast<WorkspaceSequence>(seq)->setTint(ImVec4(1, 1, 1, 1));
			tracking->getSequence()->stopTracking();
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
	for (auto&& workspaceCoreNode : getAllNodesInnerIncluded())
	{
		/*if(workspaceCoreNode->m_focused && !workspaceCoreNode->m_selected)
		{
			workspaceCoreNode->deleteActionDiwne();
			Ptr<WorkspaceTransformation> trn = std::dynamic_pointer_cast<WorkspaceTransformation>(workspaceCoreNode);
			if (trn != nullptr)
			{
				trn->deleteActionDiwne();
			}
			break;
		}
		else */
		    if(workspaceCoreNode->m_selected)
		{
			workspaceCoreNode->deleteActionDiwne();

			Ptr<WorkspaceTransformation> trn = std::dynamic_pointer_cast<WorkspaceTransformation>(workspaceCoreNode);
			if (trn != nullptr)
			{
				trn->deleteActionDiwne();
			}
		}
	}
}

void WorkspaceDiwne::copySelectedNodes()
{
	LOG_INFO("Copying nodes");
	//Preventing double duplication of selected transformations in a sequence
	for (auto node : getSelectedNodesInnerIncluded())
	{
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
		if (seq)
		{
			for (auto transform : seq->getInnerWorkspaceNodes())
			{
				transform->setSelected(false);
			}
		}
	}
	copiedNodes = copyNodes(getSelectedNodesInnerIncluded());
}

void WorkspaceDiwne::pasteSelectedNodes()
{
	LOG_INFO("Pasting nodes");
	deselectNodes();
	pasteNodes(copiedNodes);
}

void WorkspaceDiwne::duplicateClickedNode()
{
	//TODO add a case for selected inner transform - duplicate both sequence and transform?
	LOG_INFO("Duplicating")
	//Preventing double duplication of selected transformations in a sequence
	for (auto node : getSelectedNodesInnerIncluded())
	{
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
		if (seq)
		{
			for (auto transform : seq->getInnerWorkspaceNodes())
			{
				transform->setSelected(false);
			}
		}
	}

	//Duplicating either focused node or all selected nodes
	auto selectedNodes = getSelectedNodesInnerIncluded();

	for (const Ptr<GuiNode>& node : getAllNodesInnerIncluded()){
		if(node->m_focused){
			if(node->m_selected)
			{
				deselectNodes();
				node->setSelected(true);
				// copy and paste to ensure connections
				pasteNodes(copyNodes(selectedNodes));
			}
			else
			{
				deselectNodes();
				node->setSelected(true);
				duplicateNode(node);
			}
		}
	}
}


void WorkspaceDiwne::duplicateSelectedNodes()
{
	LOG_INFO("Duplicating")
	//Preventing double duplication of selected transformations in a sequence
	for (auto node : getSelectedNodesInnerIncluded())
	{
		Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(node);
		if (seq)
		{
			for (auto transform : seq->getInnerWorkspaceNodes())
			{
				transform->setSelected(false);
			}
		}
	}

	auto selectedNodes = getSelectedNodesInnerIncluded();

	// copy and paste to ensure connections
	pasteNodes(copyNodes(selectedNodes));

	for (auto node : selectedNodes)
	{
		node->setSelected(false);
	}
}


void WorkspaceDiwne::deselectNodes(){
	for (auto node : getAllNodesInnerIncluded()){
		node->setSelected(false);
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
			addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::Free>>();
		}
		if (ImGui::MenuItem("translation"))
		{
			addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::Translation>>();
		}

		if (ImGui::BeginMenu("rotation"))
		{

			if (ImGui::MenuItem("eulerAngleX"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::EulerX>>();
			}
			if (ImGui::MenuItem("eulerAngleY"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::EulerY>>();
			}
			if (ImGui::MenuItem("eulerAngleZ"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::EulerZ>>();
			}
			if (ImGui::MenuItem("axisAngle"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::AxisAngle>>();
			}
			if (ImGui::MenuItem("quat"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::Quat>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("scale"))
		{
			addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::Scale>>();
		}
		//            if (ImGui::MenuItem("non-uniform scale"))
		//            {
		//                addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::Scale>>();
		//                m_workspaceCoreNodes.back()->getNodebase()->as<Core::Transformation>()->disableSynergies();
		//            }
		if (ImGui::MenuItem("lookAt"))
		{
			addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::LookAt>>();
		}
		if (ImGui::BeginMenu("projection"))
		{
			if (ImGui::MenuItem("ortho"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::Ortho>>();
			}
			if (ImGui::MenuItem("perspective"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::Perspective>>();
			}
			if (ImGui::MenuItem("frustrum"))
			{
				addNodeToPositionOfPopup<WorkspaceTransformation_s<ETransformType::Frustum>>();
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
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeTranslation>>();
			}
			if (ImGui::MenuItem("euler AngleX"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeEulerX>>();
			}
			if (ImGui::MenuItem("euler AngleY"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeEulerY>>();
			}
			if (ImGui::MenuItem("euler AngleZ"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeEulerZ>>();
			}
			if (ImGui::MenuItem("rotate"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeAxisAngle>>();
			}
			if (ImGui::MenuItem("scale"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeScale>>();
			}
			if (ImGui::MenuItem("ortho"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeOrtho>>();
			}
			if (ImGui::MenuItem("perspective"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakePerspective>>();
			}
			if (ImGui::MenuItem("frustrum"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeFrustum>>();
			}
			if (ImGui::MenuItem("lookAt"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MakeLookAt>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("matrix"))
		{
			if (ImGui::MenuItem("matrix"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixToMatrix>>();
			}
			/*
			    if (ImGui::MenuItem("trackball"))
			    {
			        addNodeToPositionOfPopup<WorkspaceTrackball>();
			    }
			     */
			if (ImGui::MenuItem("inversion"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Inversion>>();
			}
			if (ImGui::MenuItem("transpose"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Transpose>>();
			}
			if (ImGui::MenuItem("determinant"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Determinant>>();
			}
			if (ImGui::MenuItem("mat * mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixMulMatrix>>();
			}
			if (ImGui::MenuItem("mat + mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixAddMatrix>>();
			}
			if (ImGui::MenuItem("mat * vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixMulVector>>();
			}
			if (ImGui::MenuItem("vec4 * mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorMulMatrix>>();
			}
			if (ImGui::MenuItem("float * mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixMulFloat>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("vec3"))
		{
			if (ImGui::MenuItem("vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3ToVector3>>();
			}
			if (ImGui::MenuItem("show vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::ShowVector3>>();
			}
			if (ImGui::MenuItem("vec3 x vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3CrossVector3>>();
			}
			if (ImGui::MenuItem("vec3 . vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3DotVector3>>();
			}
			if (ImGui::MenuItem("vec3 + vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3AddVector3>>();
			}
			if (ImGui::MenuItem("vec3 - vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3SubVector3>>();
			}
			if (ImGui::MenuItem("float * vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3MulFloat>>();
			}
			if (ImGui::MenuItem("normalize vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::NormalizeVector3>>();
			}
			if (ImGui::MenuItem("length(vec3)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3Length>>();
			}
			if (ImGui::MenuItem("mix vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MixVector3>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("vec4"))
		{
			if (ImGui::MenuItem("vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector4ToVector4>>();
			}
			if (ImGui::MenuItem("vec4 . vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorDotVector>>();
			}
			if (ImGui::MenuItem("vec4 + vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorAddVector>>();
			}
			if (ImGui::MenuItem("vec4 - vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorSubVector>>();
			}
			if (ImGui::MenuItem("float * vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorMulFloat>>();
			}
			if (ImGui::MenuItem("normalize vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::NormalizeVector>>();
			}
			if (ImGui::MenuItem("perspective division"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorPerspectiveDivision>>();
			}
			if (ImGui::MenuItem("mix vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MixVector>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("quat"))
		{
			if (ImGui::MenuItem("quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatToQuat>>();
			}
			if (ImGui::MenuItem("quat(float, vec3)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatVecToQuat>>();
			}
			if (ImGui::MenuItem("quat(angle, axis)"))
			{
				// addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::AngleAxisToQuat>>();
				addNodeToPositionOfPopup<WorkspaceAngleAxisToQuat>();
			}
			if (ImGui::MenuItem("quat(vec3, vec3)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VecVecToQuat>>();
			}
			if (ImGui::MenuItem("quat -> float, vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatToFloatVec>>();
			}
			if (ImGui::MenuItem("quat -> angle, axis"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatToAngleAxis>>();
			}
			if (ImGui::MenuItem("float * quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatMulQuat>>();
			}
			if (ImGui::MenuItem("quat * quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatMulQuat>>();
			}
			if (ImGui::MenuItem("quat -> euler"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatToEuler>>();
			}
			if (ImGui::MenuItem("euler -> quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::EulerToQuat>>();
			}
			if (ImGui::MenuItem("slerp"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatSlerp>>();
			}
			if (ImGui::MenuItem("long way slerp"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatLongWaySlerp>>();
			}
			if (ImGui::MenuItem("lerp"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatLerp>>();
			}
			if (ImGui::MenuItem("quat conjugate"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::ConjQuat>>();
			}
			if (ImGui::MenuItem("qvq*"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatVecConjQuat>>();
			}
			if (ImGui::MenuItem("inverse quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatInverse>>();
			}
			if (ImGui::MenuItem("normalize quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::NormalizeQuat>>();
			}
			if (ImGui::MenuItem("length(quat)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatLength>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("float"))
		{
			if (ImGui::MenuItem("float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatToFloat>>();
			}
			if (ImGui::MenuItem("clamp float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::ClampFloat>>();
			}
			if (ImGui::MenuItem("float cycle"))
			{
				addNodeToPositionOfPopup<WorkspaceCycle>();
			}
			if (ImGui::MenuItem("float * float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatMulFloat>>();
			}
			if (ImGui::MenuItem("float / float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatDivFloat>>();
			}
			if (ImGui::MenuItem("float + float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatAddFloat>>();
			}
			if (ImGui::MenuItem("float ^ float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatPowFloat>>();
			}
			if (ImGui::MenuItem("mix float"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MixFloat>>();
			}
			if (ImGui::MenuItem("sin & cos(float)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatSinCos>>();
			}
			if (ImGui::MenuItem("asin & acos(float)"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::ASinACos>>();
			}
			if (ImGui::MenuItem("signum"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Signum>>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("conversion"))
		{
			if (ImGui::MenuItem("mat -> TR"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixToTR>>();
			}
			if (ImGui::MenuItem("TR -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::TRToMatrix>>();
			}
			if (ImGui::MenuItem("mat -> vecs4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixToVectors>>();
			}
			if (ImGui::MenuItem("mat -> quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixToQuat>>();
			}
			if (ImGui::MenuItem("mat -> floats"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::MatrixToFloats>>();
			}
			if (ImGui::MenuItem("vecs4 -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorsToMatrix>>();
			}
			if (ImGui::MenuItem("vec4 -> vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorToVector3>>();
			}
			if (ImGui::MenuItem("vec4 -> floats"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::VectorToFloats>>();
			}
			if (ImGui::MenuItem("vecs3 -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vectors3ToMatrix>>();
			}
			if (ImGui::MenuItem("vec3 -> vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3ToVector>>();
			}
			if (ImGui::MenuItem("vec3 -> floats"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Vector3ToFloats>>();
			}
			if (ImGui::MenuItem("quat -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatToMatrix>>();
			}
			if (ImGui::MenuItem("quat -> floats"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::QuatToFloats>>();
			}
			if (ImGui::MenuItem("floats -> mat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatsToMatrix>>();
			}
			if (ImGui::MenuItem("floats -> vec4"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatsToVector>>();
			}
			if (ImGui::MenuItem("floats -> vec3"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatsToVector3>>();
			}
			if (ImGui::MenuItem("floats -> quat"))
			{
				addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::FloatsToQuat>>();
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
		addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Pulse>>();
	}
	if (ImGui::MenuItem("screen"))
	{
		addNodeToPositionOfPopup<WorkspaceScreen>();
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
			m_workspaceCoreNodes.clear();
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
		if (ImGui::MenuItem("to all", "LShift+A"))
		{
			zoomToAll();
		}
		if (ImGui::MenuItem("to selection", "LShift+X"))
		{
			zoomToSelected();
		}
		ImGui::EndMenu();
	}
}

bool WorkspaceDiwne::beforeBegin()
{
	m_workspaceDiwneAction = WorkspaceDiwneAction::None;
	m_linksToDraw.clear();
	m_allowUnselectingNodes = !InputManager::isAxisActive("NOTunselectAll");
	m_reconnectCameraToSequence = false;
	if (m_resizeDataWidth)
	{
		m_resizeDataWidth = false;
		for (auto node : getAllNodesInnerIncluded())
		{
			node->setDataItemsWidth();
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

	bool interaction_happen = false;
	m_workspaceCoreNodes.erase(std::remove_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
	                                          [](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool
	                                          { return node->getRemoveFromWorkspace(); }),
	                           m_workspaceCoreNodes.end());

	int number_of_nodes = m_workspaceCoreNodes.size();
	int node_count = number_of_nodes - 1; /* -1 for space for top node drawn above links */
	if (number_of_nodes > 0)
	{
		m_channelSplitter.Split(ImGui::GetWindowDrawList(), number_of_nodes + 1 /*+1 for links channel on top */);

		/* draw nodes from back to begin (front to back) for catch interactions in
		 * right order */
		int prev_size = m_workspaceCoreNodes.size();
		bool takeSnap = false;
		for (auto it = m_workspaceCoreNodes.rbegin(); it != m_workspaceCoreNodes.rend(); ++it)
		{
			if (it == m_workspaceCoreNodes.rbegin()) /* node on top */
			{
				m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), number_of_nodes); /* top node is above links */
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

	/* \todo JH \todo MH maybe some different/other actions */
	if (m_diwneAction == DIWNE::DiwneAction::InteractingContent)
	{
		m_takeSnap |= interaction_happen;
	}
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

Ptr<WorkspaceModel> WorkspaceDiwne::getSequenceModel(Ptr<WorkspaceSequence> seq)
{
	std::vector<Ptr<WorkspaceModel>> models;
	Ptr<WorkspaceModel> ret = nullptr;
	for (auto const& node : m_workspaceCoreNodes)
	{
		Ptr<WorkspaceModel> model = std::dynamic_pointer_cast<WorkspaceModel>(node);
		if (model && model->getInputs()[0]->isConnected())
		{
			models.push_back(std::dynamic_pointer_cast<WorkspaceModel>(node));
		};
	}

	for (auto const& model : models){
		WorkspaceCoreOutputPin* pin = model->getInputs()[0]->getLink().getStartPin();
		WorkspaceSequence* tempSeq;
		while (pin != nullptr){
			tempSeq = dynamic_cast<WorkspaceSequence*>(&(pin->getNode()));
			if(tempSeq == nullptr) break;

			if(tempSeq->getId() == seq->getId())
			{
				ret = model;
				break;
			}
			pin = tempSeq->getInputs()[0]->getLink().getStartPin();
		}
	}
	return ret;
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
					node->setSelected(false);
			}
		}
	}

	/* ===== reaction to actions ===== */
	manipulatorStartCheck3D();

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
	return interaction_happen;
}

bool WorkspaceDiwne::afterEnd()
{
	m_workspaceDiwneActionPreviousFrame = m_workspaceDiwneAction;
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
		case EValueType::Matrix:
			addTypeConstructorNode<WorkspaceOperator<ENodeType::MatrixToMatrix>>();
			break;
		case EValueType::Vec3:
			addTypeConstructorNode<WorkspaceOperator<ENodeType::Vector3ToVector3>>();
			break;
		case EValueType::Vec4:
			addTypeConstructorNode<WorkspaceOperator<ENodeType::Vector4ToVector4>>();
			break;
		case EValueType::Quat:
			addTypeConstructorNode<WorkspaceOperator<ENodeType::QuatToQuat>>();
			break;
		case EValueType::Float:
			addTypeConstructorNode<WorkspaceOperator<ENodeType::FloatToFloat>>();
			break;
		case EValueType::Pulse:
			addTypeConstructorNode<WorkspaceOperator<ENodeType::Pulse>>();
			break;
		case EValueType::Screen:
			addTypeConstructorNode<WorkspaceCamera>();
			break;
		case EValueType::MatrixMul:
			addTypeConstructorNode<WorkspaceSequence>();
			break;
		}
		return true;
	}
	return false;
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
		coreNodeIter ith_selected_node = std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
		                                              [nodesToShift, i](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool
		                                              { return node->getId() == nodesToShift.at(i)->getId(); });

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
		coreNodeIter ith_selected_node = std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
		                                              [nodesToShift, i](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool
		                                              { return node->getId() == nodesToShift.at(i)->getId(); });

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
		                                           [this](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool
		                                           { return node.get() == this->mp_lastActiveNode.get(); });

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
		std::vector<Ptr<WorkspaceNodeWithCoreData>> selected_nodes = getSelectedNodesInnerIncluded();
		if (selected_nodes.size() == 1)
		{
			Ptr<WorkspaceTransformation> selected_transformation =
			    std::dynamic_pointer_cast<WorkspaceTransformation>(selected_nodes[0]);
			if (selected_transformation != nullptr)
			{
				Application::get().world()->manipulatorsSetMatrix(selected_transformation, nullptr);
			} /* \todo JH \todo MH \todo PF why not pass sequence of transformation?
			   */
		}
		else
		{
			Application::get().world()->manipulatorsSetMatrix(nullptr, nullptr);
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

bool WorkspaceDiwne::bypassZoomAction() { return InputManager::isAxisActive("scroll") != 0; }
bool WorkspaceDiwne::bypassDragAction() { return InputManager::isAxisActive("pan") != 0; }
bool WorkspaceDiwne::bypassHoldAction() { return InputManager::isAxisActive("pan") != 0; }
bool WorkspaceDiwne::bypassUnholdAction() { return InputManager::isAxisActive("pan") == 0; }

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
	m_resizeDataWidth = true;
	return Diwne::processZoom();
}

/* ========================================== */
/* ===== W o r k s p a c e  W i n d o w ===== */
/* ========================================== */
WorkspaceWindow::WorkspaceWindow(bool show) : IWindow(show), m_wholeApplication(Application::get())
{
	g_workspaceDiwne = new WorkspaceDiwne(&settingsDiwne);

	// Input actions for workspace window.
	Input.bindAction("selectAll", EKeyState::Pressed, [&]() { g_workspaceDiwne->selectAll(); });
	Input.bindAction("invertSelection", EKeyState::Pressed, [&]() { g_workspaceDiwne->invertSelection(); });
	Input.bindAction("zoomToAll", EKeyState::Pressed, [&]() { g_workspaceDiwne->zoomToAll(); });
	Input.bindAction("zoomToSelected", EKeyState::Pressed, [&]() { g_workspaceDiwne->zoomToSelected(); });
	Input.bindAction("delete", EKeyState::Pressed, [&]() { g_workspaceDiwne->deleteCallback(); });
	Input.bindAction("copy", EKeyState::Pressed, [&]() { g_workspaceDiwne->copySelectedNodes(); });
	Input.bindAction("paste", EKeyState::Pressed, [&]() { g_workspaceDiwne->pasteSelectedNodes(); });
	Input.bindAction("duplicate", EKeyState::Pressed, [&]() { g_workspaceDiwne->duplicateClickedNode(); });
	Input.bindAction("duplicateSelected", EKeyState::Pressed, [&]() { g_workspaceDiwne->duplicateSelectedNodes(); });
	Input.bindAction("trackingSmoothLeft", EKeyState::Pressed, [&]() {g_workspaceDiwne->trackingSmoothLeft(); });
	Input.bindAction("trackingSmoothRight", EKeyState::Pressed, [&]() { g_workspaceDiwne->trackingSmoothRight(); });
	Input.bindAction("trackingJaggedLeft", EKeyState::Pressed, [&]() {g_workspaceDiwne->trackingJaggedLeft(); });
	Input.bindAction("trackingJaggedRight", EKeyState::Pressed, [&]() { g_workspaceDiwne->trackingJaggedRight(); });
	Input.bindAction("trackingModeSwitch", EKeyState::Pressed, [&]() { g_workspaceDiwne->trackingModeSwitch(); });
	Input.bindAction("trackingSwitch", EKeyState::Pressed, [&]() { g_workspaceDiwne->trackingSwitch(); });
	Input.bindAction("trackingSwitchOn", EKeyState::Pressed, [&]() { g_workspaceDiwne->trackingSwitchOn(); });
	Input.bindAction("trackingSwitchOff", EKeyState::Pressed, [&]() { g_workspaceDiwne->trackingSwitchOff(); });

	App::getModule<StateManager>().setOriginator(this);
}

WorkspaceWindow::~WorkspaceWindow()
{
	g_workspaceDiwne->m_workspaceCoreNodes.clear();
	delete g_workspaceDiwne;
}

// Node builder functions.
//

Memento WorkspaceWindow::getState()
{
	Memento memento;

	SerializationVisitor visitor(memento);
	visitor.dump(getNodeEditor().m_workspaceCoreNodes);

	return memento;
}

void WorkspaceWindow::setState(const Memento& memento)
{
	getNodeEditor().m_workspaceCoreNodes.clear();

	NodeDeserializer::createFrom(memento);

	/*
	const auto findNode = [](std::vector<GuiNodePtr>& nodes, Core::ID id) -> GuiNodePtr
	{
		for (const auto& node : nodes)
			if (node->getNodebase()->getId() == id)
				return node;

		return nullptr;
	};

	//

	std::map<Core::ID, Core::ID> oldToNewID;

	getNodeEditor().m_workspaceCoreNodes.clear();

	const auto& operators = memento["workspace"]["operators"];
	for (auto& value : operators.GetArray())
	{
		const auto node = NodeDeserializer::createOperator(value);
		oldToNewID[value["id"].GetInt()] = node->getNodebase()->getId();
	}

	//

	for (auto& value : memento["workspace"]["sequences"].GetArray())
	{
		const auto node = NodeDeserializer::createSequence(value);
		oldToNewID[value["id"].GetInt()] = node->getNodebase()->getId();
	}

	//

	for (auto& value : memento["workspace"]["cycles"].GetArray())
	{
		const auto cycle = addNodeToNodeEditorNoSave<WorkspaceCycle>();
		NodeDeserializer::assignCommon(value, cycle);
		oldToNewID[value["id"].GetInt()] = cycle->getNodebase()->getId();
	}

	//

	for (auto& value : memento["workspace"]["cameras"].GetArray())
	{
		const auto camera = addNodeToNodeEditorNoSave<WorkspaceCamera>();
		NodeDeserializer::assignCommon(value, camera);
		oldToNewID[value["id"].GetInt()] = camera->getNodebase()->getId();

		const auto& viewValue = value["sequences"].GetArray()[0];
		NodeDeserializer::assignSequence(viewValue, camera->getView());

		const auto& projValue = value["sequences"].GetArray()[1];
		NodeDeserializer::assignSequence(projValue, camera->getProjection());
	}

	//

	for (auto& value : memento["workspace"]["screens"].GetArray())
	{
		const auto screen = addNodeToNodeEditorNoSave<WorkspaceScreen>();
		NodeDeserializer::assignCommon(value, screen);
		oldToNewID[value["id"].GetInt()] = screen->getNodebase()->getId();
	}

	//

	for (auto& value : memento["workspace"]["models"].GetArray())
	{
		const auto model = addNodeToNodeEditorNoSave<WorkspaceModel>();
		NodeDeserializer::assignCommon(value, model);
		oldToNewID[value["id"].GetInt()] = model->getNodebase()->getId();
	}

	//

	const auto& transforms = memento["workspace"]["transforms"];
	for (auto& value : transforms.GetArray())
	{
		const auto transform = NodeDeserializer::createTransform(value);
		oldToNewID[value["id"].GetInt()] = transform->getNodebase()->getId();
	}

	// connect edges

	const auto& edges = memento["workspace"]["edges"];
	auto& workspaceNodes = getNodeEditor().m_workspaceCoreNodes;

	for (auto& edge : edges.GetArray())
	{
		auto oldLhsID = edge[0].GetInt();
		auto oldRhsID = edge[2].GetInt();
		auto lhsID = oldToNewID.at(edge[0].GetInt());
		auto rhsID = oldToNewID.at(edge[2].GetInt());
		auto lhs = findNode(workspaceNodes, lhsID);
		auto rhs = findNode(workspaceNodes, rhsID);
		if (lhs && rhs)
		{
			auto lhsPin = edge[1].GetInt();
			auto rhsPin = edge[3].GetInt();

			connectNodesNoSave(lhs, rhs, lhsPin, rhsPin);
		}
	}
	 */
}

void WorkspaceWindow::clear() { getNodeEditor().m_workspaceCoreNodes.clear(); }

//

WorkspaceDiwne& WorkspaceWindow::getNodeEditor() { return *g_workspaceDiwne; }

void WorkspaceWindow::render()
{

	ImGui::PushStyleColor(ImGuiCol_TabActive, App::get().getUI()->getTheme().get(EColor::DockTabActive));
	/* Draw to window only if is visible - call ImGui::End() everytime */
	if (ImGui::Begin(setName("Workspace").c_str(), getShowPtr(),
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

		ImGui::EndMenu();
	}
}

//

bool connectNodesNoSave(GuiNodePtr lhs, GuiNodePtr rhs, int lhsPin, int rhsPin)
{
	auto plugResult = Core::GraphManager::plug(lhs->getNodebase(), rhs->getNodebase(), lhsPin, rhsPin);
	if (plugResult != ENodePlugResult::Ok)
	{
		LOG_INFO("Cannot connect pin{} to pin{} of nodes {} and {}", lhs->getNodebase()->getSignature(),
		         rhs->getNodebase()->getSignature(), lhsPin, rhsPin);
		return false;
	}
	else
	{
		std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(rhs)
		    ->getInputs()
		    .at(rhsPin)
		    ->setConnectedWorkspaceOutput(
		        std::static_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(lhs)->getOutputs().at(lhsPin).get());
		return true;
	}
}
