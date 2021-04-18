#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"

using namespace Core;

TEST(SequenceTest, SequenceCanContainMatrices)
{
	auto seq = arrangeSequence();

	glm::mat4 expectedMat(1.0f);
	for (auto& mat : seq->getMatrices())
	{
		expectedMat *= mat->getData().getMat4();
	}

	auto currentMat = seq->getData().getMat4();
	EXPECT_EQ(expectedMat, currentMat);
}

TEST(SequenceTest, AddMatrixToTail)
{
	auto seq = Builder::createSequence();
	auto mat = Builder::createTransform<Free>();

	seq->addMatrix(mat);
	EXPECT_EQ(1, seq->getMatrices().size());
}

TEST(SequenceTest, MatricesCanBeMoved)
{
	auto seq = arrangeSequence();

	// Pop first matrices in the sequence.
	auto mat1 = seq->popMatrix(0);
	auto mat2 = seq->popMatrix(0);
	EXPECT_FALSE(mat1->isInSequence() && mat2->isInSequence());

	{
		auto expectedSequenceValue = getMatProduct(seq->getMatrices());
		EXPECT_EQ(expectedSequenceValue, seq->getData().getMat4());
	}

	// Add them back.
	seq->addMatrix(mat2, 1);
	seq->addMatrix(mat1, 2);
	EXPECT_TRUE(mat1->isInSequence() && mat2->isInSequence());
	{
		auto expectedSequenceValue = getMatProduct(seq->getMatrices());
		EXPECT_EQ(expectedSequenceValue, seq->getData().getMat4());
	}
}

TEST(SequenceTest, MatricesCanBeSwapped)
{
  auto seq = arrangeSequence();

	auto firstMat = seq->getMatrices().front();
	auto lastMat = seq->getMatrices().back();

	seq->swap(0, 2);

	EXPECT_EQ(lastMat, seq->getMatrices().front());
	EXPECT_EQ(firstMat, seq->getMatrices().back());
}

TEST(SequenceTest, UpdateIsCalledOnMatrixValueChange)
{
	auto seq = arrangeSequence();

	auto& matrices = seq->getMatrices();
	auto firstTwoMatricesProduct = matrices[0]->getData().getMat4() * matrices[1]->getData().getMat4();
	auto mat3NewValue = glm::translate(generateVec3());

	auto& mat = seq->getMatRef(seq->getMatrices().size() - 1);
	setValue_expectOk(mat, mat3NewValue);

	EXPECT_EQ(seq->getData().getMat4(), firstTwoMatricesProduct * mat3NewValue);
}

TEST(SequenceTest, InternalValueCanBeReadByOperator)
{
	auto seq = arrangeSequence();
	auto matMulMatNode = Core::Builder::createNode<ENodeType::MatrixMulMatrix>();
	auto identityMatNode = Core::Builder::createTransform<Core::Free>();

	{
		setValue_expectOk(identityMatNode, glm::mat4(1.0f));
	}
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

TEST(SequenceTest, InternalValueCanBeSetFromOutside)
{
	auto seq = arrangeSequence();

	auto matNode = Core::Builder::createTransform<Core::Free>();
	setValue_expectOk(matNode, generateMat4());

	auto plugResult = GraphManager::plugSequenceValueInput(seq, matNode);
	EXPECT_EQ(ENodePlugResult::Ok, plugResult);

	EXPECT_EQ(matNode->getData().getMat4(), seq->getData().getMat4());
}

/**
 *    _______
 *   /       \
 * seq1 --- seq2
 */
TEST(SequenceTest, RightSequenceValueOutputCanBePluggedToParentSequenceValueInput)
{
	auto seq1 = arrangeSequence();
	auto seq2 = arrangeSequence();

	GraphManager::plug(seq1, seq2, 0, 0);

	plug_expectOk(seq2, seq1, 1, 1);

	EXPECT_EQ(seq1->getData().getMat4(), seq2->getData().getMat4());
}

TEST(SequenceTest, LeftSequenceValueOutputCanBePluggedToParentSequenceValueInput)
{
	auto seq1 = arrangeSequence();
	auto seq2 = arrangeSequence();

	GraphManager::plug(seq1, seq2, 0, 0);

	auto plugResult = GraphManager::plug(seq1, seq2, 1, 1);
	EXPECT_EQ(ENodePlugResult::Ok, plugResult);

	EXPECT_EQ(seq1->getData().getMat4(), seq2->getData().getMat4());
}

TEST(SequenceTest, ThreeSequencesComposeMatrices)
{
  auto seq1 = arrangeSequence();
  auto seq2 = arrangeSequence();
  auto seq3 = arrangeSequence();

  {
    auto expectedMat = getMatProduct(seq1->getMatrices());
    EXPECT_EQ(expectedMat, seq1->getData().getMat4());
	}
  {
    plug_expectOk(seq1, seq2, 0, 0);
		auto expectedMat = getMatProduct(seq1->getMatrices()) * getMatProduct(seq2->getMatrices());
    EXPECT_EQ(expectedMat, seq2->getData().getMat4());
	}
  {
		plug_expectOk(seq2, seq3, 0, 0);
    auto expectedMat = getMatProduct(seq1->getMatrices()) * getMatProduct(seq2->getMatrices()) * getMatProduct(seq3->getMatrices());
    EXPECT_EQ(expectedMat, seq3->getData().getMat4());
  }
}
