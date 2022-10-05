#include "gtest/gtest.h"

#include "Core/Nodes/Operations.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

#include "Common.h"
#include "Core/Nodes/Utils.h"
#include "Generator.h"

/// Needs to be read carefully!
TEST(UndoRedoTest, Basic)
{
	createTestApplication();

	const auto workspace = I3T::getUI()->getWindowPtr<WorkspaceWindow>();
	ASSERT_TRUE(workspace != nullptr);
	const auto& nodes = workspace->getNodeEditor().getAllNodes();
	ASSERT_TRUE(nodes.empty());

	addNodeToNodeEditor<WorkspaceOperator<ENodeType::FloatToFloat>>();
	const float newValue = 10.0f;
	setValue_expectOk(nodes[0]->getNodebase(), newValue);
	StateManager::instance().takeSnapshot();
	EXPECT_TRUE(StateManager::instance().canUndo());
	EXPECT_FALSE(StateManager::instance().canRedo());
	EXPECT_TRUE(StateManager::instance().getMementosCount() == 1 + 2);
	EXPECT_TRUE(!nodes.empty());

	StateManager::instance().undo(); // set node to initial value
	StateManager::instance()
	    .undo(); // set to the initial state (without any nodes)
	EXPECT_FALSE(StateManager::instance().canUndo());
	EXPECT_TRUE(StateManager::instance().canRedo());
	EXPECT_TRUE(nodes.empty());

	StateManager::instance().redo();
	EXPECT_TRUE(nodes.size() == 1);

	setValue_expectOk(nodes[0]->getNodebase(), newValue);
	StateManager::instance().takeSnapshot();
	EXPECT_FALSE(StateManager::instance().canRedo());

	addNodeToNodeEditor<WorkspaceOperator<ENodeType::FloatToFloat>>();

	connectNodes(nodes[0], nodes[1], 0, 0);
	EXPECT_EQ(nodes[1]->getNodebase()->getData().getFloat(), newValue);
	EXPECT_TRUE(nodes[1]->getNodebase()->getIn(0).isPluggedIn());

	StateManager::instance().undo();
	EXPECT_EQ(nodes[1]->getNodebase()->getData().getFloat(), 0.0f);
	EXPECT_FALSE(nodes[1]->getNodebase()->getIn(0).isPluggedIn());

	StateManager::instance().redo();
	EXPECT_EQ(nodes[1]->getNodebase()->getData().getFloat(), newValue);
	EXPECT_TRUE(nodes[1]->getNodebase()->getIn(0).isPluggedIn());

	//

	int i = 0;
	int iExpected = StateManager::instance().getMementosCount() - 1;
	while (StateManager::instance().canUndo())
	{
		StateManager::instance().undo();
		++i;
	}
	EXPECT_EQ(iExpected, i);

	EXPECT_TRUE(nodes.empty());

	//

	while (StateManager::instance().canRedo())
	{
		StateManager::instance().redo();
	}

	EXPECT_TRUE(nodes.size() == 2);
	EXPECT_TRUE(nodes[1]->getNodebase()->getData().getFloat() == newValue);
	EXPECT_TRUE(nodes[1]->getNodebase()->getIn(0).isPluggedIn());

	addNodeToNodeEditor<WorkspaceOperator<ENodeType::MatrixToMatrix>>();
	const auto mat = generateMat4();
	setValue_expectOk(nodes[2]->getNodebase(), mat);
	StateManager::instance().takeSnapshot();

	StateManager::instance().undo();
	StateManager::instance().redo();
	EXPECT_TRUE(Math::eq(mat, nodes[2]->getNodebase()->getData().getMat4()));

	destroyTestApplication();
}
