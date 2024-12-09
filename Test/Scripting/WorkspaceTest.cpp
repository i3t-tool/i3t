/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "gtest/gtest.h"

#include "Core/Application.h"
#include "GUI/Workspace/Nodes/ScriptingNode.h"
#include "Scripting/ScriptingModule.h"

#include "I3TUtil.h"

using WorkspaceScriptingTest = ApplicationTest;

TEST_F(WorkspaceScriptingTest, NodeData)
{
	using namespace std::string_literals;

	auto& scripting = App::getModule<ScriptingModule>();

	auto result = scripting.runScript(R"(
	   local nodes_count = #I3T.get_all_nodes()
	   assert(nodes_count == 0)
   )");
	EXPECT_TRUE(result);

	result = scripting.runScript(R"(
	   local floatNode = Operator.new("FloatToFloat")
	   local transformNode = Transform.new("Scale")

	   local nodes_count = #I3T.get_all_nodes()
	   assert(nodes_count == 2)
   )");
	EXPECT_TRUE(result);
}

TEST_F(WorkspaceScriptingTest, FoltaScenes)
{
	using namespace std::string_literals;

	auto& scripting = App::getModule<ScriptingModule>();

	for (const auto& file : fs::directory_iterator("Data/Scripts"))
	{
		if (file.path().extension() != ".lua")
		{
			continue;
		}

		auto posixPath = file.path().string();
		std::replace(posixPath.begin(), posixPath.end(), '\\', '/');

		auto path = file.path().string();
		std::string script = "I3T.workspace.clear()\n" + fmt::format("I3T.load_script_from(\"{}\")", posixPath);

		auto result = scripting.runScript(script.c_str());
		EXPECT_TRUE(result);
	}
}

TEST_F(WorkspaceScriptingTest, Sequence)
{
	auto& scripting = App::getModule<ScriptingModule>();

	auto result = scripting.runScript(R"(
		local vector = Vec3.new(1, 2, 3)
		do
			local sequence = Sequence.new()
			transform = Transform.new("Scale")
			sequence:push(transform)

			transform:set_default_value("scale", vector)
		end

		local sequence = I3T.get_all_nodes()[1]:as_sequence()
		assert(sequence.push)

		assert(sequence:get_mat4(1) ~= nil)
		assert(sequence:get_mat4(2) ~= nil)
		assert(transform:get_mat4() ~= nil)

		assert(Math.equals(sequence:get_mat4(1), transform:get_mat4()))
		assert(Math.equals(sequence:get_mat4(2), transform:get_mat4()))
		)");
	EXPECT_TRUE(result);
}

TEST_F(WorkspaceScriptingTest, ScriptingNode_EmptyOperation)
{
	auto& scripting = App::getModule<ScriptingModule>();

	auto script = R"(
		self.operation = Operation.new(
			{},
			{},
			{},
			{}
		)
		self.on_init = function() print(1) end
		self.on_update_values = function() print(2) end
	)";

	auto nextId = Core::IdGenerator::next();
	auto result = Workspace::createScript(nextId, script);
	Workspace::removeScript(nextId);

	EXPECT_TRUE(result.has_value());
}

TEST_F(WorkspaceScriptingTest, ScriptingNode_NonEmptyOperation)
{
	auto& scripting = App::getModule<ScriptingModule>();

	auto script = R"(
		self.operation = Operation.new(
			{ValueType.Float, ValueType.Float},
			{ValueType.Vec3},
			{"x", "y"},
			{"result"}
		)
		self.on_init = function() print(1) end
		self.on_update_values = function() print(2) end
	)";

	auto nextId = Core::IdGenerator::next();
	auto result = Workspace::createScript(nextId, script);
	Workspace::removeScript(nextId);

	EXPECT_TRUE(result.has_value());
}

TEST_F(WorkspaceScriptingTest, ScriptingNode_ComplexExample)
{
	auto& scripting = App::getModule<ScriptingModule>();

	auto script = R"(
		self.operation = Operation.new(
			{ValueType.Float, ValueType.Float},
			{ValueType.Vec3},
			{"x", "y"},
			{"result"}
		)
		self.on_init = function() print(1) end
		self.on_update_values = function() print(2) end
	)";

	auto nextId = Core::IdGenerator::next();
	auto result = Workspace::createScript(nextId, script);
	Workspace::removeScript(nextId);

	EXPECT_TRUE(result.has_value());
}
