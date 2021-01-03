/**
 * \file	GUI/Elements/Windows/WorkspaceWindow.h
 *
 * A Workspace window elements declaration.

 TODO: Is there so many static / global variable for some reason? Every global variable make code extremely unreadable... and leads to possible bugs
 */

#pragma once

/* ImGUI Node editor include */ //{       /* bracket just for folding block in Code-Blocks - hopefully will work or at least do not hinder in other IDEs*/
// #include <application.h> TODO: Maybe its just from example of node editor...
#include "GUI/NodeEditorUtilities/Builders.h"
#include "GUI/NodeEditorUtilities/Widgets.h"

#include <imgui_node_editor.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
//} /*ImGUI Node editor include - end*/


/* I3T include */ //{

#include "GUI/Elements/IWindow.h" // TODO:

#include "glm/glm.hpp" // TODO: What it do?
// #include "imgui.h" TODO: Already in imgui_node_editor.h

//} /*I3T include end*/


/* Namespace shortcut */ //{

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax; /* TODO: not use namespace */
using ax::Widgets::IconType; /* TODO: not use namespace */

//} /* Namespace shortcut - end */


/* >>> structures, class <<< */ //{


/*! \enum PinType
    \brief types (close to data types) of Pin
 */
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

/*! \enum PinKind
    \brief kinds (in/out) of Pin
 */
enum class PinKind
{
    Output,
    Input
};

/*! \enum NodeType
    \brief types of Node
 */
enum class NodeType
{
  Blueprint,
  Comment,
  Mat4x4
};


/*! \struct Pin
    \brief use to connecting Link to Node
 */
struct Node; /* Just for using in struct Pin*/
struct Pin
{
    ed::PinId   ID; /*! \brief unique (among Pins) identificator */
    ::Node*     Node; /*! \brief Node the Pin belongs to */
    std::string Name; /*! \brief Name of Pin */
    PinType     Type; /*! \brief Type of pin \sa PinType */
    PinKind     Kind; /*! \brief Kind of pin \sa PinKind */

    /*! \fn Pin
        \brief default Pin constructor
        \param[in] id int identificator
        \param[in] name const char*
        \param[in] type PinType of Pin

        PinKind is set to input. Node is set to nullptr.
    */
    Pin(int id, const char* name, PinType type) : ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input){}
};

/*! \struct Node TODO: Create class from this and then inherit other type of Nodes
    \brief Main element of Workspace - matrix, operator, vector, ...
 */
struct Node
{
    ed::NodeId ID;  /*! \brief unique (among Nodes) identificator */
    std::string Name;   /*! \brief Name of Node */
    std::vector<Pin> Inputs;    /*! \brief Vector of input Pins */
    std::vector<Pin> Outputs;   /*! \brief Vector of output Pins */
    ImColor Color;  /*! \brief Color of Node */
    NodeType Type;  /*! \brief Type of Node */
    ImVec2 Size;    /*! \brief Size of TODO: what */

    std::string State;  /*! \brief State TODO: what it means? */
    std::string SavedState; /*! \brief last State that was saved TODO: I guess... */

    /*! \fn Node
        \brief default Node constructor
        \param[in] id int identificator
        \param[in] name const char*
        \param[in] color ImColor of Node, default ImColor(255, 255, 255)

        NodeType is set to Blueprint TODO: what is it?. Size is set to Size(0, 0)
    */
    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255)) : ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0){}
};

/*! \struct Link
    \brief use for connecting Pins (consequently Nodes)
 */
struct Link
{
    ed::LinkId ID;  /*! \brief unique (among Links) identificator */

    ed::PinId StartPinID; /*! \brief Pin id from which Link comes from */
    ed::PinId EndPinID; /*! \brief Pin id to which Link goes to */

    ImColor Color; /*! \brief Color of Link  */

    /*! \fn Link
        \brief default Link constructor
        \param[in] id int identificator
        \param[in] startPinId ed::PinId of start Pin
        \param[in] endPinId ed::PinId of end Pin

        Color is set to Color(255, 255, 255)
    */
    Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId) : ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255){}
};

/*! \struct Matrix4x4 TODO: Create this as inherited type of Node with data* atribut
    \brief data will be get from Core -> data* can be directly in Node
 */
struct Matrix4x4
{
  // Node* node;
  ed::NodeId ID;
  glm::mat4 data;
};

/*! \struct NodeIdLess
    \brief TODO: I have no idea what it do - it is used in s_NodeTouchTime...
 */
struct NodeIdLess
{
    bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
    {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};


/*! \class WorkspaceWindow
    \extends public IWindow
    \brief TODO: Workspace window class for assuming node editor
 */
class WorkspaceWindow : public IWindow
{
public:
  WorkspaceWindow(bool show)
      : IWindow(show){
            // g_Context = ed::CreateEditor(&config);
        };

  virtual ~WorkspaceWindow();
    /*! \fn virtual void WorkspaceWindow::render() TODO: why virtual?
        \brief render one frame of Workspace window
        \details include rendering of popup menus etc...

    */
  virtual void render();
  static glm::mat4 DrawInScene();
};


//} /* >>> structures, class <<< - end */


/* >>> static functions <<< */ //{
/*! \fn static int GetNextId()
    \brief Compute value of identificator for elements
    \return Value of identificator for added element */
static int GetNextId();

/*! \fn static inline ImRect ImGui_GetItemRect()
    \brief Get ImRect of last (active/added ?) item
    \return ImRect : New ImRect with position and size of last item
*/
static inline ImRect ImGui_GetItemRect();

/*! \fn static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
    \brief Enlarge given ImRect (create new / enlarge given ?)
     \param[in/out?] rect ImRect& for enlarge
     \param[in] x float value added to left and right
     \param[in] y float value added to up an d down
    \return ImRect New or the same?
*/
static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y);


/*! \fn static ed::LinkId GetNextLinkId()
    \brief TODO: Probably return (safe)pointer to ... I really dont know... there is always new value of nextId...
    \return TODO: Probably safe pointer
*/
static ed::LinkId GetNextLinkId();


/*! \fn static void TouchNode(ed::NodeId id)
    \brief save s_TouchTime for node
    \sa s_TouchTime
*/
static void TouchNode(ed::NodeId id);


/*! \fn static float GetTouchProgress(ed::NodeId id)
    \brief TODO: I am really not sure
    \return float TODO: something with time
*/
static float GetTouchProgress(ed::NodeId id);


/*! \fn static void UpdateTouch()
    \brief TODO: I am really not sure - update "second"
*/
static void UpdateTouch();


/*! \fn static Node* FindNode(ed::NodeId id)
    \brief search for Node by its id
    \param[in] id NodeId of Node function search for
    \return Node* of Node with given id or nullptr when not found
*/
static Node* FindNode(ed::NodeId id);


/*! \fn static Link* FindLink(ed::LinkId id)
    \brief search for Link by its id
    \param[in] id LinkId of Link function search for
    \return Link* of Link with given id or nullptr when not found
*/
static Link* FindLink(ed::LinkId id);


/*! \fn static Pin* FindPin(ed::PinId id)
    \brief search for Pin by its id
    \param[in] id PinId of Pin function search for
    \return Pin* of Pin with given id or nullptr when not found
*/
static Pin* FindPin(ed::PinId id);


/*! \fn static bool IsPinLinked(ed::PinId id)
    \brief check whether some Link is connect to Pin with given id
    \param[in] id PinId of Pin function check
    \return true if some Link is connect to Pin, false otherwise or if Pin id not exist
*/
static Pin* FindPin(ed::PinId id);


/*! \fn static bool CanCreateLink(Pin* a, Pin* b)
    \brief check whether it is possible to connect two Pins
    \param[in] a,b Pin* to check possibility of connection between
    \return true if it is possible create Link from a to b, false otherwise
*/
static bool CanCreateLink(Pin* a, Pin* b);


/*! \fn static void BuildNode(Node* node)
    \brief set right values (Node and Kind) to all Pins in node
    \param[in] node Node* to manage Pins in
*/
static void BuildNode(Node* node);


/*! \fn static Node* SpawnMat4x4Node()
    \brief construct Mat4x4Node
    \param[in] node Node* to manage Pins in
*/
static Node* SpawnMat4x4Node();

/*! \fn static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
    \brief TODO: Something split... :-D
    \param[in] lots of them...
*/
static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);



//} /* >>> static functions <<< - end */

/* >>>> functions <<< */ //{

/*! \fn void BuildNodes()
    \brief build all Nodes
    \sa BuildNode
*/
void BuildNodes();


/*! \fn const char* Application_GetName()
    \brief create some application name
    \return string application name
*/
const char* Application_GetName();


/*! \fn ImColor GetIconColor(PinType type)
    \brief select/construct color for given PinType
    \return ImColor
*/
ImColor GetIconColor(PinType type);


/*! \fn void DrawPinIcon(const Pin& pin, bool connected, int alpha)
    \brief draw icon of Pin TODO: probably on place of cursor?
    \param[in] pin const Pin& use for select IconType based on PinType
    \param[in] connected bool of pin connection state
    \param[in] alpha int of transparency channel of ImColor of icom
*/
void DrawPinIcon(const Pin& pin, bool connected, int alpha);


/*! \fn void ShowStyleEditor(bool* show = nullptr)
    \brief TODO: I am not sure what it does...
    \param[in] show bool* TODO: show what?
*/
void ShowStyleEditor(bool* show = nullptr);


/*! \fn void ShowLeftPane(float paneWidth)
    \brief TODO: I am not sure what it does... and inside is really mesh... :-D
    \param[in] paneWidth float width of pane TODO: pane where hard coded to left? or what is it?
*/
void ShowLeftPane(float paneWidth);


/*! \fn void popupMenu(bool& createNewNode, Pin* newNodeLinkPin, Pin* newLinkPin, ed::NodeId& contextNodeId, ed::PinId& contextPinId, ed::LinkId& contextLinkId)
    \brief TODO: Its something as SpawnXxxNode am I right? Whether yes, should we rename it to some consistent names?
*/
void popupMenu(bool& createNewNode, Pin* newNodeLinkPin, Pin* newLinkPin, ed::NodeId& contextNodeId,
               ed::PinId& contextPinId, ed::LinkId& contextLinkId);


/*! \fn void DeleteNode()
    \brief Delete all Nodes and Links (TODO: split to two fcn or rename) queried for deletion
*/
void DeleteNode();


/*! \fn Matrix4x4* findMatFromNode(ed::NodeId& nodeId) // TODO: Remove this - Node should know reference to his data... ( and do not data know reference to its node... )
    \brief Get Matrix4x4 belongs to given Node
*/
Matrix4x4* findMatFromNode(ed::NodeId& nodeId);


/*! \fn Link* findLinkFromStartPin(ed::PinId id)
    \brief Get Link start at given Pin
    \param[in] id PinId of Pin by which is Link searched
    \return Link* to Link connected to given Pin
*/
Link* findLinkFromStartPin(ed::PinId id);


/*! \fn Node* findNodeByInputPin(ed::PinId id) // TODO: Remove it - Pin know Node to which it belong
*/
Node* findNodeByInputPin(ed::PinId id);



/*! \fn void fromArrayToVec4(glm::vec4& vec, float arr[4])
    \brief convert float array of size 4 to glm::vec4
    \param[out] vec glm::vec4& result object
    \param[in] arr float* input values -> must be of size 4*sizeof(float)
*/
void fromArrayToVec4(glm::vec4& vec, float arr[4]);


/*! \fn void fromVecToArray4(glm::vec4& vec, float arr[4])
    \brief convert glm::vec4 to float array of size 4
    \param[in] vec glm::vec4& input object
    \param[out] arr float* result values -> should be of size 4*sizeof(float)
*/
void fromVecToArray4(glm::vec4& vec, float arr[4]);


/*! \fn void DrawBlueprintNodes(util::BlueprintNodeBuilder& builder, Pin* newLinkPin)
    \brief convert glm::vec4 to float array of size 4
    \param[in] vec glm::vec4& input object
    \param[out] arr float[4] result values
*/
void DrawBlueprintNodes(util::BlueprintNodeBuilder& builder, Pin* newLinkPin);


/*! \fn void WorkspaceWindow_init() TODO: rename to WorkspaceWindow::init()
    \brief initialize Workspace window
    \details configuration setting

*/
void WorkspaceWindow_init();


/*! \fn void WorkspaceWindow_finalize() TODO: rename to WorkspaceWindow::finalize()
    \brief finalize Workspace window
    \details release textures and destroy EditorContext

*/
void WorkspaceWindow_finalize();


/*! \fn void RenderNodes(util::BlueprintNodeBuilder& builder, Pin* newLinkPin)
    \brief render all nodes from s_Nodes TODO: avoid using global variables
*/
void RenderNodes(util::BlueprintNodeBuilder& builder, Pin* newLinkPin);


/*! \fn void RenderMat4x4Node(util::BlueprintNodeBuilder& builder, Node& node, Pin* newLinkPin)
    \brief render Mat4x4 Node TODO: inputs
*/
void RenderMat4x4Node(util::BlueprintNodeBuilder& builder, Node& node, Pin* newLinkPin);





//} /* >>>> functions <<< - end */




