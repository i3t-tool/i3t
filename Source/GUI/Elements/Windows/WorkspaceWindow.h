/**
 * \file	GUI/Elements/Windows/WorkspaceWindow.h
 *
 *  A Workspace window elements declaration.
 */
#pragma once

#include "GUI/Elements/IWindow.h"

#include "GUI/NodeEditorUtilities/builders.h"
#include "GUI/NodeEditorUtilities/widgets.h"

#include <glm/glm.hpp>
#include <imgui_node_editor.h>
#include "../../../Core/Nodes/Node.h"
#include "../../../Core/Nodes/NodeImpl.h"
#include "../../../Core/Nodes/GraphManager.h"
#include "Core/Application.h"
#include <memory>
#include "pgr.h"
#include "Config.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>


/*! \fn static inline ImRect ImGui_GetItemRect()
    \brief Get ImRect of last (active/added ?) item
    \return ImRect : New ImRect with position and size of last item
*/
static inline ImRect ImGui_GetItemRect()
{
  return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

/*! \fn static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
    \brief Enlarge given ImRect (create new / enlarge given ?)
     \param[in/out?] rect ImRect& for enlarge
     \param[in] x float value added to left and right
     \param[in] y float value added to up an d down
    \return ImRect New
*/
static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
{
  auto result = rect;
  result.Min.x -= x;
  result.Min.y -= y;
  result.Max.x += x;
  result.Max.y += y;
  return result;
}

using namespace ax;
using ax::Widgets::IconType;

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;


static ed::EditorContext* m_Editor = nullptr; /*! \brief Object for store workspace scene */

/*! \enum PinKind
    \brief kinds (in/out) of Pin
 */
enum class PinKind
{
  Output,
  Input
};

class Namespace; /* Just for using in struct Pin*/
struct Link;

/*! \struct Pin
    \brief use to connecting Link to Node
 */
struct GUIPin
{
  ed::PinId ID; /*! \brief unique (among Pins) identificator */
  ::Namespace* Node; /*! \brief Node the Pin belongs to */
  std::string Name; /*! \brief Name of Pin */
  EValueType Type;   /*! \brief Type of pin \sa PinType */
  PinKind Kind;  /*! \brief Kind of pin \sa PinKind */

  /*! \fn Pin
        \brief default Pin constructor
        \param[in] id int identificator
        \param[in] name const char*
        \param[in] type PinType of Pin

        PinKind is set to input. Node is set to nullptr.
    */
  GUIPin(int id, const char* name, EValueType type) : ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
  {
  }
};

class Namespace
{
    public:

      ed::NodeId ID; /*! \brief unique (among Nodes) identificator */
      std::string Name; /*! \brief Name of Node */
      std::vector<GUIPin*> Inputs; /*! \brief Vector of input Pins */
      std::vector<GUIPin*> Outputs; /*! \brief Vector of output Pins */
      ImColor Color; /*! \brief Color of Node */
      ENodeType Type; /*! \brief Type of Node */
      ImVec2 Size;  /*! \brief Size of box */
      std::unique_ptr<Core::NodeBase> nodebase; /*! \brief reference to core */


      std::string State; /*! \brief e.g. selected */
      std::string SavedState; /*! \brief last State that was saved */

      /*! \fn Node
        \brief default Node constructor with no parameters
      */
      Namespace() {
          Color = ImColor(255, 255, 255);
          Type = ENodeType::Float;
      }

      /*! \fn Node
        \brief default Node constructor
        \param[in] id int identificator
        \param[in] name const char*
        \param[in] color ImColor of Node, default ImColor(255, 255, 255)
        */

      Namespace(int id, const char* name, ImColor color = ImColor(255, 255, 255))
          : ID(id), Name(name), Color(color), Size(0, 0)
      {
      }

       virtual Namespace* SpawnNode(std::vector<Namespace*> *s_nodes)=0;

       virtual void drawBox(util::NodeBuilder& builder)=0;

       virtual void drawOutputs(util::NodeBuilder& builder, GUIPin* newLinkPin) = 0;

       virtual void drawInputs(util::NodeBuilder& builder, GUIPin* newLinkPin) = 0;
};

/*! \struct Link
    \brief use for connecting Pins (consequently Nodes)
 */
struct Link
{
  ed::LinkId ID; /*! \brief unique (among Links) identificator */

  GUIPin* StartPin; /*! \brief Pin from which Link comes from */
  GUIPin* EndPin; /*! \brief Pin to which Link goes to */

  ImColor Color; /*! \brief Color of Link  */

  /*! \fn Link
    \brief default Link constructor
    \param[in] id int identificator
    \param[in] Pin* pointer of start Pin
    \param[in] Pin* pointer of end Pin

    Color is set to Color(255, 255, 255)
*/
  Link(ed::LinkId id, GUIPin* StartPin, GUIPin* EndPin)
      : ID(id), StartPin(StartPin), EndPin(EndPin), Color(255, 255, 255)
  {
  }
};

/* >>>> init static values <<<< */ //{

/* TODO: Its clear, but... TODO: for what is use */

static const int s_PinIconSize = 24; /*! \brief Size of Pin icons TODO: take from (move to) Const.h */
static const float s_TouchTime = 1.0f; /*! \brief TODO: take values from (move to) Const.h */

/* >>> variables <<< */ //{

static std::vector<Namespace*> s_Nodes; /*! \brief All Nodes */
static std::vector<Link> s_Links;  /*! \brief All Links */
static ImTextureID s_HeaderBackground = nullptr;  /*! Header background for Nodes */
// static ImTextureID s_SampleImage = nullptr;
//static ImTextureID s_SaveIcon = nullptr;
//static ImTextureID s_RestoreIcon = nullptr;

struct NodeIdLess
{
  bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const { return lhs.AsPointer() < rhs.AsPointer(); }
};

static std::map<ed::NodeId, float, NodeIdLess> s_NodeTouchTime;

static int s_NextId = 1; /*! \brief prepared identificator of next added element */


/*! \fn static void BuildNode(Node* node)
    \brief set right values (Node and Kind) to all Pins in node
    \param[in] node Node* to manage Pins in
*/
static void BuildGUINode(Namespace* node)
{
  for (auto& input : node->Inputs)
  {
    input->Node = node;
    input->Kind = PinKind::Input;
  }

  for (auto& output : node->Outputs)
  {
    output->Node = node;
    output->Kind = PinKind::Output;
  }
}

/*! \fn void BuildNodes()
    \brief build all Nodes
    \sa BuildNode
*/
inline void BuildGUINodes()
{
  for (auto& node : s_Nodes)
    BuildGUINode(node);
}


/*! \fn static int GetNextId()
    \brief Compute value of identificator for elements
    \return Value of identificator for added element */
 inline int GetNextId()
{
  return s_NextId++;
}

static ed::LinkId GetNextLinkId()
{
  return ed::LinkId(GetNextId());
}

/*! \fn static void TouchNode(ed::NodeId id)
    \brief save s_TouchTime for node
    \sa s_TouchTime
*/
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

/*! \fn static Node* FindNode(ed::NodeId id)
    \brief search for Node by its id
    \param[in] id NodeId of Node function search for
    \return Node* of Node with given id or nullptr when not found
*/
static Namespace* FindNode(ed::NodeId id)
{
  for (auto& node : s_Nodes)
    if (node->ID == id)
      return node;

  return nullptr;
}

/*! \fn static Link* FindLink(ed::LinkId id)
    \brief search for Link by its id
    \param[in] id LinkId of Link function search for
    \return Link* of Link with given id or nullptr when not found
*/
static Link* FindLink(ed::LinkId id)
{
  for (auto& link : s_Links)
    if (link.ID == id)
      return &link;

  return nullptr;
}

/*! \fn static Pin* FindPin(ed::PinId id)
    \brief search for Pin by its id
    \param[in] id PinId of Pin function search for
    \return Pin* of Pin with given id or nullptr when not found
*/
static GUIPin* FindPin(ed::PinId id)
{
  if (!id)
    return nullptr;

  for (auto& node : s_Nodes)
  {
    for (auto& pin : node->Inputs)
      if (pin->ID == id)
        return pin;

    for (auto& pin : node->Outputs)
      if (pin->ID == id)
        return pin;
  }

  return nullptr;
}

/*! \fn static bool IsPinLinked(ed::PinId id)
    \brief check whether some Link is connect to Pin with given id
    \param[in] id PinId of Pin function check
    \return true if some Link is connect to Pin, false otherwise or if Pin id not exist
*/
static bool IsPinLinked(ed::PinId id)
{
  if (!id)
    return false;

  for (auto& link : s_Links)
    if (link.StartPin->ID == id || link.EndPin->ID == id)
      return true;

  return false;
}

/*! \fn static bool CanCreateLink(Pin* a, Pin* b)
    \brief check whether it is possible to connect two Pins
    \param[in] a,b Pin* to check possibility of connection between
    \return true if it is possible create Link from a to b, false otherwise
*/
static bool CanCreateLink(GUIPin* a, GUIPin* b)
{
  if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
    return false;

  return true;
}


/*! \fn void fromArrayToVec4(glm::vec4& vec, float arr[4])
    \brief convert float array of size 4 to glm::vec4
    \param[out] vec glm::vec4& result object
    \param[in] arr float* input values -> must be of size 4*sizeof(float)
*/
inline void fromArrayToVec4(glm::vec4& vec, float arr[4])
{
  vec.x = arr[0];
  vec.y = arr[1];
  vec.z = arr[2];
  vec.w = arr[3];
}

/*! \fn void fromVecToArray4(glm::vec4& vec, float arr[4])
    \brief convert glm::vec4 to float array of size 4
    \param[in] vec glm::vec4& input object
    \param[out] arr float* result values -> should be of size 4*sizeof(float)
*/
inline void fromVecToArray4(glm::vec4& vec, float arr[4])
{
  arr[0] = vec.x;
  arr[1] = vec.y;
  arr[2] = vec.z;
  arr[3] = vec.w;
}

/*! \fn ImColor GetIconColor(PinType type)
    \brief select/construct color for given PinType
    \return ImColor
*/
// TODO change to core pintypes and fuse with DrawPinIcon
inline ImColor GetIconColor(EValueType type)
{
  switch (type)
  {  // TODO: Define colors in Const.h
  case EValueType::Float:
    return ImColor(255, 255, 255);
  case EValueType::Matrix:
    return ImColor(220, 48, 48);
  case EValueType::MatrixMul:
    return ImColor(68, 201, 156);
  case EValueType::Pulse:
    return ImColor(147, 226, 74);
  case EValueType::Quat:
    return ImColor(124, 21, 153);
  case EValueType::Screen:
    return ImColor(51, 150, 215);
  case EValueType::Vec3:
    return ImColor(218, 0, 183);
  case EValueType::Vec4:
    return ImColor(255, 48, 48);
  default:
    // TODO: Log error - unknown PinType
    return ImColor(0, 0, 0);
  }
};

/*! \fn void DrawPinIcon(const Pin& pin, bool connected, int alpha)
    \brief draw icon of Pin TODO: probably on place of cursor?
    \param[in] pin const Pin& use for select IconType based on PinType
    \param[in] connected bool of pin connection state
    \param[in] alpha int of transparency channel of ImColor of icom
*/
//TODO change to core pintypes
inline void DrawPinIcon(const GUIPin& pin, bool connected, int alpha)
{
  IconType iconType;
  ImColor color = GetIconColor(pin.Type);
  color.Value.w = alpha / 255.0f;
  switch (pin.Type)
  {
  case EValueType::Matrix:
    iconType = IconType::Flow;
    break;
  case EValueType::MatrixMul:
    iconType = IconType::Circle;
    break;
  case EValueType::Pulse:
    iconType = IconType::Circle;
    break;
  case EValueType::Float:
    iconType = IconType::Circle;
    break;
  case EValueType::Quat:
    iconType = IconType::Circle;
    break;
  case EValueType::Screen:
    iconType = IconType::Circle;
    break;
  case EValueType::Vec3:
    iconType = IconType::Circle;
    break;
  case EValueType::Vec4:
    iconType = IconType::Square;
    break;
  default:
    // TODO: Log error Unknown PinType
    return;
  }

  ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
};

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

    //releaseTexture(s_RestoreIcon);
    //releaseTexture(s_SaveIcon);
    releaseTexture(s_HeaderBackground);

    if (m_Editor)
    {
      ed::DestroyEditor(m_Editor);
      m_Editor = nullptr;
    }
  };
  virtual void render();
  //static glm::mat4 DrawInScene();
};
