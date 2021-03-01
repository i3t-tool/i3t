#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

Ptr<Core::Sequence> arrangeSequence()
{
  // Create seq. and matrices.
  auto seq = Builder::createSequence();

  auto mat1 = Builder::createNode<ENodeType::Matrix>();
  auto mat2 = Builder::createNode<ENodeType::Matrix>();
  auto mat3 = Builder::createNode<ENodeType::Matrix>();

  mat1->setValue(glm::mat4(2.0f));
  mat2->setValue(glm::mat4(6.0f));
  mat3->setValue(glm::mat4(10.0f));

  // Insert matrices into the seq.
  seq->addMatrix(std::move(mat1), 0);
  seq->addMatrix(std::move(mat2), 1);
  seq->addMatrix(std::move(mat3), 2);

  return seq;
}

TEST(CreateSequence, SequenceCanBeCreated)
{
  auto seq = arrangeSequence();

  seq->updateValues(0);

  EXPECT_EQ(glm::mat4(120.0f), seq->getData().getMat4());
}

TEST(MatrixManipulation, MatricesCanBeMoved)
{
  auto seq = arrangeSequence();

  // Pop first matrices in the sequence.
  auto mat1 = seq->popMatrix(0);
  auto mat2 = seq->popMatrix(0);
  seq->updateValues(0);

  EXPECT_EQ(glm::mat4(10.0f), seq->getData().getMat4());

  // Add them back.
  seq->addMatrix(std::move(mat2), 1);
  seq->addMatrix(std::move(mat1), 2);
  seq->updateValues(0);

  EXPECT_EQ(glm::mat4(120.0f), seq->getData().getMat4());
}
