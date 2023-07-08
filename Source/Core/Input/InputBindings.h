/**
 * \author Martin Herich <hericmar@fel.cvut.cz>
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
