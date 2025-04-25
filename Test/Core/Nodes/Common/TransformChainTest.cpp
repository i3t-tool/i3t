/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "Core/Nodes/Tracking.h"
#include "Trees.h"

using namespace Core;

class TransformChainTest : public GraphManagerTestFixture
{};

void testChain(TransformChain& chain, const std::vector<TransformInfo>& expected)
{
	auto [nodes, infos] = chain.begin().collectWithInfo();

	ASSERT_EQ(nodes.size(), expected.size());
	ASSERT_EQ(infos.size(), expected.size());

	for (int i = 0; i < nodes.size(); i++)
	{
		ASSERT_TRUE(infos[i] == expected[i])
		    << "Got: " << infos[i].toString() << "\nExpected: " << expected[i].toString();
		ASSERT_EQ(nodes[i], expected[i].currentNode);
	}
}

TEST_F(TransformChainTest, BeginAndEnd)
{
	auto s = BranchedSequenceTree();

	// Create sequence–root path from "branch1" sequence to root sequence.
	TransformChain tree(s.branch1Sequence);

	// Get iterator which points to last matrix in branch1.
	auto it = tree.begin();
	EXPECT_TRUE(it != tree.end());
	EXPECT_TRUE(*it == s.branch1Sequence->getMatrices().back());
}

TEST_F(TransformChainTest, Advance)
{
	TrickyTestTree s;

	TransformChain tree(s.sequence5);

	auto it = tree.begin();
	ASSERT_TRUE(it != tree.end());

	Ptr<Node> matrix;

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence5.get());
	EXPECT_TRUE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.sequence5);
	++it;
	ASSERT_TRUE(it != tree.end());

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence3.get());
	EXPECT_FALSE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.sequence3->getMatrices().back());
	++it;
	ASSERT_TRUE(it != tree.end());

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence2.get());
	EXPECT_TRUE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.sequence2);
	++it;
	ASSERT_TRUE(it != tree.end());

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence1.get());
	EXPECT_FALSE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.sequence1->getMatrices()[1]);
	++it;
	ASSERT_TRUE(it != tree.end());

	matrix = *it;
	EXPECT_EQ(it.getSequence(), s.sequence1.get());
	EXPECT_FALSE(it.transformInfo().isExternal);
	EXPECT_EQ(matrix, s.sequence1->getMatrices()[0]);
	ASSERT_TRUE(it.valid());
	++it;

	// Iterator is invalid.
	ASSERT_EQ(it.getSequence(), nullptr);
	ASSERT_TRUE(it == tree.end());
	ASSERT_FALSE(it.valid());
}

TEST_F(TransformChainTest, collect)
{
	TrickyTestTree s;
	TransformChain tree(s.sequence5);

	std::vector<Ptr<Node>> expected = {
	    s.sequence5,
	    s.sequence3->getMatrices().back(),
	    s.sequence2,
	    s.sequence1->getMatrices()[1],
	    s.sequence1->getMatrices()[0],
	};

	const auto result = tree.begin().collect();

	EXPECT_EQ(expected, result);
}

TEST_F(TransformChainTest, TransformChainIterator)
{
	auto s = BranchedSequenceTree();

	// Create sequence–root path from "branch1" sequence to root sequence.
	TransformChain tree(s.branch1Sequence);

	std::vector<Ptr<Node>> expectedMatrices = s.branch1ToRootMatrices;

	// Iterate through matrices.
	{
		std::vector<Ptr<Node>> result;

		// Get iterator which points to last matrix in branch1.
		auto it = tree.begin();

		EXPECT_TRUE(it != tree.end());

		// Collect all matrices until iterator is out of tree.
		while (it != tree.end())
		{
			Ptr<Node> matrix = *it;
			result.push_back(matrix);
			++it;
		}

		EXPECT_EQ(expectedMatrices, result);
	}
}

TEST_F(TransformChainTest, TransformChainIteratorOnTwoEmptySequences)
{
	auto leftSequence = GraphManager::createSequence();
	auto rightSequence = GraphManager::createSequence();

	plug_expectOk(leftSequence, rightSequence, 0, 0);

	TransformChain tree(rightSequence);

	// Get iterator which points to last matrix in branch1.
	auto it = tree.begin();

	EXPECT_TRUE(it == tree.end());
}

TEST_F(TransformChainTest, GetNonEmptyParentSequence)
{
	auto s1 = GraphManager::createSequence();
	auto s2 = GraphManager::createSequence();
	auto s3 = GraphManager::createSequence();

	plug_expectOk(s1, s2, I3T_SEQ_OUT_MAT, I3T_SEQ_IN_MAT);
	plug_expectOk(s2, s3, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);

	auto t1 = Builder::createTransform<ETransformType::Translation>();
	t1->setDefaultValue("translate", glm::vec3(1, 1, 1));
	s1->pushMatrix(t1);
	/*
	s1->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	s1->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	s1->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	s1->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	 */

	s2->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	// Never true.
	// EXPECT_EQ(s2->getInternalData().getMat4(), glm::translate(glm::vec3(1, 1, 1)));

	s3->pushMatrix(Builder::createTransform<ETransformType::Translation>());

	auto s1Parent = GraphManager::getParentSequence(s1, true);
	auto s2Parent = GraphManager::getParentSequence(s2, true);
	auto s3Parent = GraphManager::getParentSequence(s3, true);
	EXPECT_EQ(s1Parent, nullptr);
	EXPECT_EQ(s2Parent, nullptr);
	EXPECT_EQ(s3Parent, s2);

	TransformChain tree(s3);
	auto [matrices, info] = tree.begin().collectWithInfo();
	EXPECT_EQ(matrices.size(), 2);
	EXPECT_EQ(info.size(), 2);

	EXPECT_EQ(info[0].isExternal, false);
	EXPECT_EQ(info[0].sequence, s3);

	EXPECT_EQ(info[1].isExternal, true);
	// EXPECT_NE(info[1].sequence, s1.get());
	// EXPECT_NE(info[1].sequence, s3.get());
	EXPECT_EQ(info[1].sequence, s2);
	EXPECT_EQ(info[1].sequence->getId(), s2->getId());

	auto* t = GraphManager::startTracking(s3, TrackingDirection::LeftToRight);
	const auto t3 = s3->getMatrices().back();
	t->setParam(0.2f);
	EXPECT_NE(t->getInterpolatedTransformID(), s1->getId());
	EXPECT_EQ(t->getInterpolatedTransformID(), s2->getId());
	EXPECT_NE(t->getInterpolatedTransformID(), s3->getId());
	EXPECT_FLOAT_EQ(s2->getTrackingData()->progress, 2 * 0.2f);
	EXPECT_FLOAT_EQ(t3->getTrackingData()->progress, 0.0f);

	t->setParam(0.8f);
	EXPECT_NE(t->getInterpolatedTransformID(), s1->getId());
	EXPECT_NE(t->getInterpolatedTransformID(), s2->getId());
	EXPECT_EQ(t->getInterpolatedTransformID(), t3->getId());
	EXPECT_FLOAT_EQ(s2->getTrackingData()->progress, 1.0f);
	EXPECT_FLOAT_EQ(t3->getTrackingData()->progress, 1.0f - 2 * 0.2f);
}

TEST_F(TransformChainTest, MatrixInputPlugged)
{
	const auto sequence = GraphManager::createSequence();
	const auto matrix = Builder::createOperator<Core::EOperatorType::MatrixToMatrix>();
	plug_expectOk(matrix, sequence, 0, I3T_SEQ_IN_MAT);

	TransformChain tree(sequence);

	{
		auto it = tree.begin();
		EXPECT_TRUE(it != tree.end());
		EXPECT_EQ(*it, sequence);
	}
}

TransformInfo tInfo(Ptr<Sequence> sequence = nullptr, Ptr<Node> currentNode = nullptr, Ptr<Camera> camera = nullptr,
                    bool isExternal = false, TransformType type = TransformType::Model, int dataIndex = 0)
{
	TransformInfo info;
	info.currentNode = currentNode;
	info.sequence = sequence;
	info.camera = camera;
	info.isExternal = isExternal;
	info.type = type;
	info.dataIndex = dataIndex;
	return info;
}

TEST_F(TransformChainTest, GeneralCamTree_Default)
{
	GeneralCamTree tree(false);
	TransformChain chain(tree.seq5);
	std::vector<TransformInfo> expected = {
	    tInfo(tree.seq5, tree.seq5->getMatrices()[0]), tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[0]), tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[1]), tInfo(tree.seq1, tree.seq1->getMatrices()[0])};

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_IncludeCamera)
{
	GeneralCamTree tree(false);
	TransformChain chain = TransformChain(tree.seq5).skipEmptySequences().ignoreCamera(false);

	// clang-format off
	std::vector<TransformInfo> expected = {
	    tInfo(tree.seq5, tree.seq5->getMatrices()[0]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[0]),
	    tInfo(tree.cam1->getView(), tree.cam1->getView()->getMatrices()[0], tree.cam1, false, TransformType::View),
	    tInfo(tree.cam1->getProj(), tree.cam1->getProj()->getMatrices()[1], tree.cam1, false, TransformType::Projection),
	    tInfo(tree.cam1->getProj(), tree.cam1->getProj()->getMatrices()[0], tree.cam1, false, TransformType::Projection)
	};
	// clang-format on

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_IncludeDisconnectedCam)
{
	GeneralCamTree tree(false);
	GraphManager::unplugInput(tree.seq1, I3T_SEQ_IN_MUL);
	TransformChain chain(tree.seq5);
	chain.ignoreCamera(false);
	std::vector<TransformInfo> expected = {
	    tInfo(tree.seq5, tree.seq5->getMatrices()[0]), tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[0]), tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[1]), tInfo(tree.seq1, tree.seq1->getMatrices()[0])};

	testChain(chain, expected);

	// Plug it back again
	plug_expectOk(tree.cam1, tree.seq1, I3T_CAMERA_OUT_MUL, I3T_SEQ_IN_MUL);

	expected = {
	    tInfo(tree.seq5, tree.seq5->getMatrices()[0]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[0]),
	    tInfo(tree.cam1->getView(), tree.cam1->getView()->getMatrices()[0], tree.cam1, false, TransformType::View),
	    tInfo(tree.cam1->getProj(), tree.cam1->getProj()->getMatrices()[1], tree.cam1, false,
	          TransformType::Projection),
	    tInfo(tree.cam1->getProj(), tree.cam1->getProj()->getMatrices()[0], tree.cam1, false,
	          TransformType::Projection)};

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_IncludeCamAndEmptySeq)
{
	GeneralCamTree tree(true); // Empty camera
	// Copy assignemt on purpose
	TransformChain chain = TransformChain(tree.seq5).ignoreCamera(false).skipEmptySequences(false);
	// clang-format off
	std::vector<TransformInfo> expected = {
	    tInfo(tree.seq5, tree.seq5->getMatrices()[0]),
		tInfo(tree.seq4, nullptr),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
		tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[0])
	};
	// clang-format on

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_IncludeEmptyCamAndEmptySeq)
{
	GeneralCamTree tree(true); // Empty camera
	TransformChain chain(TransformChain(tree.seq5));
	chain.skipEmptySequences(false);
	chain.skipEmptyCamera(false);

	std::vector<TransformInfo> expected = {
	    tInfo(tree.seq5, tree.seq5->getMatrices()[0]), tInfo(tree.seq4, nullptr),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[1]), tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[2]), tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[0])};

	testChain(chain, expected);

	// Must disable camera ignore
	chain.ignoreCamera(false);

	expected = {tInfo(tree.seq5, tree.seq5->getMatrices()[0]),
	            tInfo(tree.seq4, nullptr),
	            tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
	            tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	            tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	            tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	            tInfo(tree.seq1, tree.seq1->getMatrices()[0]),
	            tInfo(tree.cam1->getView(), nullptr, tree.cam1, false, TransformType::View),
	            tInfo(tree.cam1->getProj(), nullptr, tree.cam1, false, TransformType::Projection)};

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_BeginInCamera)
{
	GeneralCamTree tree(false);
	TransformChain chain(tree.cam1->getView(), tree.cam1);

	// clang-format off
	std::vector<TransformInfo> expected = {};
	// clang-format on

	testChain(chain, expected);

	chain.ignoreCamera(false); // Camera must not be ignored for us to begin in it

	// clang-format off
	expected = {
		tInfo(tree.cam1->getView(), tree.cam1->getView()->getMatrices()[0], tree.cam1, false, TransformType::View),
		tInfo(tree.cam1->getProj(), tree.cam1->getProj()->getMatrices()[1], tree.cam1, false, TransformType::Projection),
		tInfo(tree.cam1->getProj(), tree.cam1->getProj()->getMatrices()[0], tree.cam1, false, TransformType::Projection)
	};
	// clang-format on

	testChain(chain, expected);
	//

	chain = TransformChain(tree.cam1->getProj(), tree.cam1).ignoreCamera(false);
	// clang-format off
	expected = {
		tInfo(tree.cam1->getProj(), tree.cam1->getProj()->getMatrices()[1], tree.cam1, false, TransformType::Projection),
		tInfo(tree.cam1->getProj(), tree.cam1->getProj()->getMatrices()[0], tree.cam1, false, TransformType::Projection)
	};
	// clang-format on

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_BeginInCameraNoCamRef)
{
	// Try with empty camera
	GeneralCamTree tree(true);
	TransformChain chain(tree.cam1->getView()); // Omitted the tree.cam1 pointer

	std::vector<TransformInfo> expected = {};
	testChain(chain, expected);

	chain = TransformChain(tree.cam1->getView()); // Omitted the tree.cam1 pointer
	chain.ignoreCamera(false);
	chain.skipEmptySequences(false);

	expected = {tInfo(tree.cam1->getView(), nullptr, nullptr)};
	testChain(chain, expected);

	// Now try with non-empty camera
	GeneralCamTree tree2(false);

	chain = TransformChain(tree2.cam1->getView()); // Omitted the tree.cam1 pointer
	expected = {tInfo(tree2.cam1->getView(), tree2.cam1->getView()->getMatrices()[0], nullptr)};

	testChain(chain, expected);
	chain.ignoreCamera(false);
	testChain(chain, expected);

	// Try with proj
	chain = TransformChain(tree2.cam1->getProj()); // Omitted the tree.cam1 pointer
	expected = {tInfo(tree2.cam1->getProj(), tree2.cam1->getProj()->getMatrices()[1], nullptr),
	            tInfo(tree2.cam1->getProj(), tree2.cam1->getProj()->getMatrices()[0], nullptr)};

	testChain(chain, expected);
	chain.ignoreCamera(false);
	testChain(chain, expected);
}


TEST_F(TransformChainTest, GeneralCamTree_OperatorInBeginSeq)
{
	GeneralCamTree tree(true); // Empty camera
	TransformChain chain(TransformChain(tree.seq6));

	std::vector<TransformInfo> expected = {
	    tInfo(tree.seq6, tree.seq6, nullptr, true, TransformType::Model, I3T_SEQ_IN_MAT),
	    tInfo(tree.seq3, tree.seq3->getMatrices()[1]),
	    tInfo(tree.seq3, tree.seq3->getMatrices()[0]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[0])};

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_BeginInEmptySeq)
{
	GeneralCamTree tree(false);
	TransformChain chain(TransformChain(tree.seq4));

	std::vector<TransformInfo> expected = {
	    tInfo(tree.seq2, tree.seq2->getMatrices()[1]), tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[2]), tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[0]),
	};

	testChain(chain, expected);

	chain.skipEmptySequences(false);

	expected = {tInfo(tree.seq4, nullptr),
	            tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
	            tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	            tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	            tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	            tInfo(tree.seq1, tree.seq1->getMatrices()[0])};

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_BeginInEmptyCameraSeq)
{
	GeneralCamTree tree(true); // Empty camera
	TransformChain chain(TransformChain(tree.cam1->getView(), tree.cam1));
	chain.skipEmptyCamera(true);

	std::vector<TransformInfo> expected = {};
	testChain(chain, expected);

	chain.ignoreCamera(false);

	testChain(chain, expected);

	chain.skipEmptyCamera(false);

	testChain(chain, expected);

	chain.skipEmptySequences(false);

	expected = {tInfo(tree.cam1->getView(), nullptr, tree.cam1, false, TransformType::View),
	            tInfo(tree.cam1->getProj(), nullptr, tree.cam1, false, TransformType::Projection)};

	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_CameraWithExternalData)
{
	GeneralCamTree tree(false, true);
	TransformChain chain(TransformChain(tree.seq2));
	chain.ignoreCamera(false);

	std::vector<TransformInfo> expected = {
	    tInfo(tree.seq2, tree.seq2->getMatrices()[1]),
	    tInfo(tree.seq2, tree.seq2->getMatrices()[0]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[2]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[1]),
	    tInfo(tree.seq1, tree.seq1->getMatrices()[0]),
	    tInfo(tree.cam1->getView(), tree.cam1->getView(), tree.cam1, true, TransformType::View, I3T_SEQ_IN_MAT),
	    tInfo(tree.cam1->getProj(), tree.cam1->getProj(), tree.cam1, true, TransformType::Projection, I3T_SEQ_IN_MAT)};
	testChain(chain, expected);
}

TEST_F(TransformChainTest, GeneralCamTree_OperatorInBeginCamera)
{
	GeneralCamTree tree(false, true);
	TransformChain chain(TransformChain(tree.cam1->getView(), tree.cam1));
	chain.ignoreCamera(false);

	std::vector<TransformInfo> expected = {
	    tInfo(tree.cam1->getView(), tree.cam1->getView(), tree.cam1, true, TransformType::View, I3T_SEQ_IN_MAT),
	    tInfo(tree.cam1->getProj(), tree.cam1->getProj(), tree.cam1, true, TransformType::Projection, I3T_SEQ_IN_MAT)};
	testChain(chain, expected);
}
