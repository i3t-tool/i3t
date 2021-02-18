#pragma once

#include "GUI/Elements/IWindow.h"

#include "GUI/NodeEditorUtilities/Builders.h" /* \todo soubor s malym pismenkem na zacatku neexistuje - porad mi to prosim neprepisujte :-D */
#include "GUI/NodeEditorUtilities/Widgets.h"

#include <glm/glm.hpp>

#include "../../../Core/Nodes/Node.h"
#include "../../../Core/Nodes/NodeImpl.h"
#include "../../../Core/Nodes/GraphManager.h"
#include "Core/Application.h"
#include <memory>
#include "pgr.h"
#include "Config.h"
#include "GUI/Elements/Nodes/WorkspaceElements.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_node_editor.h>
#include <imgui_internal.h>
#include <imgui_node_editor_internal.h>

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>


using ax::Widgets::IconType;
namespace util = ax::NodeEditor::Utilities;

namespace ne = ax::NodeEditor; /* /ed/ is used in imgui_node_editir.cpp for some other namespace - so for clearity I use /ne/ */

/*! \enum PinKind
    \brief kinds (in/out) of Pin \todo maybe unused - this info is in Core
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
extern std::map<EValueType, ImColor> WorkspacePinColor;

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
extern std::map<EValueType, IconType> WorkspacePinShape;


/*! \class Link
    \brief use for connecting Pins (consequently Nodes)
 */
class WorkspaceLink
{
    public:
  ne::LinkId ID; /*! \brief unique (among Links) identificator */

  WorkspacePin* StartPin; /*! \brief Pin from which Link comes from */
  WorkspacePin* EndPin; /*! \brief Pin to which Link goes to */

  ImColor Color; /*! \brief Color of Link  */

  /*! \fn Link
    \brief default Link constructor
    \param[in] id int identificator
    \param[in] Pin* pointer of start Pin
    \param[in] Pin* pointer of end Pin

    Color is set to Color(255, 255, 255)
*/
  WorkspaceLink(ne::LinkId id, WorkspacePin* StartPin, WorkspacePin* EndPin);

  void drawLink();
};


class WorkspaceNode
{
public:
    const ne::NodeId& Id;
    std::string State; /*! \brief e.g. selected \todo what is it for? */

    ImColor Color; /*! \brief Color of Node */
    ImVec2 Size;  /*! \brief Size of box */
    float TouchTime;

    std::string HeaderLabel;
    ImTextureID HeaderBackground;


    WorkspaceNode(const ne::NodeId& id);

    virtual void drawWorkspaceNode(util::NodeBuilder& builder, WorkspacePin* newLinkPin)=0;

    virtual void drawWorkspaceNodeHeader(util::NodeBuilder& builder);


    /*! \fn static void TouchNode(ne::NodeId id) \todo for what is it ?
    \brief update TouchTime
    */
    void TouchNode(float touchTime);

    float GetTouchProgress(const float constTouchTime);

};



class WorkspaceLinkProperties
{
public:
    const ed::LinkId Id;

    WorkspaceLinkProperties(ed::LinkId id);

};

/*! \struct Pin
    \brief use to connecting Link to Node
 */
class WorkspacePinProperties
{
public:
  const ne::PinId& Id; /*! \brief unique (among Pins) identificator */
  std::string Name; /*! \brief Name of Pin */
  const PinKind Kind;  /*! \brief Kind of pin \sa PinKind */
  const EValueType& Type;

  int IconSize = 24; /*! \brief Size of Pin icon \TODO: take from (move to) Const.h */

    bool Connected;
    float Alpha;
  /*! \fn Pin
        \brief default Pin constructor
        \param[in] id int identificator
        \param[in] name const char*
        \param[in] type PinType of Pin

        PinKind is set to input. Node is set to nullptr.
    */
  WorkspacePinProperties(const ne::PinId& id, const char* name, PinKind Kind, EValueType type);


    /*! \fn bool IsPinConected(ne::PinId id)
        \brief check whether some Link is connect to Pin with given id
        \param[in] id PinId of Pin function check
        \return true if some Link is connect to Pin, false otherwise or if Pin id not exist
    */
    bool IsPinConnected();

    bool CanCreateLink(Core::Pin* b); /* \todo check in Core ? */
};

class WorkspaceNodeBaseData
{
public:

    /*see: https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function*/
    const std::unique_ptr<Core::NodeBase> Nodebase; /*! \brief reference to core
                                                         WorkspaceNodeBaseData is owner of unique pointer
                                              */

    std::vector<WorkspaceLinkProperties> WorkspaceLinksProperties;
    std::vector<WorkspacePinProperties> WorkspaceInputsProperties;
    std::vector<WorkspacePinProperties> WorkspaceOutputsProperties;

    WorkspaceNodeBaseData(std::unique_ptr<Core::NodeBase> nodebase);

    virtual void drawWorkspaceInputLinks;(util::NodeBuilder& builder);
    virtual void drawWorkspaceInputs(util::NodeBuilder& builder);
    virtual void drawWorkspaceNodeData(util::NodeBuilder& builder) = 0;
    virtual void drawWorkspaceOutputs(util::NodeBuilder& builder);


};

