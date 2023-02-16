#pragma once

#include <sstream>

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

#include "Core/Module.h"

class ScriptingModule : public Module
{
	void init() override;

public:
	bool runScript(const char* luaSource);

	std::ostringstream& outputStream() { return m_oss; }

private:
	/**
	 * @param str with no newline
	 */
	void print(const std::string& str);

	sol::state m_Lua;
	std::ostringstream m_oss;
};
