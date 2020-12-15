/**
 * \file	GUI/Elements/Windows/WorkspaceWindow.h
 *
 * A workspace element.
 */
#pragma once

#include "GUI/Elements/IWindow.h"
#include "GUI/NodeEditorUtilities/Builders.h"
#include "GUI/NodeEditorUtilities/Widgets.h"

#include "glm/glm.hpp"
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "imgui_node_editor.h"

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

static inline ImRect ImGui_GetItemRect()
{
  return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
{
  auto result = rect;
  result.Min.x -= x;
  result.Min.y -= y;
  result.Max.x += x;
  result.Max.y += y;
  return result;
}


namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

static ed::EditorContext* m_Editor = nullptr;

enum class PinType
{
  Flow,
  Bool,
  Int,
  Float,
  String,
  Object,
  Function,
  Delegate,
  Mat4x4
};

enum class PinKind
{
  Output,
  Input
};

enum class NodeType
{
  Blueprint,
  Comment
};

struct Node;

struct Pin
{
  ed::PinId ID;
  ::Node* Node;
  std::string Name;
  PinType Type;
  PinKind Kind;

  Pin(int id, const char* name, PinType type) : ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input) {}
};

struct Node
{
  ed::NodeId ID;
  std::string Name;
  std::vector<Pin> Inputs;
  std::vector<Pin> Outputs;
  ImColor Color;
  NodeType Type;
  ImVec2 Size;

  std::string State;
  std::string SavedState;

  Node(int id, const char* name, ImColor color = ImColor(255, 255, 255))
      : ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
  {
  }
};

struct Matrix4x4
{
    //Node* node;
    ed::NodeId ID;
    glm::mat4 data;
};

struct Link
{
  ed::LinkId ID;

  ed::PinId StartPinID;
  ed::PinId EndPinID;

  ImColor Color;

  Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId)
      : ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
  {
  }
};

static const int s_PinIconSize = 24;
static std::vector<Node> s_Nodes;
static std::vector<Link> s_Links;
static std::vector<Matrix4x4*> s_mats;
static ImTextureID s_HeaderBackground = nullptr;
// static ImTextureID          s_SampleImage = nullptr;
static ImTextureID s_SaveIcon = nullptr;
static ImTextureID s_RestoreIcon = nullptr;

struct NodeIdLess
{
  bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const { return lhs.AsPointer() < rhs.AsPointer(); }
};

static const float s_TouchTime = 1.0f;
static std::map<ed::NodeId, float, NodeIdLess> s_NodeTouchTime;

static int s_NextId = 1;

static int GetNextId()
{
  return s_NextId++;
}

static ed::LinkId GetNextLinkId()
{
  return ed::LinkId(GetNextId());
}

static void TouchNode(ed::NodeId id)
{
  s_NodeTouchTime[id] = s_TouchTime;
}

static float GetTouchProgress(ed::NodeId id)
{
  auto it = s_NodeTouchTime.find(id);
  if (it != s_NodeTouchTime.end() && it->second > 0.0f)
    return (s_TouchTime - it->second) / s_TouchTime;
  else
    return 0.0f;
}

static void UpdateTouch()
{
  const auto deltaTime = ImGui::GetIO().DeltaTime;
  for (auto& entry : s_NodeTouchTime)
  {
    if (entry.second > 0.0f)
      entry.second -= deltaTime;
  }
}

static Node* FindNode(ed::NodeId id)
{
  for (auto& node : s_Nodes)
    if (node.ID == id)
      return &node;

  return nullptr;
}

static Link* FindLink(ed::LinkId id)
{
  for (auto& link : s_Links)
    if (link.ID == id)
      return &link;

  return nullptr;
}

static Pin* FindPin(ed::PinId id)
{
  if (!id)
    return nullptr;

  for (auto& node : s_Nodes)
  {
    for (auto& pin : node.Inputs)
      if (pin.ID == id)
        return &pin;

    for (auto& pin : node.Outputs)
      if (pin.ID == id)
        return &pin;
  }

  return nullptr;
}

static bool IsPinLinked(ed::PinId id)
{
  if (!id)
    return false;

  for (auto& link : s_Links)
    if (link.StartPinID == id || link.EndPinID == id)
      return true;

  return false;
}

static bool CanCreateLink(Pin* a, Pin* b)
{
  if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
    return false;

  return true;
}

static void BuildNode(Node* node)
{
  for (auto& input : node->Inputs)
  {
    input.Node = node;
    input.Kind = PinKind::Input;
  }

  for (auto& output : node->Outputs)
  {
    output.Node = node;
    output.Kind = PinKind::Output;
  }
}



/**
 * Workspace - node editor.
 */
class WorkspaceWindow : public IWindow
{
public:
  WorkspaceWindow(bool show) : IWindow(show) {
    
    //g_Context = ed::CreateEditor(&config);
  };
  virtual ~WorkspaceWindow()
  {
    auto releaseTexture = [](ImTextureID& id) {
      if (id)
      {
        //Application_DestroyTexture(id);
        id = nullptr;
      }
    };

    releaseTexture(s_RestoreIcon);
    releaseTexture(s_SaveIcon);
    releaseTexture(s_HeaderBackground);

    if (m_Editor)
    {
      ed::DestroyEditor(m_Editor);
      m_Editor = nullptr;
    }
  };
  virtual void render();
  static glm::mat4 DrawInScene();
};
