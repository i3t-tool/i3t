/**
 * \file	GUI/Elements/Windows/WorkspaceWindow.h
 *
 *  A Workspace window elements declaration.
 */
#pragma once

#include "GUI/Elements/IWindow.h"

#include "GUI/NodeEditorUtilities/Builders.h" /* \todo soubor s malym pismenkem na zacatku neexistuje - porad mi to prosim neprepisujte :-D */
#include "GUI/NodeEditorUtilities/Widgets.h"

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

//using namespace ax;
using ax::Widgets::IconType;

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

/* >>> Some struct <<< \todo What is it for? */ //{

struct NodeIdLess
{
  bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const { return lhs.AsPointer() < rhs.AsPointer(); }
};

static std::map<ed::NodeId, float, NodeIdLess> s_NodeTouchTime;


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

//}

/* >>> Static functions <<< \todo make it as method of class?  */ //{

/*! \fn static inline ImRect ImGui_GetItemRect()
    \brief Get ImRect of last  ( \todo active/added ?) item
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

/*! \fn static bool CanCreateLink(Pin* a, Pin* b)
    \brief check whether it is possible to connect two Pins
    \param[in] a,b Pin* to check possibility of connection between
    \return true if it is possible create Link from a to b, false otherwise
*/
static bool CanCreateLink(WorkspacePin* a, WorkspacePin* b)
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

//} /* >>> Static functions - end <<< */

/*! \class class for Workspace window object
    \brief Store everything what Workspace window need
*/
class WorkspaceWindow  : public IWindow {
    public:

    ed::NodeId contextNodeId;
    ed::LinkId contextLinkId;
    ed::PinId contextPinId;
    bool createNewNode;
    GUIPin* newNodeLinkPin ;
    GUIPin* newLinkPin;

    float leftPaneWidth;
    float rightPaneWidth;


    const float TouchTime; /*! \brief \TODO: take values from (move to) Const.h */

    ed::EditorContext* m_Editor; /*! \brief Object for store workspace scene */

    //static std::vector<Namespace*> s_Nodes; /*! \brief All Nodes */
    std::vector<WorkspaceNode*> Nodes;
    std::vector<WorkspaceLink> Links;  /*! \brief All Links */



    WorkspaceWindow(bool show);

    /*! \fn static Node* FindNode(ed::NodeId id)
    \brief search for Node by its id
    \param[in] id NodeId of Node function search for
    \return Node* of Node with given id or nullptr when not found
*/
    WorkspaceNode* FindNode(ed::NodeId id);


/*! \fn static Link* FindLink(ed::LinkId id)
    \brief search for Link by its id
    \param[in] id LinkId of Link function search for
    \return Link* of Link with given id or nullptr when not found
*/
Link* FindLink(ed::LinkId id);



/*! \fn static Pin* FindPin(ed::PinId id)
    \brief search for Pin by its id
    \param[in] id PinId of Pin function search for
    \return Pin* of Pin with given id or nullptr when not found
*/
WorkspacePin* FindPin(ed::PinId id);

void render();


};


//static int s_NextId = 1; /*! \brief prepared identificator of next added element */


///*! \fn static void BuildNode(Node* node)
//    \brief set right values (Node and Kind) to all Pins in node
//    \param[in] node Node* to manage Pins in
//*/
//static void BuildGUINode(Namespace* node)
//{
//  for (auto& input : node->Inputs)
//  {
//    input->Node = node;
//    input->Kind = PinKind::Input;
//  }
//
//  for (auto& output : node->Outputs)
//  {
//    output->Node = node;
//    output->Kind = PinKind::Output;
//  }
//}

///*! \fn void BuildNodes()
//    \brief build all Nodes
//    \sa BuildNode
//*/
//inline void BuildGUINodes()
//{
//  for (auto& node : s_Nodes)
//    BuildGUINode(node);
//}

//
///*! \fn static int GetNextId()
//    \brief Compute value of identificator for elements
//    \return Value of identificator for added element */
// inline int GetNextId()
//{
//  return s_NextId++;
//}

//static ed::LinkId GetNextLinkId()
//{
//  return ed::LinkId(GetNextId());
//}

