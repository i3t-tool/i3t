/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"

using namespace Core;

class SequenceTest : public GraphManagerTestFixtureSuite
{};

TEST_F(SequenceTest, SequenceCanContainMatrices)
{
	auto seq = arrangeSequence();

	glm::mat4 expectedMat(1.0f);
	for (auto& mat : seq->getMatrices())
	{
		expectedMat *= mat->data().getMat4();
	}

	const auto& currentMat = seq->data().getMat4();
	EXPECT_EQ(expectedMat, currentMat);
}

TEST_F(SequenceTest, AddMatrixToTail)
{
	auto seq = GraphManager::createSequence();
	auto mat = Builder::createTransform<ETransformType::Free>();

	seq->pushMatrix(mat);
	EXPECT_EQ(1, seq->getMatrices().size());
}

TEST_F(SequenceTest, MatricesCanBeMoved)
{
	auto seq = arrangeSequence();

	// Pop first matrices in the sequence.
	const auto mat1 = seq->popMatrix(0);
	const auto mat2 = seq->popMatrix(0);
	EXPECT_FALSE(mat1->isInSequence() && mat2->isInSequence());

	{
		auto expectedSequenceValue = getMatProduct(seq->getMatrices());
		EXPECT_EQ(expectedSequenceValue, seq->data().getMat4());
	}

	// Add them back.
	seq->pushMatrix(mat2, 1);
	seq->pushMatrix(mat1, 2);
	EXPECT_TRUE(mat1->isInSequence() && mat2->isInSequence());
	{
		auto expectedSequenceValue = getMatProduct(seq->getMatrices());
		EXPECT_EQ(expectedSequenceValue, seq->data().getMat4());
	}
}

TEST_F(SequenceTest, MatricesCanBeSwapped)
{
	auto seq = arrangeSequence();

	auto firstMat = seq->getMatrices().front();
	auto lastMat = seq->getMatrices().back();

	seq->swap(0, 2);

	EXPECT_EQ(lastMat, seq->getMatrices().front());
	EXPECT_EQ(firstMat, seq->getMatrices().back());
}

TEST_F(SequenceTest, UpdateIsCalledOnMatrixValueChange)
{
	auto seq = arrangeSequence();

	auto& matrices = seq->getMatrices();
	auto firstTwoMatricesProduct = matrices[0]->data().getMat4() * matrices[1]->data().getMat4();
	auto mat3NewValue = glm::translate(generateVec3());

	auto& mat = seq->getMatRef(seq->getMatrices().size() - 1);
	setValue_expectOk(mat, mat3NewValue);

	EXPECT_EQ(seq->data().getMat4(), firstTwoMatricesProduct * mat3NewValue);
}

TEST_F(SequenceTest, InternalValueCanBeReadByOperator)
{
	auto seq = arrangeSequence();
	auto matMulMatNode = Core::Builder::createOperator<EOperatorType::MatrixMulMatrix>();
	auto identityMatNode = Core::Builder::createOperator<EOperatorType::MatrixToMatrix>();

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

	EXPECT_EQ(seq->data().getMat4(), matMulMatNode->data().getMat4());
}

TEST_F(SequenceTest, InternalValueCanBeSetFromOutside)
{
	auto seq = arrangeSequence();

	auto matNode = Builder::createOperator<EOperatorType::MatrixToMatrix>();
	setValue_expectOk(matNode, generateMat4());

	plug_expectOk(matNode, seq, I3T_OUTPUT0, I3T_SEQ_IN_MAT);

	EXPECT_EQ(matNode->data().getMat4(), seq->data().getMat4());
}

TEST_F(SequenceTest, SequenceCantBeSelfPlugged)
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
TEST_F(SequenceTest, RightSequenceValueOutputCanBePluggedToParentSequenceValueInput)
{
	/// \todo MH
	return;

	auto seq1 = arrangeSequence();
	auto seq2 = arrangeSequence();
	auto mat = Builder::createOperator<EOperatorType::MatrixToMatrix>();

	plug_expectOk(seq1, seq2, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);

	plug_expectOk(seq2, seq1, I3T_SEQ_OUT_MAT, I3T_SEQ_IN_MAT);

	// Matrix storages should be same.
	EXPECT_EQ(seq1->data(I3T_SEQ_MAT).getMat4(), seq2->data(I3T_SEQ_MAT).getMat4());

	// seq1 model matrix and seq2 stored matrices product should be same.
	EXPECT_EQ(seq1->data(I3T_SEQ_MOD).getMat4(), seq2->data(I3T_SEQ_MAT).getMat4());

	plug_expectOk(seq2, mat, I3T_SEQ_OUT_MAT, I3T_INPUT0);
	EXPECT_EQ(seq1->data(I3T_SEQ_MAT).getMat4(), mat->data(I3T_DATA0).getMat4());

	// seq2 model matrix should be same as seq1 * seq2
	EXPECT_EQ(seq2->data(2).getMat4(), seq1->data(1).getMat4() * seq2->data(1).getMat4());
}

TEST_F(SequenceTest, LeftSequenceValueOutputCanBePluggedToParentSequenceValueInput)
{
	auto seq1 = arrangeSequence();
	auto seq2 = arrangeSequence();

	plug_expectOk(seq1, seq2, 0, 0);

	auto plugResult = GraphManager::plug(seq1, seq2, 1, 1);
	EXPECT_EQ(ENodePlugResult::Ok, plugResult);

	EXPECT_EQ(seq1->data(1).getMat4(), seq2->data(1).getMat4());
}

TEST_F(SequenceTest, ThreeSequencesComposeMatrices)
{
	auto seq1 = arrangeSequence();
	auto seq2 = arrangeSequence();
	auto seq3 = arrangeSequence();

	{
		auto expectedMat = getMatProduct(seq1->getMatrices());
		EXPECT_EQ(expectedMat, seq1->data().getMat4());
	}
	{
		plug_expectOk(seq1, seq2, 0, 0);
		auto expectedMat = getMatProduct(seq1->getMatrices()) * getMatProduct(seq2->getMatrices());
		EXPECT_EQ(expectedMat, seq2->data().getMat4());
	}
	{
		plug_expectOk(seq2, seq3, 0, 0);
		auto expectedMat = getMatProduct(seq1->getMatrices()) * getMatProduct(seq2->getMatrices()) *
		                   getMatProduct(seq3->getMatrices());
		EXPECT_EQ(expectedMat, seq3->data().getMat4());
	}
}

TEST_F(SequenceTest, MultipleNotifyKeepsSameValue)
{
	auto seq = GraphManager::createSequence();
	auto transform = GraphManager::createTransform<ETransformType::Free>();
	const auto initialValue = transform->data().getMat4();

	auto newValue = generateMat4();
	transform->setValue(newValue);

	EXPECT_FALSE(Math::eq(initialValue, newValue));

	seq->pushMatrix(transform);

	for (int i = 0; i < 10; ++i)
	{
		transform->notifySequence();
	}

	EXPECT_TRUE(Math::eq(seq->data().getMat4(), newValue));
}

TEST_F(SequenceTest, ResetAndRestoreUpdatesSequenceOutputs)
{
	auto sequence = GraphManager::createSequence();
	auto transform = GraphManager::createTransform<ETransformType::Translation>();
	transform->unlock();

	sequence->pushMatrix(transform);

	auto initial = sequence->data().getMat4();

	transform->setDefaultValue("translation", generateVec3());

	const auto beforeReset = sequence->data().getMat4();
	const auto stored = sequence->data().getMat4();
	EXPECT_FALSE(Math::eq(initial, beforeReset));

	transform->saveValue();
	transform->resetMatrixFromDefaults();

	const auto afterReset = sequence->data().getMat4();
	transform->reloadValue();
	const auto afterRestore = sequence->data().getMat4();
	EXPECT_TRUE(Math::eq(afterReset, afterRestore));
	EXPECT_TRUE(Math::eq(stored, afterRestore));
}

// Tests #144
TEST_F(SequenceTest, SelfCycle)
{
	auto seq = GraphManager::createSequence();

	auto result = GraphManager::isPlugCorrect(seq->getOutput(I3T_SEQ_OUT_MOD), seq->getInput(I3T_SEQ_IN_MAT));

	EXPECT_EQ(result, ENodePlugResult::Err_Loopback);
}

// Tests #228
/**
 * seq1 --- seq2 --- seq3
 * mat ----/
 */
TEST_F(SequenceTest, WhenPluggingSequenceWithMatrixInputToSequenceWithMatrixInside_ThenLastSequenceOutputsAreCorrect)
{
	auto transform = GraphManager::createTransform<ETransformType::Free>();
	transform->setValue(2.0f, {0, 0});
	auto seq1 = GraphManager::createSequence();
	seq1->pushMatrix(transform);

	auto seq2 = GraphManager::createSequence();
	plug_expectOk(seq1, seq2, 0, 0);

	auto mat = Core::Builder::createOperator<EOperatorType::MatrixToMatrix>();
	plug_expectOk(mat, seq2, 0, 1);
	mat->setValue(2.0f, {3, 0});

	const auto expected = transform->data().getMat4() * mat->data().getMat4();
	EXPECT_TRUE(Math::eq(seq2->data().getMat4(), expected));
}
