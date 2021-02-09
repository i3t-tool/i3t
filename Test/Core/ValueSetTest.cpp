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
    scale->setDataMap(g_Scale);
    auto result = scale->setValue(mat);

    EXPECT_EQ(EValueSetResult::Err_ConstraintViolation, result);
  }
}

TEST(ResetScaleNode, ResetsNodeToInitialValues)
{
  auto scale = glm::vec3(7.f, -5.f, 3.f);

  // Create non-uniform scale
  auto scaleNode = Builder::createTransform<Core::Scale>(scale, g_Scale);

  // Set free transformation node.
  scaleNode->setDataMap(g_Free);
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
