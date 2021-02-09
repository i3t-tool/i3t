#pragma once


namespace ed = ax::NodeEditor;


/*! \enum PinKind
    \brief kinds (in/out) of Pin
 */
enum class PinKind
{
  Output,
  Input
};


///*! \fn ImColor GetIconColor(PinType type)
//    \brief select/construct color for given PinType
//    \return ImColor
//*/
//// \TODO change to core pintypes and fuse with DrawPinIcon
//inline ImColor GetIconColor(EValueType type)
//{
//  switch (type)
//  {  // \TODO: Define colors in Const.h
//  case EValueType::Float:
//    return ImColor(255, 255, 255);
//  case EValueType::Matrix:
//    return ImColor(220, 48, 48);
//  case EValueType::MatrixMul:
//    return ImColor(68, 201, 156);
//  case EValueType::Pulse:
//    return ImColor(147, 226, 74);
//  case EValueType::Quat:
//    return ImColor(124, 21, 153);
//  case EValueType::Screen:
//    return ImColor(51, 150, 215);
//  case EValueType::Vec3:
//    return ImColor(218, 0, 183);
//  case EValueType::Vec4:
//    return ImColor(255, 48, 48);
//  default:
//    // \TODO: Log error - unknown PinType
//    return ImColor(0, 0, 0);
//  }
//};
/* \todo look above - and move this to Konstant.h */
const std::map<EValueType, ImColor> WorkspacePinColor = {
    {EValueType::Float,     ImColor(255, 255, 255)},
    {EValueType::Matrix,    ImColor(220, 48, 48)},
    {EValueType::MatrixMul, ImColor(68, 201, 156)},
    {EValueType::Pulse,     ImColor(147, 226, 74)},
    {EValueType::Quat,      ImColor(124, 21, 153)},
    {EValueType::Screen,    ImColor(51, 150, 215)},
    {EValueType::Vec3,      ImColor(218, 0, 183)},
    {EValueType::Vec4,      ImColor(255, 48, 48)}
};

///*! \fn void DrawPinIcon(const Pin& pin, bool connected, int alpha)
//    \brief draw icon of Pin \TODO: probably on place of cursor?
//    \param[in] pin const Pin& use for select IconType based on PinType
//    \param[in] connected bool of pin connection state
//    \param[in] alpha int of transparency channel of ImColor of icom
//*/
//// \TODO change to core pintypes
//WorkspacePin::drawPin(){
//  IconType iconType;
//  ImColor color = GetIconColor(pin.Type);
//  color.Value.w = alpha / 255.0f;
//  switch (pin.Type)
//  {
//  case EValueType::Matrix:
//    iconType = IconType::Flow;
//    break;
//  case EValueType::MatrixMul:
//    iconType = IconType::Circle;
//    break;
//  case EValueType::Pulse:
//    iconType = IconType::Circle;
//    break;
//  case EValueType::Float:
//    iconType = IconType::Circle;
//    break;
//  case EValueType::Quat:
//    iconType = IconType::Circle;
//    break;
//  case EValueType::Screen:
//    iconType = IconType::Circle;
//    break;
//  case EValueType::Vec3:
//    iconType = IconType::Circle;
//    break;
//  case EValueType::Vec4:
//    iconType = IconType::Square;
//    break;
//  default:
//    // \TODO: Log error Unknown PinType
//    return;
//  }
//
//  ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
//
//}


/* \todo look above - and move this to Konstant.h */
const std::map<EValueType, ImColor> WorkspacePinShape = {
    {EValueType::Float,     IconType::Circle},
    {EValueType::Matrix,    IconType::Flow},
    {EValueType::MatrixMul, IconType::Circle},
    {EValueType::Pulse,     IconType::Circle},
    {EValueType::Quat,      IconType::Circle},
    {EValueType::Screen,    IconType::Circle},
    {EValueType::Vec3,      IconType::Circle},
    {EValueType::Vec4,      IconType::Square}
};

/*! \class Link
    \brief use for connecting Pins (consequently Nodes)
 */
class WorkspaceLink
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
  WorkspaceLink(ed::LinkId id, GUIPin* StartPin, GUIPin* EndPin)
      : ID(id), StartPin(StartPin), EndPin(EndPin), Color(255, 255, 255)
  {
  }
};



class WorkspaceNode;

/*! \struct Pin
    \brief use to connecting Link to Node
 */
class WorkspacePin
{
public:
  ed::PinId ID; /*! \brief unique (among Pins) identificator */
  WorkspaceNode* Node; /*! \brief Node the Pin belongs to */
  std::string Name; /*! \brief Name of Pin */
  EValueType Type;   /*! \brief Type of pin \sa PinType */
  PinKind Kind;  /*! \brief Kind of pin \sa PinKind */

  int IconSize = 24; /*! \brief Size of Pin icon \TODO: take from (move to) Const.h */

    bool conected;
    float alpha;
  /*! \fn Pin
        \brief default Pin constructor
        \param[in] id int identificator
        \param[in] name const char*
        \param[in] type PinType of Pin

        PinKind is set to input. Node is set to nullptr.
    */
  WorkspacePin(int id, const char* name, WorkspaceNode* Node, PinKind Kind, EValueType type)
  : ID(id), Name(name), Node(Node),  Kind(Kind), Type(type), conected(false), alpha(2.0) {}



/*! \fn bool IsPinConected(ed::PinId id)
    \brief check whether some Link is connect to Pin with given id
    \param[in] id PinId of Pin function check
    \return true if some Link is connect to Pin, false otherwise or if Pin id not exist
*/
bool IsPinConected();

///*! \fn void drawPin()
//    \brief draw icon of Pin \TODO: probably on place of cursor?
//*/
  virtual void drawPin();
};



class WorkspaceNode
{

    int id;
    std::string state; /*! \brief e.g. selected */

    ImColor Color; /*! \brief Color of Node */
      ENodeType Type; /*! \brief Type of Node */
      ImVec2 Size;  /*! \brief Size of box */
      float TouchTime;

public:
    WorkspaceNode(int id, std::string state)
    : id(id), state(state)
    {
        Color = ImColor(255, 255, 255);
    }

    virtual void drawNode(util::NodeBuilder& builder, GUIPin* newLinkPin) = 0;

    /*! \fn static void TouchNode(ed::NodeId id)
    \brief save s_TouchTime for node
    \sa s_TouchTime
    */
    void TouchNode();

};


class WorkspaceNodeHeader
{
public:
    std::string label;
    ImTextureID header_background;

    WorkspaceNodeHeader(std::string label, ImTextureID header_background)
    :label(label) header_background(header_background) {}

    virtual void drawNodeHeader(util::NodeBuilder& builder);

};


class WorkspaceNodeData
{
public:
    std::unique_ptr<Core::NodeBase> nodebase; /*! \brief reference to core */


    WorkspaceNodeData(std::unique_ptr<Core::NodeBase> nodebase)
    : nodebase(nodebase) {}

    virtual void drawData();
};


class WorkspaceNodeInputs
{
public:
    std::vector<GUIPin*> inputs; /*! \brief Vector of input Pins */


    WorkspaceNodeInputs(){}

    virtual void spawnInputs(const std::vector<Pin>& inputPins);
    virtual void drawInputs();
};


class WorkspaceNodeOutputs
{
public:
    std::vector<GUIPin*> outputs; /*! \brief Vector of output Pins*/


    WorkspaceNodeOutputs(){}

    virtual void spawnOutputs(const std::vector<Pin>& outputPins);
    virtual void drawOutputs(util::NodeBuilder& builder, GUIPin* newLinkPin);

};
