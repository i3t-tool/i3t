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

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "Utils/JSON.h"

TEST(JSON, MergeObject)
{
	using namespace rapidjson;

	Document d1, d2(&d1.GetAllocator());
	d1.Parse(R"({"key1":"value1","key2":"value2"})");
	d2.Parse(R"({"key3":"value3"})");

	JSON::merge(d1, d2, d1.GetAllocator());

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	d1.Accept(writer);

	EXPECT_STREQ("{\"key1\":\"value1\",\"key2\":\"value2\",\"key3\":\"value3\"}", sb.GetString());
}

// TODO: (DR) We should probably add a couple more tests for stuff like RTTR glm etc.