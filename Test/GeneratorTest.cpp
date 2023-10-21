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

#include "Generator.h"
#include "Utils/Math.h"

TEST(GeneratorTest, GeneratorShouldGenerateRandomValues)
{
	float prev = 0.0f;
	float next = 0.0f;

	for (int i = 0; i < 10; ++i)
	{
		next = generateFloat();
		EXPECT_FALSE(Math::eq(prev, next));
		prev = next;
	}
}
