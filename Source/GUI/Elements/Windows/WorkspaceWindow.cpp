/*! \file WorkspaceWindow.cpp
    \brief Drawing and manipulation with Workspace.

    Details no here now... Cca in middle is file split between example and used part
*/

#include "../Nodes/Matrix4x4Impl.h"
#include "../Nodes/NormalizeVectorImpl.h"

using namespace Core;


bool editorStart = false;
    /*! \brief flag for state of editor run */ // TODO: Use as least global variable as possible please => and it is
                                               // possible (almost) every time :-)

/* >>>> define static functions <<<< */ //{

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1,
                     float min_size2, float splitter_long_axis_size = -1.0f)
{
  using namespace ImGui;
  ImGuiContext& g = *GImGui;
  ImGuiWindow* window = g.CurrentWindow;
  ImGuiID id = window->GetID("##Splitter");
  ImRect bb;
  bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
  bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size)
                                                  : ImVec2(splitter_long_axis_size, thickness),
                                 0.0f, 0.0f);
  return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2,
                          0.0f);
}

//} /* >>>> define static functions <<<< - end */

void ShowStyleEditor(bool* show = nullptr)
{
  if (!ImGui::Begin("Style", show))
  {
    ImGui::End();
    return;
  }

  auto paneWidth = ImGui::GetContentRegionAvailWidth();

  auto& editorStyle = ed::GetStyle();
  ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
  ImGui::TextUnformatted("Values");
  ImGui::Spring();
  if (ImGui::Button("Reset to defaults"))
    editorStyle = ed::Style();
  ImGui::EndHorizontal();
  ImGui::Spacing();
  ImGui::DragFloat4("Namespace Padding", &editorStyle.NodePadding.x, 0.1f, 0.0f, 40.0f);
  ImGui::DragFloat("Namespace Rounding", &editorStyle.NodeRounding, 0.1f, 0.0f, 40.0f);
  ImGui::DragFloat("Namespace Border Width", &editorStyle.NodeBorderWidth, 0.1f, 0.0f, 15.0f);
  ImGui::DragFloat("Hovered Namespace Border Width", &editorStyle.HoveredNodeBorderWidth, 0.1f, 0.0f, 15.0f);
  ImGui::DragFloat("Selected Namespace Border Width", &editorStyle.SelectedNodeBorderWidth, 0.1f, 0.0f, 15.0f);
  ImGui::DragFloat("Pin Rounding", &editorStyle.PinRounding, 0.1f, 0.0f, 40.0f);
  ImGui::DragFloat("Pin Border Width", &editorStyle.PinBorderWidth, 0.1f, 0.0f, 15.0f);
  ImGui::DragFloat("Link Strength", &editorStyle.LinkStrength, 1.0f, 0.0f, 500.0f);
  // ImVec2  SourceDirection;
  // ImVec2  TargetDirection;
  ImGui::DragFloat("Scroll Duration", &editorStyle.ScrollDuration, 0.001f, 0.0f, 2.0f);
  ImGui::DragFloat("Flow Marker Distance", &editorStyle.FlowMarkerDistance, 1.0f, 1.0f, 200.0f);
  ImGui::DragFloat("Flow Speed", &editorStyle.FlowSpeed, 1.0f, 1.0f, 2000.0f);
  ImGui::DragFloat("Flow Duration", &editorStyle.FlowDuration, 0.001f, 0.0f, 5.0f);
  // ImVec2  PivotAlignment;
  // ImVec2  PivotSize;
  // ImVec2  PivotScale;
  // float   PinCorners;
  // float   PinRadius;
  // float   PinArrowSize;
  // float   PinArrowWidth;
  ImGui::DragFloat("Group Rounding", &editorStyle.GroupRounding, 0.1f, 0.0f, 40.0f);
  ImGui::DragFloat("Group Border Width", &editorStyle.GroupBorderWidth, 0.1f, 0.0f, 15.0f);

  ImGui::Separator();

  static ImGuiColorEditFlags edit_mode = ImGuiColorEditFlags_RGB;
  ImGui::BeginHorizontal("Color Mode", ImVec2(paneWidth, 0), 1.0f);
  ImGui::TextUnformatted("Filter Colors");
  ImGui::Spring();
  ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditFlags_RGB);
  ImGui::Spring(0);
  ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditFlags_HSV);
  ImGui::Spring(0);
  ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditFlags_HEX);
  ImGui::EndHorizontal();

  static ImGuiTextFilter filter;
  filter.Draw("", paneWidth);

  ImGui::Spacing();

  ImGui::PushItemWidth(-160);
  for (int i = 0; i < ed::StyleColor_Count; ++i)
  {
    auto name = ed::GetStyleColorName((ed::StyleColor)i);
    if (!filter.PassFilter(name))
      continue;

    ImGui::ColorEdit4(name, &editorStyle.Colors[i].x, edit_mode);
  }
  ImGui::PopItemWidth();

  ImGui::End();
}

void ShowLeftPane(float paneWidth)
{
  auto& io = ImGui::GetIO();

  ImGui::BeginChild("Selection", ImVec2(paneWidth, 0));

  paneWidth = ImGui::GetContentRegionAvailWidth();

  static bool showStyleEditor = false;
  ImGui::BeginHorizontal("Style Editor", ImVec2(paneWidth, 0));
  ImGui::Spring(0.0f, 0.0f);
  if (ImGui::Button("Zoom to Content"))
    ed::NavigateToContent();
  ImGui::Spring(0.0f);
  if (ImGui::Button("Show Flow"))
  {
    for (auto& link : s_Links)
      ed::Flow(link.ID);
  }
  ImGui::Spring();
  if (ImGui::Button("Edit Style"))
    showStyleEditor = true;
  ImGui::EndHorizontal();

  if (showStyleEditor)
    ShowStyleEditor(&showStyleEditor);

  std::vector<ed::NodeId> selectedNodes;
  std::vector<ed::LinkId> selectedLinks;
  selectedNodes.resize(ed::GetSelectedObjectCount());
  selectedLinks.resize(ed::GetSelectedObjectCount());

  int nodeCount = ed::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
  int linkCount = ed::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));

  selectedNodes.resize(nodeCount);
  selectedLinks.resize(linkCount);

  int saveIconWidth = 32;//Application_GetTextureWidth(s_SaveIcon);
  int saveIconHeight = 32; // Application_GetTextureWidth(s_SaveIcon);
  int restoreIconWidth = 32; // Application_GetTextureWidth(s_RestoreIcon);
  int restoreIconHeight = 32; // Application_GetTextureWidth(s_RestoreIcon);

  ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(paneWidth, ImGui::GetTextLineHeight()),
      ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
  ImGui::Spacing();
  ImGui::SameLine();
  ImGui::TextUnformatted("Nodes");
  ImGui::Indent();
  for (auto& node : s_Nodes)
  {
    ImGui::PushID(node->ID.AsPointer());
    auto start = ImGui::GetCursorScreenPos();

    if (const auto progress = GetTouchProgress(node->ID))
    {
      ImGui::GetWindowDrawList()->AddLine(start + ImVec2(-8, 0), start + ImVec2(-8, ImGui::GetTextLineHeight()),
                                          IM_COL32(255, 0, 0, 255 - (int)(255 * progress)), 4.0f);
    }

    bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), node->ID) != selectedNodes.end();
    if (ImGui::Selectable(
            (node->Name + "##" + std::to_string(reinterpret_cast<uintptr_t>(node->ID.AsPointer()))).c_str(),
            &isSelected))
    {
      if (io.KeyCtrl)
      {
        if (isSelected)
          ed::SelectNode(node->ID, true);
        else
          ed::DeselectNode(node->ID);
      }
      else
        ed::SelectNode(node->ID, false);

      ed::NavigateToSelection();
    }
    if (ImGui::IsItemHovered() && !node->State.empty())
      ImGui::SetTooltip("State: %s", node->State.c_str());

    auto id = std::string("(") + std::to_string(reinterpret_cast<uintptr_t>(node->ID.AsPointer())) + ")";
    auto textSize = ImGui::CalcTextSize(id.c_str(), nullptr);
    auto iconPanelPos =
        start + ImVec2(paneWidth - ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().IndentSpacing -
                           saveIconWidth - restoreIconWidth - ImGui::GetStyle().ItemInnerSpacing.x * 1,
                       (ImGui::GetTextLineHeight() - saveIconHeight) / 2);
    ImGui::GetWindowDrawList()->AddText(
        ImVec2(iconPanelPos.x - textSize.x - ImGui::GetStyle().ItemInnerSpacing.x, start.y),
        IM_COL32(255, 255, 255, 255), id.c_str(), nullptr);

    auto drawList = ImGui::GetWindowDrawList();
    ImGui::SetCursorScreenPos(iconPanelPos);
    ImGui::SetItemAllowOverlap();
    /*if (node->SavedState.empty())
    {
      if (ImGui::InvisibleButton("save", ImVec2((float)saveIconWidth, (float)saveIconHeight)))
        node->SavedState = node->State;

      if (ImGui::IsItemActive())
        drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
                           IM_COL32(255, 255, 255, 96));
      else if (ImGui::IsItemHovered())
        drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
                           IM_COL32(255, 255, 255, 255));
      else
        drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
                           IM_COL32(255, 255, 255, 160));
    }
    else
    {
      ImGui::Dummy(ImVec2((float)saveIconWidth, (float)saveIconHeight));
      drawList->AddImage(s_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
                         IM_COL32(255, 255, 255, 32));
    }*/

    ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::SetItemAllowOverlap();
    /*if (!node->SavedState.empty())
    {
      if (ImGui::InvisibleButton("restore", ImVec2((float)restoreIconWidth, (float)restoreIconHeight)))
      {
        node->State = node->SavedState;
        ed::RestoreNodeState(node->ID);
        node->SavedState.clear();
      }

      if (ImGui::IsItemActive())
        drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0),
                           ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
      else if (ImGui::IsItemHovered())
        drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0),
                           ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
      else
        drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0),
                           ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
    }
    else
    {
      ImGui::Dummy(ImVec2((float)restoreIconWidth, (float)restoreIconHeight));
      drawList->AddImage(s_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1),
                         IM_COL32(255, 255, 255, 32));
    }*/

    ImGui::SameLine(0, 0);
    ImGui::SetItemAllowOverlap();
    ImGui::Dummy(ImVec2(0, (float)restoreIconHeight));

    ImGui::PopID();
  }
  ImGui::Unindent();

  static int changeCount = 0;

  ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(paneWidth, ImGui::GetTextLineHeight()),
      ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
  ImGui::Spacing();
  ImGui::SameLine();
  ImGui::TextUnformatted("Selection");

  ImGui::BeginHorizontal("Selection Stats", ImVec2(paneWidth, 0));
  //ImGui::Text("Changed %d time%s", changeCount, changeCount > 1 ? "s" : "");
  ImGui::Spring();
  if (ImGui::Button("Deselect All"))
    ed::ClearSelection();
  ImGui::EndHorizontal();
  ImGui::Indent();
  for (int i = 0; i < nodeCount; ++i)
    ImGui::Text("Namespace (%p)", selectedNodes[i].AsPointer());
  for (int i = 0; i < linkCount; ++i)
    ImGui::Text("Link (%p)", selectedLinks[i].AsPointer());
  ImGui::Unindent();

  if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
    for (auto& link : s_Links)
      ed::Flow(link.ID);

  if (ed::HasSelectionChanged())
    ++changeCount;

  ImGui::EndChild();
}


/*! \fn void WorkspaceWindow_init() TODO: rename to WorkspaceWindow::init()
    \brief initialize Workspace window
    \details configuration setting
*/
void WorkspaceWindow_init()
{
  editorStart = true;
  ed::Config config;

  config.SettingsFile = "Blueprints.json";

  config.LoadNodeSettings = [](ed::NodeId nodeId, char* data, void* userPointer) -> size_t {
    auto node = FindNode(nodeId);
    if (!node)
      return 0;

    if (data != nullptr)
      memcpy(data, node->State.data(), node->State.size());
    return node->State.size();
  };

  config.SaveNodeSettings = [](ed::NodeId nodeId, const char* data, size_t size, ed::SaveReasonFlags reason,
                               void* userPointer) -> bool {
    auto node = FindNode(nodeId);
    if (!node)
      return false;

    node->State.assign(data, size);

    TouchNode(nodeId);

    return true;
  };

  m_Editor = ed::CreateEditor(&config);
  ed::SetCurrentEditor(m_Editor);

  // TODO read scene file
  Matrix4x4Impl* mat = new Matrix4x4Impl();
  Namespace* node = nullptr;
  node = mat->SpawnNode(&s_Nodes);

  ed::SetNodePosition(node->ID, ImVec2(-252, 220));

  ed::NavigateToContent();

  BuildGUINodes();

  // EXAMPLE of pre spawn links
  // s_Links.push_back(Link(GetNextLinkId(), s_Nodes[5].Outputs[0].ID, s_Nodes[6].Inputs[0].ID));
  // s_Links.push_back(Link(GetNextLinkId(), s_Nodes[5].Outputs[0].ID, s_Nodes[7].Inputs[0].ID));

  // s_Links.push_back(Link(GetNextLinkId(), s_Nodes[14].Outputs[0].ID, s_Nodes[15].Inputs[0].ID));

  //GLuint imageId = pgr::createTexture("/data/BlueprintBackground.png", true);
  GLuint imageId = pgr::createTexture(Config::getAbsolutePath("/Source/GUI/Elements/Windows/data/BlueprintBackground.png"), true);
  s_HeaderBackground = (void*)(intptr_t)imageId; // TODO load texture OR making a simple rectangle
  // s_SaveIcon = Application_LoadTexture("Data/ic_save_white_24dp.png");
  // s_RestoreIcon = Application_LoadTexture("Data/ic_restore_white_24dp.png");
}

void popupMenu(bool& createNewNode, GUIPin* newNodeLinkPin, ed::NodeId& contextNodeId,
               ed::PinId& contextPinId,
               ed::LinkId& contextLinkId)
{
  auto openPopupPosition = ImGui::GetMousePos();
  ed::Suspend();
  if (ed::ShowNodeContextMenu(&contextNodeId))
    ImGui::OpenPopup("Namespace Context Menu");
  else if (ed::ShowPinContextMenu(&contextPinId))
    ImGui::OpenPopup("Pin Context Menu");
  else if (ed::ShowLinkContextMenu(&contextLinkId))
    ImGui::OpenPopup("Link Context Menu");
  else if (ed::ShowBackgroundContextMenu())
  {
    ImGui::OpenPopup("Create New Namespace");
    newNodeLinkPin = nullptr;
  }
  ed::Resume();

  ed::Suspend();
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
  if (ImGui::BeginPopup("Namespace Context Menu"))
  {
    auto node = FindNode(contextNodeId);

    ImGui::TextUnformatted("Namespace Context Menu");
    ImGui::Separator();
    if (node)
    {
      ImGui::Text("ID: %p", node->ID.AsPointer());
      ImGui::Text("Type: %s", node->Type);
      ImGui::Text("Inputs: %d", (int)node->Inputs.size());
      ImGui::Text("Outputs: %d", (int)node->Outputs.size());
    }
    else
      ImGui::Text("Unknown node: %p", contextNodeId.AsPointer());
    ImGui::Separator();
    if (ImGui::MenuItem("Delete"))
      ed::DeleteNode(contextNodeId);
    ImGui::EndPopup();
  }

  if (ImGui::BeginPopup("Pin Context Menu"))
  {
    auto pin = FindPin(contextPinId);

    ImGui::TextUnformatted("Pin Context Menu");
    ImGui::Separator();
    if (pin)
    {
      ImGui::Text("ID: %p", pin->ID.AsPointer());
      if (pin->Node)
        ImGui::Text("Namespace: %p", pin->Node->ID.AsPointer());
      else
        ImGui::Text("Namespace: %s", "<none>");
    }
    else
      ImGui::Text("Unknown pin: %p", contextPinId.AsPointer());

    ImGui::EndPopup();
  }

  if (ImGui::BeginPopup("Link Context Menu"))
  {
    auto link = FindLink(contextLinkId);

    ImGui::TextUnformatted("Link Context Menu");
    ImGui::Separator();
    if (link)
    {
      ImGui::Text("ID: %p", link->ID.AsPointer());
      ImGui::Text("From: %p", link->StartPin->ID.AsPointer());
      ImGui::Text("To: %p", link->EndPin->ID.AsPointer());
    }
    else
      ImGui::Text("Unknown link: %p", contextLinkId.AsPointer());
    ImGui::Separator();
    if (ImGui::MenuItem("Delete"))
      ed::DeleteLink(contextLinkId);
    ImGui::EndPopup();
  }

  if (ImGui::BeginPopup("Create New Namespace"))
  {
    auto newNodePostion = openPopupPosition;
    ImGui::SetCursorScreenPos(ImGui::GetMousePosOnOpeningCurrentPopup());

    auto drawList = ImGui::GetWindowDrawList();
    drawList->AddCircleFilled(ImGui::GetMousePosOnOpeningCurrentPopup(), 10.0f, 0xFFFF00FF);
    
    Namespace* node = nullptr;
    if (ImGui::MenuItem("Matrix 4x4")){
        Matrix4x4Impl* mat = new Matrix4x4Impl();
        node = mat->SpawnNode(&s_Nodes);
    }
    if (ImGui::MenuItem("Normalize Vector")){
      NormalizeVectorImpl* nv = new NormalizeVectorImpl();
      node = nv->SpawnNode(&s_Nodes);
    }

    if (node)
    {

      createNewNode = true;

      ed::SetNodePosition(node->ID, newNodePostion);

      if (auto startPin = newNodeLinkPin)
      {
        auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

        for (auto& pin : pins)
        {
          if (CanCreateLink(startPin, pin))
          {
            auto endPin = pin;
            if (startPin->Kind == PinKind::Input)
              std::swap(startPin, endPin);

            s_Links.emplace_back(Link(GetNextId(), startPin, endPin));
            s_Links.back().Color = GetIconColor(startPin->Type);

            //TODO predelat indexy v ramci jedny krabicky
            //auto result = node->nodebase->plugToParent(startPin->Node->nodebase, 0, 0);
            auto result = GraphManager::plug(startPin->Node->nodebase, endPin->Node->nodebase, 0, 0);
            if (result != ENodePlugResult::Ok) {
                //print result;
            }

            break;
          }
        }
      }
    }
   
    ImGui::EndPopup();
  }
  else
    createNewNode = false;
  ImGui::PopStyleVar();
}

void DeleteNode()
{
  ed::LinkId linkId = 0;
  while (ed::QueryDeletedLink(&linkId))
  {
    if (ed::AcceptDeletedItem())
    {
      auto id = std::find_if(s_Links.begin(), s_Links.end(), [linkId](auto& link) { return link.ID == linkId; });
      if (id != s_Links.end())
        s_Links.erase(id);
    }

  }

  ed::NodeId nodeId = 0;
  while (ed::QueryDeletedNode(&nodeId))
  {
    auto node = FindNode(nodeId);
    GraphManager::unplugAll(node->nodebase);
    node->nodebase = nullptr;

    if (ed::AcceptDeletedItem())
    {
      auto id = std::find_if(s_Nodes.begin(), s_Nodes.end(), [nodeId](auto& node) { return node->ID == nodeId; });
      if (id != s_Nodes.end())
        s_Nodes.erase(id);
    }
  }
}

void DrawNodes(util::NodeBuilder& builder, GUIPin* newLinkPin)
{
  for (auto& node : s_Nodes)
  {
    builder.Begin(node->ID);

    node->drawInputs(builder, newLinkPin);

    node->drawBox(builder);

    node->drawOutputs(builder, newLinkPin);

    builder.End();
  } 

}

void WorkspaceWindow::render(){

  if (!Application::get().m_showWorkspaceWindow)
    return;

  ImGui::Begin("Workspace", &Application::get().m_showWorkspaceWindow);
  
  if (!editorStart){
      WorkspaceWindow_init();
  }

    UpdateTouch();
    auto& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ed::SetCurrentEditor(m_Editor);

    static ed::NodeId contextNodeId = 0;
    static ed::LinkId contextLinkId = 0;
    static ed::PinId contextPinId = 0;
    static bool createNewNode = false;
    static GUIPin* newNodeLinkPin = nullptr;
    static GUIPin* newLinkPin = nullptr;

    static float leftPaneWidth = 400.0f;
    static float rightPaneWidth = 800.0f;
    Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);

    ShowLeftPane(leftPaneWidth - 4.0f);

    ImGui::SameLine(0.0f, 12.0f);

    ed::Begin("Node editor");
    {
      auto cursorTopLeft = ImGui::GetCursorScreenPos();

      //util::NodeBuilder builder(s_HeaderBackground, Application_GetTextureWidth(s_HeaderBackground),Application_GetTextureHeight(s_HeaderBackground));
      util::NodeBuilder builder(s_HeaderBackground, 64, 64);

      //
      DrawNodes(builder, newLinkPin);

      //draw links
      for (auto& link : s_Links)
        ed::Link(link.ID, link.StartPin->ID, link.EndPin->ID, link.Color, 2.0f);

     //create link
     if (!createNewNode)
      {
        if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f)){
          
          auto showLabel = [](const char* label, ImColor color) {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
            auto size = ImGui::CalcTextSize(label);

            auto padding = ImGui::GetStyle().FramePadding;
            auto spacing = ImGui::GetStyle().ItemSpacing;

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

            auto rectMin = ImGui::GetCursorScreenPos() - padding;
            auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

            auto drawList = ImGui::GetWindowDrawList();
            drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
            ImGui::TextUnformatted(label);
          };

          ed::PinId startPinId = 0, endPinId = 0;
          if (ed::QueryNewLink(&startPinId, &endPinId))
          {
            auto startPin = FindPin(startPinId);
            auto endPin = FindPin(endPinId);

            newLinkPin = startPin ? startPin : endPin;

            if (startPin->Kind == PinKind::Input)
            {
              std::swap(startPin, endPin);
              std::swap(startPinId, endPinId);
            }

            if (startPin && endPin)
            {
              if (endPin == startPin)
              {
                ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
              }
              else if (endPin->Kind == startPin->Kind)
              {
                showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
                ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
              }
              else if (endPin->Node == startPin->Node)
              {
                  showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
                  ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
              }
              else if (endPin->Type != startPin->Type)
              {
                showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
                ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
              }
              else
              {
                showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                {
                  s_Links.emplace_back(Link(GetNextId(), startPin, endPin));
                  s_Links.back().Color = GetIconColor(startPin->Type);

                  auto result = GraphManager::plug(startPin->Node->nodebase, endPin->Node->nodebase, 0, 0);
                  if (result != ENodePlugResult::Ok)
                  {
                    // print result;
                  }
                }
              }
            }
          }

          ed::PinId pinId = 0;
          if (ed::QueryNewNode(&pinId))
          {
            newLinkPin = FindPin(pinId);
            if (newLinkPin)
              showLabel("+ Create Namespace", ImColor(32, 45, 32, 180));

            if (ed::AcceptNewItem())
            {
              createNewNode = true;
              newNodeLinkPin = FindPin(pinId);
              newLinkPin = nullptr;
              ed::Suspend();
              ImGui::OpenPopup("Create New Namespace");
              ed::Resume();
            }
          }
        }
        else
          newLinkPin = nullptr;

        ed::EndCreate();

        if (ed::BeginDelete())
        {
          DeleteNode();
        }
        ed::EndDelete();
      }

      ImGui::SetCursorScreenPos(cursorTopLeft);
    }

    popupMenu(createNewNode, newNodeLinkPin, contextNodeId, contextPinId, contextLinkId);
    ed::Resume();
        
    ed::End();
    ImGui::End(); 
  
}


void maybeUsefulCode()
{
  /*for (auto& node : s_Nodes)
    {
      if (node.Type != NodeType::Houdini)
        continue;

      const float rounding = 10.0f;
      const float padding = 12.0f;

      ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(229, 229, 229, 200));
      ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(125, 125, 125, 200));
      ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(229, 229, 229, 60));
      ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(125, 125, 125, 60));

      const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

      ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
      ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
      ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
      ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
      ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
      ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
      ed::PushStyleVar(ed::StyleVar_PinRadius, 6.0f);
      ed::BeginNode(node.ID);

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

          // ed::PushStyleVar(ed::StyleVar_PinArrowSize, 10.0f);
          // ed::PushStyleVar(ed::StyleVar_PinArrowWidth, 10.0f);
          ed::PushStyleVar(ed::StyleVar_PinCorners, 15);
          ed::BeginPin(pin.ID, ed::PinKind::Input);
          ed::PinPivotRect(inputsRect.GetCenter(), inputsRect.GetCenter());
          ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
          ed::EndPin();
          // ed::PopStyleVar(3);
          ed::PopStyleVar(1);

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

          ed::PushStyleVar(ed::StyleVar_PinCorners, 3);
          ed::BeginPin(pin.ID, ed::PinKind::Output);
          ed::PinPivotRect(outputsRect.GetCenter(), outputsRect.GetCenter());
          ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
          ed::EndPin();
          ed::PopStyleVar();

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

      ed::EndNode();
      ed::PopStyleVar(7);
      ed::PopStyleColor(4);

      auto drawList = ed::GetNodeBackgroundDrawList(node.ID);
    }*/

    
//Making tree nodes
 /*for (auto& node : s_Nodes)
       {
         if (node.Type != NodeType::Tree)
           continue;

         const float rounding = 5.0f;
         const float padding = 12.0f;

         const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

         ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
         ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
         ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 180, 255, 150));
         ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

         ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
         ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
         ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
         ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
         ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
         ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
         ed::PushStyleVar(ed::StyleVar_PinRadius, 5.0f);
         ed::BeginNode(node.ID);

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

           ed::PushStyleVar(ed::StyleVar_PinArrowSize, 10.0f);
           ed::PushStyleVar(ed::StyleVar_PinArrowWidth, 10.0f);
           ed::PushStyleVar(ed::StyleVar_PinCorners, 12);
           ed::BeginPin(pin.ID, ed::PinKind::Input);
           ed::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
           ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
           ed::EndPin();
           ed::PopStyleVar(3);

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

           ed::PushStyleVar(ed::StyleVar_PinCorners, 3);
           ed::BeginPin(pin.ID, ed::PinKind::Output);
           ed::PinPivotRect(outputsRect.GetTL(), outputsRect.GetBR());
           ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
           ed::EndPin();
           ed::PopStyleVar();

           if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
             outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
         }
         else
           ImGui::Dummy(ImVec2(0, padding));

         ImGui::Spring(0, padding * 2);
         ImGui::EndHorizontal();

         ImGui::EndVertical();

         ed::EndNode();
         ed::PopStyleVar(7);
         ed::PopStyleColor(4);

         auto drawList = ed::GetNodeBackgroundDrawList(node.ID);

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
    ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
    ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
    ed::BeginNode(node.ID);
    ImGui::PushID(node.ID.AsPointer());
    ImGui::BeginVertical("content");
    ImGui::BeginHorizontal("horizontal");
    ImGui::Spring(1);
    ImGui::TextUnformatted(node.Name.c_str());
    ImGui::Spring(1);
    ImGui::EndHorizontal();
    ed::Group(node.Size);
    ImGui::EndVertical();
    ImGui::PopID();
    ed::EndNode();
    ed::PopStyleColor(2);
    ImGui::PopStyleVar();

    if (ed::BeginGroupHint(node.ID))
    {
      // auto alpha   = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);
      auto bgAlpha = static_cast<int>(ImGui::GetStyle().Alpha * 255);

      // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

      auto min = ed::GetGroupMin();
      // auto max = ed::GetGroupMax();

      ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
      ImGui::BeginGroup();
      ImGui::TextUnformatted(node.Name.c_str());
      ImGui::EndGroup();

      auto drawList = ed::GetHintBackgroundDrawList();

      auto hintBounds = ImGui_GetItemRect();
      auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

      drawList->AddRectFilled(hintFrameBounds.GetTL(), hintFrameBounds.GetBR(),
                              IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f);

      drawList->AddRect(hintFrameBounds.GetTL(), hintFrameBounds.GetBR(),
                        IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f);

      // ImGui::PopStyleVar();
    }
    ed::EndGroupHint();
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
        ed::EnableShortcuts(false);
        wasActive = true;
      }
      else if (!ImGui::IsItemActive() && wasActive)
      {
        ed::EnableShortcuts(true);
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