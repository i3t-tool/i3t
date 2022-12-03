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

	const auto& currentMat = seq->getData().getMat4();
	EXPECT_EQ(expectedMat, currentMat);
}

TEST(SequenceTest, AddMatrixToTail)
{
	auto seq = Builder::createSequence();
	auto mat = Builder::createTransform<ETransformType::Free>();

	seq->addMatrix(mat);
	EXPECT_EQ(1, seq->getMatrices().size());
}

TEST(SequenceTest, MatricesCanBeMoved)
{
	auto seq = arrangeSequence();

	// Pop first matrices in the sequence.
	const auto mat1 = seq->popMatrix(0);
	const auto mat2 = seq->popMatrix(0);
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
	auto identityMatNode = Core::Builder::createNode<ENodeType::MatrixToMatrix>();

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

	auto matNode = Builder::createNode<ENodeType::MatrixToMatrix>();
	setValue_expectOk(matNode, generateMat4());

	plug_expectOk(matNode, seq, I3T_OUTPUT0, I3T_SEQ_IN_MAT);

	EXPECT_EQ(matNode->getData().getMat4(), seq->getData().getMat4());
}

TEST(SequenceTest, SequenceCantBeSelfPlugged)
{
	auto seq1 = arrangeSequence();
	auto seq2 = arrangeSequence();

	{
		auto result = GraphManager::plug(seq1, seq1, 0, 0);
		EXPECT_EQ(ENodePlugResult::Err_Loopback, result);
	}
	{
		auto result = GraphManager::plug(seq1, seq1, 1, 1);
		EXPECT_EQ(ENodePlugResult::Err_Loopback, result);
	}
	{
		auto result = GraphManager::plug(seq1, seq1, 0, 1);
		EXPECT_EQ(ENodePlugResult::Err_MismatchedPinTypes, result);
	}
	{
		auto result = GraphManager::plug(seq1, seq1, 1, 0);
		EXPECT_EQ(ENodePlugResult::Err_MismatchedPinTypes, result);
	}
}

/**
 * Connect storages.
 *    _______
 *   /       \
 * seq1 --- seq2
 *             \
 *             mat
 */
TEST(SequenceTest, RightSequenceValueOutputCanBePluggedToParentSequenceValueInput)
{
	/// \todo MH
	return;

	auto seq1 = arrangeSequence();
	auto seq2 = arrangeSequence();
	auto mat = Builder::createNode<ENodeType::MatrixToMatrix>();

	plug_expectOk(seq1, seq2, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);

	plug_expectOk(seq2, seq1, I3T_SEQ_OUT_MAT, I3T_SEQ_IN_MAT);

	// Matrix storages should be same.
	EXPECT_EQ(seq1->getData(I3T_SEQ_MAT).getMat4(), seq2->getData(I3T_SEQ_MAT).getMat4());

	// seq1 model matrix and seq2 stored matrices product should be same.
	EXPECT_EQ(seq1->getData(I3T_SEQ_MOD).getMat4(), seq2->getData(I3T_SEQ_MAT).getMat4());

	plug_expectOk(seq2, mat, I3T_SEQ_OUT_MAT, I3T_INPUT0);
	EXPECT_EQ(seq1->getData(I3T_SEQ_MAT).getMat4(), mat->getData(I3T_DATA0).getMat4());

	// seq2 model matrix should be same as seq1 * seq2
	EXPECT_EQ(seq2->getData(2).getMat4(), seq1->getData(1).getMat4() * seq2->getData(1).getMat4());
}

TEST(SequenceTest, LeftSequenceValueOutputCanBePluggedToParentSequenceValueInput)
{
	auto seq1 = arrangeSequence();
	auto seq2 = arrangeSequence();

	plug_expectOk(seq1, seq2, 0, 0);

	auto plugResult = GraphManager::plug(seq1, seq2, 1, 1);
	EXPECT_EQ(ENodePlugResult::Ok, plugResult);

	EXPECT_EQ(seq1->getData(1).getMat4(), seq2->getData(1).getMat4());
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
		auto expectedMat =
		    getMatProduct(seq1->getMatrices()) * getMatProduct(seq2->getMatrices()) * getMatProduct(seq3->getMatrices());
		EXPECT_EQ(expectedMat, seq3->getData().getMat4());
	}
}

TEST(SequenceTest, MultipleNotifyKeepsSameValue)
{
	auto seq = Builder::createSequence();
	auto transform = GraphManager::createTransform<ETransformType::Free>();
	const auto initialValue = transform->getData().getMat4();

	auto newValue = generateMat4();
	transform->setValue(newValue);

	EXPECT_FALSE(Math::eq(initialValue, newValue));

	seq->addMatrix(transform);

	for (int i = 0; i < 10; ++i)
	{
		transform->notifySequence();
	}

	EXPECT_TRUE(Math::eq(seq->getData().getMat4(), newValue));
}

TEST(SequenceTest, ResetAndRestoreUpdatesSequenceOutputs)
{
	auto seq = Builder::createSequence();
	auto transform = GraphManager::createTransform<ETransformType::Free>();
	seq->addMatrix(transform);

	transform->unlock();

	auto initial = seq->getData().getMat4();

	transform->setValue(glm::translate(generateVec3()));

	const auto beforeReset = seq->getData().getMat4();
	const auto stored = seq->getData().getMat4();
	EXPECT_FALSE(Math::eq(initial, beforeReset));

	transform->saveValue();
	transform->resetMatrixFromDefaults();

	const auto afterReset = seq->getData().getMat4();
	EXPECT_TRUE(Math::eq(initial, afterReset));

	transform->reloadValue();
	const auto afterRestore = seq->getData().getMat4();
	EXPECT_FALSE(Math::eq(afterReset, afterRestore));
	EXPECT_TRUE(Math::eq(stored, afterRestore));
}
