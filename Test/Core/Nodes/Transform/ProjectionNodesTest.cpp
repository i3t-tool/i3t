#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"

using namespace Core;

TEST(OrthoProjTest, ShouldContainBeOk)
{
	auto ortho = Builder::createTransform<ETransformType::Ortho>();
	ortho->setDefaultValue("left", -10.0f);
	ortho->setDefaultValue("right", 10.0f);
	ortho->setDefaultValue("bottom", -5.0f);
	ortho->setDefaultValue("top", 5.0f);
	ortho->setDefaultValue("near", 1.0f);
	ortho->setDefaultValue("far", 100.0f);

	auto expectedMat = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 100.0f);
	auto resultMat = ortho->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

//--- Perspective -------------------------------------------------------------

TEST(PerspectiveProjTest, ShouldBeOk)
{
	auto perspective = Builder::createTransform<ETransformType::Perspective>();
	perspective->setDefaultValue("fovy", glm::radians(150.0f));
	perspective->setDefaultValue("aspect", 1.5f);
	perspective->setDefaultValue("near", 0.01f);
	perspective->setDefaultValue("far", 100.0f);

	auto expectedMat = glm::perspective(glm::radians(150.0f), 1.5f, 0.01f, 100.0f);
	auto resultMat = perspective->getData().getMat4();

	EXPECT_EQ(expectedMat, resultMat);
}

TEST(PerspectiveProjTest, GettersAndSettersShouldBeOk)
{
	auto perspective = Builder::createTransform<ETransformType::Perspective>()
	    ->as<TransformImpl<ETransformType::Perspective>>();

	float fovy   = generateFloat();
	float aspect = generateFloat();
	float nearZ  = generateFloat();
	float farZ   = generateFloat();

	perspective->setDefaultValue("fovy", fovy);
	EXPECT_EQ(fovy, perspective->getDefaultValue("fovy").getFloat());

	perspective->setDefaultValue("aspect", aspect);
	EXPECT_EQ(aspect, perspective->getDefaultValue("aspect").getFloat());

	perspective->setDefaultValue("near", nearZ);
	EXPECT_EQ(nearZ, perspective->getDefaultValue("near").getFloat());

	perspective->setDefaultValue("far", farZ);
	EXPECT_EQ(farZ, perspective->getDefaultValue("far").getFloat());

	EXPECT_EQ(glm::perspective(fovy, aspect, nearZ, farZ), perspective->getData().getMat4());
}

//--- Frustum -----------------------------------------------------------------
TEST(FrustumTest, ShouldBeOk)
{
	auto frustum = Builder::createTransform<ETransformType::Frustum>();
	frustum->setDefaultValue("left", -15.0f);
	frustum->setDefaultValue("right", 15.0f);
	frustum->setDefaultValue("bottom", -10.0f);
	frustum->setDefaultValue("top", 10.0f);
	frustum->setDefaultValue("near", 0.01f);
	frustum->setDefaultValue("far", 100.0f);

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

	frustum->setDefaultValue("left", left);
	EXPECT_EQ(left, frustum->getDefaultValue("left").getFloat());

	frustum->setDefaultValue("right", right);
	EXPECT_EQ(right, frustum->getDefaultValue("right").getFloat());

	frustum->setDefaultValue("bottom", bottom);
	EXPECT_EQ(bottom, frustum->getDefaultValue("bottom").getFloat());

	frustum->setDefaultValue("top", top);
	EXPECT_EQ(top, frustum->getDefaultValue("top").getFloat());

	frustum->setDefaultValue("near", near);
	EXPECT_EQ(near, frustum->getDefaultValue("near").getFloat());

	frustum->setDefaultValue("far", far);
	EXPECT_EQ(far, frustum->getDefaultValue("far").getFloat());

	EXPECT_EQ(glm::frustum(left, right, bottom, top, near, far), frustum->getData().getMat4());
}

//--- Look At -----------------------------------------------------------------
TEST(LookAtTest, ShouldBeOk)
{
	auto lookAt = Builder::createTransform<ETransformType::LookAt>();
	lookAt->setDefaultValue("eye", glm::vec3{ -10.0f, 5.0f, 1.0f });
	lookAt->setDefaultValue("center", glm::vec3{ 10.0f, 8.0f, -4.0f });
	lookAt->setDefaultValue("up", glm::vec3{ 0.0f, 1.0f, 0.0f });

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
	lookAt->setDefaultValue("eye", eye);
	EXPECT_EQ(eye, lookAt->getDefaultValue("eye").getVec3());

	auto center = generateVec3();
	lookAt->setDefaultValue("center", center);
	EXPECT_EQ(center, lookAt->getDefaultValue("center").getVec3());

	auto up = generateVec3();
	lookAt->setDefaultValue("up", up);
	EXPECT_EQ(up, lookAt->getDefaultValue("up").getVec3());

	EXPECT_EQ(glm::lookAt(eye, center, up), lookAt->getData().getMat4());
}
