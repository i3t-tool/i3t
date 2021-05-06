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
#include <Core/API.h>

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <initializer_list>

/* >>> Static function <<< */ //{

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
//} /* >>> Static functions - end <<< */

using ax::Widgets::IconType;
namespace util = ax::NodeEditor::Utilities;

namespace ne = ax::NodeEditor; /* /ed/ is used in imgui_node_editor.cpp for some other namespace - so for clearity I use /ne/ */

class WorkspaceLinkProperties;
class WorkspacePinProperties;

/*! \enum PinKind
    \brief kinds (in/out) of Workspace Pin
 */
enum class PinKind
{
	Output,
	Input
};

enum class WorkspaceLevelOfDetail
{
    Full,
    SetValues,
    Label
};

extern std::map<WorkspaceLevelOfDetail, std::string> WorkspaceLevelOfDetailName;

extern std::map<EValueType, ImColor> WorkspacePinColor;
extern std::map<EValueType, IconType> WorkspacePinShape;
extern std::map<EValueType, ImColor> WorkspaceInnerPinColor;


/* This allow (almost) named argument to constructor: https://en.cppreference.com/w/cpp/language/aggregate_initialization */
struct WorkspaceNodeArgs
{
    std::string headerLabel = "default WorkspaceNode header";
    std::string nodeLabel = "Node";
    /* \todo JH it will be nice, if we are able give some default headerBackground here ... */
};

class WorkspaceNode
{
protected:
	const ne::NodeId m_id;
	std::string m_label;

	ImColor m_color; /*! \brief Color of Node */
	ImVec2 m_size;   /*! \brief Size of box */ /* \todo JH I think this is useless */
	float m_touchTime;

	std::string m_headerLabel;
	ImTextureID m_headerBackground;

public:
    WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, WorkspaceNodeArgs const& args);
    WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, std::string headerLabel = "Node", std::string nodeLabel = "Node");

    ne::NodeId const getId() const;

    virtual std::string getHeaderLabel();
    virtual ImTextureID getHeaderBackground();

	virtual void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin=nullptr, bool withPins=true);

	virtual void drawHeader(util::NodeBuilder& builder);
	virtual void drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)=0;
	virtual void drawData(util::NodeBuilder& builder, int index)=0;
	virtual void drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)=0;

	/*! \fn void TouchNode(const float constTouchTime) \todo for what is it ?
	\brief update TouchTime
	*/
	void TouchNode(const float constTouchTime);

	void UpdateTouch(const float constDeltaTime);

	float GetTouchProgress(const float constTouchTime);

	virtual bool dataAreValid();

};

class WorkspacePinProperties
{
protected:
	ne::PinId const m_id; /*! \brief unique (among Pins) identificator */

    bool m_showLabel;
	std::string m_label;    /*! \brief Name of Pin */
	int m_iconSize; /*! \brief Size of Pin icon \TODO: take from (move to) Const.h */
    ImColor m_color;

public:
    WorkspacePinProperties(ne::PinId const id, std::string label);

    ne::PinId const getId() const;
    int const getIconSize() const;
    ImColor const getColor() const;

    bool getShowLabel() const;
    std::string const getLabel() const;
};

/*! \class WorkspaceLinkProperties
    \brief Information of Link for graphic
 */
class WorkspaceLinkProperties
{
protected:
	const ne::LinkId m_id;
	ImColor m_color;
	float m_thickness;

public:
	WorkspaceLinkProperties(ne::LinkId const id);

	ne::LinkId const getId() const;
	ImColor const getColor() const;
	float const getThickness() const;

};

extern int numberOfCharWithDecimalPoint(float value, int numberOfVisibleDecimal);


