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

#include <filesystem>

#include "rttr/registration.h"

#include "Utils/JSON.h"

struct TestStruct
{
	std::string name;
	std::filesystem::path path;
	int a;
	float b;
};

RTTR_REGISTRATION
{
	rttr::registration::class_<TestStruct>("test")
	    .property("name", &TestStruct::name)
	    .property("path", &TestStruct::path)
	    .property("a", &TestStruct::a)
	    .property("b", &TestStruct::b);
}

TEST(SerdeTest, SimpleStruct)
{
	TestStruct data{"test", "/tmp/test", 1, 1.0f};

	auto serdeResult = JSON::serializeToString(data);
	ASSERT_TRUE(serdeResult);

	auto json = serdeResult.value();
	std::cout << json << std::endl;

	TestStruct result;
	EXPECT_TRUE(JSON::deserializeString(json, result));

	EXPECT_EQ(data.name, result.name);
	EXPECT_EQ(data.path, result.path);
	EXPECT_EQ(data.a, result.a);
	EXPECT_EQ(data.b, result.b);
}
