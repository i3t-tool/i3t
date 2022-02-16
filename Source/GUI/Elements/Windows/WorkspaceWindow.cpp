/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.
*/
#include "WorkspaceWindow.h"

#ifdef DIWNE_DEBUG
    bool s_diwneDebug_on;
#endif // DIWNE_DEBUG

static DIWNE::SettingsDiwne settingsDiwne;

WorkspaceDiwne WorkspaceWindow::m_workspaceDiwne(&settingsDiwne);


//std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceWindow::m_workspaceDiwne::m_workspaceCoreNodes;

// using namespace Core;

/* ======================================== */
/* ===== W o r k s p a c e  D i w n e ===== */
/* ======================================== */
WorkspaceDiwne::WorkspaceDiwne(DIWNE::SettingsDiwne* settingsDiwne)
    :   Diwne(settingsDiwne)
    ,   m_workspaceDiwneAction(WorkspaceDiwneAction::None)
    ,   m_workspaceDiwneActionPreviousFrame(WorkspaceDiwneAction::None)

{
}

void WorkspaceDiwne::selectAll()
{
    for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
    {
        workspaceCoreNode->setSelected(true);

        Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(workspaceCoreNode);
        if (seq != nullptr)
        {
            for (auto&& nodeInSequence : seq->getInnerWorkspaceNodes())
            {
                nodeInSequence->setSelected(true);
            }
        }
    }
}

//AddRectFilledDiwne(m_selectionRectangeDiwne.Min, m_selectionRectangeDiwne.Max,
//                           dragDelta.x > 0 ? ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::SelectionRectFull)) : ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::SelectionRectTouch)) );


void WorkspaceDiwne::popupContent()
{
        ImGui::Text("add...");
		ImGui::Separator();
		if (ImGui::BeginMenu("transformation"))
		{
            /* \todo JH can be done by for-cycle if somewhere is list of types with group (transformation, operator, ...) and label*/
			if (ImGui::MenuItem("free"))
			{
			    addNodeToPositionOfPopup<WorkspaceTransformationFree>();
			}
			if (ImGui::MenuItem("translation"))
			{
			    addNodeToPositionOfPopup<WorkspaceTransformationTranslation>();
			}

			if (ImGui::BeginMenu("rotation"))
			{

				if (ImGui::MenuItem("eulerAngleX"))
				{
				    addNodeToPositionOfPopup<WorkspaceTransformationEulerX>();
				}
				if (ImGui::MenuItem("eulerAngleY"))
				{
				    addNodeToPositionOfPopup<WorkspaceTransformationEulerY>();
				}
				if (ImGui::MenuItem("eulerAngleZ"))
				{
				    addNodeToPositionOfPopup<WorkspaceTransformationEulerZ>();
				}
				if (ImGui::MenuItem("axisAngle"))
				{
				    addNodeToPositionOfPopup<WorkspaceTransformationAxisAngle>();
				}
				if (ImGui::MenuItem("quat"))
				{
				    addNodeToPositionOfPopup<WorkspaceTransformationQuaternion>();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("scale"))
			{
			    addNodeToPositionOfPopup<WorkspaceTransformationScale>();
			}
			if (ImGui::MenuItem("lookAt"))
			{
			    addNodeToPositionOfPopup<WorkspaceTransformationLookAt>();
			}
			if (ImGui::BeginMenu("projection"))
			{
				if (ImGui::MenuItem("ortho"))
				{
				    addNodeToPositionOfPopup<WorkspaceTransformationOrthoProjection>();
				}
				if (ImGui::MenuItem("perspective"))
				{
                    addNodeToPositionOfPopup<WorkspaceTransformationPerspective>();
				}
				if (ImGui::MenuItem("frustrum"))
				{
				    addNodeToPositionOfPopup<WorkspaceTransformationFrustum>();
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
				    //addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::AngleAxisToQuat>>();
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

		if (ImGui::BeginMenu("Select"))
        {
            if (ImGui::MenuItem("All nodes"))
            {
                selectAll();
            }
        ImGui::EndMenu();
        }

        ImGui::Separator();

		if (ImGui::BeginMenu("Delete"))
			{
				if (ImGui::MenuItem("All nodes"))
				{
				    m_workspaceCoreNodes.clear();
				}
				if (ImGui::MenuItem("All links"))
				{
				    for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
                    {
                        Ptr<WorkspaceNodeWithCoreDataWithPins> node = std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(workspaceCoreNode);
                        if (node != nullptr)
                        {
                            for (auto && pin : node->getInputs())
                            {
                                pin->unplug();
                            }
                        }
                    }
				}
				if (ImGui::MenuItem("Selected nodes"))
				{
				    for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
                    {
                        if (workspaceCoreNode->getSelected())
                        {
                            workspaceCoreNode->setRemoveFromWorkspace(true);
                        }
                        else
                        {
                            Ptr<WorkspaceSequence> seq = std::dynamic_pointer_cast<WorkspaceSequence>(workspaceCoreNode);
                            if (seq != nullptr)
                            {
                                for (auto&& nodeInSequence : seq->getInnerWorkspaceNodes())
                                {
                                    if (nodeInSequence->getSelected()){ std::dynamic_pointer_cast<WorkspaceTransformation>(nodeInSequence)->setRemoveFromSequence(true); }
                                }
                            }
                        }



                    }
				}
				if (ImGui::MenuItem("Selected links"))
				{
				    for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
                    {
                        Ptr<WorkspaceNodeWithCoreDataWithPins> node = std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(workspaceCoreNode);
                        if (node != nullptr)
                        {
                            for (auto && pin : node->getInputs())
                            {
                                if (pin->getLink().getSelected())
                                {
                                    pin->unplug();
                                }
                            }
                        }
                    }
				}
				ImGui::EndMenu();
			}

//		if (ImGui::BeginMenu("selection"))
//		{
//
//			ImGui::Text("selection");
//			ImGui::Separator();
//			if (ImGui::MenuItem("select all", "a")) { selectAll(); }
//			if (ImGui::MenuItem("invert", "i")) { invertSelection(); }
//			ImGui::EndMenu();
//		}
//		if (ImGui::BeginMenu("zoom"))
//		{
//
//			ImGui::Text("zoom");
//			ImGui::Separator();
//			if (ImGui::MenuItem("to all", "s")) { ne::NavigateToContent(); }
//			if (ImGui::MenuItem("to selection", "d")) { ne::NavigateToSelection(); }
//			ImGui::EndMenu();
//		}


}

bool WorkspaceDiwne::beforeBegin()
{
    m_workspaceDiwneAction = WorkspaceDiwneAction::None;

    return false;
}

bool WorkspaceDiwne::beforeContent()
{
#ifdef DIWNE_DEBUG
DIWNE_DEBUG((*this), {
    if (m_workspaceDiwneActionPreviousFrame == WorkspaceDiwneAction::None) ImGui::Text("WorkspaceWindowAction::None");
    if (m_workspaceDiwneActionPreviousFrame == WorkspaceDiwneAction::CreateAndPlugTypeConstructor) ImGui::Text("WorkspaceWindowPrevAction::CreateAndPlugTypeConstructor");

    ImGui::TextUnformatted(fmt::format("WorkspaceNodes: {}", m_workspaceCoreNodes.size()).c_str());
});
#endif // DIWNE_DEBUG
    return false;
}

bool WorkspaceDiwne::content()
{
    bool interaction_happen = false;
    m_workspaceCoreNodes.erase(std::remove_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
                                              [](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool { return node->getRemoveFromWorkspace();}
                                              ),
                              m_workspaceCoreNodes.end());

    /* draw nodes from back to begin (front to back) for catch interactions in right order */
    int prev_size = m_workspaceCoreNodes.size();
    DIWNE::DrawMode draw_mode = DIWNE::DrawMode::Interacting;
    for (auto it = m_workspaceCoreNodes.rbegin(); it != m_workspaceCoreNodes.rend(); ++it)
    {
        draw_mode = interaction_happen ? DIWNE::DrawMode::JustDraw : DIWNE::DrawMode::Interacting; /* if interaction happen in one node -> no interaction in another */
        if ((*it) != nullptr) interaction_happen |= (*it)->drawNodeDiwne<WorkspaceNodeWithCoreData>(DIWNE::DrawModeNodePosition::OnItsPosition, draw_mode);
        if (prev_size != m_workspaceCoreNodes.size()) break; /* when pop from Sequence size of m_workspaceCoreNodes is affected and iterator is invalidated (at least with MVSC) */
    }

    /* two or more Nodes -> draw nodes two times (first for catch interaction, second for draw it in right order (with no interaction)) */
    if (m_workspaceCoreNodes.size() > 0)
    {
        for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
        {
            if (workspaceCoreNode != nullptr) workspaceCoreNode->drawNodeDiwne<WorkspaceNodeWithCoreData>(DIWNE::DrawModeNodePosition::OnItsPosition, DIWNE::DrawMode::JustDraw); /* nullptr can happen if moving to sequence - maybe unused here */
        }
    }

#ifdef DIWNE_DEBUG
DIWNE_DEBUG((*this), {
    if (interaction_happen){ ImGui::Text("Workspace: Interaction with nodes happen");}
});
#endif // DIWNE_DEBUG

    m_interactionAllowed = /*!interaction_happen*/ true;

    return interaction_happen;
}


bool WorkspaceDiwne::afterContent()
{
    bool interaction_happen = false;
    interaction_happen |= processCreateAndPlugTypeConstructor();

    /* ===== reaction to actions ===== */
    manipulatorStartCheck3D();

	if (getNodesSelectionChanged()) {shiftNodesToEnd(getSelectedNodes());}

    /* hold or drag or interacting or new_link && in previous frame not hold neither drag neither interacting neither new_link */
    if ( (m_diwneAction == DIWNE::DiwneAction::DragNode
          || m_diwneAction == DIWNE::DiwneAction::HoldNode
          || m_diwneAction == DIWNE::DiwneAction::InteractingContent
          || m_diwneAction == DIWNE::DiwneAction::NewLink
          || m_diwneAction == DIWNE::DiwneAction::TouchNode)
        /*&&
        !(m_diwneAction_previousFrame == DIWNE::DiwneAction::DragNode
          || m_diwneAction_previousFrame == DIWNE::DiwneAction::HoldNode
          || m_diwneAction_previousFrame == DIWNE::DiwneAction::InteractingContent
          || m_diwneAction_previousFrame == DIWNE::DiwneAction::NewLink
          || m_diwneAction_previousFrame == DIWNE::DiwneAction::TouchNode) */ // for example with pushing to sequence in prev_frame we hold/drag node and than interact content of sequence -> it is hard to cover all possibilities
        )
    {
        shiftInteractingNodeToEnd();
    }
    return interaction_happen;
}

bool WorkspaceDiwne::afterEnd()
{
    m_workspaceDiwneActionPreviousFrame = m_workspaceDiwneAction;
    return false;
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
        }
        return true;
    }
    return false;
}

void WorkspaceDiwne::shiftNodesToBegin(std::vector<Ptr<WorkspaceNodeWithCoreData>> const &nodesToShift)
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

void WorkspaceDiwne::shiftNodesToEnd(std::vector<Ptr<WorkspaceNodeWithCoreData>> const &nodesToShift)
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
			std::iter_swap(m_workspaceCoreNodes.end() - 1 - i, ith_selected_node);
		}
	}
}

void WorkspaceDiwne::shiftInteractingNodeToEnd()
{
    if(mp_lastActiveNode != nullptr && mp_lastActiveNode.get() != m_workspaceCoreNodes.back().get())
    {
        coreNodeIter draged_node_it =
				std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
										 [this](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {
											 return node.get() == this->mp_lastActiveNode.get();
										 });

		if (draged_node_it != m_workspaceCoreNodes.end() && draged_node_it != m_workspaceCoreNodes.end()-1)
		{
			std::rotate(draged_node_it, draged_node_it+1,  m_workspaceCoreNodes.end());
		}
    }
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceDiwne::getSelectedNodes()
{
    std::vector<Ptr<WorkspaceNodeWithCoreData>> selected;
    for (auto const& node : m_workspaceCoreNodes)
    {
        if (node->getSelected()){selected.push_back(node);};
    }
    return selected;
}

void WorkspaceDiwne::manipulatorStartCheck3D()
{
	if (getNodesSelectionChanged())
	{
	    std::vector<Ptr<WorkspaceNodeWithCoreData>> selected_nodes = getSelectedNodes();
	    if (selected_nodes.size() == 1)
        {
            Ptr<WorkspaceTransformation> selected_transformation = std::dynamic_pointer_cast<WorkspaceTransformation>(selected_nodes[0]);
            if (selected_transformation != nullptr){ Application::get().world()->manipulatorsSetMatrix(selected_transformation, nullptr); } /* \todo JH why not pass sequence of transformation? */
        }

	}
}

bool WorkspaceDiwne::bypassZoomAction(){ return InputManager::isAxisActive("scroll") != 0; }
bool WorkspaceDiwne::bypassDragAction(){ return InputManager::isAxisActive("pan") != 0; }
bool WorkspaceDiwne::bypassHoldAction(){return InputManager::isAxisActive("pan") != 0;}
bool WorkspaceDiwne::bypassUnholdAction(){return InputManager::isAxisActive("pan") == 0;}


/* ========================================== */
/* ===== W o r k s p a c e  W i n d o w ===== */
/* ========================================== */
WorkspaceWindow::WorkspaceWindow(bool show)
    :	IWindow(show)
    ,   m_first_frame(true)
    ,   m_wholeApplication(Application::get())
{
    // Input actions for workspace window.
	Input.bindAction("selectAll", EKeyState::Pressed, [&]() { m_workspaceDiwne.selectAll(); });

	InputManager::isActionTriggered("raisePopup", EKeyState::Released);

	InputManager::isAxisActive("dragElement");
	InputManager::isAxisActive("dragEditor");

//	Input.bindAction("invertSelection", EKeyState::Pressed, [&]() { invertSelection(); });
//	Input.bindAction("navigateToContent", EKeyState::Pressed, [&]() { ne::NavigateToContent(); });
//	Input.bindAction("center", EKeyState::Pressed, [&]() { ne::NavigateToSelection(); });
}





void WorkspaceWindow::render()
{
    /* Draw to window only if is visible - call ImGui::End() everytime */
	if (ImGui::Begin(getName("Workspace").c_str(), getShowPtr(), g_WindowFlags | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        if(m_first_frame){
            m_first_frame = false;
            //m_workspaceDiwne.addNodeToPosition<WorkspaceScreen>(ImVec2(700,200));

            //m_workspaceDiwne.addNodeToPosition<WorkspaceCycle>(ImVec2(400,200));

            m_workspaceDiwne.addNodeToPosition<WorkspaceAngleAxisToQuat>(ImVec2(400,200));


    //        m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceOperator<ENodeType::VectorToVector3>>());
    //        m_workspaceCoreNodes.back()->setNodePositionDiwne(ImVec2(700,200));
    //
    //        m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceOperator<ENodeType::MakeTranslation>>());
    //        m_workspaceCoreNodes.back()->setNodePositionDiwne(ImVec2(1000,200));
    //
    //        std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(m_workspaceCoreNodes.back())->getInputs().at(0)->plug(
    //        std::dynamic_pointer_cast<WorkspaceNodeWithCoreDataWithPins>(m_workspaceCoreNodes.front())->getOutputs().at(0).get() );
        }

        if (ImGui::BeginMenuBar())
        {
            showEditMenu();
            ImGui::EndMenuBar();
        }

        m_workspaceDiwne.drawDiwne();

    }ImGui::End();
}

//Ptr<WorkspaceSequence> WorkspaceWindow::getSequenceOfWorkspaceNode(Ptr<WorkspaceNodeWithCoreData> node)
//{
//	Ptr<Core::Transformation> transfomation_nodebase = node->getNodebase()->as<Core::Transformation>();
//	if (transfomation_nodebase)
//	{
//		Ptr<Core::NodeBase> sequence_nodebase = transfomation_nodebase->getCurrentSequence();
//		if (sequence_nodebase)
//		{
//			for (Ptr<WorkspaceNodeWithCoreData> const& workspace_node : m_workspaceCoreNodes)
//			{
//				if (workspace_node->isSequence() && workspace_node->getNodebase() == sequence_nodebase)
//				{
//					return std::dynamic_pointer_cast<WorkspaceSequence>(workspace_node);
//				}
//			}
//		}
//	}
//	return nullptr;
//}
//

//std::vector<Ptr<WorkspaceSequence>> WorkspaceWindow::getSequenceNodes()
//{
//	std::vector<Ptr<WorkspaceSequence>> allSequenceNodes;
//	Ptr<WorkspaceNodeWithCoreData>			temp;
//
//	for (Ptr<WorkspaceNodeWithCoreData> const& node : m_workspaceCoreNodes)
//	{
//		temp = getWorkspaceCoreNodeByID(node->getId());
//		if (temp->isSequence()) { allSequenceNodes.push_back(std::dynamic_pointer_cast<WorkspaceSequence>(temp)); }
//		else if (temp->isCamera())
//		{
//			Ptr<WorkspaceCamera> temp_camera = std::dynamic_pointer_cast<WorkspaceCamera>(temp);
//			allSequenceNodes.push_back(temp_camera->getProjection());
//			allSequenceNodes.push_back(temp_camera->getView());
//		}
//	}
//
//	return allSequenceNodes;
//}
//

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
