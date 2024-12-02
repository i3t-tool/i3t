/**
 * \file
 * \brief Common defines and includes for the DIWNE library.
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 *
 * \details This file is included by the base DiwneObject header and thus
 * should be available wherever DIWNE is used.
 */
#pragma once
#define DIWNE_DEBUG_ENABLED 1

static constexpr ImVec4 DIWNE_WHITE = ImVec4(1.f, 1.f, 1.f, 1.f);
static constexpr ImVec4 DIWNE_BLACK = ImVec4(0.f, 0.f, 0.f, 1.f);
static constexpr ImVec4 DIWNE_RED = ImVec4(1.f, 0.f, 0.f, 1.f);
static constexpr ImVec4 DIWNE_RED_50 = ImVec4(1.f, 0.f, 0.f, .5f);
static constexpr ImVec4 DIWNE_MAGENTA = ImVec4(1.f, 0.f, 1.f, 1.f);
static constexpr ImVec4 DIWNE_MAGENTA_50 = ImVec4(1.f, 0.f, 1.f, .5f);
static constexpr ImVec4 DIWNE_GREEN = ImVec4(0.f, 1.f, 0.f, 1.f);
static constexpr ImVec4 DIWNE_GREEN_50 = ImVec4(0.f, 1.f, 0.f, .5f);
static constexpr ImVec4 DIWNE_BLUE = ImVec4(0.f, 0.f, 1.f, 1.f);
static constexpr ImVec4 DIWNE_YELLOW = ImVec4(1.f, 1.f, 0.f, 1.f);
static constexpr ImVec4 DIWNE_YELLOW_50 = ImVec4(1.f, 1.f, 0.f, .5f);
static constexpr ImVec4 DIWNE_CYAN = ImVec4(0.f, 1.f, 1.f, 1.f);
static constexpr ImVec4 DIWNE_CYAN_50 = ImVec4(0.f, 1.f, 1.f, 1.f);
static constexpr ImVec4 DIWNE_ORANGE = ImVec4(1.f, 0.5f, 0.f, 1.f);
static constexpr ImVec4 DIWNE_ORANGE_50 = ImVec4(1.f, 0.5f, 0.f, .5f);

#if DIWNE_DEBUG_ENABLED
#define DEBUG_TEMPLATE(editor, debugCode, debugVar)                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		if ((editor).m_diwneDebug && (editor).debugVar)                                                                \
		{                                                                                                              \
			debugCode                                                                                                  \
		}                                                                                                              \
	} while (0) // do-while to prevent issues with single line statements
#define DIWNE_DEBUG_VARS()                                                                                             \
	bool m_diwneDebug = false;                                                                                         \
	bool m_diwneDebugLayout = true;                                                                                    \
	bool m_diwneDebugExtra2 = false;                                                                                   \
	bool m_diwneDebugInteractions = false;
#define DIWNE_DEBUG(editor, debugCode) DEBUG_TEMPLATE(editor, debugCode, m_diwneDebug)
#define DIWNE_DEBUG_LAYOUT(editor, debugCode) DEBUG_TEMPLATE(editor, debugCode, m_diwneDebugLayout)
#define DIWNE_DEBUG_EXTRA_2(editor, debugCode) DEBUG_TEMPLATE(editor, debugCode, m_diwneDebugExtra2)
#define DIWNE_DEBUG_INTERACTIONS(editor, debugCode) DEBUG_TEMPLATE(editor, debugCode, m_diwneDebugInteractions)

#else
#define DIWNE_DEBUG_VARS()
#define DIWNE_DEBUG(editor, debugCode)
#define DIWNE_DEBUG_LAYOUT(editor, debugCode)
#define DIWNE_DEBUG_EXTRA_2(editor, debugCode)
#define DIWNE_DEBUG_INTERACTIONS(editor, debugCode)
#endif

#include "spdlog/fmt/fmt.h" // TODO: Probably shouldn't require fmt as a dependency hmm
#include <limits>

namespace DIWNE
{
enum DiwneAction : unsigned int
{
	None,
	FocusOnObject,
	InteractingContent, /* for other unspecified interactions */
	NewLink,

	HoldNode,
	HoldPin,
	HoldLink,
	HoldWorkarea,

	DragNode,
	DragPin,
	DragLink, /* dragging already existing/connected link */
	DragWorkarea,

	TouchNode,
	TouchPin,
	TouchLink,
	TouchWorkarea,

	SelectionRectFull,
	SelectionRectTouch
};
} // namespace DIWNE