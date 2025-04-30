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

#include "glm/gtx/matrix_interpolation.hpp"

#include "Core/Nodes/GraphManager.h"

#include "Common.h"
#include "Common/Trees.h"
#include "Generator.h"
#include "Utils.h"

using namespace Core;

struct MyTree
{
	Ptr<Sequence> s1;
	Ptr<Sequence> s2;

	Ptr<Transform> mat1;
	Ptr<Transform> mat2;
	Ptr<Transform> mat3;
	Ptr<Transform> mat4;
};

MyTree arrange()
{
	auto ret = MyTree{
	    GraphManager::createSequence(),
	    GraphManager::createSequence(),
	    Builder::createTransform<ETransformType::Translation>(),
	    Builder::createTransform<ETransformType::Translation>(),
	    Builder::createTransform<ETransformType::Translation>(),
	    Builder::createTransform<ETransformType::Translation>(),
	};

	ret.s1->pushMatrix(ret.mat1);
	ret.s1->pushMatrix(ret.mat2);

	ret.s2->pushMatrix(ret.mat3);
	ret.s2->pushMatrix(ret.mat4);

	plug_expectOk(ret.s1, ret.s2);

	ret.mat1->setDefaultValue("translation", generateVec3());
	ret.mat2->setDefaultValue("translation", generateVec3());
	ret.mat3->setDefaultValue("translation", generateVec3());
	ret.mat4->setDefaultValue("translation", generateVec3());

	return ret;
}

class TrackerTest : public GraphManagerTestFixture
{};

std::vector seqIdx = {0, 0, 1, 1};
std::vector interpolating = {false, true, false, false};
std::vector progress = {1.f, 1.f, 0.f, 0.f};
std::vector chain = {false, false, false, false};
std::vector active = {true, true, false, false};

void checkInternalData(MatrixTracker* tracker, int matrixCount, std::vector<Ptr<Transform>>& transforms,
                       std::vector<Ptr<Sequence>>& seqs, std::vector<int>& seqIdx, std::vector<bool>& interpolating,
                       std::vector<float>& progress, std::vector<bool>& chain, std::vector<bool>& active,
                       std::vector<TransformSpace>& space, std::vector<bool>& modelSubtree,
                       std::vector<bool>& isInCamera, std::vector<int>& seqSeqIdx, std::vector<bool>& seqInterpolating,
                       std::vector<float>& seqProgress, std::vector<bool>& seqChain, std::vector<bool>& seqActive,
                       std::vector<TransformSpace>& seqSpace, std::vector<bool>& seqModelSubtree,
                       std::vector<bool>& seqIsInCamera)
{
	ASSERT_EQ(tracker->getMatrixCount(), matrixCount);
	ASSERT_EQ(tracker->getTrackedTransforms().size(), transforms.size());
	ASSERT_EQ(tracker->getTrackedTransforms().size(), tracker->getTransformCount());
	{
		auto& trackedTransforms = tracker->getTrackedTransforms();
		for (int i = 0; i < transforms.size(); ++i)
		{
			auto& transform = trackedTransforms[i];

			ASSERT_EQ(transform->node.lock().get(), transforms[i].get());
			EXPECT_EQ(transform->data.tIndex, i);
			EXPECT_EQ(transform->data.seqIndex, seqIdx[i]);
			EXPECT_EQ(transform->data.interpolating, interpolating[i]);
			EXPECT_FLOAT_EQ(transforms[i]->getTrackingData()->progress, progress[i]);
			EXPECT_FLOAT_EQ(transform->data.progress, progress[i]);
			EXPECT_EQ(transform->data.active, active[i]);
			EXPECT_EQ(transform->data.chain, chain[i]);
			EXPECT_EQ(transform->data.space, space[i]);
			EXPECT_EQ(transform->data.modelSubtree, modelSubtree[i]);
			EXPECT_EQ(transform->data.isInCamera, isInCamera[i]);
		}
	}

	ASSERT_EQ(tracker->getTrackedSequences().size(), seqs.size());
	{
		auto& trackedSequences = tracker->getTrackedSequences();
		for (int i = 0; i < seqs.size(); ++i)
		{
			auto& seq = trackedSequences[i];
			ASSERT_EQ(seq->node.lock().get(), seqs[i].get());
			EXPECT_EQ(seq->data.tIndex, -1);
			EXPECT_EQ(seq->data.seqIndex, seqSeqIdx[i]);
			EXPECT_EQ(seq->data.interpolating, seqInterpolating[i]);
			EXPECT_FLOAT_EQ(seqs[i]->getTrackingData()->progress, seqProgress[i]);
			EXPECT_FLOAT_EQ(seq->data.progress, seqProgress[i]);
			EXPECT_EQ(seq->data.active, seqActive[i]);
			EXPECT_EQ(seq->data.chain, seqChain[i]);
			EXPECT_EQ(seq->data.space, seqSpace[i]);
			EXPECT_EQ(seq->data.modelSubtree, seqModelSubtree[i]);
			EXPECT_EQ(seq->data.isInCamera, seqIsInCamera[i]);
		}
	}
}

/**
 * Classic tracking
 *
 * ///////////     ///////////
 * | M1 | M2 | --- | M3 | M4 |
 * ///////////     ///////////
 */
TEST_F(TrackerTest, TrackingRightToLeft)
{
	auto t = arrange();

	// Act
	auto* tracker = GraphManager::startTracking(t.s2, TrackingDirection::RightToLeft);

	std::vector mats = {t.mat4, t.mat3, t.mat2, t.mat1};
	std::vector seqs = {t.s2, t.s1};

	{
		float trackingParam = 1.0f;
		tracker->setProgress(trackingParam);

		EXPECT_EQ(tracker->getFullMatricesCount(), 0);
		EXPECT_EQ(tracker->getMatrixCount(), 4);

		tracker->update();

		auto expected =
		    t.mat1->data().getMat4() * t.mat2->data().getMat4() * t.mat3->data().getMat4() * t.mat4->data().getMat4();

		EXPECT_EQ(tracker->getFullMatricesCount(), 4);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));
		EXPECT_PRED_FORMAT2(AssertEqualMatrices, expected, tracker->getInterpolatedMatrix());

		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, 1.0f);

		// Check internal data of tracker
		std::vector seqIdx = {0, 0, 1, 1};
		std::vector interpolating = {false, false, false, true};
		std::vector progress = {1.f, 1.f, 1.f, 1.f};
		std::vector chain = {false, false, false, false};
		std::vector active = {true, true, true, true};
		std::vector modelSubtree = {false, false, false, false};
		std::vector isInCamera = {false, false, false, false};
		std::vector space = {TransformSpace::Model, TransformSpace::Model, TransformSpace::Model,
		                     TransformSpace::Model};

		std::vector seqSeqIdx = {0, 1};
		std::vector seqInterpolating = {false, true};
		std::vector seqProgress = {1.f, 1.f};
		std::vector seqChain = {true, true};
		std::vector seqActive = {true, true};
		std::vector seqModelSubtree = {false, false};
		std::vector seqIsInCamera = {false, false};
		std::vector seqSpace = {TransformSpace::Model, TransformSpace::Model};

		checkInternalData(tracker, mats.size(), mats, seqs, seqIdx, interpolating, progress, chain, active, space,
		                  modelSubtree, isInCamera, seqSeqIdx, seqInterpolating, seqProgress, seqChain, seqActive,
		                  seqSpace, seqModelSubtree, seqIsInCamera);
	}
	{
		float trackingParam = 0.85f;
		tracker->setProgress(trackingParam);
		EXPECT_EQ(tracker->getFullMatricesCount(), 4);
		tracker->update();

		float interpParam = (abs(trackingParam) - 0.75f) * 4;
		auto expected = glm::interpolate(glm::mat4(1.0f), t.mat1->data().getMat4(), interpParam) *
		                t.mat2->data().getMat4() * t.mat3->data().getMat4() * t.mat4->data().getMat4();

		EXPECT_EQ(tracker->getFullMatricesCount(), 3);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));


		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, interpParam);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, 1.0f);
	}
	{
		float trackingParam = 0.5f;
		tracker->setProgress(trackingParam);
		tracker->update();

		auto expected = t.mat3->data().getMat4() * t.mat4->data().getMat4();

		EXPECT_EQ(tracker->getFullMatricesCount(), 2);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, 1.0f);

		// Check internal data of tracker
		std::vector seqIdx = {0, 0, 1, 1};
		std::vector interpolating = {false, true, false, false};
		std::vector progress = {1.f, 1.f, 0.f, 0.f};
		std::vector chain = {false, false, false, false};
		std::vector active = {true, true, false, false};
		std::vector modelSubtree = {false, false, false, false};
		std::vector isInCamera = {false, false, false, false};
		std::vector space = {TransformSpace::Model, TransformSpace::Model, TransformSpace::Model,
		                     TransformSpace::Model};

		std::vector seqSeqIdx = {0, 1};
		std::vector seqInterpolating = {true, false};
		std::vector seqProgress = {1.f, 0.f};
		std::vector seqChain = {true, true};
		std::vector seqActive = {true, false};
		std::vector seqModelSubtree = {false, false};
		std::vector seqIsInCamera = {false, false};
		std::vector seqSpace = {TransformSpace::Model, TransformSpace::Model};

		checkInternalData(tracker, mats.size(), mats, seqs, seqIdx, interpolating, progress, chain, active, space,
		                  modelSubtree, isInCamera, seqSeqIdx, seqInterpolating, seqProgress, seqChain, seqActive,
		                  seqSpace, seqModelSubtree, seqIsInCamera);
	}
	{
		float trackingParam = 0.30f;
		tracker->setProgress(trackingParam);
		tracker->update();

		float interpParam = (abs(trackingParam) - 0.25f) * 4;
		float interpParamSeq = (abs(trackingParam)) * 2;
		auto expected =
		    glm::interpolate(glm::mat4(1.0f), t.mat3->data().getMat4(), interpParam) * t.mat4->data().getMat4();

		EXPECT_EQ(tracker->getFullMatricesCount(), 1);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, interpParam);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, 1.0f);

		// Check internal data of tracker
		std::vector seqIdx = {0, 0, 1, 1};
		std::vector interpolating = {false, true, false, false};
		std::vector progress = {1.f, interpParam, 0.f, 0.f};
		std::vector chain = {false, false, false, false};
		std::vector active = {true, true, false, false};
		std::vector modelSubtree = {false, false, false, false};
		std::vector isInCamera = {false, false, false, false};
		std::vector space = {TransformSpace::Model, TransformSpace::Model, TransformSpace::Model,
		                     TransformSpace::Model};

		std::vector seqSeqIdx = {0, 1};
		std::vector seqInterpolating = {true, false};
		std::vector seqProgress = {interpParamSeq, 0.f};
		std::vector seqChain = {true, true};
		std::vector seqActive = {true, false};
		std::vector seqModelSubtree = {false, false};
		std::vector seqIsInCamera = {false, false};
		std::vector seqSpace = {TransformSpace::Model, TransformSpace::Model};

		checkInternalData(tracker, mats.size(), mats, seqs, seqIdx, interpolating, progress, chain, active, space,
		                  modelSubtree, isInCamera, seqSeqIdx, seqInterpolating, seqProgress, seqChain, seqActive,
		                  seqSpace, seqModelSubtree, seqIsInCamera);
	}
	{
		float trackingParam = 0.0f;
		tracker->setProgress(trackingParam);
		tracker->update();

		auto expected = glm::mat4(1.0f);

		EXPECT_EQ(tracker->getFullMatricesCount(), 0);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, 0.0f);

		// Check internal data of tracker
		std::vector seqIdx = {0, 0, 1, 1};
		std::vector interpolating = {true, false, false, false};
		std::vector progress = {0.f, 0.f, 0.f, 0.f};
		std::vector chain = {false, false, false, false};
		std::vector active = {true, false, false, false};
		std::vector modelSubtree = {false, false, false, false};
		std::vector isInCamera = {false, false, false, false};
		std::vector space = {TransformSpace::Model, TransformSpace::Model, TransformSpace::Model,
		                     TransformSpace::Model};

		std::vector seqSeqIdx = {0, 1};
		std::vector seqInterpolating = {true, false};
		std::vector seqProgress = {0.f, 0.f};
		std::vector seqChain = {true, true};
		std::vector seqActive = {true, false};
		std::vector seqModelSubtree = {false, false};
		std::vector seqIsInCamera = {false, false};
		std::vector seqSpace = {TransformSpace::Model, TransformSpace::Model};

		checkInternalData(tracker, mats.size(), mats, seqs, seqIdx, interpolating, progress, chain, active, space,
		                  modelSubtree, isInCamera, seqSeqIdx, seqInterpolating, seqProgress, seqChain, seqActive,
		                  seqSpace, seqModelSubtree, seqIsInCamera);
	}
}

TEST_F(TrackerTest, TrackingLeftToRight)
{
	auto t = arrange();

	// Act
	auto* tracker = GraphManager::startTracking(t.s2, TrackingDirection::LeftToRight);

	{
		float trackingParam = 0.0f;
		tracker->setProgress(trackingParam);
		EXPECT_EQ(tracker->getFullMatricesCount(), 0);
		EXPECT_EQ(tracker->getMatrixCount(), 4);

		tracker->update();

		glm::mat4 expected{1.0f};

		EXPECT_EQ(tracker->getFullMatricesCount(), 0);
		EXPECT_EQ(tracker->getMatrixCount(), 4);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));
		EXPECT_PRED_FORMAT2(AssertEqualMatrices, expected, tracker->getInterpolatedMatrix());

		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, 0.0f);
	}
	{
		float trackingParam = 0.30f;
		tracker->setProgress(trackingParam);
		tracker->update();

		float interpParam = (abs(trackingParam) - 0.25f) * 4;
		const auto expected =
		    t.mat1->data().getMat4() * glm::interpolate(glm::mat4(1.0f), t.mat2->data().getMat4(), interpParam);

		EXPECT_EQ(tracker->getFullMatricesCount(), 1);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, interpParam);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, 0.0f);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, 0.0f);
	}
	{
		float trackingParam = 0.85f;
		tracker->setProgress(trackingParam);
		tracker->update();

		float interpParam = (abs(trackingParam) - 0.75f) * 4;
		const auto expected = t.mat1->data().getMat4() * t.mat2->data().getMat4() * t.mat3->data().getMat4() *
		                      glm::interpolate(glm::mat4(1.0f), t.mat4->data().getMat4(), interpParam);

		EXPECT_EQ(tracker->getFullMatricesCount(), 3);
		EXPECT_EQ(tracker->getMatrixCount(), 4);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, interpParam);
	}
	{
		float trackingParam = 1.0f;
		tracker->setProgress(trackingParam);
		tracker->update();

		auto expected =
		    t.mat1->data().getMat4() * t.mat2->data().getMat4() * t.mat3->data().getMat4() * t.mat4->data().getMat4();

		EXPECT_EQ(tracker->getFullMatricesCount(), 4);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat2->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat3->getTrackingData()->progress, 1.0f);
		EXPECT_FLOAT_EQ(t.mat4->getTrackingData()->progress, 1.0f);
	}
}

TEST_F(TrackerTest, TrackedModelIsUpdatedOnSequenceChange)
{
	auto sequence = GraphManager::createSequence();
	sequence->pushMatrix(Builder::createTransform<ETransformType::Translation>());

	auto model = GraphManager::createModel();
	plug_expectOk(sequence, model, I3T_SEQ_OUT_MUL, I3T_MODEL_IN_MUL);

	// start full tracking
	auto* tracker = GraphManager::startTracking(sequence, TrackingDirection::RightToLeft);
	tracker->setProgress(1.0f);
	tracker->update();

	// add new matrix
	auto mat = Builder::createTransform<ETransformType::Translation>();
	mat->setDefaultValue("translation", generateVec3());
	sequence->pushMatrix(mat);

	tracker->update();

	EXPECT_TRUE(compare(mat->data().getMat4(), tracker->getInterpolatedMatrix()));
	EXPECT_TRUE(
	    compare(mat->data().getMat4(), tracker->getTrackedModels().back()->data.modelData->m_interpolatedMatrix));
	EXPECT_TRUE(compare(mat->data().getMat4(), model->getTrackingData()->modelData->m_interpolatedMatrix));
	EXPECT_EQ(model.get(), tracker->getTrackedModels().back()->node.lock().get());
}

TEST_F(TrackerTest, TrackingIsDisabledAfterSequenceRemoval)
{
	EXPECT_FALSE(GraphManager::isTracking());
	{
		auto sequence = GraphManager::createSequence();
		auto mat = Builder::createTransform<ETransformType::Translation>();
		mat->setDefaultValue("translation", generateVec3());
		sequence->pushMatrix(mat);

		// start full tracking
		auto* tracker = GraphManager::startTracking(sequence, TrackingDirection::RightToLeft);
		tracker->setProgress(1.0f);
		tracker->update();

		EXPECT_TRUE(GraphManager::isTracking());

		EXPECT_TRUE(compare(mat->data().getMat4(), tracker->getInterpolatedMatrix()));
	}

	EXPECT_FALSE(GraphManager::isTracking());
}

TEST_F(TrackerTest, EmptySequenceAfterTrackingStart)
{
	auto sequence = GraphManager::createSequence();
	sequence->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	sequence->pushMatrix(Builder::createTransform<ETransformType::Translation>());

	// start full tracking
	auto* tracker = GraphManager::startTracking(sequence, TrackingDirection::RightToLeft);
	tracker->setProgress(1.0f);
	tracker->update();

	ASSERT_TRUE(tracker->isTracking());

	sequence->popMatrix(0);
	sequence->popMatrix(0);

	ASSERT_TRUE(tracker->isTracking());

	tracker->setProgress(1.0f);
	tracker->update();

	ASSERT_FALSE(tracker->isTracking());
}

TEST_F(TrackerTest, SimpleCameraTracking)
{
	SimpleCamTree t;

	auto* tracker = GraphManager::startTracking(t.s1, TrackingDirection::RightToLeft);

	std::vector trans = {t.translate, t.lookAt, t.persp};
	std::vector seqs = {t.s1, t.c1->getView(), t.c1->getProj()};

	{
		float trackingParam = 1.0f;
		tracker->setProgress(trackingParam);

		EXPECT_EQ(tracker->getFullMatricesCount(), 0);
		EXPECT_EQ(tracker->getMatrixCount(), 4);
		EXPECT_EQ(tracker->getTransformCount(), 3);

		tracker->m_trackInWorldSpace = true;
		tracker->update();

		EXPECT_EQ(tracker->getFullMatricesCount(), 4);
		EXPECT_EQ(tracker->getMatrixCount(), 4);
		EXPECT_EQ(tracker->getTransformCount(), 3);

		auto expected = t.persp->data().getMat4() * t.lookAt->data().getMat4() * t.translate->data().getMat4();

		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));
		EXPECT_PRED_FORMAT2(AssertEqualMatrices, expected, tracker->getInterpolatedMatrix());

		// Check internal data of tracker
		std::vector seqIdx = {0, 1, 2};
		std::vector interpolating = {false, false, true};
		std::vector progress = {1.f, 1.f, 1.f};
		std::vector chain = {false, false, false};
		std::vector active = {true, true, true};
		std::vector modelSubtree = {false, false, false};
		std::vector isInCamera = {false, true, true};
		std::vector space = {TransformSpace::Model, TransformSpace::View, TransformSpace::Projection};

		std::vector seqSeqIdx = {0, 1, 2};
		std::vector seqInterpolating = {false, false, true};
		std::vector seqProgress = {1.f, 1.f, 1.f};
		std::vector seqChain = {true, true, true};
		std::vector seqActive = {true, true, true};
		std::vector seqModelSubtree = {false, false, false};
		std::vector seqIsInCamera = {false, true, true};
		std::vector seqSpace = {TransformSpace::Model, TransformSpace::View, TransformSpace::Projection};

		checkInternalData(tracker, 4, trans, seqs, seqIdx, interpolating, progress, chain, active, space, modelSubtree,
		                  isInCamera, seqSeqIdx, seqInterpolating, seqProgress, seqChain, seqActive, seqSpace,
		                  seqModelSubtree, seqIsInCamera);

		ASSERT_EQ(seqs[0]->getTrackingData()->childrenIdxStart, 0);
		ASSERT_EQ(seqs[0]->getTrackingData()->childrenIdxEnd, 1);
		ASSERT_EQ(seqs[1]->getTrackingData()->childrenIdxStart, 1);
		ASSERT_EQ(seqs[1]->getTrackingData()->childrenIdxEnd, 2);
		ASSERT_EQ(seqs[2]->getTrackingData()->childrenIdxStart, 2);
		ASSERT_EQ(seqs[2]->getTrackingData()->childrenIdxEnd, 3);

		ASSERT_EQ(t.translate->getTrackingData()->childrenIdxStart, 0);
		ASSERT_EQ(t.translate->getTrackingData()->childrenIdxEnd, 1);
		ASSERT_EQ(t.lookAt->getTrackingData()->childrenIdxStart, 1);
		ASSERT_EQ(t.lookAt->getTrackingData()->childrenIdxEnd, 2);
		ASSERT_EQ(t.persp->getTrackingData()->childrenIdxStart, 2);
		ASSERT_EQ(t.persp->getTrackingData()->childrenIdxEnd, 4);
	}
}


TEST_F(TrackerTest, SimpleCameraTrackingInsideCamera)
{
	SimpleCamTree t;

	auto* tracker = GraphManager::startTracking(t.c1->getView(), t.c1, TrackingDirection::RightToLeft);

	ASSERT_EQ(tracker->getCameraID(), t.c1->getId());

	std::vector trans = {t.lookAt, t.persp};
	std::vector seqs = {t.c1->getView(), t.c1->getProj()};

	{
		float trackingParam = 1.0f;
		tracker->setProgress(trackingParam);

		EXPECT_EQ(tracker->getFullMatricesCount(), 0);
		EXPECT_EQ(tracker->getMatrixCount(), 3);
		EXPECT_EQ(tracker->getTransformCount(), 2);

		tracker->m_trackInWorldSpace = true;
		tracker->update();

		EXPECT_EQ(tracker->getFullMatricesCount(), 3);
		EXPECT_EQ(tracker->getMatrixCount(), 3);
		EXPECT_EQ(tracker->getTransformCount(), 2);

		auto expected = t.persp->data().getMat4() * t.lookAt->data().getMat4();

		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));
		EXPECT_PRED_FORMAT2(AssertEqualMatrices, expected, tracker->getInterpolatedMatrix());

		// Check internal data of tracker
		std::vector seqIdx = {0, 1};
		std::vector interpolating = {false, true};
		std::vector progress = {1.f, 1.f};
		std::vector chain = {false, false};
		std::vector active = {true, true};
		std::vector modelSubtree = {false, false};
		std::vector isInCamera = {true, true};
		std::vector space = {TransformSpace::View, TransformSpace::Projection};

		std::vector seqSeqIdx = {0, 1};
		std::vector seqInterpolating = {false, true};
		std::vector seqProgress = {1.f, 1.f};
		std::vector seqChain = {true, true};
		std::vector seqActive = {true, true};
		std::vector seqModelSubtree = {false, false};
		std::vector seqIsInCamera = {true, true};
		std::vector seqSpace = {TransformSpace::View, TransformSpace::Projection};

		checkInternalData(tracker, 3, trans, seqs, seqIdx, interpolating, progress, chain, active, space, modelSubtree,
		                  isInCamera, seqSeqIdx, seqInterpolating, seqProgress, seqChain, seqActive, seqSpace,
		                  seqModelSubtree, seqIsInCamera);

		ASSERT_EQ(seqs[0]->getTrackingData()->childrenIdxStart, 0);
		ASSERT_EQ(seqs[0]->getTrackingData()->childrenIdxEnd, 1);
		ASSERT_EQ(seqs[1]->getTrackingData()->childrenIdxStart, 1);
		ASSERT_EQ(seqs[1]->getTrackingData()->childrenIdxEnd, 2);

		ASSERT_EQ(t.lookAt->getTrackingData()->childrenIdxStart, 0);
		ASSERT_EQ(t.lookAt->getTrackingData()->childrenIdxEnd, 1);
		ASSERT_EQ(t.persp->getTrackingData()->childrenIdxStart, 1);
		ASSERT_EQ(t.persp->getTrackingData()->childrenIdxEnd, 3);
	}
}

TEST_F(TrackerTest, SimpleCameraTrackingReferenceSpace)
{
	SimpleCamTree t;

	auto* tracker = GraphManager::startTracking(t.s1, TrackingDirection::RightToLeft);

	std::vector trans = {t.translate, t.lookAt, t.persp};
	std::vector seqs = {t.s1, t.c1->getView(), t.c1->getProj()};

	{
		float trackingParam = 1.0f;
		tracker->setProgress(trackingParam);

		EXPECT_EQ(tracker->getFullMatricesCount(), 0);
		EXPECT_EQ(tracker->getMatrixCount(), 4);
		EXPECT_EQ(tracker->getTransformCount(), 3);

		tracker->update();

		EXPECT_EQ(tracker->getFullMatricesCount(), 4);
		EXPECT_EQ(tracker->getMatrixCount(), 4);
		EXPECT_EQ(tracker->getTransformCount(), 3);

		auto expected = t.translate->data().getMat4();

		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));
		EXPECT_PRED_FORMAT2(AssertEqualMatrices, expected, tracker->getInterpolatedMatrix());

		EXPECT_TRUE(compare(t.persp->data().getMat4(), tracker->m_iProjMatrix));
		EXPECT_PRED_FORMAT2(AssertEqualMatrices, t.persp->data().getMat4(), tracker->m_iProjMatrix);

		EXPECT_TRUE(compare(t.lookAt->data().getMat4(), tracker->m_iViewMatrix));
		EXPECT_PRED_FORMAT2(AssertEqualMatrices, t.lookAt->data().getMat4(), tracker->m_iViewMatrix);

		// Check internal data of tracker
		std::vector seqIdx = {0, 1, 2};
		std::vector interpolating = {false, false, true};
		std::vector progress = {1.f, 1.f, 1.f};
		std::vector chain = {false, false, false};
		std::vector active = {true, true, true};
		std::vector modelSubtree = {false, false, false};
		std::vector isInCamera = {false, true, true};
		std::vector space = {TransformSpace::Model, TransformSpace::View, TransformSpace::Projection};

		std::vector seqSeqIdx = {0, 1, 2};
		std::vector seqInterpolating = {false, false, true};
		std::vector seqProgress = {1.f, 1.f, 1.f};
		std::vector seqChain = {true, true, true};
		std::vector seqActive = {true, true, true};
		std::vector seqModelSubtree = {false, false, false};
		std::vector seqIsInCamera = {false, true, true};
		std::vector seqSpace = {TransformSpace::Model, TransformSpace::View, TransformSpace::Projection};

		checkInternalData(tracker, 4, trans, seqs, seqIdx, interpolating, progress, chain, active, space, modelSubtree,
		                  isInCamera, seqSeqIdx, seqInterpolating, seqProgress, seqChain, seqActive, seqSpace,
		                  seqModelSubtree, seqIsInCamera);

		ASSERT_EQ(seqs[0]->getTrackingData()->childrenIdxStart, 0);
		ASSERT_EQ(seqs[0]->getTrackingData()->childrenIdxEnd, 1);
		ASSERT_EQ(seqs[1]->getTrackingData()->childrenIdxStart, 1);
		ASSERT_EQ(seqs[1]->getTrackingData()->childrenIdxEnd, 2);
		ASSERT_EQ(seqs[2]->getTrackingData()->childrenIdxStart, 2);
		ASSERT_EQ(seqs[2]->getTrackingData()->childrenIdxEnd, 3);

		ASSERT_EQ(t.translate->getTrackingData()->childrenIdxStart, 0);
		ASSERT_EQ(t.translate->getTrackingData()->childrenIdxEnd, 1);
		ASSERT_EQ(t.lookAt->getTrackingData()->childrenIdxStart, 1);
		ASSERT_EQ(t.lookAt->getTrackingData()->childrenIdxEnd, 2);
		ASSERT_EQ(t.persp->getTrackingData()->childrenIdxStart, 2);
		ASSERT_EQ(t.persp->getTrackingData()->childrenIdxEnd, 4);
	}
}

// TODO: Figure out how to handle and test LeftToRight tracking inside the camera