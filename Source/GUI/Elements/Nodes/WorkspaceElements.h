#pragma once

#include <algorithm>
#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "spdlog/fmt/fmt.h"
#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "Config.h"
#include "Core/API.h"
#include "Core/Nodes/GraphManager.h"
#include "GUI/Elements/IWindow.h"

#include "DIWNE/diwne_include.h"

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
	Label,
	LightCycle
};

enum FloatPopupMode
{
	Angle,
	Value
};

extern std::map<WorkspaceLevelOfDetail, std::string> WorkspaceLevelOfDetailName;

/* DIWNE - \todo JH \todo MH see in .cpp to remove, but I need something what
 * use instead -> from Type get Shape and Color */
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
	WorkspaceNode(DIWNE::Diwne& diwne, DIWNE::ID id,
	              std::string const topLabel = "Header",
	              std::string const middleLabel = "Content");
	~WorkspaceNode();

	std::string getTopLabel() { return m_topLabel; };
	std::string getMiddleLabel() { return m_middleLabel; };

	void setTopLabel(std::string label) { m_topLabel = label; };
	void setMiddleLabel(std::string label) { m_middleLabel = label; };

	/* DIWNE function */
	virtual bool bypassFocusForInteractionAction();
	virtual bool beforeContent();
	virtual bool topContent();
	virtual bool middleContent();
	virtual bool leftContent();
	virtual bool rightContent();
	virtual bool bottomContent();
	virtual void deleteAction();

	bool m_removeFromWorkspaceWindow;
	bool getRemoveFromWorkspace() const { return m_removeFromWorkspaceWindow; };
	void setRemoveFromWorkspace(bool value)
	{
		m_removeFromWorkspaceWindow = value;
	};

	virtual void drawMenuDelete();
	virtual void popupContent();
};

class WorkspacePin : public DIWNE::Pin
{
protected:
	bool m_showLabel;
	std::string m_label; /*! \brief Name of Pin */

public:
	WorkspacePin(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const label);

	bool getShowLabel() const { return m_showLabel; };
	std::string const getLabel() const { return m_label; };

	void setShowLabel(bool showLabel) { m_showLabel = showLabel; };
	void setLabel(std::string label) { m_label = label; };
};

/* static functions */
extern int numberOfCharWithDecimalPoint(float value,
                                        int numberOfVisibleDecimal);
