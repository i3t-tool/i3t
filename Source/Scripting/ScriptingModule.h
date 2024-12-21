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

#include <sstream>

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

#include "Core/Module.h"
#include "Scripting/Timer.h"

struct ScriptError
{
	enum Type
	{
		SyntaxError,
		RuntimeError,
	};

	/// Line number from
	std::optional<int> line;
	std::optional<Type> type;
	std::string message;

	explicit ScriptError(std::string message) : message(std::move(message)) {}
	ScriptError(int line, Type type, std::string message) : line(line), type(type), message(std::move(message)) {}
};

class ScriptingModule : public Module
{
	void onInit() override;
	void onUpdate(double deltaSeconds) override;
	void onClose() override;

public:
	std::optional<ScriptError> runScript(const char* luaSource);

	std::ostringstream& outputStream()
	{
		return m_oss;
	}

	sol::state& environment()
	{
		return m_Lua;
	}

private:
	/**
	 * @param str with no newline
	 */
	void print(const std::string& str);

	/// Has to be destructed last!
	sol::state m_Lua;
	Chronos m_chronos;
	std::ostringstream m_oss;
};
