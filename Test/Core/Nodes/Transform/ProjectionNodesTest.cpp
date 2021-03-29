#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

TEST(OrthoProj, ShouldBeOk)
{
	auto ortho = Builder::createTransform<OrthoProj>(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 100.0f);

	auto expectedMat = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 100.0f);
	auto resultMat = ortho->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(Ortho, GettersAndSettersShouldBeOk)
{
  auto ortho = Builder::createTransform<OrthoProj>()->as<OrthoProj>();

  float left = generateFloat();
  float right = generateFloat();
  float bottom = generateFloat();
  float top = generateFloat();
  float near = generateFloat();
  float far = generateFloat();

	ortho->setLeft(left);
  EXPECT_EQ(left, ortho->getLeft());

	ortho->setRight(right);
  EXPECT_EQ(right, ortho->getRight());

	ortho->setBottom(bottom);
  EXPECT_EQ(bottom, ortho->getBottom());

	ortho->setTop(top);
  EXPECT_EQ(top, ortho->getTop());

	ortho->setNear(near);
  EXPECT_EQ(near, ortho->getNear());

	ortho->setFar(far);
  EXPECT_EQ(far, ortho->getFar());

  EXPECT_EQ(glm::ortho(left, right, bottom, top, near, far), ortho->getData().getMat4());
}

//--- Perspective -------------------------------------------------------------
TEST(PerspectiveProj, ShouldBeOk)
{
	auto perspective = Builder::createTransform<PerspectiveProj>(glm::radians(150.0f), 1.5f, 0.01f, 100.0f);

	auto expectedMat = glm::perspective(glm::radians(150.0f), 1.5f, 0.01f, 100.0f);
	auto resultMat = perspective->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(PerspectiveProj, GettersAndSettersShouldBeOk)
{
  auto perspective = Builder::createTransform<PerspectiveProj>()->as<PerspectiveProj>();

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
TEST(Frustum, ShouldBeOk)
{
	auto frustum = Builder::createTransform<Frustum>(-15.0f, 15.0f, -10.0f, 10.0f, 0.01f, 100.0f);

	auto expectedMat = glm::frustum(-15.0f, 15.0f, -10.0f, 10.0f, 0.01f, 100.0f);
	auto resultMat = frustum->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(Frustum, GettersAndSettersShouldBeOk)
{
	auto frustum = Builder::createTransform<Frustum>()->as<Frustum>();

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
TEST(LookAt, ShouldBeOk)
{
	auto lookAt = Builder::createTransform<LookAt>(glm::vec3{-10.0f, 5.0f, 1.0f}, glm::vec3{10.0f, 8.0f, -4.0f},
	                                               glm::vec3{0.0f, 1.0f, 0.0f});

	auto expectedMat =
			glm::lookAt(glm::vec3{-10.0f, 5.0f, 1.0f}, glm::vec3{10.0f, 8.0f, -4.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
	auto resultMat = lookAt->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(LookAt, GettersAndSettersShouldBeOk)
{
	auto lookAt = Builder::createTransform<LookAt>();

	auto lookAtAsTransform = lookAt->as<LookAt>();

  auto eye = generateVec3();
  lookAtAsTransform->setEye(eye);
  EXPECT_EQ(eye, lookAtAsTransform->getEye());

  auto center = generateVec3();
  lookAtAsTransform->setCenter(center);
  EXPECT_EQ(center, lookAtAsTransform->getCenter());

  auto up = generateVec3();
  lookAtAsTransform->setUp(up);
  EXPECT_EQ(up, lookAtAsTransform->getUp());

	EXPECT_EQ(glm::lookAt(eye, center, up), lookAt->getData().getMat4());
}
