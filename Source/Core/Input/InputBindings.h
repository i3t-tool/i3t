#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

#include "KeyCodes.h"

struct InputBindings final
{
  using ActionsMap = std::unordered_map<
      std::string_view,
      std::vector<Keys::Code>
  >;

  using AxisMap = std::unordered_map<
	    std::string_view,
			std::vector<std::pair<Keys::Code, float>>
  >;

  friend class InputManager;

	static void init();

	static const std::vector<Keys::Code>& getActionKeys(const char* name);
	static bool isActionCreated(const char* name);
  static void setActionKey(const char* name, Keys::Code code);
	static void removeActionKey(const char* name, Keys::Code code);

	static std::vector<std::pair<Keys::Code, float>> getAxisMappings(const char* name);
  static bool isAxisCreated(const char* name);
	static void removeAxisKey(const char* name, Keys::Code code);

private:
  static ActionsMap m_inputActions;
	static AxisMap m_inputAxis;
};
