#include "gtest/gtest.h"

#include "Generator.h"

#include "Core/Nodes/GraphManager.h"

using namespace Core;

static glm::mat4 matVal1;
static glm::mat4 matVal2;
static glm::mat4 matVal3;

Ptr<Core::Sequence> arrangeSequence()
{
	matVal1 = glm::mat4(2.0f);
	matVal2 = glm::mat4(6.0f);
	matVal3 = glm::mat4(10.0f);

  // Create seq. and matrices.
  auto seq = Core::Builder::createSequence();

  auto mat1 = Core::Builder::createTransform<Free>();
  auto mat2 = Core::Builder::createTransform<Free>();
  auto mat3 = Core::Builder::createTransform<Free>();

  mat1->setValue(matVal1);
  mat2->setValue(matVal2);
  mat3->setValue(matVal3);

  // Insert matrices into the seq.
  seq->addMatrix(std::move(mat1), 0);
  seq->addMatrix(std::move(mat2), 1);
  seq->addMatrix(std::move(mat3), 2);

  return seq;
}

TEST(SequenceTest, SequenceCanBeCreated)
{
  auto seq = arrangeSequence();

	glm::mat4 expectedMat(1.0f);
	for (auto& mat : seq->getMatrices())
  {
		expectedMat *= mat->getData().getMat4();
	}

  EXPECT_EQ(expectedMat, seq->getData().getMat4());
}

TEST(SequenceTest, MatricesCanBeMoved)
{
  auto seq = arrangeSequence();

  // Pop first matrices in the sequence.
  auto mat1 = seq->popMatrix(0);
  auto mat2 = seq->popMatrix(0);

  EXPECT_EQ(glm::mat4(10.0f), seq->getData().getMat4());

  // Add them back.
  seq->addMatrix(std::move(mat2), 1);
  seq->addMatrix(std::move(mat1), 2);

  EXPECT_EQ(glm::mat4(120.0f), seq->getData().getMat4());
}

TEST(SequenceTest, UpdateIsCalledOnMatrixValueChange)
{
	auto seq = arrangeSequence();

	auto& matrices = seq->getMatrices();
	auto firstTwoMatricesProduct = matrices[0]->getData().getMat4() * matrices[1]->getData().getMat4();
  auto mat3NewValue = glm::translate(generateVec3());

	auto& mat = seq->getMatRef(seq->getMatrices().size() - 1);
	mat->setValue(mat3NewValue);

	EXPECT_EQ(seq->getData().getMat4(), firstTwoMatricesProduct * mat3NewValue);
}

TEST(SequenceTest, InternalValueCanBeRead)
{
  auto seq = arrangeSequence();

  auto matMulMatNode = Core::Builder::createNode<ENodeType::MatrixMulMatrix>();

	auto identityMatNode = Core::Builder::createTransform<Core::Free>();
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

TEST(SequenceTest, InternalValueCanBeSet)
{
  auto seq = arrangeSequence();

  auto matNode = Core::Builder::createTransform<Core::Free>();
  matNode->setValue(generateMat4());

  auto plugResult = Core::GraphManager::plugSequenceValueInput(seq, matNode);
  EXPECT_EQ(ENodePlugResult::Ok, plugResult);

  EXPECT_EQ(matNode->getData().getMat4(), seq->getData().getMat4());
}

TEST(SequenceTest, PlugCanCreateCyclicGraph)
{
  auto seq1 = arrangeSequence();
  auto seq2 = arrangeSequence();

	GraphManager::plug(seq1, seq2, 0, 0);

	auto plugResult = GraphManager::plug(seq2, seq1, 1, 1);
	EXPECT_EQ(ENodePlugResult::Ok, plugResult);
}
