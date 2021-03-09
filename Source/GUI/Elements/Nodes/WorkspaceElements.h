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

using ax::Widgets::IconType;
namespace util = ax::NodeEditor::Utilities;

namespace ne =
		ax::NodeEditor; /* /ed/ is used in imgui_node_editor.cpp for some other namespace - so for clearity I use /ne/ */

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

enum class WorkspaceViewScale
{
    Full,
    JustChangeAble
};

extern std::map<EValueType, ImColor> WorkspacePinColor;
extern std::map<EValueType, IconType> WorkspacePinShape;

class WorkspaceNode
{
public:
	const ne::NodeId m_id;
	std::string m_state; /*! \brief e.g. selected \todo what is it for? */
	std::string m_label;
	WorkspaceViewScale m_viewScale;

	ImColor m_color; /*! \brief Color of Node */
	ImVec2 m_size;   /*! \brief Size of box */
	float m_touchTime;

	std::string m_headerLabel;
	ImTextureID m_headerBackground;


	virtual void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)=0;

	virtual void drawInputLinks()=0;
	virtual void drawHeader(util::NodeBuilder& builder);
	virtual void drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)=0;
	virtual void drawData(util::NodeBuilder& builder)=0;
	virtual void drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)=0;

	/*! \fn void TouchNode(const float constTouchTime) \todo for what is it ?
	\brief update TouchTime
	*/
	void TouchNode(const float constTouchTime);

	void UpdateTouch(const float constDeltaTime);

	float GetTouchProgress(const float constTouchTime);

//    // named-constructor-idiom
//    //  copy ctor public
//    WorkspaceNode(WorkspaceNode const& other);
//
//    static WorkspaceNode WorkspaceNodeFromCore(std::unique_ptr<Core::NodeBase> nodebase, ImTextureID headerBackground, std::string headerLabel){
//        return WorkspaceNode(nodebase->getId(), headerBackground, headerLabel);
//    }

//private:
   	/* \todo some better constructors - this are just for test*/
	WorkspaceNode(const ne::NodeId id, ImTextureID headerBackground, std::string headerLabel = "default WorkspaceNode HeaderLabel", WorkspaceViewScale viewScale = WorkspaceViewScale::Full );


};

/* \todo some better way? -> maybe use id of input pin that the link belongs to...  */
static int s_linkID = 0;
static int s_getLinkID()
{
	return s_linkID++;
}

/*! \class WorkspaceLinkProperties
    \brief Information of Link for graphic
 */
class WorkspaceLinkProperties
{
public:
	const ne::LinkId m_id;
	ImColor m_color;

	WorkspaceLinkProperties(const ne::LinkId id);
};

/*! \class WorkspacePinProperties
    \brief Information of Pin for graphic
 */
class WorkspacePinProperties
{
public:
	const ne::PinId m_id; /*! \brief unique (among Pins) identificator */
	std::string m_name;    /*! \brief Name of Pin */
	const PinKind m_kind;  /*! \brief Kind of pin \sa PinKind */
	const EValueType m_type;

	int m_iconSize; /*! \brief Size of Pin icon \TODO: take from (move to) Const.h */

	bool m_connected;
	float m_alpha;

	WorkspacePinProperties(const ne::PinId id, const char* name, PinKind kind, EValueType type);

	bool IsPinConnected(); /* \todo check in Core ? */

	bool CanCreateLink(Core::Pin* b); /* \todo check in Core ? */
};
