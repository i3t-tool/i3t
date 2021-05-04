/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.
*/

#include "WorkspaceWindow.h"

#include "Core/Input/InputManager.h"
#include "Scripting/Scripting.h"

// using namespace Core;

WorkspaceWindow::WorkspaceWindow(bool show)
		: IWindow(show), m_wholeApplication(Application::get()), ConstTouchTime(1.0f),
			m_headerBackgroundTexture((ImTextureID)(intptr_t)pgr::createTexture(
					Config::getAbsolutePath("Data/textures/blueprint_background.png"),
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
	m_ne_usable = reinterpret_cast<ax::NodeEditor::Detail::EditorContext*>(m_nodeEditorContext);

	ne::GetStyle().Colors[ne::StyleColor::StyleColor_NodeBg] = node_bg_color;
	ne::GetStyle().Colors[ne::StyleColor::StyleColor_Bg] = background_color;
	ne::GetStyle().PivotAlignment = pivot_alignment;
	//ne::GetStyle().NodeRounding = node_rounding;

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
	if (InputManager::isKeyPressed(Keys::l))
	{
		// saveWorkspace(Config::getAbsolutePath("/output.txt").c_str(), &m_workspaceCoreNodes);
		printf("press\n");
	}

	ImGui::Begin(getName("Workspace").c_str(), getShowPtr());


	UpdateTouchAllNodes();
	ne::Begin("Node editor");{
    ImVec2 cursorTopLeft = ImGui::GetCursorScreenPos();

	for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
	{
		workspaceCoreNode->drawNode(m_nodeBuilderContext, nullptr);
	}

    /* put and pop to/from Sequence */
	ne::Detail::DragAction* drag_action = m_ne_usable->GetCurrentAction() != nullptr ? m_ne_usable->GetCurrentAction()->AsDrag() : nullptr;
	if(drag_action && drag_action->IsDragging())
    {
        m_draged_nodes = getSelectedWorkspaceCoreNodes(); /* \todo JH selected node does not have to be same as draged one */
        if(m_draged_nodes.size()==1)
        {
            m_draged_node = m_draged_nodes[0];
            if(m_draged_node->isTransformation())
            {
                if (m_draged_node->inSequence())
                {
                    Ptr<WorkspaceSequence> workspace_sequence = getSequenceOfWorkspaceNode(m_draged_node);
                    assert(workspace_sequence != nullptr);

                    workspace_sequence->popNode(m_draged_node);
                    m_workspaceCoreNodes.push_back(m_draged_node);
                }

                int position_in_sequence;
                m_all_sequences = getSequenceNodes();
                for (Ptr<WorkspaceSequence> & sequence : m_all_sequences)
                {
                    position_in_sequence = sequence->getInnerPosition(ne::GetNodePosition(m_draged_node->getId()));
                    if (position_in_sequence >= 0 && ImGui::IsMouseReleased(0))
                    {
                        sequence->pushNode(m_draged_node, position_in_sequence);
                        m_workspaceCoreNodes.erase(std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(),
                                                    [this](Ptr<WorkspaceNodeWithCoreData> const &w_node) -> bool { return w_node->getId() == m_draged_node->getId(); }) );
                        break;
                    }

                }
            }
        }
    }

    /* both connected pins have to be drawn before link is drawn -> therefore separated cycle */
    for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
    {
        workspaceCoreNode->drawInputLinks();
    }

    checkUserActions();

    checkQueryElements();

    checkQueryContextMenus();

    ImGui::SetCursorScreenPos(cursorTopLeft);


	}ne::End();

	manipulatorStartCheck3D();

	ImGui::End();
}

Ptr<WorkspaceSequence> WorkspaceWindow::getSequenceOfWorkspaceNode(Ptr<WorkspaceNodeWithCoreData> node)
{
    Ptr<Core::Transformation> transfomation_nodebase = node->getNodebase()->as<Core::Transformation>();
    if(transfomation_nodebase)
    {
        Ptr<Core::NodeBase> sequence_nodebase = transfomation_nodebase->getCurrentSequence();
        if (sequence_nodebase)
        {
            for(Ptr<WorkspaceNodeWithCoreData> const &workspace_node : m_workspaceCoreNodes)
            {
                if (workspace_node->isSequence() &&  workspace_node->getNodebase() == sequence_nodebase)
                {
                    return std::dynamic_pointer_cast<WorkspaceSequence>(workspace_node);
                }
            }
        }
    }
    return nullptr;
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

/* \todo JH not used now (with change in NodeEditor) - not work yet in all cases - should avoid capturing actions in bottom nodes when overlaping ( https://github.com/thedmd/imgui-node-editor/issues/81 ) */
void WorkspaceWindow::shiftSelectedNodesToFront()
{
    if (ne::HasSelectionChanged())
    {
        shiftNodesToFront( getSelectedWorkspaceCoreNodes() );
    }
}


void WorkspaceWindow::manipulatorStartCheck3D()
{
    Ptr<WorkspaceMatrix4x4> selectedWorkspaceMatrix4x4 = nullptr;
    ne::NodeId selectedNodeID;

    if (ne::HasSelectionChanged())
    {
        World* world2= Application::get().world2();

        if ( ne::GetSelectedObjectCount() == 1 && ne::GetSelectedNodes(&selectedNodeID, 1) == 1)
        {
            selectedWorkspaceMatrix4x4 = std::dynamic_pointer_cast<WorkspaceMatrix4x4>(getWorkspaceCoreNodeByID(selectedNodeID));
        }

        world2->handlesSetMatrix(&selectedWorkspaceMatrix4x4, nullptr);

    }
}

std::vector<Ptr<WorkspaceSequence>> WorkspaceWindow::getSequenceNodes()
{
    std::vector<Ptr<WorkspaceSequence>> allSequenceNodes;
    Ptr<WorkspaceNodeWithCoreData> temp;
    for (Ptr<WorkspaceNodeWithCoreData> const &node : m_workspaceCoreNodes)
    {
        temp = getWorkspaceCoreNodeByID(node->getId());
        if (temp->isSequence())
        {
            allSequenceNodes.push_back(std::dynamic_pointer_cast<WorkspaceSequence>(temp));
        }
    }

    return allSequenceNodes;
}

Ptr<WorkspaceNodeWithCoreData> WorkspaceWindow::getWorkspaceCoreNodeByID(ne::NodeId const id)
{
    if (id)
    {
        for(Ptr<WorkspaceNodeWithCoreData> const &node : m_workspaceCoreNodes)
        {
            if(node->getId() == id){ return node; }
            else
            {
                if (node->isSequence())
                {
                    for(auto && inSequenceNode : std::dynamic_pointer_cast<WorkspaceSequence>(node)->getInnerWorkspaceNodes())
                    {
                        if(inSequenceNode->getId() == id){ return inSequenceNode; }
                    }
               }
            }
        }
    }
    return nullptr;
}

Ptr<WorkspaceNodeWithCoreData> WorkspaceWindow::getWorkspaceCoreNodeByPinID(ne::PinId const id)
{
    /* nodes in Sequence have no visible/usable pins */
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
	//ne::SelectNode();
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
        checkQueryElementsDeleting();
        checkQueryElementsCreating();

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
    coreNodeIter node_iter = std::find_if(m_workspaceCoreNodes.begin(), m_workspaceCoreNodes.end(), [nodeId](Ptr<WorkspaceNodeWithCoreData>& node) { return node->getId() == nodeId; });
    if (node_iter != m_workspaceCoreNodes.end())
    {
        m_workspaceCoreNodes.erase(node_iter);
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
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, context_menu_padding);


	if (ImGui::BeginPopup("float_context_menu")) {
		ImGui::Text("Set value...					");
		ImGui::Separator();

		ImGui::Columns(2, "floatPopupButtons", false);
		if (ImGui::SmallButton("Radians"))
			ImGui::OpenPopup("Radians");
		ImGui::NextColumn();
		if (ImGui::SmallButton("Degrees"))
			ImGui::OpenPopup("Degrees");
		ImGui::Columns(1);

		Ptr<WorkspaceNodeWithCoreData> node = getWorkspaceCoreNodeByID(m_contextNodeId);
		float newValue;
		bool valueChange = false;

		if (ImGui::BeginPopup("Radians")) {

			ImGui::Text("Set...                    ");
			ImGui::Separator();
			ImGui::Columns(2, "mycolumns2", false);  // 2-ways, no border

			if (ImGui::Selectable("-PI/6")) {
				newValue = -M_PI / 6;
				valueChange = true;
			}
			if (ImGui::Selectable("-PI/4")) {
				newValue = -M_PI / 4;
				valueChange = true;
			}
			if (ImGui::Selectable("-PI/3")) {
				newValue = -M_PI / 3;
				valueChange = true;
			}
			if (ImGui::Selectable("-PI/2")) {
				newValue = -M_PI / 2;
				valueChange = true;
			}
			if (ImGui::Selectable("-PI")) {
				newValue = -M_PI;
				valueChange = true;
			}
			if (ImGui::Selectable("-3PI/2")) {
				newValue = -3 * M_PI / 2;
				valueChange = true;
			}
			if (ImGui::Selectable("-1")) {
				newValue = -1.0f;
				valueChange = true;
			}
			ImGui::NextColumn();

			if (ImGui::Selectable("PI/6")) {
				newValue = M_PI / 6;
				valueChange = true;
			}
			if (ImGui::Selectable("PI/4")) {
				newValue = M_PI / 4;
				valueChange = true;
			}
			if (ImGui::Selectable("PI/3")) {
				newValue = M_PI / 3;
				valueChange = true;
			}
			if (ImGui::Selectable("PI/2")) {
				newValue = M_PI / 2;
				valueChange = true;
			}
			if (ImGui::Selectable("PI")) {
				newValue = M_PI;
				valueChange = true;
			}
			if (ImGui::Selectable("3PI/2")) {
				newValue = -3 * M_PI / 2;
				valueChange = true;
			}
			if (ImGui::Selectable("1")) {
				newValue = 1.0f;
				valueChange = true;
			}

			ImGui::Columns(1);

			if (ImGui::Selectable("0")) {
				newValue = 0.0f;
				valueChange = true;
			}

			ImGui::EndPopup();
		}
		if (ImGui::BeginPopup("Degrees")) {

			ImGui::Text("Set...                                    ");
			ImGui::Separator();
			ImGui::Columns(2, "mycolumns", false);
			if (ImGui::Selectable("-1/2")) {
				newValue = -1.0f / 2.0f;
				valueChange = true;
			}
			if (ImGui::Selectable("-sqrt(2)/2")) {
				newValue = -sqrt(2) / 2;
				valueChange = true;
			}
			if (ImGui::Selectable("-sqrt(3)/2")) {
				newValue = -sqrt(3) / 2;
				valueChange = true;
			}
			if (ImGui::Selectable("-1")) {
				newValue = -1.0f;
				valueChange = true;
			}

			ImGui::NextColumn();

			if (ImGui::Selectable("1/2")) {
				newValue = 1.0f / 2.0f;;
				valueChange = true;
			}
			if (ImGui::Selectable("sqrt(3)/2")) {
				newValue = sqrt(3) / 2;
				valueChange = true;
			}
			if (ImGui::Selectable("sqrt(2)/2")) {
				newValue = sqrt(2) / 2;
				valueChange = true;
			}
			if (ImGui::Selectable("1")) {
				newValue = 1.0f;
				valueChange = true;
			}
			ImGui::Columns(1);
			if (ImGui::Selectable("0")) {
				newValue = 0.0f;
				valueChange = true;
			}
			ImGui::EndPopup();
		}


		if (node->fw.name == "vector4" && valueChange) {
			glm::vec4 data = node->getNodebase()->getData().getVec4();
			data[node->fw.columns] = newValue;
			node->getNodebase()->setValue(data);
			valueChange = false;
		}
		else if (node->fw.name == "vector3" && valueChange) {
			glm::vec3 data = node->m_nodebase->getData().getVec3();
			data[node->fw.columns] = newValue;
			node->m_nodebase->setValue(data);
			valueChange = false;
		}
		else if (node->fw.name == "float" && valueChange) {
			node->m_nodebase->setValue(newValue);
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
        m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixTranslation>(m_headerBackgroundTexture));
        ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
          m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixScale>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeTranslation>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("eulerAngleX")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeEulerX>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("eulerAngleY")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeEulerY>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("eulerAngleZ")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeEulerZ>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("rotate")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixRotate>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("scale")) {
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeScale>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("ortho")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeOrtho>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("perspective")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakePerspective>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("frustrum")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeFrustum>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("lookAt")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMakeLookAt>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("matrix")) {

				ImGui::Text("matrix operators");
				ImGui::Separator();
				if (ImGui::MenuItem("matrix")) {
					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixFree>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("trackball")) {
				}
				if (ImGui::MenuItem("inversion")) {
					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixInversion>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("transpose")) {
					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixTranspose>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("determinant")) {
					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceDeterminant>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("mat * mat")) {
					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceMatrixMulMatrix>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("mat + mat")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixAddMatrix>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("mat * vec4")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixMulVector>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("vec4 * mat")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorMulMatrix>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("float * mat")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixMulFloat>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("vec3")) {

				ImGui::Text("vec3 operator");
				ImGui::Separator();
				if (ImGui::MenuItem("vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3Free>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("show vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceShowVector3>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("vec3 x vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3CrossVector3>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("vec3 . vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3DotVector3>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("vec3 + vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3AddVector3>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("vec3 - vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3SubVector3>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("float * vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3MulFloat>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("normalize vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceNormalizeVector3>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("length(vec3)")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVector3Length>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("mix vec3")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMixVector3>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("vec4")) {

				ImGui::Text("vec4 operator");
				ImGui::Separator();
				if (ImGui::MenuItem("vec4")) {
					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceVectorFree>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("vec4 . vec4")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorDotVector>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("vec4 + vec4")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorAddVector>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("vec4 - vec4")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorSubVector>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("float * vec4")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorMulFloat>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("normalize vec4")) {
					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceNormalizeVector>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("perspective division")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceVectorPerspectiveDivision>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("mix vec4")) {
					m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMixVector>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
					m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceFloatFree>(m_headerBackgroundTexture));
					ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("clamp float")) {
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceClampFloat>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("float cycle")) {
				}
				if (ImGui::MenuItem("float * float")) {
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatMulFloat>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("float / float")) {
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatDivFloat>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("float + float")) {
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatAddFloat>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("float ^ float")) {
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceFloatPowFloat>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("mix float")) {
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMixFloat>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
				}
				if (ImGui::MenuItem("sin & cos(float)")) {
				}
				if (ImGui::MenuItem("asin & acos(float")) {
				}
				if (ImGui::MenuItem("signum")) {
          m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceSignum>(m_headerBackgroundTexture));
          ne::SetNodePosition(m_workspaceCoreNodes.back()->getId(), m_newNodePostion);
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
           m_workspaceCoreNodes.push_back(std::make_shared<WorkspaceSequence>(m_headerBackgroundTexture));
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


	//ImGui::PopStyleVar();
	ne::Resume();

}












