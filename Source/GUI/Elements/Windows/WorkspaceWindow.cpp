/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.

    Details no here now... Cca in middle is file split between example and used part
*/

#include "WorkspaceWindow.h"

#include "../Nodes/WorkspaceNodeWithCoreData.h"

#include "../Nodes/WorkspaceMatrixFree.h"
#include "../Nodes/WorkspaceMatrixScale.h"
#include "../Nodes/WorkspaceMatrixTranslation.h"
#include "../Nodes/WorkspaceMatrixInversion.h"
#include "../Nodes/WorkspaceMatrixMulMatrix.h"
#include "../Nodes/WorkspaceMatrixTranspose.h"
#include "../Nodes/WorkspaceDeterminant.h"

#include "../Nodes/WorkspaceFloatFree.h"

#include "../Nodes/WorkspaceVectorFree.h"
#include "../Nodes/WorkspaceNormalizeVector.h"

#include "Core/Input/InputManager.h"
#include "Scripting/Scripting.h"


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

	/*\todo adding nodes here just for testing */
	/*--- TRANSLATION */
	// WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>(HeaderBackgroundTexture, "MatrixTranslation
	// 1")); ne::SetNodePosition(WorkspaceNodes.back()->Id, ImVec2(-252, 220));

	/*--- TRANSLATION with connection to translation */
	// WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>(HeaderBackgroundTexture, "MatrixTranslation
	// 2")); ne::SetNodePosition(WorkspaceNodes.back()->Id, ImVec2(-300, 351));

	/* \todo JH nyni nejde v Core spojovat dva operatory - proto to nefuguje... */
	// Core::GraphManager::plug(static_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(0).get())->Nodebase,
	//                         static_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(1).get())->Nodebase, 0, 0);

	/*--- MATRIX_4x4 with connection to translation*/
	m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixFree>(HeaderBackgroundTexture, "just Matrix4x4"));
	 ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, ImVec2(-500, 351));

	// Core::GraphManager::plug(static_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(0).get())->Nodebase,
	//                         static_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(2).get())->Nodebase, 0, 0);

	/*--- SCALE */
	 /*WorkspaceNodes.push_back(std::make_unique<WorkspaceMatrixScale>(HeaderBackgroundTexture, "MatrixScale 1"));
	 ne::SetNodePosition(WorkspaceNodes.back()->Id, ImVec2(-500, 251));

	/*--- NORMALIZE VECTOR */
	m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceNormalizeVector>(HeaderBackgroundTexture, "NormalizeVector 1")); 
	ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, ImVec2(100, 400));

	ne::NavigateToContent();
	ne::CenterNodeOnScreen(m_workspaceCoreNodes.back()->m_id);

	//////////////////////////////////
	//printf("len %lld \n",WorkspaceNodes.size());
	/*WorkspaceNodeWithCoreData*wnbd=dynamic_cast<WorkspaceNodeWithCoreData*>(WorkspaceNodes.at(2).get());

	Ptr<Core::NodeBase> child=wnbd->Nodebase;
	std::vector<Core::Pin>cp= child->getInputPins();
	Ptr<Core::NodeBase> parent=Core::GraphManager::getParent(child);
	const Operation* uu = child->getOperation();

	//cp[0].getParentPin()->getMaster();
	//WorkspaceNodes.at(0).get()->
	glm::mat4 m = glm::mat4(0.14f);
	ValueSetResult vsr=parent->setValue(m);
	ValueSetResult vsr2= child->setValue(m);

	DataStore ds=child->getData();
	glm::mat4 mm=ds.getMat4();*/

	// LoadWorkspace(Config::getAbsolutePath("/load.c").c_str(),&WorkspaceNodes);

	// SaveWorkspace(Config::getAbsolutePath("/save.c").c_str(), &WorkspaceNodes);

	ne::GetStyle().Colors[ne::StyleColor::StyleColor_NodeBg] = ImColor(67, 103, 152);
	ne::GetStyle().Colors[ne::StyleColor::StyleColor_Bg] = ImColor(158, 158, 158);
	ne::GetStyle().PivotAlignment = ImVec2(0.0f, 0.0f);
	

	
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

	if (InputManager::isKeyPressed(Keys::l))
	{
		// SaveWorkspace(Config::getAbsolutePath("/output.txt").c_str(), &m_workspaceCoreNodes);
		printf("press\n");
	}
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Workspace", getShowPtr());

	ImGui::PopStyleVar();

	UpdateTouchAllNodes();
	ne::Begin("Node editor");{
    ImVec2 cursorTopLeft = ImGui::GetCursorScreenPos();

	for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
	{
		workspaceCoreNode->drawNode(NodeBuilderContext, nullptr);
	}

    /* both connected pins have to be drawn before link is drawn -> therefore separated for */
    for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
    {
        workspaceCoreNode->drawInputLinks();
    }

    checkQueryElements();

    checkQueryContextMenus();

    ImGui::SetCursorScreenPos(cursorTopLeft);


	}ne::End();

	shiftSelectedNodesToFront();

	ImGui::End();
}

/* \todo JH not work yet - should avoid capturing actions in bottom nodes when overlaping ( https://github.com/thedmd/imgui-node-editor/issues/81 ) */
void WorkspaceWindow::shiftSelectedNodesToFront()
{
    if (ne::HasSelectionChanged())
    {
        std::vector<Ptr<WorkspaceNodeWithCoreData>> selectedCoreNodes = getSelectedWorkspaceCoreNodes();

        for(int i=0; i < selectedCoreNodes.size(); i++)
        {

            coreNodeIter ith_selected_node = std::find_if(m_workspaceCoreNodes.begin(),
                                                        m_workspaceCoreNodes.end(),
                                                        [selectedCoreNodes, i](Ptr<WorkspaceNodeWithCoreData> const &node) -> bool { return node->m_id == selectedCoreNodes.at(i)->m_id; });

            if (ith_selected_node != m_workspaceCoreNodes.end())
            {
              std::iter_swap(m_workspaceCoreNodes.begin()+i, ith_selected_node);
            }
        }
    }
}

void WorkspaceWindow::manipulatorStartCheck3D()
{
    Ptr<WorkspaceNodeWithCoreData> selectedCoreNode;
    ne::NodeId selectedNodeID;

    if (ne::HasSelectionChanged() && ne::GetSelectedNodes(&selectedNodeID, 1) == 1 )
    {
        selectedCoreNode = getWorkspaceCoreNodeByID(selectedNodeID);
        if (selectedCoreNode != nullptr)
        {
            /* \todo JH call manipulator with selectedCoreNode->m_nodebase */
        }
        else
        {
            /* \todo JH call manipulator with null */
        }
    }
}


Ptr<WorkspaceNodeWithCoreData> WorkspaceWindow::getWorkspaceCoreNodeByID(ne::NodeId const id){
    if (id)
    {
        for(Ptr<WorkspaceNodeWithCoreData> const &node : m_workspaceCoreNodes)
        {
            if(node->m_id == id){ return node; }
        }
    }
    return nullptr;
}

/* \todo JH I want to return Ptr<WorkspaceNodeWithCoreData>, but not know how to do it... */
Ptr<WorkspaceNodeWithCoreData> WorkspaceWindow::getWorkspaceCoreNodeByPinID(ne::PinId const id){
    if (id)
    {
        for (Ptr<WorkspaceNodeWithCoreData> const &node : m_workspaceCoreNodes)
        {
            for (Ptr<WorkspaceCorePinProperties> const &pin : node->m_workspaceInputsProperties)
            {
                if (pin->m_id == id)
                {
                    return node;
                }
            }

            for (Ptr<WorkspaceCorePinProperties> const &pin : node->m_workspaceOutputsProperties)
            {
                if (pin->m_id == id)
                {
                    return node;
                }
            }
        }
    }

    return nullptr;
}

Ptr<WorkspaceCorePinProperties> WorkspaceWindow::getWorkspacePinPropertiesByID(ne::PinId const id)
{
    if (id)
    {
        for (Ptr<WorkspaceNodeWithCoreData> const &node : m_workspaceCoreNodes)
        {
            for (Ptr<WorkspaceCorePinProperties> const &pin : node->m_workspaceInputsProperties)
            {
                if (pin->m_id == id)
                {
                    return pin;
                }
            }

            for (Ptr<WorkspaceCorePinProperties> const &pin : node->m_workspaceOutputsProperties)
            {
                if (pin->m_id == id)
                {
                    return pin;
                }
            }
        }
    }

    return nullptr;
}

std::vector<Ptr<WorkspaceNodeWithCoreData>> WorkspaceWindow::getSelectedWorkspaceCoreNodes()
{
    std::vector<Ptr<WorkspaceNodeWithCoreData>> allSelectedCoreNodes;
    std::vector<ne::NodeId> allSelectedNodesIDs;
    Ptr<WorkspaceNodeWithCoreData> temp;

    int numOfSelectedObjects = ne::GetSelectedObjectCount(); /* not Nodes only */

    allSelectedNodesIDs.resize(numOfSelectedObjects);
    int numOfSelectedNodes = ne::GetSelectedNodes(allSelectedNodesIDs.data(), numOfSelectedObjects);
    allSelectedNodesIDs.shrink_to_fit();

    allSelectedCoreNodes.reserve(numOfSelectedNodes);
    for (auto nodeId : allSelectedNodesIDs)
    {
        temp = getWorkspaceCoreNodeByID(nodeId);
        if (temp)
        {
            allSelectedCoreNodes.push_back(temp);
        }
    }
    allSelectedCoreNodes.shrink_to_fit();

    return allSelectedCoreNodes;
}

void WorkspaceWindow::showPopUpLabel(std::string label, ImColor color)
{
    const char* label_c = label.c_str();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
    ImVec2 labelSize = ImGui::CalcTextSize(label_c);

    ImVec2 padding = ImGui::GetStyle().FramePadding;
    ImVec2 spacing = ImGui::GetStyle().ItemSpacing;

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

    ImVec2 rectMin = ImGui::GetCursorScreenPos() - padding;
    ImVec2 rectMax = ImGui::GetCursorScreenPos() + labelSize + padding;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(rectMin, rectMax, color, labelSize.y * 0.15f); /* \todo JH remove constant here */
    ImGui::TextUnformatted(label_c);
}

void WorkspaceWindow::checkQueryElementsCreating()
{
    if (ne::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        checkQueryLinkCreate();
        // checkQueryNewNode(); /* \todo JH not work well.. block other manipulations */
    }
    else
    {
        m_pinPropertiesForNewLink = nullptr;
    }
    ne::EndCreate();

}

void WorkspaceWindow::checkQueryElements()
{
    if (!m_createNewNode)
    {
        checkQueryElementsCreating();
        checkQueryElementsDeleting();

    }
}

void WorkspaceWindow::checkQueryLinkCreate()
{
    ne::PinId startPinId = 0, endPinId = 0;
    if (ne::QueryNewLink(&startPinId, &endPinId))
    {
        Ptr<WorkspaceCorePinProperties> startPin = getWorkspacePinPropertiesByID(startPinId);
        Ptr<WorkspaceCorePinProperties> endPin   = getWorkspacePinPropertiesByID(endPinId);

        /* \todo JH comment where it is used */
        m_pinPropertiesForNewLink = startPin ? startPin : endPin;

        if (startPin && endPin && (startPin->getKind() != endPin->getKind()) ) /* \todo JH check kind in Core? */
        {
            if (startPin->getKind() == PinKind::Input)
            {
                std::swap(startPin, endPin);
            }


            /* \todo JH manage different result of trying to connect (probably use showPopUpLabel() ) */
            switch (Core::GraphManager::plug(startPin->m_node.m_nodebase,
                                             endPin->m_node.m_nodebase,
                                             startPin->m_pin.getIndex(),
                                             endPin->m_pin.getIndex() ))
            {
                case ENodePlugResult::Ok:
                    showPopUpLabel("Connected", ImColor(0,255,0)); /* \todo JH remove constant here */
                    break;
            }
        }
    }

}

void WorkspaceWindow::checkQueryContextMenus()
{
    m_openPopupMenuPosition = ImGui::GetMousePos();
    ne::Suspend();
	if (ne::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("Create New Node"); /* \todo JH take string from some constants file */
	}
	else if (ne::ShowNodeContextMenu(&m_contextNodeId))
	{
		Ptr<WorkspaceNodeWithCoreData> node = getWorkspaceCoreNodeByID(m_contextNodeId);
		if (node->fw.showMyPopup) {
			ImGui::OpenPopup("float_context_menu");
			node->fw.showMyPopup = false;
		}else{
			ImGui::OpenPopup("Node Context Menu");
		}
		
	}
	ne::Resume();

	ne::Suspend();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8)); /* \todo JH take graphic setting from constants file */

	if (ImGui::BeginPopup("float_context_menu")) {
		ImGui::Text("float");
		ImGui::Separator();
		Ptr<WorkspaceNodeWithCoreData> node = getWorkspaceCoreNodeByID(m_contextNodeId);
		float newValue;
		bool valueChange = false;
		if (ImGui::Selectable("Set to zero")) {
			newValue = 0.0f;
			valueChange = true;
		}
		if (ImGui::Selectable("Set to PI")) {
			newValue = 3.1415f;
			valueChange = true;
		}

		if (node->fw.name == "vector4" && valueChange) {
			glm::vec4 data = node->m_nodebase->getData().getVec4();
			data[node->fw.columns] = newValue;
			node->m_nodebase->setValue(data);
			valueChange = false;
		}
		else if (node->fw.name == "matrix4x4" && valueChange) {
			node->m_nodebase->setValue(newValue, { node->fw.columns, node->fw.rows });
			valueChange = false;
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Node Context Menu") && !(ImGui::BeginPopup("float_context_menu"))) {

        Ptr<WorkspaceNodeWithCoreData> node = getWorkspaceCoreNodeByID(m_contextNodeId);

		if (node) {
			ImGui::Text("ID: %p", m_contextNodeId.AsPointer());
			ImGui::Separator();
		}
		else {
			ImGui::Text("Unknown node: %p", m_contextNodeId.AsPointer());
		}

		if (ImGui::MenuItem("Delete")) {
			if (ne::BeginDelete())
			{
				checkQueryNodeDelete();
			}
			
			ne::DeleteNode(m_contextNodeId); /* \todo JH check whether node remain in nodeeditor */
			ne::EndDelete();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Create New Node"))
	{
		m_newNodePostion = m_openPopupMenuPosition;

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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixScale>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixFree>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
				}
				if (ImGui::MenuItem("trackball")) {
				}
				if (ImGui::MenuItem("inversion")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixInversion>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
				}
				if (ImGui::MenuItem("transpose")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixTranspose>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
				}
				if (ImGui::MenuItem("determinant")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceDeterminant>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
				}
				if (ImGui::MenuItem("mat * mat")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixMulMatrix>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorFree>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceNormalizeVector>(HeaderBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
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
//					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatFree>(HeaderBackgroundTexture));
//					ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, m_newNodePostion);
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

}

void WorkspaceWindow::checkQueryNodeCreate()
{
    ne::PinId pinId = 0;
    if (ne::QueryNewNode(&pinId))
    {
        m_pinPropertiesForNewLink = getWorkspacePinPropertiesByID(pinId);
        if (m_pinPropertiesForNewLink)
        {
            showPopUpLabel("+ Create Node", ImColor(32, 45, 32, 180)); /* \todo JH remove constant here */
        }

        if (ne::AcceptNewItem())
        {
            m_createNewNode  = true;
            m_pinPropertiesForNewNodeLink = m_pinPropertiesForNewLink;
            m_pinPropertiesForNewLink = nullptr;

            ne::Suspend();
            ImGui::OpenPopup("Create New Node");
            ne::Resume();
        }
    }
}

void WorkspaceWindow::checkQueryElementsDeleting()
{
    if (ne::BeginDelete())
    {
        checkQueryLinkDelete();
        checkQueryNodeDelete();
    }
    ne::EndDelete();
}

void WorkspaceWindow::checkQueryLinkDelete()
{
    ne::LinkId linkId = 0;
    while (ne::QueryDeletedLink(&linkId))
    {
        if (ne::AcceptDeletedItem())
        {
            Ptr<WorkspaceCorePinProperties> inputPin = getWorkspacePinPropertiesByID(ne::PinId(linkId.Get())); /* link has same id as pin to which is connected as input */

            Core::GraphManager::unplugInput(inputPin->m_node.m_nodebase, inputPin->m_pin.getIndex());
        }
    }
}

void WorkspaceWindow::checkQueryNodeDelete()
{
    ne::NodeId nodeId = 0;
    while (ne::QueryDeletedNode(&nodeId))
    {
        if (ne::AcceptDeletedItem())
        {
            coreNodeIter id = std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(), [nodeId](Ptr<WorkspaceNodeWithCoreData>& node) { return node->m_id == nodeId; });
            if (id != m_workspaceCoreNodes.end())
                m_workspaceCoreNodes.erase(id);
        }
    }
}


void WorkspaceWindow::UpdateTouchAllNodes()
{
	const auto deltaTime = ImGui::GetIO().DeltaTime;
	for (auto&& workspaceNode : m_workspaceCoreNodes)
	{
		workspaceNode->UpdateTouch(deltaTime);
	}
}