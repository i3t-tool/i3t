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

namespace ne = ax::NodeEditor; /* /ed/ is used in imgui_node_editor.cpp for some other namespace - so for clearity I use /ne/ */

class WorkspaceLinkProperties;
class WorkspacePinProperties;


/*! \enum PinKind
    \brief kinds (in/out) of Pin \todo maybe unused - this info is in Core
 */
enum class PinKind
{
  Output,
  Input
};

extern std::map<EValueType, ImColor> WorkspacePinColor;
extern std::map<EValueType, IconType> WorkspacePinShape;



class WorkspaceNode
{
public:
    const ne::NodeId& Id;
    std::string State; /*! \brief e.g. selected \todo what is it for? */
    std::string Label;

    ImColor Color; /*! \brief Color of Node */
    ImVec2 Size;  /*! \brief Size of box */
    float TouchTime;

    std::string HeaderLabel;
    ImTextureID HeaderBackground;

    /* \todo some better constructors - this are just for test*/
    WorkspaceNode(const ne::NodeId& id, ImTextureID headerBackground);
    WorkspaceNode(const ne::NodeId& id, std::string headerLabel, ImTextureID headerBackground);

    virtual void drawWorkspaceNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)=0;

    virtual void drawWorkspaceNodeHeader(util::NodeBuilder& builder);


    /*! \fn static void TouchNode(ne::NodeId id) \todo for what is it ?
    \brief update TouchTime
    */
    void TouchNode(float touchTime);

    float GetTouchProgress(const float constTouchTime);

};


/*! \class WorkspaceLinkProperties
    \brief Information of Link for graphic
 */
class WorkspaceLinkProperties
{
public:
    const ne::LinkId Id;
    ImColor Color;

    WorkspaceLinkProperties(const ne::LinkId id);

};

/*! \class WorkspacePinProperties
    \brief Information of Pin for graphic
 */
class WorkspacePinProperties
{
public:
    const ne::PinId& Id; /*! \brief unique (among Pins) identificator */
    std::string Name; /*! \brief Name of Pin */
    const PinKind Kind;  /*! \brief Kind of pin \sa PinKind */
    const EValueType Type;

    int IconSize; /*! \brief Size of Pin icon \TODO: take from (move to) Const.h */

    bool Connected;
    float Alpha;

    WorkspacePinProperties(const ne::PinId& id, const char* name, PinKind kind, EValueType type);


    bool IsPinConnected(); /* \todo check in Core ? */

    bool CanCreateLink(Core::Pin* b); /* \todo check in Core ? */
};

typedef std::vector<Core::Pin>::const_iterator pinIter;
typedef std::vector< std::unique_ptr<WorkspacePinProperties> >::const_iterator pinPropIter;
typedef std::vector< std::unique_ptr<WorkspaceLinkProperties> >::const_iterator linkPropIter;

class WorkspaceNodeBaseData
{
public:

    /*see: https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function*/
    const std::unique_ptr<Core::NodeBase> Nodebase; /*! \brief reference to core
                                                         WorkspaceNodeBaseData is owner of unique pointer
                                                    */

    std::vector< std::unique_ptr<WorkspaceLinkProperties> > WorkspaceLinksProperties;
    std::vector< std::unique_ptr<WorkspacePinProperties> > WorkspaceInputsProperties;
    std::vector< std::unique_ptr<WorkspacePinProperties> > WorkspaceOutputsProperties;

    WorkspaceNodeBaseData(std::unique_ptr<Core::NodeBase> nodebase);

    virtual void drawWorkspaceInputLinks(util::NodeBuilder& builder);
    virtual void drawWorkspaceInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);
    virtual void drawWorkspaceNodeData(util::NodeBuilder& builder) = 0;
    virtual void drawWorkspaceOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin);


};

