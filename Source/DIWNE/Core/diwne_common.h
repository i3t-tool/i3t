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
static constexpr ImVec4 DIWNE_MAGENTA = ImVec4(1.f, 0.f, 1.f, 1.f);
static constexpr ImVec4 DIWNE_MAGENTA_50 = ImVec4(1.f, 0.f, 1.f, .5f);
static constexpr ImVec4 DIWNE_GREEN = ImVec4(0.f, 1.f, 0.f, 1.f);
static constexpr ImVec4 DIWNE_GREEN_50 = ImVec4(0.f, 1.f, 0.f, .5f);
static constexpr ImVec4 DIWNE_BLUE = ImVec4(0.f, 0.f, 1.f, 1.f);
static constexpr ImVec4 DIWNE_YELLOW = ImVec4(1.f, 1.f, 0.f, 1.f);
static constexpr ImVec4 DIWNE_YELLOW_50 = ImVec4(1.f, 1.f, 0.f, .5f);
static constexpr ImVec4 DIWNE_TEAL = ImVec4(0.f, 1.f, 1.f, 1.f);
static constexpr ImVec4 DIWNE_ORANGE = ImVec4(1.f, 0.5f, 0.f, 1.f);

#if DIWNE_DEBUG_ENABLED
#define DIWNE_DEBUG(node_editor, debugCode)                                                                            \
	do                                                                                                                 \
	{                                                                                                                  \
		if (node_editor.m_diwneDebug)                                                                                  \
		{                                                                                                              \
			debugCode                                                                                                  \
		}                                                                                                              \
	} while (0) // do-while to prevent issues with single line statements
#define DIWNE_DEBUG_EXTRA_1(node_editor, debugCode)                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		if (node_editor.m_diwneDebug && node_editor.m_diwneDebugExtra1)                                                \
		{                                                                                                              \
			debugCode                                                                                                  \
		}                                                                                                              \
	} while (0) // do-while to prevent issues with single line statements
#define DIWNE_DEBUG_EXTRA_2(node_editor, debugCode)                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		if (node_editor.m_diwneDebug && node_editor.m_diwneDebugExtra2)                                                \
		{                                                                                                              \
			debugCode                                                                                                  \
		}                                                                                                              \
	} while (0) // do-while to prevent issues with single line statements
#else
#define DIWNE_DEBUG(node_editor, debugCode)
#define DIWNE_DEBUG_EXTRA_1(node_editor, debugCode)
#define DIWNE_DEBUG_EXTRA_2                                                                                            \
	;                                                                                                                  \
	(node_editor, debugCode)
#endif

#include "spdlog/fmt/fmt.h"
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