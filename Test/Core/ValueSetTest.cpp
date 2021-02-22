#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Transform.h"

using namespace Core;

TEST(SetWrongMatToScaleNode, ActionShouldNotBePermitted)
{
  // Create uniform scale.
  auto scale = Builder::createTransform<Core::Scale>();

  {
    // Set new uniform scale.
    auto result = scale->setValue(glm::scale(glm::vec3(3.0f, 3.0f, 3.0f)));

    EXPECT_EQ(EValueSetResult::Ok, result);
  }
  {
    // Set new non-uniform scale.
    auto result = scale->setValue(glm::scale(glm::vec3(1.0f, 5.0f, 3.0f)));

    EXPECT_EQ(EValueSetResult::Err_ConstraintViolation, result);
  }
  {
    // Create free transform.
    glm::mat4 mat(1.0f);
    mat[1][3] = 165.0f;

    // Set non uniform scale constraint.
    scale->setDataMap(Transform::g_Scale);
    auto result = scale->setValue(mat);

    EXPECT_EQ(EValueSetResult::Err_ConstraintViolation, result);
  }
}

TEST(ResetScaleNode, ResetsNodeToInitialValues)
{
  auto scale = glm::vec3(7.f, -5.f, 3.f);

  // Create non-uniform scale
  auto scaleNode = Builder::createTransform<Core::Scale>(scale, Transform::g_Scale);

  // Set free transformation node.
  scaleNode->setDataMap(Transform::g_Free);
  glm::mat4 mat(1.0f);
  mat[1][3] = 165.0f;

  auto result = scaleNode->setValue(mat);
  EXPECT_EQ(EValueSetResult::Ok, result);

  {
    auto data = scaleNode->getInternalData().getMat4();

    EXPECT_TRUE(Math::eq(data, mat));
  }
  {
    // Reset to initial values and state.
    scaleNode->reset();
    auto data = scaleNode->getInternalData().getMat4();

    auto tmp = glm::scale(scale);

    EXPECT_TRUE(Math::eq(data, tmp));
  }
}

TEST(UniformScaleSynergies, OneValueSetShouldFollowUniformScaleSynergies)
{
  auto scale = glm::vec3(-2.0f);
  auto scaleMat = glm::scale(scale);

  auto scaleNode = Builder::createTransform<Core::Scale>(scale, Transform::g_UniformScale);

  {
    // Invalid coordinates.
    auto result = scaleNode->setValue(-2.0f, {3, 1});
    EXPECT_EQ(EValueSetResult::Err_ConstraintViolation, result);
  }
  {
    // Valid coordinates.
    auto result = scaleNode->setValue(-2.0f, {1, 1});
    EXPECT_EQ(EValueSetResult::Ok, result);

    auto data = scaleNode->getData().getMat4();

    EXPECT_EQ(data, scaleMat);
  }
}

TEST(TranslationCoordsValidation, ShouldBeValid)
{
  auto translationNode = Builder::createTransform<Core::Translation>();

  {
    // Invalid coordinates.
    auto result = translationNode->setValue(-2.0f, {0, 3});
    EXPECT_EQ(EValueSetResult::Err_ConstraintViolation, result);
  }
  {
    // Valid coordinates.
    auto result = translationNode->setValue(-2.0f, {3, 0});
    EXPECT_EQ(EValueSetResult::Ok, result);

    result = translationNode->setValue(-2.0f, {3, 1});
    EXPECT_EQ(EValueSetResult::Ok, result);

    result = translationNode->setValue(-2.0f, {3, 2});
    EXPECT_EQ(EValueSetResult::Ok, result);

    auto translMat = glm::translate(glm::vec3(-2.0f, -2.0f, -2.0f));
    auto data = translationNode->getData().getMat4();

    EXPECT_EQ(translMat, data);
  }
}

TEST(EulerXOneValueSet, ShouldBeCorrect)
{
  auto rotXNode = Builder::createTransform<EulerRotX>();

  {
    // mat[1][1], cos(T)
    auto rads = 0.5f;

    auto result = rotXNode->setValue(glm::cos(rads), {1, 1});
    EXPECT_EQ(EValueSetResult::Ok, result);

    auto mat = rotXNode->getData().getMat4();
    auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(Math::eq(expectedMat, mat));
  }
  {
    // mat[1][2], sin(T)
    auto rads = 0.4f;

    auto result = rotXNode->setValue(glm::sin(rads), {1, 2});
    EXPECT_EQ(EValueSetResult::Ok, result);

    auto mat = rotXNode->getData().getMat4();
    auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(Math::eq(expectedMat, mat));
  }
  {
    // mat[2][1], -sin(T)
    auto rads = 0.8f;

    auto result = rotXNode->setValue(-glm::sin(rads), {2, 1});
    EXPECT_EQ(EValueSetResult::Ok, result);

    auto mat = rotXNode->getData().getMat4();
    auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(Math::eq(expectedMat, mat));
  }
  {
    // mat[2][2], cos(T)
    auto rads = 1.0f;

    auto result = rotXNode->setValue(glm::cos(rads), {2, 2});
    EXPECT_EQ(EValueSetResult::Ok, result);

    auto mat = rotXNode->getData().getMat4();
    auto expectedMat = glm::rotate(rads, glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(Math::eq(expectedMat, mat));
  }
}
