/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.
*/
#include "WorkspaceWindow.h"

static DIWNE::SettingsDiwne settingsDiwne;

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceWindow::m_workspaceCoreNodes;

// using namespace Core;

WorkspaceWindow::WorkspaceWindow(bool show)
    :	IWindow(show)
    ,   Diwne(settingsDiwne, this)
    ,   m_wholeApplication(Application::get())
    ,   ConstTouchTime(1.0f)
    //,   m_headerBackgroundTexture( (ImTextureID) (intptr_t) pgr::createTexture(Config::getAbsolutePath("Data/textures/blueprint_background.png"), true)) // \TODO load texture OR making a simple rectangle
//		,m_nodeBuilderContext(util::NodeBuilder(m_headerBackgroundTexture, 64, 64))
{

// Input actions for workspace window.
//	Input.bindAction("selectAll", EKeyState::Pressed, [&]() { selectAll(); });
//	Input.bindAction("invertSelection", EKeyState::Pressed, [&]() { invertSelection(); });
//	Input.bindAction("navigateToContent", EKeyState::Pressed, [&]() { ne::NavigateToContent(); });
//	Input.bindAction("center", EKeyState::Pressed, [&]() { ne::NavigateToSelection(); });

}

WorkspaceWindow::~WorkspaceWindow()
{
	/* lambda for releasing texture \todo what function to call instead of Application_DestroyTexture()?  */
	//    auto releaseTexture = [](ImTextureID& id) {
	//      if (id)
	//      {
	//        Application_DestroyTexture(id);
	//        id = nullptr;
	//      }
	//    };
	//    releaseTexture(HeaderBackground);



}

void WorkspaceWindow::popupBackgroundContent()
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
//			if (ImGui::BeginMenu("matrix"))
//			{
//				if (ImGui::MenuItem("matrix"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceMatrixFree>();
//				}
//				if (ImGui::MenuItem("trackball"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceTrackball>();
//				}
//				if (ImGui::MenuItem("inversion"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceMatrixInversion>();
//				}
//				if (ImGui::MenuItem("transpose"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceMatrixTranspose>();
//				}
//				if (ImGui::MenuItem("determinant"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceDeterminant>();
//				}
//				if (ImGui::MenuItem("mat * mat"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceMatrixMulMatrix>();
//				}
//				if (ImGui::MenuItem("mat + mat"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceMatrixAddMatrix>();
//				}
//				if (ImGui::MenuItem("mat * vec4"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceMatrixMulVector>();
//				}
//				if (ImGui::MenuItem("vec4 * mat"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceVectorMulMatrix>();
//				}
//				if (ImGui::MenuItem("float * mat"))
//				{
//				    addNodeToPositionOfPopup<WorkspaceMatrixMulFloat>();
//				}
//				ImGui::EndMenu();
//			}
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
				    addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::AngleAxisToQuat>>();
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
				if (ImGui::MenuItem("float cycle")) /* \todo JH ??? */
				{
				    // addNodeToPositionOfPopup<WorkspaceCycle>();
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
		    //addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Screen>>();
			addNodeToPositionOfPopup<WorkspaceScreen>();  //PF
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
				    /* \todo JH - disconnect all */
				}
				if (ImGui::MenuItem("Selected nodes"))
				{
				    for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
                    {
                        if (workspaceCoreNode->getSelected()){ workspaceCoreNode->setRemoveFromWorkspaceWindow(true); }

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
				if (ImGui::MenuItem("Selected links"))
				{
				    /* \todo JH */
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

void WorkspaceWindow::render()
{
    /* Draw to window only if is visible - call ImGui::End() everytime */
	if (ImGui::Begin(getName("Workspace").c_str(), getShowPtr(), g_WindowFlags | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {


	if (ImGui::BeginMenuBar())
	{
		showEditMenu();

		ImGui::EndMenuBar();
	}

	//UpdateTouchAllNodes();

    if(first_frame){
        first_frame = false;
        //m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceSequence>());
        //m_workspaceCoreNodes.back()->setNodePositionDiwne(ImVec2(700,200));


        //m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceOperator<ENodeType::VectorToVector3>>());
        //m_workspaceCoreNodes.back()->setNodePositionDiwne(ImVec2(700,200));

    }

	Begin("DIWNE Workspace");
#ifdef DIWNE_DEBUG
	if (m_diwneAction == DIWNE::DiwneAction::DragNode) ImGui::Text("Dragging node");
	if (m_diwneAction == DIWNE::DiwneAction::None) ImGui::Text("NoneAction");
	if (m_diwneAction == DIWNE::DiwneAction::NewLink) ImGui::Text("New link");
#endif // DIWNE_DEBUG

    m_workspaceCoreNodes.erase(std::remove_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
                                              [](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool { return node->getRemoveFromWorkspaceWindow();}
                                              ),
                              m_workspaceCoreNodes.end());
	for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
    {
        if (workspaceCoreNode != nullptr) m_inner_interaction_happen |= workspaceCoreNode->drawNodeDiwne(*this); /* nullptr can happen if moving to sequence */

    }

#ifdef DIWNE_DEBUG
    if (m_diwneAction == DIWNE::DiwneAction::DragNode)
    {
        Ptr<WorkspaceTransformation> tr = std::dynamic_pointer_cast<WorkspaceTransformation>(m_draged_hold_node);
        if (tr)
        {
            std::vector<ImVec2> interaction_points = tr->getInteractionPointsWithSequence();
            ImGui::Text(fmt::format("{}:{} {}:{} {}:{}", interaction_points[0].x, interaction_points[0].y, interaction_points[1].x, interaction_points[1].y, interaction_points[2].x, interaction_points[2].y).c_str());
        }
    }
#endif // DIWNE_DEBUG


//
//		checkUserActions();
//
//		checkQueryElements();
//
//		checkQueryContextMenus();
//
//		ImGui::SetCursorScreenPos(cursorTopLeft);
//	}
//	ne::End();

	manipulatorStartCheck3D();

	if (getNodesSelectionChanged()) {shiftNodesToEnd(getSelectedNodes());}

    /* hold or drag && in previous frame not hold neither drag */
	if ( (m_diwneAction == DIWNE::DiwneAction::DragNode || m_diwneAction == DIWNE::DiwneAction::HoldNode) &&
         !(m_previousFrameDiwneAction == DIWNE::DiwneAction::DragNode || m_previousFrameDiwneAction == DIWNE::DiwneAction::HoldNode))
    {
        shiftDragedOrHoldNodeToEnd();
    }

	//checkQueryContextMenus();

    End();

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
//void WorkspaceWindow::checkUserActions()
//{
//	if (ImGui::IsMouseClicked(1)) /* right button */ { m_rightClickPosition = ImGui::GetMousePos(); }
//}
//
void WorkspaceWindow::shiftNodesToBegin(std::vector<Ptr<WorkspaceNodeWithCoreData>> const &nodesToShift)
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

void WorkspaceWindow::shiftNodesToEnd(std::vector<Ptr<WorkspaceNodeWithCoreData>> const &nodesToShift)
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

void WorkspaceWindow::shiftDragedOrHoldNodeToEnd()
{
    if(m_draged_hold_node != nullptr)
    {
        coreNodeIter draged_node_it =
				std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
										 [this](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {
											 return node->getId() == this->m_draged_hold_node->getId();
										 });

		if (draged_node_it != m_workspaceCoreNodes.end())
		{
			std::iter_swap(m_workspaceCoreNodes.end() - 1, draged_node_it);
		}
    }
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceWindow::getSelectedNodes()
{
    std::vector<Ptr<WorkspaceNodeWithCoreData>> selected;
    for (auto const& node : m_workspaceCoreNodes)
    {
        if (node->getSelected()){selected.push_back(node);};
    }
    return selected;
}

void WorkspaceWindow::manipulatorStartCheck3D()
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
