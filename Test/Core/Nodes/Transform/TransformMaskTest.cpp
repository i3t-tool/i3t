#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

TEST(TransformMaskTest, _)
{
	auto scale = Core::Builder::createTransform<ETransformType::Scale>();

	scale->lock();
	scale->enableSynergies();
	EXPECT_EQ(Core::EValueState::EditableSyn, scale->getValueState({0, 0}));
	EXPECT_EQ(Core::EValueState::EditableSyn, scale->getValueState({1, 1}));
	EXPECT_EQ(Core::EValueState::EditableSyn, scale->getValueState({2, 2}));
	EXPECT_EQ(Core::EValueState::Locked, scale->getValueState({3, 3}));

	scale->disableSynergies();
	EXPECT_EQ(Core::EValueState::Editable, scale->getValueState({0, 0}));
	EXPECT_EQ(Core::EValueState::Editable, scale->getValueState({1, 1}));
	EXPECT_EQ(Core::EValueState::Editable, scale->getValueState({2, 2}));
	EXPECT_EQ(Core::EValueState::Locked, scale->getValueState({3, 3}));

	scale->unlock();
	EXPECT_EQ(Core::EValueState::Editable, scale->getValueState({3, 3}));
}
