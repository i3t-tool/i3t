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

TEST(JSON, SaveLoadGlmVector)
{
	using namespace rapidjson;

	std::string str;

	glm::vec2 v2 = glm::vec2(3.14159, 1.00004);
	str = JSON::serializeVec2(v2);
	glm::vec2 v2_d = JSON::deserializeVec2(str);
	EXPECT_EQ(v2, v2_d);

	glm::vec3 v3 = glm::vec3(3.14159, 7.79, 0.33333);
	str = JSON::serializeVec3(v3);
	glm::vec3 v3_d = JSON::deserializeVec3(str);
	EXPECT_EQ(v3, v3_d);

	glm::vec4 v4 = glm::vec4(3.14159, 7.79, 0.33333, -5.93929);
	str = JSON::serializeVec4(v4);
	glm::vec4 v4_d = JSON::deserializeVec4(str);
	EXPECT_EQ(v4, v4_d);
}

TEST(JSON, SaveLoadGlmVectorWithErrorHandl)
{
	using namespace rapidjson;

	std::string str;

	bool error = true;

	glm::vec2 v2 = glm::vec2(3.14159, 1.00004);
	str = JSON::serializeVec2(v2);
	glm::vec2 v2_d = JSON::deserializeVec2(str, &error);
	EXPECT_EQ(v2, v2_d);
	EXPECT_FALSE(error);
	error = true;

	glm::vec3 v3 = glm::vec3(3.14159, 7.79, 0.33333);
	str = JSON::serializeVec3(v3);
	glm::vec3 v3_d = JSON::deserializeVec3(str, &error);
	EXPECT_EQ(v3, v3_d);
	EXPECT_FALSE(error);
	error = true;

	glm::vec4 v4 = glm::vec4(3.14159, 7.79, 0.33333, -5.93929);
	str = JSON::serializeVec4(v4);
	glm::vec4 v4_d = JSON::deserializeVec4(str, &error);
	EXPECT_EQ(v4, v4_d);
	EXPECT_FALSE(error);
	error = true;
}

TEST(JSON, LoadGlmVectorMalformed)
{
	using namespace rapidjson;

	std::string str;

	bool error = false;

	glm::vec2 v2 = glm::vec2(3.14159, 1.00004);
	str = JSON::serializeVec2(v2);
	str += "gibberish";
	glm::vec2 v2_d = JSON::deserializeVec2(str, &error);
	EXPECT_EQ(v2_d, glm::vec2());
	EXPECT_TRUE(error);
	error = false;

	glm::vec3 v3 = glm::vec3(3.14159, 7.79, 0.33333);
	str = JSON::serializeVec3(v3);
	str += "gibberish";
	glm::vec3 v3_d = JSON::deserializeVec3(str, &error);
	EXPECT_EQ(v3_d, glm::vec3());
	EXPECT_TRUE(error);
	error = false;

	glm::vec4 v4 = glm::vec4(3.14159, 7.79, 0.33333, -5.93929);
	str = JSON::serializeVec4(v4);
	str += "gibberish";
	glm::vec4 v4_d = JSON::deserializeVec4(str, &error);
	EXPECT_EQ(v4_d, glm::vec4());
	EXPECT_TRUE(error);
	error = false;

	v3 = glm::vec3(3.14159, 7.79, 0.33333);
	str = "3892.0aaaaaa";
	str += JSON::serializeVec3(v3);
	v3_d = JSON::deserializeVec3(str, &error);
	EXPECT_EQ(v3_d, glm::vec3());
	EXPECT_TRUE(error);
	error = false;

	str = "";
	v2_d = JSON::deserializeVec2(str, &error);
	EXPECT_EQ(v2_d, glm::vec2());
	EXPECT_TRUE(error);
	error = false;

	str = "";
	v3_d = JSON::deserializeVec3(str, &error);
	EXPECT_EQ(v3_d, glm::vec3());
	EXPECT_TRUE(error);

	str = "";
	v4_d = JSON::deserializeVec4(str, &error);
	EXPECT_EQ(v4_d, glm::vec4());
	EXPECT_TRUE(error);
	error = false;

	str = "sadassd ]";
	v2_d = JSON::deserializeVec2(str, &error);
	EXPECT_EQ(v2_d, glm::vec2());
	EXPECT_TRUE(error);
	error = false;

	str = "sadassd ]";
	v3_d = JSON::deserializeVec3(str, &error);
	EXPECT_EQ(v3_d, glm::vec3());
	EXPECT_TRUE(error);
	error = false;

	str = "sadassd ]";
	v4_d = JSON::deserializeVec4(str, &error);
	EXPECT_EQ(v4_d, glm::vec4());
	EXPECT_TRUE(error);
	error = false;
}