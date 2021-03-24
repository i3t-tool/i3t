#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(EulerXOneValueSet, ShouldBeCorrect)
{
	auto rotXNode = Builder::createTransform<EulerRotX>();

	{
		// mat[1][1], cos(T)
		auto rads = 0.5f;

		auto result = rotXNode->setValue(glm::cos(rads), {1, 1});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[1][2], sin(T)
		auto rads = 0.4f;

		auto result = rotXNode->setValue(glm::sin(rads), {1, 2});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[2][1], -sin(T)
		auto rads = 0.8f;

		auto result = rotXNode->setValue(-glm::sin(rads), {2, 1});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[2][2], cos(T)
		auto rads = 1.0f;

		auto result = rotXNode->setValue(glm::cos(rads), {2, 2});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotXNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
}

TEST(EulerXAngleSet, ShouldBeCorrect)
{
	std::array<NodePtr, 3> rots = {Builder::createTransform<EulerRotX>(), Builder::createTransform<EulerRotY>(),
	                               Builder::createTransform<EulerRotZ>()};
	auto angle = glm::radians(85.0f);
	std::array<glm::mat4, 3> expectedMatrices = {
			glm::rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f)),
			glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)),
			glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)),
	};

	for (int i = 0; i < 3; ++i)
	{
		auto result = rots[i]->setValue(angle);
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);
		EXPECT_EQ(expectedMatrices[i], rots[i]->getData().getMat4());
	}
}

TEST(EulerYOneValueSet, ShouldBeCorrect)
{
	auto rotYNode = Builder::createTransform<EulerRotY>();

	{
		// mat[0][0], cos(T)
		auto rads = 0.5f;

		auto val = glm::cos(rads);

		auto result = rotYNode->setValue(val, {0, 0});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotYNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 1.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[2][0], sin(T)
		auto rads = 0.4f;

		auto result = rotYNode->setValue(glm::sin(rads), {2, 0});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotYNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 1.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[0][2], -sin(T)
		auto rads = 0.8f;

		auto result = rotYNode->setValue(-glm::sin(rads), {0, 2});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotYNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 1.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[2][2], cos(T)
		auto rads = 1.0f;

		auto result = rotYNode->setValue(glm::cos(rads), {2, 2});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotYNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 1.0f, 0.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
}

TEST(EulerZOneValueSet, ShouldBeCorrect)
{
	auto rotZNode = Builder::createTransform<EulerRotZ>();

	{
		// mat[0][0], cos(T)
		auto rads = 0.5f;

		auto result = rotZNode->setValue(glm::cos(rads), {0, 0});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotZNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[0][1], sin(T)
		auto rads = 0.4f;

		auto result = rotZNode->setValue(glm::sin(rads), {0, 1});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotZNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[1][0], -sin(T)
		auto rads = 0.8f;

		auto result = rotZNode->setValue(-glm::sin(rads), {1, 0});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotZNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
	{
		// mat[1][1], cos(T)
		auto rads = 1.0f;

		auto result = rotZNode->setValue(glm::cos(rads), {1, 1});
		EXPECT_EQ(ValueSetResult::Status::Ok, result.status);

		auto mat = rotZNode->getData().getMat4();
		auto expectedMat = glm::rotate(rads, glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(expectedMat, mat));
	}
}
