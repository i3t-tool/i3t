/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.

    Details no here now... Cca in middle is file split between example and used part
*/

#include "WorkspaceWindow.h"

#include "../Nodes/DeterminantImpl.h"
#include "../Nodes/FloatImpl.h"
#include "../Nodes/FloatMulFloatImpl.h"
#include "../Nodes/MatrixImpl.h"
#include "../Nodes/NormalizeVectorImpl.h"

#include "../Nodes/WorkspaceNodeWithCoreData.h"

#include "../Nodes/WorkspaceMatrixTranslation.h"
#include "../Nodes/WorkspaceMatrixScale.h"

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
	m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>( HeaderBackgroundTexture ));
	ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, ImVec2(-252, 220));

    /*--- TRANSLATION with connection to translation */
	m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixTranslation>(HeaderBackgroundTexture));
	ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, ImVec2(-300, 351));

	/* \todo JH nyni nejde v Core spojovat dva operatory - proto to nefuguje... */
//	Core::GraphManager::plug(m_workspaceCoreNodes.at(0)->m_nodebase,
//	                         m_workspaceCoreNodes.at(1)->m_nodebase, 0, 0);

	/*--- MATRIX_4x4 with connection to translation*/
	m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrix4x4>(HeaderBackgroundTexture));
	ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, ImVec2(-500, 351));

//    Core::GraphManager::plug(m_workspaceCoreNodes.at(0)->m_nodebase,
//	                         m_workspaceCoreNodes.at(2)->m_nodebase, 0, 0);

    /*--- SCALE */
	m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceMatrixScale>(HeaderBackgroundTexture));
	ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, ImVec2(-500, 351));

	/*--- NORMALIZE VECTOR */
	m_workspaceCoreNodes.push_back(std::make_unique<WorkspaceNormalizeVector>(HeaderBackgroundTexture));
	ne::SetNodePosition(m_workspaceCoreNodes.back()->m_id, ImVec2(100, 400));

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
	ne::Begin("Node editor");{
    ImVec2 cursorTopLeft = ImGui::GetCursorScreenPos();

	for (auto&& workspaceCoreNode : m_workspaceCoreNodes)
	{
		workspaceCoreNode->drawNode(NodeBuilderContext, nullptr);
	}

    /* both connected pins have to be drawn before link is drawn -> therefore separated for */
    for (auto& workspaceCoreNode : m_workspaceCoreNodes)
    {
        workspaceCoreNode->drawInputLinks();
    }

    checkQueryElements();

    checkQueryContextMenus();

    ImGui::SetCursorScreenPos(cursorTopLeft);

    shiftSelectedNodesToFront();




	}ne::End();

	ImGui::End();
	/* ed::SetCurrentEditor(m_Editor); \todo maybe call it here for static load of current editor setting (if changed)
	 */
/*
	//    auto& io = ImGui::GetIO();
	//
	//    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

	//
	//    contextNodeId = 0;
	//    contextLinkId = 0;
	//    contextPinId = 0;
	//    createNewNode = false;
	//    newNodeLinkPin = nullptr;
	//    newLinkPin = nullptr;
	//
	//    leftPaneWidth = 400.0f;
	//    rightPaneWidth = 800.0f;
	//
	//
	//    Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);
	//
	//    ShowLeftPane(leftPaneWidth - 4.0f);
	//
	//    ImGui::SameLine(0.0f, 12.0f);
	//
	//    ne::Begin("Node editor");
	//    {
	//      auto cursorTopLeft = ImGui::GetCursorScreenPos();
	//
	//
	//     //create link
	//     if (!createNewNode)
	//      {
	//        if (ne::BeginCreate(ImColor(255, 255, 255), 2.0f)){
	//
	//          auto showLabel = [](const char* label, ImColor color) {
	//            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
	//            auto size = ImGui::CalcTextSize(label);
	//
	//            auto padding = ImGui::GetStyle().FramePadding;
	//            auto spacing = ImGui::GetStyle().ItemSpacing;
	//
	//            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));
	//
	//            auto rectMin = ImGui::GetCursorScreenPos() - padding;
	//            auto rectMax = ImGui::GetCursorScreenPos() + size + padding;
	//
	//            auto drawList = ImGui::GetWindowDrawList();
	//            drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
	//            ImGui::TextUnformatted(label);
	//          };
	//
	//          ne::PinId startPinId = 0, endPinId = 0;
	//          if (ne::QueryNewLink(&startPinId, &endPinId))
	//          {
	//            auto startPin = FindPin(startPinId);
	//            auto endPin = FindPin(endPinId);
	//
	//            newLinkPin = startPin ? startPin : endPin;
	//
	//            if (startPin->Kind == PinKind::Input)
	//            {
	//              std::swap(startPin, endPin);
	//              std::swap(startPinId, endPinId);
	//            }
	//
	//            if (startPin && endPin)
	//            {
	//              if (endPin == startPin)
	//              {
	//                ne::RejectNewItem(ImColor(255, 0, 0), 2.0f);
	//              }
	//              else if (endPin->Kind == startPin->Kind)
	//              {
	//                showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
	//                ne::RejectNewItem(ImColor(255, 0, 0), 2.0f);
	//              }
	//              else if (endPin->Node == startPin->Node)
	//              {
	//                  showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
	//                  ne::RejectNewItem(ImColor(255, 0, 0), 1.0f);
	//              }
	//              else if (endPin->Type != startPin->Type)
	//              {
	//                showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
	//                ne::RejectNewItem(ImColor(255, 128, 128), 1.0f);
	//              }
	//              else
	//              {
	//                showLabel("+ Create Link", ImColor(32, 45, 32, 180));
	//                if (ne::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
	//                {
	//                  Links.emplace_back(WorkspaceLink(getNew_s_ID(), startPin, endPin));
	//                  Links.back().Color = WorkspacePinColor[startPin->Type];
	//
	//                  auto result = GraphManager::plug(startPin->Node->Nodebase, endPin->Node->Nodebase, 0, 0);
	//                  if (result != ENodePlugResult::Ok)
	//                  {
	//                    // print result;
	//                  }
	//                }
	//              }
	//            }
	//          }
	//
	//          ne::PinId pinId = 0;
	//          if (ne::QueryNewNode(&pinId))
	//          {
	//            newLinkPin = FindPin(pinId);
	//            if (newLinkPin)
	//              showLabel("+ Create Namespace", ImColor(32, 45, 32, 180));
	//
	//            if (ne::AcceptNewItem())
	//            {
	//              createNewNode = true;
	//              newNodeLinkPin = FindPin(pinId);
	//              newLinkPin = nullptr;
	//              ne::Suspend();
	//              ImGui::OpenPopup("Create New Namespace");
	//              ne::Resume();
	//            }
	//          }
	//        }
	//        else
	//          newLinkPin = nullptr;
	//
	//        ne::EndCreate();
	//
	//        if (ne::BeginDelete())
	//        {
	//          DeleteNode();
	//        }
	//        ne::EndDelete();
	//      }
	//
	//      ImGui::SetCursorScreenPos(cursorTopLeft);
	//    }
	//
	//    popupMenu(createNewNode, newNodeLinkPin, contextNodeId, contextPinId, contextLinkId);
	//    ne::Resume();
	//
	*/
}

/* \todo JH not work yet - should avoid capturing actions in bottom nodes when overlaping ( https://github.com/thedmd/imgui-node-editor/issues/81 ) */
void WorkspaceWindow::shiftSelectedNodesToFront()
{
    if (ne::HasSelectionChanged())
    {
        std::vector<Ptr<WorkspaceNodeWithCoreData>> selectedCoreNodes = getSelectedWorkspaceCoreNodes();

        for(int i=0; i < selectedCoreNodes.size(); i++)
        {

            coreNodeIter node = std::find_if(m_workspaceCoreNodes.begin(),
                                            m_workspaceCoreNodes.end(),
                                            [selectedCoreNodes, i](Ptr<WorkspaceNodeWithCoreData> const &n) -> bool { return n->m_id == selectedCoreNodes.at(i)->m_id; });

            if (node != m_workspaceCoreNodes.end())
            {
              std::iter_swap(m_workspaceCoreNodes.begin()+i, node);
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
    if (ne::ShowLinkContextMenu(&m_contextLinkId))
        ImGui::OpenPopup("Link Context Menu");
    else if (ne::ShowPinContextMenu(&m_contextPinId))
        ImGui::OpenPopup("Pin Context Menu");
    else if (ne::ShowNodeContextMenu(&m_contextNodeId))
        ImGui::OpenPopup("Node Context Menu");
    else if (ne::ShowBackgroundContextMenu())
    {
        ImGui::OpenPopup("Create New Node");
        m_pinPropertiesForNewNodeLink = nullptr;
    }
    ne::Resume();

    ne::Suspend();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("Node Context Menu"))
    {
//        auto node = FindNode(contextNodeId);
//
//        ImGui::TextUnformatted("Node Context Menu");
//        ImGui::Separator();
//        if (node)
//        {
//            ImGui::Text("ID: %p", node->ID.AsPointer());
//            ImGui::Text("Type: %s", node->Type == NodeType::Blueprint ? "Blueprint" : (node->Type == NodeType::Tree ? "Tree" : "Comment"));
//            ImGui::Text("Inputs: %d", (int)node->Inputs.size());
//            ImGui::Text("Outputs: %d", (int)node->Outputs.size());
//        }
//        else
//            ImGui::Text("Unknown node: %p", contextNodeId.AsPointer());
//        ImGui::Separator();
//        if (ImGui::MenuItem("Delete"))
//            ed::DeleteNode(contextNodeId);
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("Pin Context Menu"))
    {
//        auto pin = FindPin(contextPinId);
//
//        ImGui::TextUnformatted("Pin Context Menu");
//        ImGui::Separator();
//        if (pin)
//        {
//            ImGui::Text("ID: %p", pin->ID.AsPointer());
//            if (pin->Node)
//                ImGui::Text("Node: %p", pin->Node->ID.AsPointer());
//            else
//                ImGui::Text("Node: %s", "<none>");
//        }
//        else
//            ImGui::Text("Unknown pin: %p", contextPinId.AsPointer());

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("Link Context Menu"))
    {
//        auto link = FindLink(contextLinkId);
//
//        ImGui::TextUnformatted("Link Context Menu");
//        ImGui::Separator();
//        if (link)
//        {
//            ImGui::Text("ID: %p", link->ID.AsPointer());
//            ImGui::Text("From: %p", link->StartPinID.AsPointer());
//            ImGui::Text("To: %p", link->EndPinID.AsPointer());
//        }
//        else
//            ImGui::Text("Unknown link: %p", contextLinkId.AsPointer());
//        ImGui::Separator();
//        if (ImGui::MenuItem("Delete"))
//            ed::DeleteLink(contextLinkId);
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("Create New Node"))
    {
//        auto newNodePostion = openPopupPosition;
//        //ImGui::SetCursorScreenPos(ImGui::GetMousePosOnOpeningCurrentPopup());
//
//        //auto drawList = ImGui::GetWindowDrawList();
//        //drawList->AddCircleFilled(ImGui::GetMousePosOnOpeningCurrentPopup(), 10.0f, 0xFFFF00FF);
//
//        Node* node = nullptr;
//        if (ImGui::MenuItem("Input Action"))
//            node = SpawnInputActionNode();
//        if (ImGui::MenuItem("Output Action"))
//            node = SpawnOutputActionNode();
//        if (ImGui::MenuItem("Branch"))
//            node = SpawnBranchNode();
//        if (ImGui::MenuItem("Do N"))
//            node = SpawnDoNNode();
//        if (ImGui::MenuItem("Set Timer"))
//            node = SpawnSetTimerNode();
//        if (ImGui::MenuItem("Less"))
//            node = SpawnLessNode();
//        if (ImGui::MenuItem("Weird"))
//            node = SpawnWeirdNode();
//        if (ImGui::MenuItem("Trace by Channel"))
//            node = SpawnTraceByChannelNode();
//        if (ImGui::MenuItem("Print String"))
//            node = SpawnPrintStringNode();
//        ImGui::Separator();
//        if (ImGui::MenuItem("Comment"))
//            node = SpawnComment();
//        ImGui::Separator();
//        if (ImGui::MenuItem("Sequence"))
//            node = SpawnTreeSequenceNode();
//        if (ImGui::MenuItem("Move To"))
//            node = SpawnTreeTaskNode();
//        if (ImGui::MenuItem("Random Wait"))
//            node = SpawnTreeTask2Node();
//        ImGui::Separator();
//        if (ImGui::MenuItem("Message"))
//            node = SpawnMessageNode();
//        ImGui::Separator();
//        if (ImGui::MenuItem("Transform"))
//            node = SpawnHoudiniTransformNode();
//        if (ImGui::MenuItem("Group"))
//            node = SpawnHoudiniGroupNode();
//
//        if (node)
//        {
//            BuildNodes();
//
//            createNewNode = false;
//
//            ed::SetNodePosition(node->ID, newNodePostion);
//
//            if (auto startPin = newNodeLinkPin)
//            {
//                auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;
//
//                for (auto& pin : pins)
//                {
//                    if (CanCreateLink(startPin, &pin))
//                    {
//                        auto endPin = &pin;
//                        if (startPin->Kind == PinKind::Input)
//                            std::swap(startPin, endPin);
//
//                        s_Links.emplace_back(Link(GetNextId(), startPin->ID, endPin->ID));
//                        s_Links.back().Color = GetIconColor(startPin->Type);
//
//                        break;
//                    }
//                }
//            }
//        }

        ImGui::EndPopup();
    }
    else
        m_createNewNode = false;
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

// bool WorkspaceWindow::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1,
//                     float min_size2, float splitter_long_axis_size = -1.0f)
//{
//  using namespace ImGui;
//  ImGuiContext& g = *GImGui;
//  ImGuiWindow* window = g.CurrentWindow;
//  ImGuiID id = window->GetID("##Splitter");
//  ImRect bb;
//  bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
//  bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size)
//                                                  : ImVec2(splitter_long_axis_size, thickness),
//                                 0.0f, 0.0f);
//  return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2,
//                          0.0f);
//}
//
//
// void WorkspaceWindow::ShowStyleEditor(bool* show = nullptr)
//{
//  if (!ImGui::Begin("Style", show))
//  {
//    ImGui::End();
//    return;
//  }
//
//  auto paneWidth = ImGui::GetContentRegionAvailWidth();
//
//  auto& editorStyle = ne::GetStyle();
//  ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
//  ImGui::TextUnformatted("Values");
//  ImGui::Spring();
//  if (ImGui::Button("Reset to defaults"))
//    editorStyle = ne::Style();
//  ImGui::EndHorizontal();
//  ImGui::Spacing();
//  ImGui::DragFloat4("Namespace Padding", &editorStyle.NodePadding.x, 0.1f, 0.0f, 40.0f);
//  ImGui::DragFloat("Namespace Rounding", &editorStyle.NodeRounding, 0.1f, 0.0f, 40.0f);
//  ImGui::DragFloat("Namespace Border Width", &editorStyle.NodeBorderWidth, 0.1f, 0.0f, 15.0f);
//  ImGui::DragFloat("Hovered Namespace Border Width", &editorStyle.HoveredNodeBorderWidth, 0.1f, 0.0f, 15.0f);
//  ImGui::DragFloat("Selected Namespace Border Width", &editorStyle.SelectedNodeBorderWidth, 0.1f, 0.0f, 15.0f);
//  ImGui::DragFloat("Pin Rounding", &editorStyle.PinRounding, 0.1f, 0.0f, 40.0f);
//  ImGui::DragFloat("Pin Border Width", &editorStyle.PinBorderWidth, 0.1f, 0.0f, 15.0f);
//  ImGui::DragFloat("Link Strength", &editorStyle.LinkStrength, 1.0f, 0.0f, 500.0f);
//  // ImVec2  SourceDirection;
//  // ImVec2  TargetDirection;
//  ImGui::DragFloat("Scroll Duration", &editorStyle.ScrollDuration, 0.001f, 0.0f, 2.0f);
//  ImGui::DragFloat("Flow Marker Distance", &editorStyle.FlowMarkerDistance, 1.0f, 1.0f, 200.0f);
//  ImGui::DragFloat("Flow Speed", &editorStyle.FlowSpeed, 1.0f, 1.0f, 2000.0f);
//  ImGui::DragFloat("Flow Duration", &editorStyle.FlowDuration, 0.001f, 0.0f, 5.0f);
//  // ImVec2  PivotAlignment;
//  // ImVec2  PivotSize;
//  // ImVec2  PivotScale;
//  // float   PinCorners;
//  // float   PinRadius;
//  // float   PinArrowSize;
//  // float   PinArrowWidth;
//  ImGui::DragFloat("Group Rounding", &editorStyle.GroupRounding, 0.1f, 0.0f, 40.0f);
//  ImGui::DragFloat("Group Border Width", &editorStyle.GroupBorderWidth, 0.1f, 0.0f, 15.0f);
//
//  ImGui::Separator();
//
//  static ImGuiColorEditFlags edit_mode = ImGuiColorEditFlags_RGB;
//  ImGui::BeginHorizontal("Color Mode", ImVec2(paneWidth, 0), 1.0f);
//  ImGui::TextUnformatted("Filter Colors");
//  ImGui::Spring();
//  ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditFlags_RGB);
//  ImGui::Spring(0);
//  ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditFlags_HSV);
//  ImGui::Spring(0);
//  ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditFlags_HEX);
//  ImGui::EndHorizontal();
//
//  static ImGuiTextFilter filter;
//  filter.Draw("", paneWidth);
//
//  ImGui::Spacing();
//
//  ImGui::PushItemWidth(-160);
//  for (int i = 0; i < ne::StyleColor_Count; ++i)
//  {
//    auto name = ne::GetStyleColorName((ne::StyleColor)i);
//    if (!filter.PassFilter(name))
//      continue;
//
//    ImGui::ColorEdit4(name, &editorStyle.Colors[i].x, edit_mode);
//  }
//  ImGui::PopItemWidth();
//
//  ImGui::End();
//}
//
// void WorkspaceWindow::ShowLeftPane(float paneWidth)
//{
//  auto& io = ImGui::GetIO();
//
//  ImGui::BeginChild("Selection", ImVec2(paneWidth, 0));
//
//  paneWidth = ImGui::GetContentRegionAvailWidth();
//
//  static bool showStyleEditor = false;
//  ImGui::BeginHorizontal("Style Editor", ImVec2(paneWidth, 0));
//  ImGui::Spring(0.0f, 0.0f);
//  if (ImGui::Button("Zoom to Content"))
//    ne::NavigateToContent();
//  ImGui::Spring(0.0f);
//  if (ImGui::Button("Show Flow"))
//  {
//    for (auto& link : Links)
//      ne::Flow(link.ID);
//  }
//  ImGui::Spring();
//  if (ImGui::Button("Edit Style"))
//    showStyleEditor = true;
//  ImGui::EndHorizontal();
//
//  if (showStyleEditor)
//    ShowStyleEditor(&showStyleEditor);
//
//  std::vector<ne::NodeId> selectedNodes;
//  std::vector<ne::LinkId> selectedLinks;
//  selectedNodes.resize(ne::GetSelectedObjectCount());
//  selectedLinks.resize(ne::GetSelectedObjectCount());
//
//  int nodeCount = ne::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
//  int linkCount = ne::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));
//
//  selectedNodes.resize(nodeCount);
//  selectedLinks.resize(linkCount);
//
//  int saveIconWidth = 32;//Application_GetTextureWidth(s_SaveIcon);
//  int saveIconHeight = 32; // Application_GetTextureWidth(s_SaveIcon);
//  int restoreIconWidth = 32; // Application_GetTextureWidth(s_RestoreIcon);
//  int restoreIconHeight = 32; // Application_GetTextureWidth(s_RestoreIcon);
//
//  ImGui::GetWindowDrawList()->AddRectFilled(
//      ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(paneWidth, ImGui::GetTextLineHeight()),
//      ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
//  ImGui::Spacing();
//  ImGui::SameLine();
//  ImGui::TextUnformatted("Nodes");
//  ImGui::Indent();
//  for (auto& node : Nodes)
//  {
//    ImGui::PushID(node->ID.AsPointer());
//    auto start = ImGui::GetCursorScreenPos();
//
//    if (const auto progress = GetTouchProgress(node->ID))
//    {
//      ImGui::GetWindowDrawList()->AddLine(start + ImVec2(-8, 0), start + ImVec2(-8, ImGui::GetTextLineHeight()),
//                                          IM_COL32(255, 0, 0, 255 - (int)(255 * progress)), 4.0f);
//    }
//
//    bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), node->ID) != selectedNodes.end();
//    if (ImGui::Selectable(
//            (node->Name + "##" + std::to_string(reinterpret_cast<uintptr_t>(node->ID.AsPointer()))).c_str(),
//            &isSelected))
//    {
//      if (io.KeyCtrl)
//      {
//        if (isSelected)
//          ne::SelectNode(node->ID, true);
//        else
//          ne::DeselectNode(node->ID);
//      }
//      else
//        ne::SelectNode(node->ID, false);
//
//      ne::NavigateToSelection();
//    }
//    if (ImGui::IsItemHovered() && !node->State.empty())
//      ImGui::SetTooltip("State: %s", node->State.c_str());
//
//    auto id = std::string("(") + std::to_string(reinterpret_cast<uintptr_t>(node->ID.AsPointer())) + ")";
//    auto textSize = ImGui::CalcTextSize(id.c_str(), nullptr);
//    auto iconPanelPos =
//        start + ImVec2(paneWidth - ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().IndentSpacing -
//                           saveIconWidth - restoreIconWidth - ImGui::GetStyle().ItemInnerSpacing.x * 1,
//                       (ImGui::GetTextLineHeight() - saveIconHeight) / 2);
//    ImGui::GetWindowDrawList()->AddText(
//        ImVec2(iconPanelPos.x - textSize.x - ImGui::GetStyle().ItemInnerSpacing.x, start.y),
//        IM_COL32(255, 255, 255, 255), id.c_str(), nullptr);
//
//    auto drawList = ImGui::GetWindowDrawList();
//    ImGui::SetCursorScreenPos(iconPanelPos);
//    ImGui::SetItemAllowOverlap();
//    /*if (node->SavedState.empty())
//    {
//      if (ImGui::InvisibleButton("save", ImVec2((float)saveIconWidth, (float)saveIconHeight)))
//        node->SavedState = node->State;
//
//      if (ImGui::IsItemActive())
//        drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
//                           IM_COL32(255, 255, 255, 96));
//      else if (ImGui::IsItemHovered())
//        drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
//                           IM_COL32(255, 255, 255, 255));
//      else
//        drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
//                           IM_COL32(255, 255, 255, 160));
//    }
//    else
//    {
//      ImGui::Dummy(ImVec2((float)saveIconWidth, (float)saveIconHeight));
//      drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
//                         IM_COL32(255, 255, 255, 32));
//    }*/
//
//    ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
//    ImGui::SetItemAllowOverlap();
//    /*if (!node->SavedState.empty())
//    {
//      if (ImGui::InvisibleButton("restore", ImVec2((float)restoreIconWidth, (float)restoreIconHeight)))
//      {
//        node->State = node->SavedState;
//        ne::RestoreNodeState(node->ID);
//        node->SavedState.clear();
//      }
//
//      if (ImGui::IsItemActive())
//        drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0),
//                           ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
//      else if (ImGui::IsItemHovered())
//        drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0),
//                           ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
//      else
//        drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0),
//                           ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
//    }
//    else
//    {
//      ImGui::Dummy(ImVec2((float)restoreIconWidth, (float)restoreIconHeight));
//      drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1,
//      1),
//                         IM_COL32(255, 255, 255, 32));
//    }*/
//
//    ImGui::SameLine(0, 0);
//    ImGui::SetItemAllowOverlap();
//    ImGui::Dummy(ImVec2(0, (float)restoreIconHeight));
//
//    ImGui::PopID();
//  }
//  ImGui::Unindent();
//
//  static int changeCount = 0;
//
//  ImGui::GetWindowDrawList()->AddRectFilled(
//      ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(paneWidth, ImGui::GetTextLineHeight()),
//      ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
//  ImGui::Spacing();
//  ImGui::SameLine();
//  ImGui::TextUnformatted("Selection");
//
//  ImGui::BeginHorizontal("Selection Stats", ImVec2(paneWidth, 0));
//  //ImGui::Text("Changed %d time%s", changeCount, changeCount > 1 ? "s" : "");
//  ImGui::Spring();
//  if (ImGui::Button("Deselect All"))
//    ne::ClearSelection();
//  ImGui::EndHorizontal();
//  ImGui::Indent();
//  for (int i = 0; i < nodeCount; ++i)
//    ImGui::Text("Namespace (%p)", selectedNodes[i].AsPointer());
//  for (int i = 0; i < linkCount; ++i)
//    ImGui::Text("Link (%p)", selectedLinks[i].AsPointer());
//  ImGui::Unindent();
//
//  if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
//    for (auto& link : s_Links)
//      ne::Flow(link.ID);
//
//  if (ne::HasSelectionChanged())
//    ++changeCount;
//
//  ImGui::EndChild();
//}
//

// void popupMenu(bool& createNewNode, GUIPin* newNodeLinkPin, ne::NodeId& contextNodeId,
//               ne::PinId& contextPinId,
//               ne::LinkId& contextLinkId)
//{
//  auto openPopupPosition = ImGui::GetMousePos();
//  ne::Suspend();
//  if (ne::ShowNodeContextMenu(&contextNodeId))
//    ImGui::OpenPopup("Namespace Context Menu");
//  else if (ne::ShowPinContextMenu(&contextPinId))
//    ImGui::OpenPopup("Pin Context Menu");
//  else if (ne::ShowLinkContextMenu(&contextLinkId))
//    ImGui::OpenPopup("Link Context Menu");
//  else if (ne::ShowBackgroundContextMenu())
//  {
//    ImGui::OpenPopup("Create New Namespace");
//    newNodeLinkPin = nullptr;
//  }
//  ne::Resume();
//
//  ne::Suspend();
//  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
//  if (ImGui::BeginPopup("Namespace Context Menu"))
//  {
//    auto node = FindNode(contextNodeId);
//
//    ImGui::TextUnformatted("Namespace Context Menu");
//    ImGui::Separator();
//    if (node)
//    {
//      ImGui::Text("ID: %p", node->ID.AsPointer());
//      //ImGui::Text("Type: %s", static_cast<ENodeType>(node->Type)); //TODO print type
//      ImGui::Text("Inputs: %d", (int)node->Inputs.size());
//      ImGui::Text("Outputs: %d", (int)node->Outputs.size());
//    }
//    else
//      ImGui::Text("Unknown node: %p", contextNodeId.AsPointer());
//    ImGui::Separator();
//    if (ImGui::MenuItem("Delete"))
//      ne::DeleteNode(contextNodeId);
//    ImGui::EndPopup();
//  }
//
//  if (ImGui::BeginPopup("Pin Context Menu"))
//  {
//    auto pin = FindPin(contextPinId);
//
//    ImGui::TextUnformatted("Pin Context Menu");
//    ImGui::Separator();
//    if (pin)
//    {
//      ImGui::Text("ID: %p", pin->ID.AsPointer());
//      if (pin->Node)
//        ImGui::Text("Namespace: %p", pin->Node->ID.AsPointer());
//      else
//        ImGui::Text("Namespace: %s", "<none>");
//    }
//    else
//      ImGui::Text("Unknown pin: %p", contextPinId.AsPointer());
//
//    ImGui::EndPopup();
//  }
//
//  if (ImGui::BeginPopup("Link Context Menu"))
//  {
//    auto link = FindLink(contextLinkId);
//
//    ImGui::TextUnformatted("Link Context Menu");
//    ImGui::Separator();
//    if (link)
//    {
//      ImGui::Text("ID: %p", link->ID.AsPointer());
//      ImGui::Text("From: %p", link->StartPin->ID.AsPointer());
//      ImGui::Text("To: %p", link->EndPin->ID.AsPointer());
//    }
//    else
//      ImGui::Text("Unknown link: %p", contextLinkId.AsPointer());
//    ImGui::Separator();
//    if (ImGui::MenuItem("Delete"))
//      ne::DeleteLink(contextLinkId);
//    ImGui::EndPopup();
//  }
//
//  if (ImGui::BeginPopup("Create New Namespace"))
//  {
//    auto newNodePostion = openPopupPosition;
//    ImGui::SetCursorScreenPos(ImGui::GetMousePosOnOpeningCurrentPopup());
//
//    auto drawList = ImGui::GetWindowDrawList();
//    drawList->AddCircleFilled(ImGui::GetMousePosOnOpeningCurrentPopup(), 10.0f, 0xFFFF00FF);
//
//    Namespace* node = nullptr;
//    if (ImGui::MenuItem("Matrix 4x4")){
//      MatrixImpl* mat = new MatrixImpl();
//        node = mat->SpawnNode(&s_Nodes);
//    }
//    if (ImGui::MenuItem("Normalize Vector")){
//      NormalizeVectorImpl* nv = new NormalizeVectorImpl();
//      node = nv->SpawnNode(&s_Nodes);
//    }
//    if (ImGui::MenuItem("Determinant"))
//    {
//      DeterminantImpl* det = new DeterminantImpl();
//      node = det->SpawnNode(&s_Nodes);
//    }
//    if (ImGui::MenuItem("Float"))
//    {
//      FloatImpl* fl = new FloatImpl();
//      node = fl->SpawnNode(&s_Nodes);
//    }
//    if (ImGui::MenuItem("Float * Float"))
//    {
//      FloatMulFloatImpl* fl = new FloatMulFloatImpl();
//      node = fl->SpawnNode(&s_Nodes);
//    }
//
//    if (node)
//    {
//
//      createNewNode = true;
//
//      ne::SetNodePosition(node->ID, newNodePostion);
//
//      if (auto startPin = newNodeLinkPin)
//      {
//        auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;
//
//        for (auto& pin : pins)
//        {
//          if (CanCreateLink(startPin, pin))
//          {
//            auto endPin = pin;
//            if (startPin->Kind == PinKind::Input)
//              std::swap(startPin, endPin);
//
//            s_Links.emplace_back(Link(GetNextId(), startPin, endPin));
//            s_Links.back().Color = GetIconColor(startPin->Type);
//
//            //TODO predelat indexy v ramci jedny krabicky
//            //auto result = node->nodebase->plugToParent(startPin->Node->nodebase, 0, 0);
//            auto result = GraphManager::plug(startPin->Node->nodebase, endPin->Node->nodebase, 0, 0);
//            if (result != ENodePlugResult::Ok) {
//                //print result;
//            }
//
//            break;
//          }
//        }
//      }
//    }
//
//    ImGui::EndPopup();
//  }
//  else
//    createNewNode = false;
//  ImGui::PopStyleVar();
//}
//
// void DeleteNode()
//{
//  ne::LinkId linkId = 0;
//  while (ne::QueryDeletedLink(&linkId))
//  {
//    if (ne::AcceptDeletedItem())
//    {
//      auto id = std::find_if(Links.begin(), Links.end(), [linkId](auto& link) { return link.ID == linkId; });
//      if (id != Links.end())
//        Links.erase(id);
//    }
//
//  }
//
//  ne::NodeId nodeId = 0;
//  while (ne::QueryDeletedNode(&nodeId))
//  {
//    auto node = FindNode(nodeId);
//    GraphManager::unplugAll(node->nodebase);
//    node->nodebase = nullptr;
//
//    if (ne::AcceptDeletedItem())
//    {
//      auto id = std::find_if(Nodes.begin(), Nodes.end(), [nodeId](auto& node) { return node->ID == nodeId; });
//      if (id != Nodes.end())
//        Nodes.erase(id);
//    }
//  }
//}
//

void maybeUsefulCode()
{
	/*for (auto& node : s_Nodes)
	  {
	    if (node.Type != NodeType::Houdini)
	      continue;

	    const float rounding = 10.0f;
	    const float padding = 12.0f;

	    ne::PushStyleColor(ne::StyleColor_NodeBg, ImColor(229, 229, 229, 200));
	    ne::PushStyleColor(ne::StyleColor_NodeBorder, ImColor(125, 125, 125, 200));
	    ne::PushStyleColor(ne::StyleColor_PinRect, ImColor(229, 229, 229, 60));
	    ne::PushStyleColor(ne::StyleColor_PinRectBorder, ImColor(125, 125, 125, 60));

	    const auto pinBackground = ne::GetStyle().Colors[ne::StyleColor_NodeBg];

	    ne::PushStyleVar(ne::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
	    ne::PushStyleVar(ne::StyleVar_NodeRounding, rounding);
	    ne::PushStyleVar(ne::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
	    ne::PushStyleVar(ne::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
	    ne::PushStyleVar(ne::StyleVar_LinkStrength, 0.0f);
	    ne::PushStyleVar(ne::StyleVar_PinBorderWidth, 1.0f);
	    ne::PushStyleVar(ne::StyleVar_PinRadius, 6.0f);
	    ne::BeginNode(node.ID);

	    ImGui::BeginVertical(node.ID.AsPointer());
	    if (!node.Inputs.empty())
	    {
	      ImGui::BeginHorizontal("inputs");
	      ImGui::Spring(1, 0);

	      ImRect inputsRect;
	      int inputAlpha = 200;
	      for (auto& pin : node.Inputs)
	      {
	        ImGui::Dummy(ImVec2(padding, padding));
	        inputsRect = ImGui_GetItemRect();
	        ImGui::Spring(1, 0);
	        inputsRect.Min.y -= padding;
	        inputsRect.Max.y -= padding;

	        // ne::PushStyleVar(ne::StyleVar_PinArrowSize, 10.0f);
	        // ne::PushStyleVar(ne::StyleVar_PinArrowWidth, 10.0f);
	        ne::PushStyleVar(ne::StyleVar_PinCorners, 15);
	        ne::BeginPin(pin.ID, ne::PinKind::Input);
	        ne::PinPivotRect(inputsRect.GetCenter(), inputsRect.GetCenter());
	        ne::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
	        ne::EndPin();
	        // ne::PopStyleVar(3);
	        ne::PopStyleVar(1);

	        auto drawList = ImGui::GetWindowDrawList();
	        drawList->AddRectFilled(inputsRect.GetTL(), inputsRect.GetBR(),
	                                IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y),
	                                         (int)(255 * pinBackground.z), inputAlpha),
	                                4.0f, 15);
	        drawList->AddRect(inputsRect.GetTL(), inputsRect.GetBR(),
	                          IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y),
	                                   (int)(255 * pinBackground.z), inputAlpha),
	                          4.0f, 15);

	        if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
	          inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
	      }

	      // ImGui::Spring(1, 0);
	      ImGui::EndHorizontal();
	    }

	    ImGui::BeginHorizontal("content_frame");
	    ImGui::Spring(1, padding);

	    ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
	    ImGui::Dummy(ImVec2(160, 0));
	    ImGui::Spring(1);
	    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	    ImGui::TextUnformatted(node.Name.c_str());
	    ImGui::PopStyleColor();
	    ImGui::Spring(1);
	    ImGui::EndVertical();
	    auto contentRect = ImGui_GetItemRect();

	    ImGui::Spring(1, padding);
	    ImGui::EndHorizontal();

	    if (!node.Outputs.empty())
	    {
	      ImGui::BeginHorizontal("outputs");
	      ImGui::Spring(1, 0);

	      ImRect outputsRect;
	      int outputAlpha = 200;
	      for (auto& pin : node.Outputs)
	      {
	        ImGui::Dummy(ImVec2(padding, padding));
	        outputsRect = ImGui_GetItemRect();
	        ImGui::Spring(1, 0);
	        outputsRect.Min.y += padding;
	        outputsRect.Max.y += padding;

	        ne::PushStyleVar(ne::StyleVar_PinCorners, 3);
	        ne::BeginPin(pin.ID, ne::PinKind::Output);
	        ne::PinPivotRect(outputsRect.GetCenter(), outputsRect.GetCenter());
	        ne::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
	        ne::EndPin();
	        ne::PopStyleVar();

	        auto drawList = ImGui::GetWindowDrawList();
	        drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR(),
	                                IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y),
	                                         (int)(255 * pinBackground.z), outputAlpha),
	                                4.0f, 15);
	        drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR(),
	                          IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y),
	                                   (int)(255 * pinBackground.z), outputAlpha),
	                          4.0f, 15);

	        if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
	          outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
	      }

	      ImGui::EndHorizontal();
	    }

	    ImGui::EndVertical();

	    ne::EndNode();
	    ne::PopStyleVar(7);
	    ne::PopStyleColor(4);

	    auto drawList = ne::GetNodeBackgroundDrawList(node.ID);
	  }*/

	// Making tree nodes
	/*for (auto& node : s_Nodes)
	      {
	        if (node.Type != NodeType::Tree)
	          continue;

	        const float rounding = 5.0f;
	        const float padding = 12.0f;

	        const auto pinBackground = ne::GetStyle().Colors[ne::StyleColor_NodeBg];

	        ne::PushStyleColor(ne::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
	        ne::PushStyleColor(ne::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
	        ne::PushStyleColor(ne::StyleColor_PinRect, ImColor(60, 180, 255, 150));
	        ne::PushStyleColor(ne::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

	        ne::PushStyleVar(ne::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
	        ne::PushStyleVar(ne::StyleVar_NodeRounding, rounding);
	        ne::PushStyleVar(ne::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
	        ne::PushStyleVar(ne::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
	        ne::PushStyleVar(ne::StyleVar_LinkStrength, 0.0f);
	        ne::PushStyleVar(ne::StyleVar_PinBorderWidth, 1.0f);
	        ne::PushStyleVar(ne::StyleVar_PinRadius, 5.0f);
	        ne::BeginNode(node.ID);

	        ImGui::BeginVertical(node.ID.AsPointer());
	        ImGui::BeginHorizontal("inputs");
	        ImGui::Spring(0, padding * 2);

	        ImRect inputsRect;
	        int inputAlpha = 200;
	        if (!node.Inputs.empty())
	        {
	          auto& pin = node.Inputs[0];
	          ImGui::Dummy(ImVec2(0, padding));
	          ImGui::Spring(1, 0);
	          inputsRect = ImGui_GetItemRect();

	          ne::PushStyleVar(ne::StyleVar_PinArrowSize, 10.0f);
	          ne::PushStyleVar(ne::StyleVar_PinArrowWidth, 10.0f);
	          ne::PushStyleVar(ne::StyleVar_PinCorners, 12);
	          ne::BeginPin(pin.ID, ne::PinKind::Input);
	          ne::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
	          ne::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
	          ne::EndPin();
	          ne::PopStyleVar(3);

	          if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
	            inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
	        }
	        else
	          ImGui::Dummy(ImVec2(0, padding));

	        ImGui::Spring(0, padding * 2);
	        ImGui::EndHorizontal();

	        ImGui::BeginHorizontal("content_frame");
	        ImGui::Spring(1, padding);

	        ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
	        ImGui::Dummy(ImVec2(160, 0));
	        ImGui::Spring(1);
	        ImGui::TextUnformatted(node.Name.c_str());
	        ImGui::Spring(1);
	        ImGui::EndVertical();
	        auto contentRect = ImGui_GetItemRect();

	        ImGui::Spring(1, padding);
	        ImGui::EndHorizontal();

	        ImGui::BeginHorizontal("outputs");
	        ImGui::Spring(0, padding * 2);

	        ImRect outputsRect;
	        int outputAlpha = 200;
	        if (!node.Outputs.empty())
	        {
	          auto& pin = node.Outputs[0];
	          ImGui::Dummy(ImVec2(0, padding));
	          ImGui::Spring(1, 0);
	          outputsRect = ImGui_GetItemRect();

	          ne::PushStyleVar(ne::StyleVar_PinCorners, 3);
	          ne::BeginPin(pin.ID, ne::PinKind::Output);
	          ne::PinPivotRect(outputsRect.GetTL(), outputsRect.GetBR());
	          ne::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
	          ne::EndPin();
	          ne::PopStyleVar();

	          if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
	            outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
	        }
	        else
	          ImGui::Dummy(ImVec2(0, padding));

	        ImGui::Spring(0, padding * 2);
	        ImGui::EndHorizontal();

	        ImGui::EndVertical();

	        ne::EndNode();
	        ne::PopStyleVar(7);
	        ne::PopStyleColor(4);

	        auto drawList = ne::GetNodeBackgroundDrawList(node.ID);

	        drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
	                                IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y),
	                                         (int)(255 * pinBackground.z), inputAlpha),
	                                4.0f, 12);
	        // ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
	        drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
	                          IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y),
	                                   (int)(255 * pinBackground.z), inputAlpha),
	                          4.0f, 12);
	        // ImGui::PopStyleVar();
	        drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
	                                IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y),
	                                         (int)(255 * pinBackground.z), outputAlpha),
	                                4.0f, 3);
	        // ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
	        drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
	                          IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y),
	                                   (int)(255 * pinBackground.z), outputAlpha),
	                          4.0f, 3);
	        // ImGui::PopStyleVar();
	        drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
	        // ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
	        drawList->AddRect(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(48, 128, 255, 100), 0.0f);
	        // ImGui::PopStyleVar();
	      }*/

	// Make comment nodes
	/*for (auto& node : s_Nodes)
	{
	  if (node.Type != NodeType::Comment)
	    continue;

	  const float commentAlpha = 0.75f;

	  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
	  ne::PushStyleColor(ne::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
	  ne::PushStyleColor(ne::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
	  ne::BeginNode(node.ID);
	  ImGui::PushID(node.ID.AsPointer());
	  ImGui::BeginVertical("content");
	  ImGui::BeginHorizontal("horizontal");
	  ImGui::Spring(1);
	  ImGui::TextUnformatted(node.Name.c_str());
	  ImGui::Spring(1);
	  ImGui::EndHorizontal();
	  ne::Group(node.Size);
	  ImGui::EndVertical();
	  ImGui::PopID();
	  ne::EndNode();
	  ne::PopStyleColor(2);
	  ImGui::PopStyleVar();

	  if (ne::BeginGroupHint(node.ID))
	  {
	    // auto alpha   = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);
	    auto bgAlpha = static_cast<int>(ImGui::GetStyle().Alpha * 255);

	    // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

	    auto min = ne::GetGroupMin();
	    // auto max = ne::GetGroupMax();

	    ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
	    ImGui::BeginGroup();
	    ImGui::TextUnformatted(node.Name.c_str());
	    ImGui::EndGroup();

	    auto drawList = ne::GetHintBackgroundDrawList();

	    auto hintBounds = ImGui_GetItemRect();
	    auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

	    drawList->AddRectFilled(hintFrameBounds.GetTL(), hintFrameBounds.GetBR(),
	                            IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f);

	    drawList->AddRect(hintFrameBounds.GetTL(), hintFrameBounds.GetBR(),
	                      IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f);

	    // ImGui::PopStyleVar();
	  }
	  ne::EndGroupHint();
	}*/

	/*
	 if (output.Type == PinType::String)
	{
	  static char buffer[128] = "Edit Me\nMultiline!";
	  static bool wasActive = false;

	  ImGui::PushItemWidth(100.0f);
	  ImGui::InputText("##edit", buffer, 127);
	  ImGui::PopItemWidth();
	  if (ImGui::IsItemActive() && !wasActive)
	  {
	    ne::EnableShortcuts(false);
	    wasActive = true;
	  }
	  else if (!ImGui::IsItemActive() && wasActive)
	  {
	    ne::EnableShortcuts(true);
	    wasActive = false;
	  }
	  ImGui::Spring(0);
	}
	*/

	/*
	if (input.Type == PinType::Bool)
	{
	  ImGui::Button("Hello");
	  ImGui::Spring(0);
	}
	*/
}
