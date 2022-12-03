#include "gtest/gtest.h"

#include "glm/gtx/matrix_interpolation.hpp"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"

using namespace Core;

struct MyTree
{
	SequencePtr s1;
	SequencePtr s2;

	TransformPtr mat1;
	TransformPtr mat2;
	TransformPtr mat3;
	TransformPtr mat4;
};

MyTree arrange()
{
	auto ret = MyTree{
	    Builder::createSequence(),
	    Builder::createSequence(),
	    Builder::createTransform<ETransformType::Translation>(),
	    Builder::createTransform<ETransformType::Translation>(),
	    Builder::createTransform<ETransformType::Translation>(),
	    Builder::createTransform<ETransformType::Translation>(),
	};

	ret.s1->addMatrix(ret.mat1);
	ret.s1->addMatrix(ret.mat2);

	ret.s2->addMatrix(ret.mat3);
	ret.s2->addMatrix(ret.mat4);

	plug_expectOk(ret.s1, ret.s2);

	return ret;
}

/**
 * /////////     /////////
 * | M | M | --- | M | M |
 * /////////     /////////
 */
TEST(TrackerTest, TrackingFromRightToLeft)
{
	auto t = arrange();

	// Act
	auto interp = MatrixTracker(t.s2);

	{
		float trackingParam = 1.0f;
		interp.setParam(trackingParam);

		auto expected = glm::mat4(1.0f);

		EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
	}
	{
		float trackingParam = 0.85f;
		interp.setParam(trackingParam);

		float interpParam = 1.0f - fmod(trackingParam * 4, 3.0f);
		auto expected = glm::interpolate(glm::mat4(1.0f), t.mat4->getData().getMat4(), interpParam);

		EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
	}
	{
		float trackingParam = 0.30f;
		interp.setParam(trackingParam);

		float interpParam = 1.0f - fmod(trackingParam * 4, 1.0f);
		auto expected = glm::interpolate(glm::mat4(1.0f), t.mat2->getData().getMat4(), interpParam) *
		                t.mat3->getData().getMat4() * t.mat4->getData().getMat4();

		EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
	}
	{
		float trackingParam = 0.0f;
		interp.setParam(trackingParam);

		auto expected = t.mat1->getData().getMat4() * t.mat2->getData().getMat4() * t.mat3->getData().getMat4() *
		                t.mat4->getData().getMat4();

		EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
	}
}

// Reversed tracking.
TEST(TrackerTest, TrackingFromLeftToRight)
{
	auto t = arrange();

	// Act
	auto interp = MatrixTracker(t.s2);
	interp.setMode(true);

	{
		float trackingParam = 1.0f;
		interp.setParam(trackingParam);

		auto expected = t.mat1->getData().getMat4() * t.mat2->getData().getMat4() * t.mat3->getData().getMat4() *
		                t.mat4->getData().getMat4();

		EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
	}
	{
		float trackingParam = 0.85f;
		interp.setParam(trackingParam);

		float interpParam = (abs(trackingParam) - 0.75f) * 4;
		auto expected = t.mat1->getData().getMat4() * t.mat2->getData().getMat4() * t.mat3->getData().getMat4() *
		                glm::interpolate(glm::mat4(1.0f), t.mat4->getData().getMat4(), interpParam);

		EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
	}
	{
		float trackingParam = 0.30f;
		interp.setParam(trackingParam);

		float interpParam = (abs(trackingParam) - 0.25f) * 4;
		auto expected =
		    t.mat1->getData().getMat4() * glm::interpolate(glm::mat4(1.0f), t.mat2->getData().getMat4(), interpParam);

		EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
	}
	{
		float trackingParam = 0.0f;
		interp.setParam(trackingParam);

		auto expected = glm::mat4(1.0f);

		EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
	}
}

TEST(TrackerTest, EmptySequenceTrackingShouldGiveSameResult)
{
	auto s = Builder::createSequence();

	auto tracker = MatrixTracker(s);

	for (int i = 0; i < 5; ++i)
	{
		tracker.setParam(generateFloat(0.0f, 1.0f));
		EXPECT_TRUE(Math::eq(glm::mat4(1.0f), tracker.getInterpolatedMatrix()));
	}
}
