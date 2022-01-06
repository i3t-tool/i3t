#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

TEST(OrthoProjTest, ShouldContainBeOk)
{
	auto ortho = Builder::createTransform<ETransformType::Ortho>(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 100.0f);

	auto expectedMat = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 100.0f);
	auto resultMat = ortho->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

//--- Perspective -------------------------------------------------------------

TEST(PerspectiveProjTest, ShouldBeOk)
{
	auto perspective = Builder::createTransform<ETransformType::Perspective>(glm::radians(150.0f), 1.5f, 0.01f, 100.0f);

	auto expectedMat = glm::perspective(glm::radians(150.0f), 1.5f, 0.01f, 100.0f);
	auto resultMat = perspective->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(PerspectiveProjTest, GettersAndSettersShouldBeOk)
{
	auto perspective = Builder::createTransform<ETransformType::Perspective>()
	    ->as<TransformImpl<ETransformType::Perspective>>();

	float FOW = generateFloat();
	float aspect = generateFloat();
	float nearZ = generateFloat();
	float farZ = generateFloat();

	perspective->setFOW(FOW);
	EXPECT_EQ(FOW, perspective->getFOW());

	perspective->setAspect(aspect);
	EXPECT_EQ(aspect, perspective->getAspect());

	perspective->setZNear(nearZ);
	EXPECT_EQ(nearZ, perspective->getZNear());

	perspective->setZFar(farZ);
	EXPECT_EQ(farZ, perspective->getZFar());

	EXPECT_EQ(glm::perspective(FOW, aspect, nearZ, farZ), perspective->getData().getMat4());
}

//--- Frustum -----------------------------------------------------------------
TEST(FrustumTest, ShouldBeOk)
{
	auto frustum = Builder::createTransform<ETransformType::Frustum>(-15.0f, 15.0f, -10.0f, 10.0f, 0.01f, 100.0f);

	auto expectedMat = glm::frustum(-15.0f, 15.0f, -10.0f, 10.0f, 0.01f, 100.0f);
	auto resultMat = frustum->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(FrustumTest, GettersAndSettersShouldBeOk)
{
	auto frustum = Builder::createTransform<ETransformType::Frustum>()
	    ->as<TransformImpl<ETransformType::Frustum>>();

	float left = generateFloat();
	float right = generateFloat();
	float bottom = generateFloat();
	float top = generateFloat();
	float near = generateFloat();
	float far = generateFloat();

	frustum->setLeft(left);
	EXPECT_EQ(left, frustum->getLeft());

	frustum->setRight(right);
	EXPECT_EQ(right, frustum->getRight());

	frustum->setBottom(bottom);
	EXPECT_EQ(bottom, frustum->getBottom());

	frustum->setTop(top);
	EXPECT_EQ(top, frustum->getTop());

	frustum->setNear(near);
	EXPECT_EQ(near, frustum->getNear());

	frustum->setFar(far);
	EXPECT_EQ(far, frustum->getFar());

	EXPECT_EQ(glm::frustum(left, right, bottom, top, near, far), frustum->getData().getMat4());
}

//--- Look At -----------------------------------------------------------------
TEST(LookAtTest, ShouldBeOk)
{
	auto lookAt = Builder::createTransform<ETransformType::LookAt>(glm::vec3{-10.0f, 5.0f, 1.0f}, glm::vec3{10.0f, 8.0f, -4.0f},
	                                               glm::vec3{0.0f, 1.0f, 0.0f});

	auto expectedMat =
			glm::lookAt(glm::vec3{-10.0f, 5.0f, 1.0f}, glm::vec3{10.0f, 8.0f, -4.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
	auto resultMat = lookAt->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(LookAtTest, GettersAndSettersShouldBeOk)
{
	auto lookAt = Builder::createTransform<ETransformType::LookAt>()
	    ->as<TransformImpl<ETransformType::LookAt>>();

	auto eye = generateVec3();
	lookAt->setEye(eye);
	EXPECT_EQ(eye, lookAt->getEye());

	auto center = generateVec3();
	lookAt->setCenter(center);
	EXPECT_EQ(center, lookAt->getCenter());

	auto up = generateVec3();
	lookAt->setUp(up);
	EXPECT_EQ(up, lookAt->getUp());

	EXPECT_EQ(glm::lookAt(eye, center, up), lookAt->getData().getMat4());
}
