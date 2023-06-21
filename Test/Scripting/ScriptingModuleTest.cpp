#include "gtest/gtest.h"

#include "Scripting/ScriptingModule.h"

TEST(ScriptingModuleTest, VariablesArePerservedBetweenScripts)
{
	const auto scripting = std::make_unique<ScriptingModule>();
	((Module*) scripting.get())->init();

	scripting->runScript(R"(
		number = 24
		number2 = 24.5
		important_string = "woof woof"
		a_function = function () return 100 end
		some_table = { value = 24 }
	)");

	scripting->runScript(R"(
		t = {
			number = 24,
			number2 = 24.5,
			important_string = "woof woof",
			a_function = function () return 100 end,
			some_table = { value = 24 }
		}
	)");

	scripting->runScript(R"(
		assert(t.number == number)
		assert(t.number2 == number2)
		assert(t.important_string == important_string)
		assert(t.a_function() == a_function())
		assert(t.some_table.value == some_table.value)
	)");
}
