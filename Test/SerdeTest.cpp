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

	auto serdeResult = JSON::serialize(data);
	ASSERT_TRUE(serdeResult);

	auto json = serdeResult.value();
	std::cout << json << std::endl;

	TestStruct result;
	EXPECT_TRUE(JSON::deserialize(json, result));

	EXPECT_EQ(data.name, result.name);
	EXPECT_EQ(data.path, result.path);
	EXPECT_EQ(data.a, result.a);
	EXPECT_EQ(data.b, result.b);
}
