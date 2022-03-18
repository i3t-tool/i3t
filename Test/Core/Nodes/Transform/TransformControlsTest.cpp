#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Core/Nodes/Utils.h"
#include "Generator.h"

using namespace Core;

TEST(TransformControlsTest, _)
{
	auto translation = GraphManager::createTransform<ETransformType::Translation>();

	translation->setDefaultValue("translation", generateVec3());
	auto expectedValues = translation->getData().getMat4();

	translation->saveValues();

	translation->unlock();
	setValue_expectOk(translation, 5.0f, { 1, 1 });

	translation->reloadValues();

	EXPECT_TRUE(Math::eq(expectedValues, translation->getData().getMat4()));
}
