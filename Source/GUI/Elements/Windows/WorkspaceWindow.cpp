/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.

    Details no here now... Cca in middle is file split between example and used part
*/

#include "WorkspaceWindow.h"

#include "../Nodes/WorkspaceNodeWithCoreData.h"

#include "../Nodes/WorkspaceMatrixTranslation.h"
#include "../Nodes/WorkspaceMatrixScale.h"
#include "../Nodes/WorkspaceMatrixInversion.h"
#include "../Nodes/WorkspaceDeterminant.h"

#include "../Nodes/WorkspaceVector4.h"
#include "../Nodes/WorkspaceNormalizeVector.h"

#include "Core/InputController.h"

// using namespace Core;

WorkspaceWindow::WorkspaceWindow(bool show)
		: IWindow(show), WholeApplication(Application::get()), ConstTouchTime(1.0f),
			HeaderBackgroundTexture((void*)(intptr_t)pgr::createTexture(
					Config::getAbsolutePath("/Source/GUI/Elements/Windows/data/BlueprintBackground.png"),
					true)) // \TODO load texture OR making a simple rectangle
			,
			NodeBuilderContext(util::NodeBuilder(HeaderBackgroundTexture, 64, 64))
{

	/* Setting of configuration from blueprint-example */
	ne::Config config;                       /* \todo used only once in ed::CreateEditor() ? */
	config.SettingsFile = "Blueprints.json"; /* \todo Rename to some more tight filename*/
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

	NodeEditorContext = ne::CreateEditor(&config);

	ne::SetCurrentEditor(NodeEditorContext);

	/* \todo adding nodes here just for testing */
	/*--- TRANSLATION */
	WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>( HeaderBackgroundTexture, WorkspaceMatrixTranslationArgs{} ));
	ne::SetNodePosition(WorkspaceNodes.back()->m_id, ImVec2(-252, 220));

    /*--- TRANSLATION with connection to translation */
	WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>(HeaderBackgroundTexture, WorkspaceMatrixTranslationArgs{.viewScale=WorkspaceViewScale::Label, .headerLabel="MatrixTranslation 2", .nodeLabel="Translation label"}));
	ne::SetNodePosition(WorkspaceNodes.back()->m_id, ImVec2(-300, 351));

	/* \todo JH nyni nejde v Core spojovat dva operatory - proto to nefuguje... */
	Core::GraphManager::plug(static_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(0).get())->m_nodebase,
	                         static_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(1).get())->m_nodebase, 0, 0);

	/*--- MATRIX_4x4 with connection to translation*/
	WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrix4x4>(HeaderBackgroundTexture, WorkspaceMatrix4x4Args{.headerLabel="just Matrix4x4 1"}));
	ne::SetNodePosition(WorkspaceNodes.back()->m_id, ImVec2(-500, 351));

    Core::GraphManager::plug(static_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(0).get())->m_nodebase,
	                         static_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(2).get())->m_nodebase, 0, 0);

    /*--- SCALE */
	WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixScale>(HeaderBackgroundTexture, WorkspaceMatrixScaleArgs{}));
	ne::SetNodePosition(WorkspaceNodes.back()->m_id, ImVec2(-500, 351));

	/*--- NORMALIZE VECTOR */
	WorkspaceNodes.push_back(std::make_unique<WorkspaceNormalizeVector>(HeaderBackgroundTexture, WorkspaceNormalizeVectorArgs{}));
	ne::SetNodePosition(WorkspaceNodes.back()->m_id, ImVec2(100, 400));



	ne::NavigateToContent();
	//////////////////////////////////
		//WorkspaceNodes.at(0).get().
//
//
//	WorkspaceNodeBaseData*wnbd=dynamic_cast<WorkspaceNodeBaseData*>(WorkspaceNodes.at(1).get());
//
//	Ptr<Core::NodeBase> child=wnbd->Nodebase;
//	std::vector<Core::Pin>cp= child->getInputPins();
//	Ptr<Core::NodeBase> parent=Core::GraphManager::getParent(child);
//	const Operation* uu = child->getOperation();
//
//	//cp[0].getParentPin()->getMaster();
//	//WorkspaceNodes.at(0).get()->
//	glm::mat4 m = glm::mat4(0.14f);
//	ValueSetResult vsr=parent->setValue(m);
//	ValueSetResult vsr2= child->setValue(m);
//	//ValueSetResult vsr2= child->setValue(m,Core::Transform::g_Translate);
//	//printf("%d\n",vsr2.status);
//	DataStore ds=child->getData();
//	glm::mat4 mm=ds.getMat4();
//
//	//std::vector<WorkspaceMatrix4x4>*ww=
//
//	LoadWorkspace(Config::getAbsolutePath("/load.txt").c_str(),&WorkspaceNodes);
//
//	SaveWorkspace(Config::getAbsolutePath("/output.txt").c_str(), &WorkspaceNodes);

	// GLuint imageId = pgr::createTexture("/data/BlueprintBackground.png", true);
	//    GLuint imageId =
	//    pgr::createTexture(Config::getAbsolutePath("/Source/GUI/Elements/Windows/data/BlueprintBackground.png"),
	//    true); HeaderBackground = (void*)(intptr_t)imageId; // TODO load texture OR making a simple rectangle
	//    // s_SaveIcon = Application_LoadTexture("Data/ic_save_white_24dp.png");
	//    // s_RestoreIcon = Application_LoadTexture("Data/ic_restore_white_24dp.png");
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

	if (NodeEditorContext)
	{
		ne::DestroyEditor(NodeEditorContext);
		NodeEditorContext = nullptr;
	}
}

void WorkspaceWindow::render()
{
	if (InputController::isKeyPressed(Keys::l)) {
		//SaveWorkspace(Config::getAbsolutePath("/output.txt").c_str(), &WorkspaceNodes);
		printf("press\n");
	}
	ImGui::Begin("Workspace", getShowPtr());

	UpdateTouchAllNodes();
	ne::Begin("Node editor");

	for (auto&& WorkspaceNode : WorkspaceNodes)
	{
		WorkspaceNode->drawNode(NodeBuilderContext, nullptr);
	}

    for (auto& workspaceNode : WorkspaceNodes)
    {
        dynamic_cast<WorkspaceNodeWithCoreData*>(workspaceNode.get())
            ->drawInputLinks(); /* \todo skip nodes with no inputs...*/
    }



	// Popup menu
	static ne::NodeId contextNodeId = 0;
	auto openPopupPosition = ImGui::GetMousePos();
	ne::Suspend();
	if (ne::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("Create New Node");
	}
	else if (ne::ShowNodeContextMenu(&contextNodeId))
	{
		ImGui::OpenPopup("Node Context Menu");
	}
	ne::Resume();

	ne::Suspend();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

	if (ImGui::BeginPopup("Node Context Menu")) {

		auto node = FindNode(contextNodeId);

		if (node) {
			ImGui::Text("ID: %p", contextNodeId.AsPointer());
			ImGui::Separator();
		}
		else {
			ImGui::Text("Unknown node: %p", contextNodeId.AsPointer());
		}

		if (ImGui::MenuItem("Delete")) {
			if (ne::BeginDelete())
			{

				ne::NodeId nodeId = 0;
				while (ne::QueryDeletedNode(&nodeId))
				{
					if (ne::AcceptDeletedItem())
					{
						auto id = std::find_if(WorkspaceNodes.begin(), WorkspaceNodes.end(), [nodeId](auto& node) { return node.ID == nodeId; });
						if (id != WorkspaceNodes.end())
							WorkspaceNodes.erase(id);
					}
				}
			}
			ne::EndDelete();
			ne::DeleteNode(contextNodeId);
		}	
		ImGui::EndPopup();
	}
		
	

	if (ImGui::BeginPopup("Create New Node"))
	{
		auto newNodePostion = openPopupPosition;


		ImGui::Text("add...");
		ImGui::Separator();
		if (ImGui::BeginMenu("transformation")) {

			ImGui::Text("add transforamtion");
			ImGui::Separator();
			if (ImGui::MenuItem("free")) {
			}
			if (ImGui::MenuItem("translation")) {
			}
			if (ImGui::BeginMenu("rotation")) {

				ImGui::Text("add rotation");
				ImGui::Separator();
				if (ImGui::MenuItem("eulerAngleX")) {
				}
				if (ImGui::MenuItem("eulerAngleY")) {
				}
				if (ImGui::MenuItem("eulerAngleZ")) {
				}
				if (ImGui::MenuItem("rotate")) {
				}
				if (ImGui::MenuItem("quat")) {
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("scale")) {

				ImGui::Text("add scale");
				ImGui::Separator();
				if (ImGui::MenuItem("uniform scale")) {
				}
				if (ImGui::MenuItem("scale")) {

				}
				ImGui::EndMenu();

			}
			if (ImGui::MenuItem("lookAt")) {
			}
			if (ImGui::BeginMenu("projection")) {

				ImGui::Text("add projection");
				ImGui::Separator();
				if (ImGui::MenuItem("ortho")) {
				}
				if (ImGui::MenuItem("perspective")) {
				}
				if (ImGui::MenuItem("frustrum")) {
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("operator")) {

			ImGui::Text("add operator");
			ImGui::Separator();
			if (ImGui::BeginMenu("transformation")) {

				ImGui::Text("transformation operators");
				ImGui::Separator();
				if (ImGui::MenuItem("translate")) {
					WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>(HeaderBackgroundTexture, "Translate"));
					ne::SetNodePosition(WorkspaceNodes.back()->m_id, openPopupPosition);
				}
				if (ImGui::MenuItem("eulerAngleX")) {
				}
				if (ImGui::MenuItem("eulerAngleY")) {
				}
				if (ImGui::MenuItem("eulerAngleZ")) {
				}
				if (ImGui::MenuItem("rotate")) {
				}
				if (ImGui::MenuItem("scale")) {
					WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixScale>(HeaderBackgroundTexture, "Scale"));
					ne::SetNodePosition(WorkspaceNodes.back()->m_id, openPopupPosition);
				}
				if (ImGui::MenuItem("ortho")) {
				}
				if (ImGui::MenuItem("perspective")) {
				}
				if (ImGui::MenuItem("frustrum")) {
				}
				if (ImGui::MenuItem("lookAt")) {
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("matrix")) {

				ImGui::Text("matrix operators");
				ImGui::Separator();
				if (ImGui::MenuItem("matrix")) {
					WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrix4x4>(HeaderBackgroundTexture, "Matrix4x4"));
					ne::SetNodePosition(WorkspaceNodes.back()->m_id, openPopupPosition);
				}
				if (ImGui::MenuItem("trackball")) {
				}
				if (ImGui::MenuItem("inversion")) {
					WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixInversion>(HeaderBackgroundTexture, "Matrix Inversion"));
					ne::SetNodePosition(WorkspaceNodes.back()->m_id, openPopupPosition);
				}
				if (ImGui::MenuItem("transpose")) {
				}
				if (ImGui::MenuItem("determinant")) {
					WorkspaceNodes.push_back(std::make_unique<WorkspaceDeterminant>(HeaderBackgroundTexture, "Determinant"));
					ne::SetNodePosition(WorkspaceNodes.back()->m_id, openPopupPosition);
				}
				if (ImGui::MenuItem("mat * mat")) {
				}
				if (ImGui::MenuItem("mat + mat")) {
				}
				if (ImGui::MenuItem("mat * vec4")) {
				}
				if (ImGui::MenuItem("vec4 * mat")) {
				}
				if (ImGui::MenuItem("float * mat")) {
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("vec3")) {

				ImGui::Text("vec3 operator");
				ImGui::Separator();
				if (ImGui::MenuItem("vec3")) {
				}
				if (ImGui::MenuItem("show vec3")) {
				}
				if (ImGui::MenuItem("vec3 x vec3")) {
				}
				if (ImGui::MenuItem("vec3 . vec3")) {
				}
				if (ImGui::MenuItem("vec3 + vec3")) {
				}
				if (ImGui::MenuItem("vec3 - vec3")) {
				}
				if (ImGui::MenuItem("float * vec3")) {
				}
				if (ImGui::MenuItem("normalize vec3")) {
				}
				if (ImGui::MenuItem("length(vec3)")) {
				}
				if (ImGui::MenuItem("mix vec3")) {
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("vec4")) {

				ImGui::Text("vec4 operator");
				ImGui::Separator();
				if (ImGui::MenuItem("vec4")) {
					WorkspaceNodes.push_back(std::make_unique<WorkspaceVector4>(HeaderBackgroundTexture, "Vector 4"));
					ne::SetNodePosition(WorkspaceNodes.back()->m_id, openPopupPosition);
				}
				if (ImGui::MenuItem("vec4 . vec4")) {
				}
				if (ImGui::MenuItem("vec4 + vec4")) {
				}
				if (ImGui::MenuItem("vec4 - vec4")) {
				}
				if (ImGui::MenuItem("float * vec4")) {
				}
				if (ImGui::MenuItem("normalize vec4")) {
					WorkspaceNodes.push_back(std::make_unique<WorkspaceNormalizeVector>(HeaderBackgroundTexture, "Normalize vector"));
					ne::SetNodePosition(WorkspaceNodes.back()->m_id, openPopupPosition);
				}
				if (ImGui::MenuItem("perspective division")) {
				}
				if (ImGui::MenuItem("mix vec4")) {
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("quat")) {

				ImGui::Text("quat operator");
				ImGui::Separator();
				if (ImGui::MenuItem("quat")) {
				}
				if (ImGui::MenuItem("quat(float, vec3)")) {
				}
				if (ImGui::MenuItem("quat(angle, axis)")) {
				}
				if (ImGui::MenuItem("quat(vec3, vec3)")) {
				}
				if (ImGui::MenuItem("quat -> float, vec3")) {
				}
				if (ImGui::MenuItem("quat -> angle, axis")) {
				}
				if (ImGui::MenuItem("float * quat")) {
				}
				if (ImGui::MenuItem("quat * quat")) {
				}
				if (ImGui::MenuItem("quat -> euler")) {
				}
				if (ImGui::MenuItem("euler -> quat")) {
				}
				if (ImGui::MenuItem("slerp")) {
				}
				if (ImGui::MenuItem("long way slerp")) {
				}
				if (ImGui::MenuItem("lerp")) {
				}
				if (ImGui::MenuItem("quat conjugate")) {
				}
				if (ImGui::MenuItem("qvq*")) {
				}
				if (ImGui::MenuItem("inverse quat")) {
				}
				if (ImGui::MenuItem("normalize quat")) {
				}
				if (ImGui::MenuItem("length(quat)")) {
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("float")) {

				ImGui::Text("float operator");
				ImGui::Separator();
				if (ImGui::MenuItem("float")) {
				}
				if (ImGui::MenuItem("clamp float")) {
				}
				if (ImGui::MenuItem("float cycle")) {
				}
				if (ImGui::MenuItem("float * float")) {
				}
				if (ImGui::MenuItem("float / float")) {
				}
				if (ImGui::MenuItem("float + float")) {
				}
				if (ImGui::MenuItem("float ^ float")) {
				}
				if (ImGui::MenuItem("mix float")) {
				}
				if (ImGui::MenuItem("sin & cos(float)")) {
				}
				if (ImGui::MenuItem("asin & acos(float")) {
				}
				if (ImGui::MenuItem("signum")) {
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("conversion")) {

				ImGui::Text("conversion operator");
				ImGui::Separator();
				if (ImGui::MenuItem("mat -> TR")) {
				}
				if (ImGui::MenuItem("TR -> mat")) {
				}
				if (ImGui::MenuItem("mat -> vecs4")) {
				}
				if (ImGui::MenuItem("mat -> quat")) {
				}
				if (ImGui::MenuItem("mat -> floats")) {
				}
				if (ImGui::MenuItem("vecs4 -> mat")) {
				}
				if (ImGui::MenuItem("vec4 -> vec3")) {
				}
				if (ImGui::MenuItem("vec4 -> floats")) {
				}
				if (ImGui::MenuItem("vecs3 -> mat")) {
				}
				if (ImGui::MenuItem("vec3 -> vec4")) {
				}
				if (ImGui::MenuItem("vec3 -> floats")) {
				}
				if (ImGui::MenuItem("quat -> mat")) {
				}
				if (ImGui::MenuItem("quat -> floats")) {
				}
				if (ImGui::MenuItem("floats -> mat")) {
				}
				if (ImGui::MenuItem("floats -> vec4")) {
				}
				if (ImGui::MenuItem("floats -> vec3")) {
				}
				if (ImGui::MenuItem("floats -> quat")) {
				}
				ImGui::EndMenu();

			}
			ImGui::EndMenu();

		}
		if (ImGui::MenuItem("sequence")) {
		}
		if (ImGui::MenuItem("camera")) {
		}
		if (ImGui::MenuItem("screen")) {
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("selection")) {

			ImGui::Text("selection");
			ImGui::Separator();
			if (ImGui::MenuItem("select all")) { //SS todo make hotkey hint
			}
			if (ImGui::MenuItem("invert")) { //SS todo make hotkey hint
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("zoom")) {

			ImGui::Text("zoom");
			ImGui::Separator();
			if (ImGui::MenuItem("to all")) { //SS todo make hotkey hint
			}
			if (ImGui::MenuItem("to selection")) { //SS todo make hotkey hint
			}
			ImGui::EndMenu();

		}

		ImGui::EndPopup();
	}
	
	
	ImGui::PopStyleVar();
	ne::Resume();

	ne::End();

	ImGui::End();
	
}

void WorkspaceWindow::UpdateTouchAllNodes()
{
	const auto deltaTime = ImGui::GetIO().DeltaTime;
	for (auto&& WorkspaceNode : WorkspaceNodes)
	{
		WorkspaceNode->UpdateTouch(deltaTime);
	}
}


WorkspaceNode* WorkspaceWindow::FindNode(ne::NodeId id)
{
	for (auto&& WorkspaceNode : WorkspaceNodes) {
		if (WorkspaceNode->m_id == id)
			return WorkspaceNode.get();
	}
	return nullptr;
}
