#include "gtest/gtest.h"

#include "State/StateManager.h"
#include "State/Stateful.h"

class TestWorkspace : public IStateful
{
public:
	Memento getState() override
	{
		State state;
		return Memento{state};
	}

	void setState(const Memento& state) override {}

	void onStateChange(const std::string& winTitlePostfix) override {}
};

/// \todo MH StateHandlingTest
/*
TEST(StateHandlingTest, UndoRedo)
{
    TestWorkspace workspace;

    StateManager::instance().setOriginator(&workspace);
    StateManager::instance().createEmptyScene();

    EXPECT_FALSE(StateManager::instance().canUndo());
    EXPECT_FALSE(StateManager::instance().canRedo());

    StateManager::instance().takeSnapshot();

    EXPECT_TRUE(StateManager::instance().canUndo());
    EXPECT_FALSE(StateManager::instance().canRedo());

    StateManager::instance().undo();

    EXPECT_FALSE(StateManager::instance().canUndo());
    EXPECT_TRUE(StateManager::instance().canRedo());

    StateManager::instance().redo();

    EXPECT_TRUE(StateManager::instance().canUndo());
    EXPECT_FALSE(StateManager::instance().canRedo());
}
 */
