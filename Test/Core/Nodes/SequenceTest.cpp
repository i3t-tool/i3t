#include "gtest/gtest.h"

#include "Generator.h"

#include "Core/Nodes/GraphManager.h"

static glm::mat4 matVal1;
static glm::mat4 matVal2;
static glm::mat4 matVal3;

Ptr<Core::Sequence> arrangeSequence()
{
	matVal1 = glm::mat4(2.0f);
	matVal2 = glm::mat4(6.0f);
	matVal3 = glm::mat4(10.0f);

  // Create seq. and matrices.
  auto seq = Builder::createSequence();

  auto mat1 = Builder::createNode<ENodeType::Matrix>();
  auto mat2 = Builder::createNode<ENodeType::Matrix>();
  auto mat3 = Builder::createNode<ENodeType::Matrix>();

  mat1->setValue(matVal1);
  mat2->setValue(matVal2);
  mat3->setValue(matVal3);

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

	auto expectedMat = (matVal1 * matVal2) * matVal3;

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

TEST(Sequence, InternalValueCanBeRead)
{
  auto seq = arrangeSequence();

  auto matMulMatNode = Builder::createNode<ENodeType::MatrixMulMatrix>();

	auto identityMatNode = Builder::createTransform<Core::Free>();
	identityMatNode->setValue(glm::mat4(1.0f));

  {
    auto plugResult = Core::GraphManager::plugSequenceValueOutput(seq, matMulMatNode);
    EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}
  {
    auto plugResult = Core::GraphManager::plug(identityMatNode, matMulMatNode, 0, 1);
    EXPECT_EQ(ENodePlugResult::Ok, plugResult);
	}

  EXPECT_EQ(seq->getData().getMat4(), matMulMatNode->getData().getMat4());
}

TEST(Sequence, InternalValueCanBeSet)
{
  auto seq = arrangeSequence();

  auto matNode = Builder::createTransform<Core::Free>();
  matNode->setValue(generateMat4());

  auto plugResult = Core::GraphManager::plugSequenceValueInput(seq, matNode);
  EXPECT_EQ(ENodePlugResult::Ok, plugResult);

  EXPECT_EQ(matNode->getData().getMat4(), seq->getData().getMat4());
}
