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