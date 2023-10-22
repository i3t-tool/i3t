/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include "Core/Defs.h"
#include "InputManager.h"
#include "KeyCodes.h"

struct InputBindings final
{
	struct ActionMapping
	{
		Keys::Code code;
		Modifiers modifiers;
	};

	struct AxisMapping
	{
		Keys::Code code;
		float value;
		Modifiers modifiers;
	};

	using ActionsMap = std::unordered_map<std::string, std::vector<ActionMapping>>;

	using AxesMap = std::unordered_map<std::string, std::vector<AxisMapping>>;

	friend class InputManager;

	static void init();

	static const std::vector<ActionMapping>& getActionMapping(const char* name);
	static bool isActionCreated(const char* name);
	static void setAction(const char* name);
	static void setActionKey(const char* name, Keys::Code code, ModifiersList modifiers = ModifiersList());
	static void removeActionKey(const char* name, Keys::Code code);

	static std::vector<AxisMapping> getAxisMapping(const char* name);
	static bool isAxisCreated(const char* name);
	static void setAxis(const char* name);
	static void setAxisKey(const char* name, float scale, Keys::Code code, ModifiersList modifiers = ModifiersList());
	static void removeAxisKey(const char* name, Keys::Code code);

private:
	static ActionsMap m_inputActions;
	static AxesMap m_inputAxis;
};
