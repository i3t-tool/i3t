/**
 * \file LocaleTest
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "I3T.h"

#include "I3TUtil.h"

/**
 * Ensures that printf-like standard C functions format using the '.' (dot) decimal separator.
 */
TEST(LocaleTest, StandardLocaleIsUsed)
{
	auto app = initI3T();

	char buffer[100];
	int count = snprintf(buffer, 100, "%.2f", 3.6);
	ASSERT_TRUE(count == 4);
	ASSERT_TRUE(!strcmp(buffer, "3.60"));
}
