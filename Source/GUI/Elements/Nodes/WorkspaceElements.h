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
extern std::map<Core::EValueType, EColor> WorkspacePinColorBackground;

extern std::map<Core::EValueType, DIWNE::IconType> WorkspacePinShapeBackground;

extern std::map<Core::EValueType, DIWNE::IconType> WorkspacePinShapeForeground;

extern std::map<Core::EValueType, EColor> WorkspacePinColorForeground;

// endtodo

class WorkspaceNode : public DIWNE::Node
{
protected:
	std::string m_topLabel;
	std::string m_middleLabel;

public:
	WorkspaceNode(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const topLabel = "Header",
	              std::string const middleLabel = "Content");
	~WorkspaceNode();

	std::string getTopLabel()
	{
		return m_topLabel;
	}

	std::string getMiddleLabel()
	{
		return m_middleLabel;
	}

	void setTopLabel(std::string label)
	{
		m_topLabel = label;
	}

	void setMiddleLabel(std::string label)
	{
		m_middleLabel = label;
	}

	/* DIWNE function */
	virtual bool bypassFocusForInteractionAction() override;
	virtual bool beforeContent() override;
	virtual bool topContent() override;
	virtual bool middleContent() override;
	virtual bool leftContent() override;
	virtual bool rightContent() override;
	virtual bool bottomContent() override;
	virtual void deleteAction() override;

	bool m_removeFromWorkspaceWindow;
	bool getRemoveFromWorkspace() const
	{
		return m_removeFromWorkspaceWindow;
	};
	void setRemoveFromWorkspace(bool value)
	{
		m_removeFromWorkspaceWindow = value;
	};

	bool m_duplicateNode = false;
	bool getDuplicateNode() const
	{
		return m_duplicateNode;
	};
	void setDuplicateNode(bool value)
	{
		m_duplicateNode = value;
	};

	virtual void drawMenuDelete();
	virtual void popupContent();
};

class WorkspacePin : public DIWNE::Pin
{};

/* static functions */
extern int numberOfCharWithDecimalPoint(float value, int numberOfVisibleDecimal);
