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
#include "Scripting/ScriptingModule.h"

#include "I3TUtil.h"

TEST_F(ApplicationTest, WorkspaceBase)
{
	using namespace std::string_literals;

	auto& scripting = App::getModule<ScriptingModule>();

	auto result = scripting.runScript(R"(
		local nodes_count = #get_all_nodes()
		assert(nodes_count == 0)
	)");
	EXPECT_TRUE(result);

	result = scripting.runScript(R"(
		local floatNode = Operator.new("FloatToFloat")
		local transformNode = Transform.new("Scale")

		local nodes_count = #get_all_nodes()
		assert(nodes_count == 2)
	)");
	EXPECT_TRUE(result);
}
