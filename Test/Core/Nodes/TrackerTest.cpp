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

class DummyModelProxy : public Core::IModelProxy
{
public:
	DummyModelProxy()
	{
		m_model = GraphManager::createModel();
	}
	~DummyModelProxy() override = default;

	void update(const glm::mat4& transform) override
	{
		m_model->setValue(transform);
	};
	Ptr<Core::Model> getModel() override
	{
		return m_model;
	};

private:
	Ptr<Model> m_model;
};

class TrackerTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		GraphManager::init();
	}

	void TearDown() override
	{
		GraphManager::destroy();
	}
};

/**
 * Classic tracking.
 *
 * ///////////     ///////////
 * | M1 | M2 | --- | M3 | M4 |
 * ///////////     ///////////
 */
TEST_F(TrackerTest, TrackingRightToLeft)
{
	auto t = arrange();

	// Act
	auto tracker = t.s2->startTracking(std::make_unique<DummyModelProxy>());

	{
		float trackingParam = 1.0f;
		tracker->setParam(trackingParam);

		auto expected = t.mat1->getData().getMat4() * t.mat2->getData().getMat4() * t.mat3->getData().getMat4() *
		                t.mat4->getData().getMat4();

		EXPECT_EQ(tracker->fullMatricesCount(), 4);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));
		EXPECT_PRED_FORMAT2(AssertEqualMatrices, expected, tracker->getInterpolatedMatrix());

		EXPECT_FLOAT_EQ(t.mat1->getActivePart(), 1.0f);
		EXPECT_FLOAT_EQ(t.mat2->getActivePart(), 1.0f);
		EXPECT_FLOAT_EQ(t.mat3->getActivePart(), 1.0f);
		EXPECT_FLOAT_EQ(t.mat4->getActivePart(), 1.0f);
	}
	{
		float trackingParam = 0.85f;
		tracker->setParam(trackingParam);

		float interpParam = (abs(trackingParam) - 0.75f) * 4;
		auto expected = glm::interpolate(glm::mat4(1.0f), t.mat1->getData().getMat4(), interpParam) *
		                t.mat2->getData().getMat4() * t.mat3->getData().getMat4() * t.mat4->getData().getMat4();

		EXPECT_EQ(tracker->fullMatricesCount(), 3);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getActivePart(), interpParam);
		EXPECT_FLOAT_EQ(t.mat2->getActivePart(), 1.0f);
		EXPECT_FLOAT_EQ(t.mat3->getActivePart(), 1.0f);
		EXPECT_FLOAT_EQ(t.mat4->getActivePart(), 1.0f);
	}
	{
		float trackingParam = 0.30f;
		tracker->setParam(trackingParam);

		float interpParam = (abs(trackingParam) - 0.25f) * 4;
		auto expected =
		    glm::interpolate(glm::mat4(1.0f), t.mat3->getData().getMat4(), interpParam) * t.mat4->getData().getMat4();

		EXPECT_EQ(tracker->fullMatricesCount(), 1);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getActivePart(), 0.0f);
		EXPECT_FLOAT_EQ(t.mat2->getActivePart(), 0.0f);
		EXPECT_FLOAT_EQ(t.mat3->getActivePart(), interpParam);
		EXPECT_FLOAT_EQ(t.mat4->getActivePart(), 1.0f);
	}
	{
		float trackingParam = 0.0f;
		tracker->setParam(trackingParam);

		auto expected = glm::mat4(1.0f);

		EXPECT_EQ(tracker->fullMatricesCount(), 0);
		EXPECT_TRUE(compare(expected, tracker->getInterpolatedMatrix()));

		EXPECT_FLOAT_EQ(t.mat1->getActivePart(), 0.0f);
		EXPECT_FLOAT_EQ(t.mat2->getActivePart(), 0.0f);
		EXPECT_FLOAT_EQ(t.mat3->getActivePart(), 0.0f);
		EXPECT_FLOAT_EQ(t.mat4->getActivePart(), 0.0f);
	}
}

TEST_F(TrackerTest, TrackedModelIsUpdatedOnSequenceChange)
{
	auto sequence = GraphManager::createSequence();
	sequence->pushMatrix(Builder::createTransform<ETransformType::Translation>());

	// start full tracking
	auto tracker = sequence->startTracking(std::make_unique<DummyModelProxy>());
	tracker->setParam(1.0f);

	// add new matrix
	auto mat = Builder::createTransform<ETransformType::Translation>();
	mat->setDefaultValue("translation", generateVec3());
	sequence->pushMatrix(mat);

	EXPECT_TRUE(compare(mat->getData().getMat4(), tracker->getInterpolatedMatrix()));
	EXPECT_TRUE(compare(mat->getData().getMat4(), tracker->getModel()->getData().getMat4()));
}

TEST_F(TrackerTest, TrackingIsDisabledAfterSequenceRemoval)
{
	EXPECT_FALSE(GraphManager::isTrackingEnabled());
	{
		auto sequence = GraphManager::createSequence();
		auto mat = Builder::createTransform<ETransformType::Translation>();
		mat->setDefaultValue("translation", generateVec3());
		sequence->pushMatrix(mat);

		// start full tracking
		auto tracker = sequence->startTracking(std::make_unique<DummyModelProxy>());
		tracker->setParam(1.0f);

		EXPECT_TRUE(GraphManager::isTrackingEnabled());

		EXPECT_TRUE(compare(mat->getData().getMat4(), tracker->getInterpolatedMatrix()));
	}

	EXPECT_FALSE(GraphManager::isTrackingEnabled());
}

TEST_F(TrackerTest, EmptySequence)
{
	auto sequence = GraphManager::createSequence();
	sequence->pushMatrix(Builder::createTransform<ETransformType::Translation>());
	sequence->pushMatrix(Builder::createTransform<ETransformType::Translation>());

	// start full tracking
	auto tracker = sequence->startTracking(std::make_unique<DummyModelProxy>());
	tracker->setParam(1.0f);

	sequence->popMatrix(0);
	sequence->popMatrix(0);

	tracker->setParam(1.0f);
}
