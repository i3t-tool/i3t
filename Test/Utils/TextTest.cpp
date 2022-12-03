#include "Utils/Text.h"
#include "gtest/gtest.h"

TEST(TextTest, Hash3)
{
	size_t a = hash_string("hey", "there", "test");
	size_t b = hash_string("hey", "there", "test");
	ASSERT_EQ(a, b);

	b = hash_string("hey", "test", "there");
	size_t c = hash_string("test", "there", "hey");
	ASSERT_EQ(b, c);
	ASSERT_EQ(a, b);

	a = hash_string("hey", "there", "test");
	b = hash_string("ayey", "there", "test");
	ASSERT_NE(a, b);
	b = hash_string("hey", "therees", "test");
	ASSERT_NE(a, b);
	b = hash_string("hey", "there", "teest");
	ASSERT_NE(a, b);

	a = hash_string("", "", "");
	b = hash_string("", "", "");
	ASSERT_EQ(a, b);

	a = hash_string(" ", "", "");
	b = hash_string(" ", "", "");
	ASSERT_EQ(a, b);

	a = hash_string(" ", "", "");
	b = hash_string(" ", "", " ");
	ASSERT_NE(a, b);

	a = hash_string(" ", "e", " ");
	b = hash_string(" ", "", " ");
	ASSERT_NE(a, b);

	a = hash_string("a", "", "");
	b = hash_string("a", "", "");
	ASSERT_EQ(a, b);

	a = hash_string("a", "", "");
	b = hash_string("a", "b", "");
	ASSERT_NE(a, b);

	a = hash_string("a", "b", "");
	b = hash_string("a", "b", "");
	ASSERT_EQ(a, b);

	a = hash_string("a", std::string(), "");
	b = hash_string("a", "", std::string());
	ASSERT_EQ(a, b);

	a = hash_string("a", std::string(), std::string());
	b = hash_string("a", "b", "");
	ASSERT_NE(a, b);

	a = hash_string("a", "b", std::string());
	b = hash_string("a", "b", std::string());
	ASSERT_EQ(a, b);

	ASSERT_EQ(hash_string("hey", "there", "test"), hash_string("hey", "there", "test"));
	ASSERT_EQ(hash_string("hey", "there", "test"), hash_string("hey", "there", "test"));
	ASSERT_EQ(hash_string("hey", "there", "test"), hash_string("hey", "there", "test"));
}

TEST(TextTest, Hash2)
{
	size_t a = hash_string("hey", "there");
	size_t b = hash_string("hey", "there");
	ASSERT_EQ(a, b);

	a = hash_string("hey", "there");
	b = hash_string("there", "hey");
	ASSERT_EQ(a, b);

	a = hash_string("hey", "there");
	b = hash_string("ayey", "there");
	ASSERT_NE(a, b);

	a = hash_string("", "");
	b = hash_string("", "");
	ASSERT_EQ(a, b);

	a = hash_string(" ", "");
	b = hash_string(" ", "");
	ASSERT_EQ(a, b);

	a = hash_string(" ", "");
	b = hash_string("", " ");
	ASSERT_EQ(a, b);

	a = hash_string(" ", "");
	b = hash_string(" ", " ");
	ASSERT_NE(a, b);

	a = hash_string(" ", "e");
	b = hash_string(" ", "");
	ASSERT_NE(a, b);

	a = hash_string("a", "");
	b = hash_string("a", "");
	ASSERT_EQ(a, b);

	a = hash_string("a", "", "");
	b = hash_string("a", "b", "");
	ASSERT_NE(a, b);

	a = hash_string("a", "b", "");
	b = hash_string("a", "b", "");
	ASSERT_EQ(a, b);

	a = hash_string("a", std::string());
	b = hash_string("a", "");
	ASSERT_EQ(a, b);
}

TEST(TextTest, Hash1_2)
{
	size_t a = hash_string("hey");
	size_t b = hash_string("hey", "");
	ASSERT_EQ(a, b);

	a = hash_string("hey");
	b = hash_string("hey", std::string());
	ASSERT_EQ(a, b);
}

TEST(TextTest, Hash2_3)
{
	size_t a = hash_string("hey", "man");
	size_t b = hash_string("hey", "", "man");
	size_t c = hash_string("man", "hey", "");
	size_t d = hash_string("man", "hey");
	ASSERT_EQ(a, b);
	ASSERT_EQ(c, a);
	ASSERT_EQ(a, d);

	a = hash_string("hey", "mum");
	b = hash_string("hey", " ", "man");
	c = hash_string("man", "hey", "e");
	d = hash_string("man", "hey");
	ASSERT_NE(a, b);
	ASSERT_NE(a, c);
	ASSERT_NE(a, d);
	ASSERT_NE(b, c);
	ASSERT_NE(b, d);
}