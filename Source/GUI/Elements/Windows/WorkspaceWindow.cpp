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
#include "../Nodes/WorkspaceSequence.h"

#include "../Nodes/WorkspaceFloatFree.h"

#include "../Nodes/WorkspaceVectorFree.h"
#include "../Nodes/WorkspaceNormalizeVector.h"

#include "Core/Input/InputManager.h"
#include "Scripting/Scripting.h"


// using namespace Core;

WorkspaceWindow::WorkspaceWindow(bool show)
		: IWindow(show), m_wholeApplication(Application::get()), ConstTouchTime(1.0f),
			m_headerBackgroundTexture((ImTextureID)(intptr_t)pgr::createTexture(
					Config::getAbsolutePath("/Source/GUI/Elements/Windows/data/BlueprintBackground.png"),
					true)) // \TODO load texture OR making a simple rectangle
			,
			m_nodeBuilderContext(util::NodeBuilder(m_headerBackgroundTexture, 64, 64))
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

	m_nodeEditorContext = ne::CreateEditor(&config);
	ne::SetCurrentEditor(m_nodeEditorContext);

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

	if (m_nodeEditorContext)
	{
		ne::DestroyEditor(m_nodeEditorContext);
		m_nodeEditorContext = nullptr;
	}
}

void WorkspaceWindow::render()
{
    ne::SetCurrentEditor(m_nodeEditorContext);

	if (InputManager::isKeyPressed(Keys::l))
	{
		// SaveWorkspace(Config::getAbsolutePath("/output.txt").c_str(), &m_workspaceCoreNodes);
		printf("press\n");
	}
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin(getName("Workspace").c_str(), getShowPtr());

	ImGui::PopStyleVar();

	UpdateTouchAllNodes();
	ne::Begin("Node editor");{
    ImVec2 cursorTopLeft = ImGui::GetCursorScreenPos();

	for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
	{
	    if (workspaceCoreNode->isSequence())
        {
            for(auto && inSequenceNode : std::dynamic_pointer_cast<WorkspaceSequence>(workspaceCoreNode)->getInnerWorkspaceNodes())
            {
                inSequenceNode->drawNode(m_nodeBuilderContext, nullptr);
            }
        }
		workspaceCoreNode->drawNode(m_nodeBuilderContext, nullptr);
	}

    /* both connected pins have to be drawn before link is drawn -> therefore separated for */
    for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
    {
        workspaceCoreNode->drawInputLinks();
    }

    checkUserActions();

    checkQueryElements();

    checkQueryContextMenus();

    ImGui::SetCursorScreenPos(cursorTopLeft);


	}ne::End();

    checkSequenceSelections();

	shiftSelectedNodesToFront();

	manipulatorStartCheck3D();

	ImGui::End();
}

void WorkspaceWindow::checkSequenceSelections()
{
    if (ne::HasSelectionChanged())
    {
        for(Ptr<WorkspaceNodeWithCoreData> const &node : getSelectedWorkspaceCoreNodes())
        {
            if (node->isSequence())
            {
                //std::vector<ne::Detail::ObjectWrapper<ne::Detail::Node>> & nodeeditor_nodes = ne::EditorContext::getNodeEditorNodes_addedFunction();
                std::vector<ne::Detail::ObjectWrapper<ne::Detail::Node>> & nodeeditor_nodes = m_nodeEditorContext->getNodeEditorNodes_addedFunction();

                std::vector<Ptr<WorkspaceNodeWithCoreData>> innerWorkspaceNodes = std::dynamic_pointer_cast<WorkspaceSequence>(node)->getInnerWorkspaceNodes();
                /* \todo JH there is better way how to shift nodes to end - std::rotate() std::stable_partition() */
                for(int i=0; i < innerWorkspaceNodes.size(); i++)
                {
                    auto ith_inner_node = std::find_if(nodeeditor_nodes.begin(),
                                                        nodeeditor_nodes.end(),
                                                        [innerWorkspaceNodes, i](ne::Detail::ObjectWrapper<ne::Detail::Node> & ne_node) -> bool { return (ne::NodeId)(ne_node->ID()) == innerWorkspaceNodes.at(i)->getId(); });

                    if (ith_inner_node != nodeeditor_nodes.end())
                    {
                      std::iter_swap(nodeeditor_nodes.end()-1-i, ith_inner_node);
                    }
                }

            }
        }
    }
}

void WorkspaceWindow::checkUserActions()
{
    if (ImGui::IsMouseClicked(1)) /* right button */
    {
        m_rightClickPosition = ImGui::GetMousePos();
    }
}

void WorkspaceWindow::shiftNodesToFront(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodesToShift)
{
    for(int i=0; i < nodesToShift.size(); i++)
        {
            coreNodeIter ith_selected_node = std::find_if(m_workspaceCoreNodes.begin(),
                                                        m_workspaceCoreNodes.end(),
                                                        [nodesToShift, i](Ptr<WorkspaceNodeWithCoreData> const &node) -> bool { return node->getId() == nodesToShift.at(i)->getId(); });

            if (ith_selected_node != m_workspaceCoreNodes.end())
            {
              std::iter_swap(m_workspaceCoreNodes.begin()+i, ith_selected_node);
            }
        }
}

void WorkspaceWindow::shiftNodesToBack(std::vector<Ptr<WorkspaceNodeWithCoreData>> nodesToShift)
{
    for(int i=0; i < nodesToShift.size(); i++)
    {
        coreNodeIter ith_selected_node = std::find_if(m_workspaceCoreNodes.begin(),
                                                    m_workspaceCoreNodes.end(),
                                                    [nodesToShift, i](Ptr<WorkspaceNodeWithCoreData> const &node) -> bool { return node->getId() == nodesToShift.at(i)->getId(); });

        if (ith_selected_node != m_workspaceCoreNodes.end())
        {
          std::iter_swap(m_workspaceCoreNodes.end()-1-i, ith_selected_node);
        }
    }
}

/* \todo JH not work yet in all cases - should avoid capturing actions in bottom nodes when overlaping ( https://github.com/thedmd/imgui-node-editor/issues/81 ) */
void WorkspaceWindow::shiftSelectedNodesToFront()
{
    if (ne::HasSelectionChanged())
    {
        shiftNodesToFront( getSelectedWorkspaceCoreNodes() );
    }
}

//void WorkspaceWindow::shiftTransformationInSequenceNodesToFront()
//{
//    std::vector<Ptr<WorkspaceNodeWithCoreData>> allSequences;
//
//    for (auto sequence_node : m_workspaceCoreNodes)
//    {
//        if (sequence_node->isSequence())
//        {
//                                    /* not work here - invadlid use of incomplete type EditorContext*/
//            std::stable_partition(m_nodeEditorContext->getNodeEditorNodes_addedFunction().begin(), m_nodeEditorContext->getNodeEditorNodes_addedFunction().end(),
//                                    [&sequence_node](ne::Node* node_from_nodeeditor) -> bool
//                                    {
//                                        return std::find_if(sequence_node->getInnerWorkspaceNodes.begin(), sequence_node->getInnerWorkspaceNodes.end(),
//                                                            [node_from_nodeeditor](Ptr<WorkspaceNodeWithCoreData> const &node_from_sequence) -> bool { return node_from_nodeeditor->getId() == node_from_sequence->getId(); }
//                                                            )
//                                                == sequence_node->getInnerWorkspaceNodes.end();
//
//                                    }
//            );
//        }
//    }
//}

void WorkspaceWindow::manipulatorStartCheck3D()
{
    Ptr<WorkspaceMatrix4x4> selectedWorkspaceMatrix4x4 = nullptr;
    ne::NodeId selectedNodeID;

    if (ne::HasSelectionChanged())
    {
        World2* world2= Application::get().world2();

        if ( ne::GetSelectedObjectCount() == 1 && ne::GetSelectedNodes(&selectedNodeID, 1) == 1)
        {
            selectedWorkspaceMatrix4x4 = std::dynamic_pointer_cast<WorkspaceMatrix4x4>(getWorkspaceCoreNodeByID(selectedNodeID));
        }

        world2->handlesSetMatrix(selectedWorkspaceMatrix4x4.get());

    }
}

Ptr<WorkspaceNodeWithCoreData> WorkspaceWindow::getWorkspaceCoreNodeByID(ne::NodeId const id)
{
    if (id)
    {
        for(Ptr<WorkspaceNodeWithCoreData> const &node : m_workspaceCoreNodes)
        {
            if(node->getId() == id){ return node; }
        }
    }
    return nullptr;
}

Ptr<WorkspaceNodeWithCoreData> WorkspaceWindow::getWorkspaceCoreNodeByPinID(ne::PinId const id)
{
    if (id)
    {
        for (Ptr<WorkspaceNodeWithCoreData> const &node : m_workspaceCoreNodes)
        {
            for (Ptr<WorkspaceCorePinProperties> const &pin : node->getInputsProperties())
            {
                if (pin->getId() == id)
                {
                    return node;
                }
            }

            for (Ptr<WorkspaceCorePinProperties> const &pin : node->getOutputsProperties())
            {
                if (pin->getId() == id)
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
            for (Ptr<WorkspaceCorePinProperties> const &pin : node->getInputsProperties())
            {
                if (pin->getId() == id)
                {
                    return pin;
                }
            }

            for (Ptr<WorkspaceCorePinProperties> const &pin : node->getOutputsProperties())
            {
                if (pin->getId() == id)
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
    Ptr<WorkspaceSequence> temp_sequence;

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

        if ( startPin && endPin) /* \todo JH check kind in Core? */
        {
            if (startPin->getKind() == PinKind::Input)
            {
                std::swap(startPin, endPin);
            }

            switch (Core::GraphManager::isPlugCorrect(&(endPin->getCorePin()), &(startPin->getCorePin())))
            {
                case ENodePlugResult::Ok:
                    showPopUpLabel("Connection possible", ImColor(0,255,0)); /* \todo JH remove constant here */
                    if (!ImGui::GetIO().MouseDown[0])
                    {
                        Core::GraphManager::plug(startPin->getNode().getNodebase(),
                                                 endPin->getNode().getNodebase(),
                                                 startPin->getIndex(),
                                                 endPin->getIndex() );
                    }
                    break;
                /* \todo JH react informatively to other result too */
                default:
                    showPopUpLabel("Connection not possible", ImColor(255,0,0)); /* \todo JH remove constant here */

            }
        }
    }

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

            Core::GraphManager::unplugInput(inputPin->getNode().getNodebase(), inputPin->getIndex());
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
            NodeDelete(nodeId);
        }
    }
}

void WorkspaceWindow::NodeDelete(ne::NodeId const nodeId)
{
    coreNodeIter id = std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(), [nodeId](Ptr<WorkspaceNodeWithCoreData>& node) { return node->getId() == nodeId; });
    if (id != m_workspaceCoreNodes.end())
    {
        m_workspaceCoreNodes.erase(id);
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

void WorkspaceWindow::checkQueryContextMenus()
{
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
			glm::vec4 data = node->getNodebase()->getData().getVec4();
			data[node->fw.columns] = newValue;
			node->getNodebase()->setValue(data);
			valueChange = false;
		}
		else if (node->fw.name == "matrix4x4" && valueChange) {
			node->getNodebase()->setValue(newValue, { node->fw.columns, node->fw.rows });
			valueChange = false;
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Node Context Menu") && !(ImGui::BeginPopup("float_context_menu"))) {

        Ptr<WorkspaceNodeWithCoreData> context_node = getWorkspaceCoreNodeByID(m_contextNodeId);

		if (context_node) {
			ImGui::Text("ID: %p", m_contextNodeId.AsPointer());
			ImGui::Separator();
		}
		else {
			ImGui::Text("Unknown node: %p", m_contextNodeId.AsPointer());
		}

		context_node->drawMenuSetDataMap();
        context_node->drawMenuLevelOfDetail();
        context_node->drawMenuSetPrecision();

        if (ImGui::MenuItem("Delete")) {
            ne::DeleteNode(context_node->getId());
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Create New Node"))
	{
		m_newNodePostion = m_rightClickPosition;

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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixScale>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixFree>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("trackball")) {
				}
				if (ImGui::MenuItem("inversion")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixInversion>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("transpose")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixTranspose>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("determinant")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceDeterminant>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("mat * mat")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixMulMatrix>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorFree>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceNormalizeVector>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatFree>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
           m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceSequence>(m_headerBackgroundTexture));
            ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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

