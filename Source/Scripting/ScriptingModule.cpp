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
#include "Scripting/ScriptingModule.h"

#include <functional>
#include <string_view>

#include "Commands/ApplicationCommands.h"
#include "Core/Defs.h"
#include "Core/Nodes/NodeData.h"
#include "Core/Nodes/Operations.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Scripting/Environment.h"
#include "Utils/Format.h"
#include "Utils/Variant.h"

std::function<void(const std::string& str)> g_printRef;

void print(const std::string& str)
{
	g_printRef(str);
}

//----------------------------------------------------------------------------//

void ScriptingModule::onInit()
{
	// Setup I3T commands
	ConsoleCommand::addListener([this](std::string c) {
		runScript(c.c_str());
	});

	// Setup Lua

	g_printRef = [this](const std::string& str) {
		print(str);
	};

	//

	m_Lua.open_libraries(sol::lib::base, sol::lib::package);

	m_Lua["I3T"] = m_Lua.create_table();

	//

	// clang-format off
	m_Lua.new_usertype<ImVec2>(
	    "Vec2", sol::constructors<ImVec2(), ImVec2(float, float)>(),
	    "x", &ImVec2::x,
	    "y", &ImVec2::y,
	    sol::meta_function::to_string, [](const ImVec2& vec) {
		    return std::to_string(vec.x) + "\t" + std::to_string(vec.y);
	    });

	m_Lua.new_usertype<glm::vec3>(
	    "Vec3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
	    "x", &glm::vec3::x,
	    "y", &glm::vec3::y,
	    "z", &glm::vec3::z,
	    sol::meta_function::to_string, [](const glm::vec3 self) {
		    return Utils::toString(self);
		});

	m_Lua.new_usertype<glm::vec4>(
	    "Vec4", sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
	    "x", &glm::vec4::x,
	    "y", &glm::vec4::y,
	    "z", &glm::vec4::z,
	    "w", &glm::vec4::w,
	    sol::meta_function::to_string, [](const glm::vec4 self) {
			return Utils::toString(self);
		});

	m_Lua.new_usertype<glm::mat4>(
	    "Mat4", sol::constructors<glm::mat4, glm::mat4(float), glm::mat4(glm::vec4, glm::vec4, glm::vec4, glm::vec4)>(),
	    "x", [](const glm::mat4 self) {
		    return self[0];
	    },
	    "y", [](const glm::mat4 self) {
		    return self[1];
	    },
	    "z", [](const glm::mat4 self) {
		    return self[2];
	    },
	    "w", [](const glm::mat4 self) {
		    return self[3];
	    },
	    sol::meta_function::to_string, [](const glm::mat4 self) {
		    return Utils::toString(self, true);
	    });

	//

    /*
	m_Lua.set_function("eval", [this](const std::string& scriptSource) {
		runScript(scriptSource.c_str());
	});

	m_Lua.set_function("eval_from_file", [this](const std::string& path) {
		if (!fs::exists(fs::path(path)))
		{
			print("file does not exist");
			return false;
		}
		return true;
	});
     */

	auto api = m_Lua["I3T"];

	api["save_script"] = [](const std::string& path) {
		/// \todo
	};

	//

	m_Lua.set_function("help", [this]() {
		print("This is an I3T scripting module running on Lua "
		      "(sol " SOL_VERSION_STRING ").");
	});

	//

	m_Lua.set_function("_AppendToBuffer", [this](const std::string& str) {
		print(str);
	});

	m_Lua.script(R"(
		local _Print = print
		print = function(...)
			local args = {...}
			if #args == 0 then
				_AppendToBuffer("nil")
				return
			end

			local printResult = ""
			for i,v in ipairs(args) do
				printResult = printResult .. tostring(v) .. "\t"
			end
			_AppendToBuffer(printResult)
		end
)");

	m_Lua.set_function("load_scene", [](const std::string& path) {
		/// \todo
	});

	m_Lua.set_function("save_scene", [](const std::string& path) {
		/// \todo
	});
	// clang-format on

	//-- Timers --------------------------------------------------------------------------------------------------------

	m_Lua.set_function("set_timer", [this](double intervalSeconds, sol::protected_function callback) {
		callback.set_error_handler(m_Lua["print"]);

		return m_chronos.setTimer(intervalSeconds, callback);
	});

	m_Lua.set_function("clear_timer", [this](Ptr<Timer> timer) {
		if (timer)
		{
			m_chronos.clearTimer(timer);
		}
	});

	//------------------------------------------------------------------------------------------------------------------

	Registry::registerAll(m_Lua);
}

void ScriptingModule::onUpdate(double deltaSeconds)
{
	m_chronos.update(deltaSeconds);
}

void ScriptingModule::onClose()
{
	// Destroy timers before the workspace, because of weird DINWE nodes ownership issues.
	m_chronos = {};
}

bool ScriptingModule::runScript(const char* luaSource)
{
	// static int counter = 0;

	try
	{
		// auto chunkName = fmt::format("script_{}", counter++);
		// m_Lua.safe_script(luaSource, chunkName);
		m_Lua.safe_script(luaSource);
	}
	catch (const std::exception& e)
	{
		print(e.what());
		return false;
	}

	return true;
}

void ScriptingModule::print(const std::string& str)
{
	LOG_INFO(str);
	m_oss << str << "\n";
}
