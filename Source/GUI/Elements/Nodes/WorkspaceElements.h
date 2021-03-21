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
#include <initializer_list>

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
    SetValues,
    Label
};

extern std::map<EValueType, ImColor> WorkspacePinColor;
extern std::map<EValueType, IconType> WorkspacePinShape;

/* This allow (almost) named argument to constructor: https://en.cppreference.com/w/cpp/language/aggregate_initialization */
struct WorkspaceNodeArgs
{
    WorkspaceViewScale viewScale = WorkspaceViewScale::Full;
    std::string headerLabel = "default WorkspaceNode header";
    std::string nodeLabel = "Node";
    /* \todo JH it will be nice, if we are able give some default headerBackground here ... */
};

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

	WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, WorkspaceNodeArgs const& args);
    WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground);

};

/*! \class WorkspaceLinkProperties
    \brief Information of Link for graphic
 */
class WorkspaceLinkProperties
{
public:
	const ne::LinkId m_id;
	ImColor m_color;

	WorkspaceLinkProperties(ne::LinkId const id);
};


