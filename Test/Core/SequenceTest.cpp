#include "gtest/gtest.h"

#include "Core/NodeBuilder.h"

TEST(CreateSequence, SequenceCanBeCreated)
{
  auto seq = Builder::createSequence();

  auto mat1 = Builder::createNode<ENodeType::Matrix>();
  auto mat2 = Builder::createNode<ENodeType::Matrix>();
  auto mat3 = Builder::createNode<ENodeType::Matrix>();

  mat1->getInternalData().setValue(glm::mat4(2.0f));
  mat2->getInternalData().setValue(glm::mat4(6.0f));
  mat3->getInternalData().setValue(glm::mat4(10.0f));

  seq->addMatrix(std::move(mat1), 0);
  seq->addMatrix(std::move(mat2), 1);
  seq->addMatrix(std::move(mat3), 2);

  seq->updateValues(0);

  EXPECT_EQ(glm::mat4(120.0f), seq->getInternalData().getMat4());
}
