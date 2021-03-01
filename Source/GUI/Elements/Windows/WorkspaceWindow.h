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

#include "../../../Core/Nodes/GraphManager.h"
#include "../../../Core/Nodes/Node.h"
#include "../../../Core/Nodes/NodeImpl.h"
#include "Config.h"
#include "Core/Application.h"
#include "GUI/Elements/Nodes/WorkspaceElements.h"
#include "pgr.h"
#include <memory>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <imgui_node_editor.h>
#include <imgui_node_editor_internal.h>

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

// using namespace ax;
// using ax::Widgets::IconType;

namespace ne = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

/* >>> Some struct <<< \todo What is it for? */ //{

struct NodeIdLess
{
	bool operator()(const ne::NodeId& lhs, const ne::NodeId& rhs) const { return lhs.AsPointer() < rhs.AsPointer(); }
};

static std::map<ne::NodeId, float, NodeIdLess> s_NodeTouchTime;

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
// static bool CanCreateLink(WorkspacePinProperties* a, WorkspacePinProperties* b)
//{
//  if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
//    return false;
//
//  return true;
//}

//} /* >>> Static functions - end <<< */

/*! \class class for Workspace window object
    \brief Store everything what Workspace window need
*/
class WorkspaceWindow : public IWindow
{
public:
	I3T_WINDOW(WorkspaceWindow)

	Application& WholeApplication;
	ne::EditorContext* NodeEditorContext; /*! \brief Object for store workspace scene */

	ImTextureID HeaderBackgroundTexture;

	util::NodeBuilder NodeBuilderContext; /* \todo builder as variable of WorkspceWindow?*/
																				//

	// static std::vector<Namespace*> s_Nodes; /*! \brief All Nodes */
	std::vector<std::unique_ptr<WorkspaceNode>> WorkspaceNodes; /*! \brief All WorkspaceNodes */

	//    ne::NodeId contextNodeId;
	//    ne::LinkId contextLinkId;
	//    ne::PinId contextPinId;
	//    bool createNewNode;
	//    WorkspacePin* newNodeLinkPin ;
	//    WorkspacePin* newLinkPin;
	//
	//    float leftPaneWidth;
	//    float rightPaneWidth;

	ImTextureID HeaderBackground; /* ImTextureID is not id, but void* - so whatever application needs */

	const float ConstTouchTime; /*! \brief \TODO: take values from (move to) Const.h */

	WorkspaceWindow(bool show);
	~WorkspaceWindow();

	/*! \fn static Node* FindNode(ne::NodeId id)
\brief search for Node by its id
\param[in] id NodeId of Node function search for
\return Node* of Node with given id or nullptr when not found
*/
	WorkspaceNode* FindNode(ne::NodeId id);

	/*! \fn static Link* FindLink(ne::LinkId id)
	    \brief search for Link by its id
	    \param[in] id LinkId of Link function search for
	    \return Link* of Link with given id or nullptr when not found
	*/
	//    WorkspaceLink* FindLink(ne::LinkId id);

	/*! \fn static Pin* FindPin(ne::PinId id)
	    \brief search for Pin by its id
	    \param[in] id PinId of Pin function search for
	    \return Pin* of Pin with given id or nullptr when not found
	*/
	//    WorkspacePin* FindPin(ne::PinId id);

	void UpdateTouchAllNodes();

	void WorkspaceDrawNodes(util::NodeBuilder& builder, Core::Pin* newLinkPin);

	void render();

	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2,
	              float splitter_long_axis_size = -1.0f);

	void ShowLeftPane(float paneWidth);

	void ShowStyleEditor(bool* show = nullptr);
};
