#include "gtest/gtest.h"

#include "glm/gtx/matrix_interpolation.hpp"

#include "Core/Nodes/GraphManager.h"

#include "Generator.h"
#include "Utils.h"

using namespace Core;

/**
 * /////////     /////////
 * | M | M | --- | M | M |
 * /////////     /////////
 */
TEST(TrackerTest, _)
{
	auto s1 = Builder::createSequence();
	auto s2 = Builder::createSequence();

	auto translation1 = Builder::createTransform<Translation>(generateVec3());
	auto translation2 = Builder::createTransform<Translation>(generateVec3());
	auto translation3 = Builder::createTransform<Translation>(generateVec3());
	auto translation4 = Builder::createTransform<Translation>(generateVec3());

	s1->addMatrix(translation1);
	s1->addMatrix(translation2);

	s2->addMatrix(translation3);
	s2->addMatrix(translation4);

	plug_expectOk(s1, s2);

	float trackingParam = 0.85f;

	auto interp = MatrixTracker(s2);
	interp.setParam(trackingParam);

	auto lhs = translation1->getData().getMat4() * translation2->getData().getMat4() * translation3->getData().getMat4();

	float interpParam = fmod(trackingParam * 4, 3);
	auto expected = lhs * glm::interpolate(glm::mat4(1.0f), translation4->getData().getMat4(), interpParam);

	EXPECT_TRUE(Math::eq(expected, interp.getInterpolatedMatrix()));
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
