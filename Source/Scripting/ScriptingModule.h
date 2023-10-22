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

class ScriptingModule : public Module
{
	void onInit() override;

public:
	bool runScript(const char* luaSource);

	std::ostringstream& outputStream()
	{
		return m_oss;
	}

private:
	/**
	 * @param str with no newline
	 */
	void print(const std::string& str);

	sol::state m_Lua;
	std::ostringstream m_oss;
};
