#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

TEST(OrthoProj, ShouldBeOk)
{
	auto ortho = Builder::createTransform<OrthoProj>(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 100.0f);

	auto expectedMat = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 100.0f);
	auto resultMat = ortho->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(PerspectiveProj, ShouldBeOk)
{
	auto perspective = Builder::createTransform<PerspectiveProj>(glm::radians(150.0f), 1.5f, 0.01f, 100.0f);

	auto expectedMat = glm::perspective(glm::radians(150.0f), 1.5f, 0.01f, 100.0f);
	auto resultMat = perspective->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(Frustum, ShouldBeOk)
{
	auto frustum = Builder::createTransform<Frustum>(-15.0f, 15.0f, -10.0f, 10.0f, 0.01f, 100.0f);

	auto expectedMat = glm::frustum(-15.0f, 15.0f, -10.0f, 10.0f, 0.01f, 100.0f);
	auto resultMat = frustum->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(LookAt, ShouldBeOk)
{
	auto lookAt = Builder::createTransform<LookAt>(glm::vec3{-10.0f, 5.0f, 1.0f}, glm::vec3{10.0f, 8.0f, -4.0f},
	                                               glm::vec3{0.0f, 1.0f, 0.0f});

	auto expectedMat =
			glm::lookAt(glm::vec3{-10.0f, 5.0f, 1.0f}, glm::vec3{10.0f, 8.0f, -4.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
	auto resultMat = lookAt->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}
