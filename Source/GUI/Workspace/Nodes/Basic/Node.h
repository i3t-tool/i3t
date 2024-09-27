/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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
#include "Core/Nodes/GraphManager.h"
#include "GUI/Elements/IWindow.h"
#include "I3T.h"

#include "DIWNE/diwne_include.h"

namespace Workspace
{
enum class LevelOfDetail
{
	Full,
	SetValues,
	Label,
	LightCycle
};

/**
 * \brief Values in the popup menu for floats
 */
enum FloatPopupMode
{
	Angle, ///< angles in radians (and degs in the brackets)
	Value  ///< selected sin and cos values and numbers from 0 to 4
};

extern std::map<LevelOfDetail, std::string> LevelOfDetailName;

class Node : public DIWNE::Node
{
protected:
	std::string m_topLabel;
	std::string m_middleLabel;

public:
	Node(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const topLabel = "Header",
	     std::string const middleLabel = "Content");
	~Node();

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
	virtual void popupContent() override;
};
} // namespace Workspace