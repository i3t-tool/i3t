/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.
*/
#include "WorkspaceWindow.h"


static DIWNE::SettingsDiwne settingsDiwne;

// using namespace Core;

WorkspaceWindow::WorkspaceWindow(bool show)
    :	IWindow(show)
    ,   Diwne(settingsDiwne, this)
    ,   m_wholeApplication(Application::get())
    ,   ConstTouchTime(1.0f)
    //,   m_headerBackgroundTexture( (ImTextureID) (intptr_t) pgr::createTexture(Config::getAbsolutePath("Data/textures/blueprint_background.png"), true)) // \TODO load texture OR making a simple rectangle
//		,m_nodeBuilderContext(util::NodeBuilder(m_headerBackgroundTexture, 64, 64))
{

	/* Setting of configuration from blueprint-example */
//	ne::Config config;											 /* \todo used only once in ed::CreateEditor() ? */
//	config.SettingsFile = "Blueprints.json"; /* \todo Rename to some more tight filename*/
																					 /* \todo Setting of load and store functions
	                                         //    config.LoadNodeSettings = [](ed::NodeId nodeId, char* data, void* userPointer) -> size_t
	                                         //    {
	                                         //        auto node = FindNode(nodeId);
	                                         //        if (!node)
	                                         //            return 0;
	                                         //
	                                         //        if (data != nullptr)
	                                         //            memcpy(data, node->State.data(), node->State.size());
	                                         //        return node->State.size();
	                                         //    };
	                                         //
	                                         //    config.SaveNodeSettings = [](ed::NodeId nodeId, const char* data, size_t size, ed::SaveReasonFlags reason,
	                                         void* userPointer) -> bool
	                                         //    {
	                                         //        auto node = FindNode(nodeId);
	                                         //        if (!node)
	                                         //            return false;
	                                         //
	                                         //        node->State.assign(data, size);
	                                         //
	                                         //        TouchNode(nodeId);
	                                         //
	                                         //        return true;
	                                         //    };
	                                         */

//	m_nodeEditorContext = ne::CreateEditor(&config);
//	ne::SetCurrentEditor(m_nodeEditorContext);
//	m_ne_usable = reinterpret_cast<ax::NodeEditor::Detail::EditorContext*>(m_nodeEditorContext);

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
//		if (ImGui::MenuItem("sequence"))
//		{
//		    addNodeToPositionOfPopup<WorkspaceSequence>();
//		}
//		if (ImGui::MenuItem("camera"))
//		{
//		    addNodeToPositionOfPopup<WorkspaceCamera>();
//		}
		if (ImGui::MenuItem("model"))
		{
		    addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Model>>();
		}
		if (ImGui::MenuItem("pulse"))
		{
		    addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Pulse>>();
		}
		if (ImGui::MenuItem("screen"))
		{
		    addNodeToPositionOfPopup<WorkspaceOperator<ENodeType::Screen>>();
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
                    m_workspaceCoreNodes.erase(
                        std::remove_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
                                      [](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {return node->getSelected();}
                                      ),
                          m_workspaceCoreNodes.end());
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
//        m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceSequence>());
//        m_workspaceCoreNodes.back()->setNodePositionDiwne(ImVec2(700,200));


        m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceOperator<ENodeType::MakeTranslation>>());
        m_workspaceCoreNodes.back()->setNodePositionDiwne(ImVec2(700,200));

        m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceTransformationFree>());
        m_workspaceCoreNodes.back()->setNodePositionDiwne(ImVec2(1000,200));



//        m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceSequence>());
//        m_workspaceCoreNodes.back()->setNodePositionDiwne(ImVec2(1200,300));
//
//        std::shared_ptr<WorkspaceSequence> seq = std::static_pointer_cast<WorkspaceSequence>(m_workspaceCoreNodes.back());
//        std::shared_ptr<WorkspaceTransformationFree> trans = std::make_shared<WorkspaceTransformationFree>();
//        trans->setNodePositionDiwne(ImVec2(1200,500));
//        std::shared_ptr<WorkspaceTransformationFree> trans2 = std::make_shared<WorkspaceTransformationFree>();
//        trans2->setNodePositionDiwne(ImVec2(1400,500));
//        seq->pushNode(trans, 0);
//        seq->pushNode(trans2, 1);
    }

	Begin("DIWNE Workspace");

    m_workspaceCoreNodes.erase(std::remove_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
                                              [](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool { return node->getRemoveFromWorkspaceWindow();}
                                              ),
                              m_workspaceCoreNodes.end());
	for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
    {
        if (workspaceCoreNode != nullptr) m_inner_interaction_happen |= workspaceCoreNode->drawNodeDiwne(*this); /* nullptr can happen if moving to sequence */

    }

//    if (m_diwne.getBackgroudPopupRaise())
//    {
//        m_diwne.popupDiwneRaise("BackgroundPopup", &backgroundPopupContent, m_diwne, m_workspaceCoreNodes);
//    }

//	ne::Begin("Node editor");
//	{
//		ImVec2 cursorTopLeft = ImGui::GetCursorScreenPos();
//
//		for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
//		{
//			Theme& t = I3T::getTheme();
//			if (workspaceCoreNode->isTransformation()) { t.transformationColorTheme(); }
//			else
//			{
//				t.operatorColorTheme();
//			}
//			workspaceCoreNode->drawNode(m_nodeBuilderContext, nullptr);
//			t.returnFloatColorToDefault();
//		}
//
////		/* put and pop to/from Sequence */
//		if (m_diwneAction == DIWNE::DiwneAction::DragNode)
//		{
//		    WorkspaceNodeWithCoreData *node = (WorkspaceNodeWithCoreData *)m_draged_node; /* \todo JH better way to cast */
//			if (node != nullptr) /* should not be never if DiwneAction::DragNode is set */
//			{
//				if (node->isTransformation())
//				{
////					Theme& t = I3T::getTheme();
////					t.transformationColorTheme();
////					if (node->inSequence())
////					{
////						Ptr<WorkspaceSequence> workspace_sequence = getSequenceOfWorkspaceNode(node);
////						assert(workspace_sequence != nullptr);
////
////						workspace_sequence->popNode(node);
////						m_workspaceCoreNodes.push_back(node);
////					}
//
//					int position_in_sequence;
//					m_all_sequences = getSequenceNodes();
//					for (Ptr<WorkspaceSequence>& sequence : m_all_sequences)
//					{
//						ImVec2 nodeTopMiddlePosition = ne::GetNodePosition(node->getId());
//						ImVec2 nodeSize							 = ne::GetNodeSize(node->getId());
//						nodeTopMiddlePosition.x += nodeSize.x / 2;
//
//						position_in_sequence =
//								sequence->getInnerPosition({nodeTopMiddlePosition, nodeTopMiddlePosition + ImVec2(0, nodeSize.y / 2),
//																						nodeTopMiddlePosition + ImVec2(0, nodeSize.y)});
//						sequence->setPostionOfDummyData(position_in_sequence);
//
//						if (position_in_sequence >= 0)
//						{
//							sequence->setWidthOfDummy(ne::GetNodeSize(node->getId()).x);
//							if (ImGui::IsMouseReleased(0))
//							{
//								sequence->pushNode(node, position_in_sequence);
//								m_workspaceCoreNodes.erase(std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
//																												[this](Ptr<WorkspaceNodeWithCoreData> const& w_node) -> bool {
//																													return w_node->getId() == m_draged_node->getId();
//																												}));
//								sequence->setPostionOfDummyData(-1);
//								break;
//							}
//						}
//					}
//				}
//			}
//		}
//
//		/* both connected pins have to be drawn before link is drawn -> therefore separated cycle */
//		for (auto&& workspaceCoreNode : m_workspaceCoreNodes) { workspaceCoreNode->drawInputLinks(); }
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

	//manipulatorStartCheck3D();

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
//void WorkspaceWindow::shiftNodesToFront(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodesToShift)
//{
//	for (int i = 0; i < nodesToShift.size(); i++)
//	{
//		coreNodeIter ith_selected_node =
//				std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
//										 [nodesToShift, i](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {
//											 return node->getId() == nodesToShift.at(i)->getId();
//										 });
//
//		if (ith_selected_node != m_workspaceCoreNodes.end())
//		{
//			std::iter_swap(m_workspaceCoreNodes.begin() + i, ith_selected_node);
//		}
//	}
//}
//
//void WorkspaceWindow::shiftNodesToBack(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodesToShift)
//{
//	for (int i = 0; i < nodesToShift.size(); i++)
//	{
//		coreNodeIter ith_selected_node =
//				std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
//										 [nodesToShift, i](Ptr<WorkspaceNodeWithCoreData> const& node) -> bool {
//											 return node->getId() == nodesToShift.at(i)->getId();
//										 });
//
//		if (ith_selected_node != m_workspaceCoreNodes.end())
//		{
//			std::iter_swap(m_workspaceCoreNodes.end() - 1 - i, ith_selected_node);
//		}
//	}
//}
//
///* \todo JH not used now (with change in NodeEditor) - not work yet in all cases - should avoid capturing actions in bottom nodes when overlaping ( https://github.com/thedmd/imgui-node-editor/issues/81 ) */
//void WorkspaceWindow::shiftSelectedNodesToFront()
//{
//	if (ne::HasSelectionChanged()) { shiftNodesToFront(getSelectedWorkspaceCoreNodes()); }
//}
//
//
//void WorkspaceWindow::manipulatorStartCheck3D()
//{
//	Ptr<WorkspaceMatrix4x4> selectedWorkspaceMatrix4x4 = nullptr;
//	ne::NodeId							selectedNodeID;
//
//	if (ne::HasSelectionChanged())
//	{
//		World* world = Application::get().world();
//
//		if (ne::GetSelectedObjectCount() == 1 && ne::GetSelectedNodes(&selectedNodeID, 1) == 1)
//		{
//			selectedWorkspaceMatrix4x4 =
//					std::dynamic_pointer_cast<WorkspaceMatrix4x4>(getWorkspaceCoreNodeByID(selectedNodeID));
//		}
//
//		world->manipulatorsSetMatrix(&selectedWorkspaceMatrix4x4, nullptr);
//	}
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
//Ptr<WorkspaceNodeWithCoreData> WorkspaceWindow::getWorkspaceCoreNodeByID(ne::NodeId const id)
//{
//	if (id)
//	{
//		for (Ptr<WorkspaceNodeWithCoreData> const& node : m_workspaceCoreNodes)
//		{
//			if (node->getId() == id) { return node; }
//			else
//			{
//				if (node->isSequence())
//				{
//					for (auto&& inSequenceNode : std::dynamic_pointer_cast<WorkspaceSequence>(node)->getInnerWorkspaceNodes())
//					{
//						if (inSequenceNode->getId() == id) { return inSequenceNode; }
//					}
//				}
//			}
//		}
//	}
//	return nullptr;
//}
//
//Ptr<WorkspaceNodeWithCoreData> WorkspaceWindow::getWorkspaceCoreNodeByPinID(ne::PinId const id)
//{
//	/* nodes in Sequence have no visible/usable pins */
//	if (id)
//	{
//		for (Ptr<WorkspaceNodeWithCoreData> const& node : m_workspaceCoreNodes)
//		{
//			for (Ptr<WorkspaceCorePinProperties> const& pin : node->getInputsProperties())
//			{
//				if (pin->getId() == id) { return node; }
//			}
//
//			for (Ptr<WorkspaceCorePinProperties> const& pin : node->getOutputsProperties())
//			{
//				if (pin->getId() == id) { return node; }
//			}
//		}
//	}
//
//	return nullptr;
//}
//
//Ptr<WorkspaceCorePinProperties> WorkspaceWindow::getWorkspacePinPropertiesByID(ne::PinId const id)
//{
//	if (id)
//	{
//		for (Ptr<WorkspaceNodeWithCoreData> const& node : m_workspaceCoreNodes)
//		{
//			for (Ptr<WorkspaceCorePinProperties> const& pin : node->getInputsProperties())
//			{
//				if (pin->getId() == id) { return pin; }
//			}
//
//			for (Ptr<WorkspaceCorePinProperties> const& pin : node->getOutputsProperties())
//			{
//				if (pin->getId() == id) { return pin; }
//			}
//		}
//	}
//
//	return nullptr;
//}
//
//std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceWindow::getSelectedWorkspaceCoreNodes()
//{
//	std::vector<Ptr<WorkspaceNodeWithCoreData>> allSelectedCoreNodes;
//	std::vector<ne::NodeId>											allSelectedNodesIDs;
//	Ptr<WorkspaceNodeWithCoreData>							temp;
//	//ne::SelectNode();
//	int numOfSelectedObjects = ne::GetSelectedObjectCount(); /* not Nodes only */
//
//	allSelectedNodesIDs.resize(numOfSelectedObjects);
//	int numOfSelectedNodes = ne::GetSelectedNodes(allSelectedNodesIDs.data(), numOfSelectedObjects);
//	allSelectedNodesIDs.shrink_to_fit();
//
//	allSelectedCoreNodes.reserve(numOfSelectedNodes);
//	for (auto nodeId : allSelectedNodesIDs)
//	{
//		temp = getWorkspaceCoreNodeByID(nodeId);
//		if (temp) { allSelectedCoreNodes.push_back(temp); }
//	}
//	allSelectedCoreNodes.shrink_to_fit();
//
//	return allSelectedCoreNodes;
//}



//void WorkspaceWindow::checkQueryElementsCreating()
//{
//	if (ne::BeginCreate(ImColor(255, 255, 255), 2.0f))
//	{
//		checkQueryLinkCreate();
//		// checkQueryNewNode(); /* \todo JH not work well.. block other manipulations */
//	}
//	else
//	{
//		m_pinPropertiesForNewLink = nullptr;
//	}
//	ne::EndCreate();
//}
//
//void WorkspaceWindow::checkQueryElements()
//{
//	if (!m_createNewNode)
//	{
//		checkQueryElementsDeleting();
//		checkQueryElementsCreating();
//	}
//}
//
//void WorkspaceWindow::checkQueryLinkCreate()
//{
//	ne::PinId startPinId = 0, endPinId = 0;
//	if (ne::QueryNewLink(&startPinId, &endPinId))
//	{
//		Ptr<WorkspaceCorePinProperties> startPin = getWorkspacePinPropertiesByID(startPinId);
//		Ptr<WorkspaceCorePinProperties> endPin	 = getWorkspacePinPropertiesByID(endPinId);
//
//		/* \todo JH comment where it is used */
//		m_pinPropertiesForNewLink = startPin ? startPin : endPin;
//
//		if (startPin && endPin) /* \todo JH check kind in Core? */
//		{
//			if (startPin->getKind() == PinKind::Input) { std::swap(startPin, endPin); }
//
//			switch (Core::GraphManager::isPlugCorrect(&(endPin->getCorePin()), &(startPin->getCorePin())))
//			{
//			case ENodePlugResult::Ok:
//				showPopUpLabel("Connection possible", I3T::getColor(EColor::Nodes_ConnectionPossible));
//				if (!ImGui::GetIO().MouseDown[0])
//				{
//					Core::GraphManager::plug(startPin->getNode().getNodebase(), endPin->getNode().getNodebase(),
//																	 startPin->getIndex(), endPin->getIndex());
//				}
//				break;
//			/* \todo JH react informatively to other result too */
//			default:
//				showPopUpLabel("Connection not possible", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
//			}
//		}
//	}
//}
//
//void WorkspaceWindow::checkQueryNodeCreate()
//{
//	ne::PinId pinId = 0;
//	if (ne::QueryNewNode(&pinId))
//	{
//		m_pinPropertiesForNewLink = getWorkspacePinPropertiesByID(pinId);
//		if (m_pinPropertiesForNewLink) { showPopUpLabel("+ Create Node", I3T::getColor(EColor::Nodes_CreateNode)); }
//
//		if (ne::AcceptNewItem())
//		{
//			m_createNewNode								= true;
//			m_pinPropertiesForNewNodeLink = m_pinPropertiesForNewLink;
//			m_pinPropertiesForNewLink			= nullptr;
//
//			ne::Suspend();
//			ImGui::OpenPopup("Create New Node");
//			ne::Resume();
//		}
//	}
//}
//
//void WorkspaceWindow::checkQueryElementsDeleting()
//{
//	if (ne::BeginDelete())
//	{
//		checkQueryLinkDelete();
//		checkQueryNodeDelete();
//	}
//	ne::EndDelete();
//}
//
//void WorkspaceWindow::checkQueryLinkDelete()
//{
//	ne::LinkId linkId = 0;
//	while (ne::QueryDeletedLink(&linkId))
//	{
//		if (ne::AcceptDeletedItem())
//		{
//			Ptr<WorkspaceCorePinProperties> inputPin = getWorkspacePinPropertiesByID(
//					ne::PinId(linkId.Get())); /* link has same id as pin to which is connected as input */
//
//			Core::GraphManager::unplugInput(inputPin->getNode().getNodebase(), inputPin->getIndex());
//		}
//	}
//}
//
//void WorkspaceWindow::checkQueryNodeDelete()
//{
//	ne::NodeId nodeId = 0;
//	while (ne::QueryDeletedNode(&nodeId))
//	{
//		if (ne::AcceptDeletedItem()) { NodeDelete(nodeId); }
//	}
//}
//
//void WorkspaceWindow::NodeDelete(ne::NodeId const nodeId)
//{
//	coreNodeIter node_iter =
//			std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
//									 [nodeId](Ptr<WorkspaceNodeWithCoreData>& node) { return node->getId() == nodeId; });
//	if (node_iter != m_workspaceCoreNodes.end()) { m_workspaceCoreNodes.erase(node_iter); }
//}
//
//void WorkspaceWindow::selectAll()
//{
//	for (Ptr<WorkspaceNodeWithCoreData> const& node : m_workspaceCoreNodes) { ne::SelectNode(node->getId(), true); }
//}
//void WorkspaceWindow::invertSelection()
//{
//	auto selected = m_ne_usable->GetSelectedObjects();
//	bool all			= true;
//
//	for (Ptr<WorkspaceNodeWithCoreData> const& node : m_workspaceCoreNodes)
//	{
//
//		for (auto s : selected)
//		{
//			try
//			{
//				auto n = s->AsNode();
//				if (n->m_ID == node->getId())
//				{
//					ne::DeselectNode(node->getId());
//					all = false;
//					break;
//				}
//			}
//			catch (...)
//			{
//				continue;
//			}
//		}
//
//		if (all) { ne::SelectNode(node->getId(), true); }
//
//		all = true;
//	}
//}
//
//void WorkspaceWindow::UpdateTouchAllNodes()
//{
//	const auto deltaTime = ImGui::GetIO().DeltaTime;
//	for (auto&& workspaceNode : m_workspaceCoreNodes) { workspaceNode->UpdateTouch(deltaTime); }
//}

//void WorkspaceWindow::checkQueryContextMenus()
//{
//	ne::Suspend();
//	if (ne::ShowBackgroundContextMenu())
//	{
//		ImGui::OpenPopup("Create New Node"); /* \todo JH take string from some constants file */
//	}
//	else if (ne::ShowNodeContextMenu(&m_contextNodeId))
//	{
//		Ptr<WorkspaceNodeWithCoreData> node = getWorkspaceCoreNodeByID(m_contextNodeId);
//		if (node->fw.showMyPopup)
//		{
//			ImGui::OpenPopup("float_context_menu");
//			node->fw.showMyPopup = false;
//		}
//		else
//		{
//			ImGui::OpenPopup("Node Context Menu");
//		}
//	}
//	ne::Resume();
//
//	ne::Suspend();
//
//
//	if (ImGui::BeginPopup("Create New Node"))
//	{
//		m_newNodePostion = m_rightClickPosition;
//
//		ImGui::Text("add...");
//		ImGui::Separator();
//		if (ImGui::BeginMenu("transformation"))
//		{
//
//			ImGui::Text("add transforamtion");
//			ImGui::Separator();
//			if (ImGui::MenuItem("free"))
//			{
//				m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceTransformationFree>(m_headerBackgroundTexture));
//				ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//			}
//			if (ImGui::MenuItem("translation"))
//			{
//				m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixTranslation>(m_headerBackgroundTexture));
//				ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//			}
//			if (ImGui::BeginMenu("rotation"))
//			{
//
//				ImGui::Text("add rotation");
//				ImGui::Separator();
//				if (ImGui::MenuItem("eulerAngleX"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceEulerX>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("eulerAngleY"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceEulerY>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("eulerAngleZ"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceEulerZ>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("rotate"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceAxisAngle>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatRot>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			if (ImGui::MenuItem("scale"))
//			{
//				m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixScale>(m_headerBackgroundTexture));
//				ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//			}
//			if (ImGui::MenuItem("lookAt"))
//			{
//				m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceLookAt>(m_headerBackgroundTexture));
//				ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//			}
//			if (ImGui::BeginMenu("projection"))
//			{
//
//				ImGui::Text("add projection");
//				ImGui::Separator();
//				if (ImGui::MenuItem("ortho"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceOrtho>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("perspective"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspacePerspective>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("frustrum"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFrustum>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			ImGui::EndMenu();
//		}
//		if (ImGui::BeginMenu("operator"))
//		{
//
//			ImGui::Text("add operator");
//			ImGui::Separator();
//			if (ImGui::BeginMenu("transformation"))
//			{
//
//				ImGui::Text("transformation operators");
//				ImGui::Separator();
//				if (ImGui::MenuItem("translate"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeTranslation>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("euler AngleX"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeEulerX>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("euler AngleY"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeEulerY>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("euler AngleZ"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeEulerZ>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("rotate"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeAxisAngle>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("scale"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeScale>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("ortho"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeOrtho>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("perspective"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakePerspective>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("frustrum"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeFrustum>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("lookAt"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeLookAt>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("matrix"))
//			{
//
//				ImGui::Text("matrix operators");
//				ImGui::Separator();
//				if (ImGui::MenuItem("matrix"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixFree>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("trackball"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceTrackball>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("inversion"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixInversion>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("transpose"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixTranspose>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("determinant"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceDeterminant>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mat * mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixMulMatrix>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mat + mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixAddMatrix>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mat * vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixMulVector>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec4 * mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorMulMatrix>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float * mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixMulFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("vec3"))
//			{
//
//				ImGui::Text("vec3 operator");
//				ImGui::Separator();
//				if (ImGui::MenuItem("vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3Free>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("show vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceShowVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec3 x vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3CrossVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec3 . vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3DotVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec3 + vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3AddVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec3 - vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3SubVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float * vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3MulFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("normalize vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceNormalizeVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("length(vec3)"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3Length>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mix vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMixVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("vec4"))
//			{
//
//				ImGui::Text("vec4 operator");
//				ImGui::Separator();
//				if (ImGui::MenuItem("vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceVectorFree>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec4 . vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorDotVector>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec4 + vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorAddVector>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec4 - vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorSubVector>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float * vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorMulFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("normalize vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceNormalizeVector>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("perspective division"))
//				{
//					m_workspaceCoreNodes.push_back(
//							std::make_unique<WorkspaceVectorPerspectiveDivision>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mix vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMixVector>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("quat"))
//			{
//
//				ImGui::Text("quat operator");
//				ImGui::Separator();
//				if (ImGui::MenuItem("quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatFree>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat(float, vec3)"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatVecToQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat(angle, axis)"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceAngleAxisToQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat(vec3, vec3)"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVecVecToQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat -> float, vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatToFloatVec>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat -> angle, axis"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatToAngleAxis>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float * quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatMulQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat * quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatMulQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat -> euler"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatToEuler>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("euler -> quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceEulerToQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("slerp"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatSlerp>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("long way slerp"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatLongWaySlerp>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("lerp"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatLerp>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat conjugate"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceConjQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("qvq*"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceQuatVecConjQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("inverse quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatInverse>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("normalize quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceNormalizeQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("length(quat)"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceQuatLength>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("float"))
//			{
//
//				ImGui::Text("float operator");
//				ImGui::Separator();
//				if (ImGui::MenuItem("float"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceFloatFree>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("clamp float"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceClampFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float cycle"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceCycle>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float * float"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatMulFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float / float"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatDivFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float + float"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatAddFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("float ^ float"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatPowFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mix float"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMixFloat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("sin & cos(float)"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatSinCos>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("asin & acos(float)"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceASinACos>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("signum"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceSignum>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("conversion"))
//			{
//
//				ImGui::Text("conversion operator");
//				ImGui::Separator();
//				if (ImGui::MenuItem("mat -> TR"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixToTR>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("TR -> mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceTRToMatrix>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mat -> vecs4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixToVectors>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mat -> quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixToQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("mat -> floats"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixToFloats>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vecs4 -> mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorsToMatrix>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec4 -> vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorToVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec4 -> floats"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorToFloats>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vecs3 -> mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectors3ToMatrix>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec3 -> vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3ToVector>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("vec3 -> floats"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3ToFloats>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat -> mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatToMatrix>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("quat -> floats"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceQuatToFloats>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("floats -> mat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatsToMatrix>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("floats -> vec4"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatsToVector>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("floats -> vec3"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatsToVector3>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				if (ImGui::MenuItem("floats -> quat"))
//				{
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatsToQuat>(m_headerBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//				}
//				ImGui::EndMenu();
//			}
//			ImGui::EndMenu();
//		}
//		if (ImGui::MenuItem("sequence"))
//		{
//			m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceSequence>(m_headerBackgroundTexture));
//			ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//		}
//		if (ImGui::MenuItem("camera"))
//		{
//			m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceCamera>(m_headerBackgroundTexture));
//			ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//		}
//		if (ImGui::MenuItem("model"))
//		{
//			m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceModel>(m_headerBackgroundTexture));
//			ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//		}
//		if (ImGui::MenuItem("pulse"))
//		{
//			m_workspaceCoreNodes.push_back(std::make_shared<WorkspacePulse>(m_headerBackgroundTexture));
//			ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//		}
//		if (ImGui::MenuItem("screen"))
//		{
//			m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceScreen>(m_headerBackgroundTexture));
//			ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
//		}
//
//		ImGui::Separator();
//
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
//
//		ImGui::EndPopup();
//	}
//
//	ne::Resume();
//}

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
