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

#include "imgui.h"
#include "imgui_internal.h"

#include "Config.h"
#include "Core/Nodes/GraphManager.h"
#include "GUI/Elements/IWindow.h"
#include "I3T.h"

#include "DIWNE/diwne_include.h"

#include "DIWNE/Basic/BasicNode.h"

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

class Node : public DIWNE::BasicNode
{
public:
	Node(DIWNE::NodeEditor& diwne, std::string label = "Header");

	// TODO: Ideally delete this whole class <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  as, duplication could be a DIWNE feature
	//  FloatPopup mode can be moved to CoreNode
	//  LOD could be a part of DIWNE
	//  If not, maybe rename to something like I3TNode to avoid name clash with DIWNE

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
	void popupContent(DIWNE::DrawInfo& context) override;
	bool allowDragStart() const override;
};
} // namespace Workspace