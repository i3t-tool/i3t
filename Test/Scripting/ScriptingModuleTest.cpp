/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "Scripting/ScriptingModule.h"

TEST(ScriptingModuleTest, FalseOnFail)
{
	const auto scripting = std::make_unique<ScriptingModule>();
	((Module*) scripting.get())->onInit();

	auto maybeErr = scripting->runScript("assert(false)");
	EXPECT_TRUE(maybeErr);
}

TEST(ScriptingModuleTest, VariablesArePerservedBetweenScripts)
{
	const auto scripting = std::make_unique<ScriptingModule>();
	((Module*) scripting.get())->onInit();

	auto maybeErr = scripting->runScript(R"(
		number = 24
		number2 = 24.5
		important_string = "woof woof"
		a_function = function () return 100 end
		some_table = { value = 24 }
	)");
	EXPECT_FALSE(maybeErr);

	maybeErr = scripting->runScript(R"(
		t = {
			number = 24,
			number2 = 24.5,
			important_string = "woof woof",
			a_function = function () return 100 end,
			some_table = { value = 24 }
		}
	)");
	EXPECT_FALSE(maybeErr);

	maybeErr = scripting->runScript(R"(
		assert(t.number == number)
		assert(t.number2 == number2)
		assert(t.important_string == important_string)
		assert(t.a_function() == a_function())
		assert(t.some_table.value == some_table.value)
	)");
	EXPECT_FALSE(maybeErr);
}

TEST(ScriptingModuleTest, MathFunctions)
{
	const auto scripting = std::make_unique<ScriptingModule>();
	((Module*) scripting.get())->onInit();

	auto maybeErr = scripting->runScript(R"(
		assert(Math.equals(1.0002, 1.0001))
		assert(Math.equals(Vec3.new(1, 1, 1), Vec3.new(1, 1, 1)))
	)");
	EXPECT_FALSE(maybeErr);
}
