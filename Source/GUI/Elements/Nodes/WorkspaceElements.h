#pragma once

#include <algorithm>
#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <string>
#include "spdlog/fmt/fmt.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "Config.h"
#include "Core/API.h"
#include "Core/Nodes/GraphManager.h"
#include "GUI/Elements/IWindow.h"
#include "GUI/NodeEditorUtilities/Builders.h" /* \todo soubor s malym pismenkem na zacatku neexistuje - porad mi to prosim neprepisujte :-D */
#include "GUI/NodeEditorUtilities/Widgets.h"

#include <../Source/DIWNE/diwne.h>
#include <../Source/DIWNE/Node.h>
#include <../Source/DIWNE/Pin.h>

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

enum FloatPopupMode
{
    Radians,
    Degree
};

extern std::map<WorkspaceLevelOfDetail, std::string> WorkspaceLevelOfDetailName;

/* DIWNE - \todo JH to remove, but I need something what use instead -> from Type get Shape and Color */
extern std::map<EValueType, EColor> WorkspacePinColorBackground;

extern std::map<EValueType, DIWNE::IconType> WorkspacePinShapeBackground;

extern std::map<EValueType, DIWNE::IconType> WorkspacePinShapeForeground;

extern std::map<EValueType, EColor> WorkspacePinColorForeground;

// endtodo


class WorkspaceNode : public DIWNE::Node
{
protected:
    std::string m_topLabel;
    std::string m_middleLabel;
public:
    WorkspaceNode(DIWNE::ID id, std::string const topLabel="Header", std::string const middleLabel="Content");

    std::string getTopLabel(){return m_topLabel;};
    std::string getMiddleLabel(){return m_middleLabel;};

    void setTopLabel(std::string label){m_topLabel = label;};
    void setMiddleLabel(std::string label){m_middleLabel = label;};

	/* DIWNE function */
    virtual bool topContent(DIWNE::Diwne &diwne);
    virtual bool middleContent(DIWNE::Diwne &diwne);
};

class WorkspacePin : public DIWNE::Pin
{
protected:
	bool        m_showLabel;
	std::string m_label;		/*! \brief Name of Pin */

public:
    WorkspacePin(DIWNE::ID id, std::string label);

	bool getShowLabel() const { return m_showLabel; };
	std::string const getLabel() const { return m_label; };

    void setShowLabel(bool showLabel) { m_showLabel = showLabel; };
	void setLabel(std::string label) { m_label = label; };
};

///*! \class WorkspaceLinkProperties
//    \brief Information of Link for graphic
// */
//class WorkspaceLinkProperties
//{
//protected:
//	const ne::LinkId m_id;
//	ImColor					 m_color;
//	float						 m_thickness;
//
//public:
//	WorkspaceLinkProperties(ne::LinkId const id);
//
//	ne::LinkId const getId() const;
//	ImColor const		 getColor() const;
//	float const			 getThickness() const;
//};

/* static functions */
extern int numberOfCharWithDecimalPoint(float value, int numberOfVisibleDecimal);

