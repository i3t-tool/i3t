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

	const auto workspace = I3T::getWindowPtr<WorkspaceWindow>();
	ASSERT_TRUE(workspace != nullptr);
	const auto& nodes = workspace->getNodeEditor().getAllNodes();
	ASSERT_TRUE(nodes.empty());

	addNodeToNodeEditor<WorkspaceOperator<EOperatorType::FloatToFloat>>();
	const float newValue = 10.0f;
	setValue_expectOk(nodes[0]->getNodebase(), newValue);
	App::getModule<StateManager>().takeSnapshot();
	EXPECT_TRUE(App::getModule<StateManager>().canUndo());
	EXPECT_FALSE(App::getModule<StateManager>().canRedo());
	EXPECT_TRUE(App::getModule<StateManager>().getMementosCount() == 1 + 2);
	EXPECT_TRUE(!nodes.empty());

	// set node to initial value
	App::getModule<StateManager>().undo();
	// set to the initial state (without any nodes)
	App::getModule<StateManager>().undo();
	EXPECT_FALSE(App::getModule<StateManager>().canUndo());
	EXPECT_TRUE(App::getModule<StateManager>().canRedo());
	EXPECT_TRUE(nodes.empty());

	App::getModule<StateManager>().redo();
	EXPECT_TRUE(nodes.size() == 1);

	setValue_expectOk(nodes[0]->getNodebase(), newValue);
	App::getModule<StateManager>().takeSnapshot();
	EXPECT_FALSE(App::getModule<StateManager>().canRedo());

	addNodeToNodeEditor<WorkspaceOperator<EOperatorType::FloatToFloat>>();

	connectNodes(nodes[0], nodes[1], 0, 0);
	EXPECT_EQ(nodes[1]->getNodebase()->getData().getFloat(), newValue);
	EXPECT_TRUE(nodes[1]->getNodebase()->getInput(0).isPluggedIn());

	App::getModule<StateManager>().undo();
	EXPECT_EQ(nodes[1]->getNodebase()->getData().getFloat(), 0.0f);
	EXPECT_FALSE(nodes[1]->getNodebase()->getInput(0).isPluggedIn());

	App::getModule<StateManager>().redo();
	EXPECT_EQ(nodes[1]->getNodebase()->getData().getFloat(), newValue);
	EXPECT_TRUE(nodes[1]->getNodebase()->getInput(0).isPluggedIn());

	//

	int i = 0;
	int iExpected = App::getModule<StateManager>().getMementosCount() - 1;
	while (App::getModule<StateManager>().canUndo())
	{
		App::getModule<StateManager>().undo();
		++i;
	}
	EXPECT_EQ(iExpected, i);

	EXPECT_TRUE(nodes.empty());

	//

	while (App::getModule<StateManager>().canRedo())
	{
		App::getModule<StateManager>().redo();
	}

	EXPECT_TRUE(nodes.size() == 2);
	EXPECT_TRUE(nodes[1]->getNodebase()->getData().getFloat() == newValue);
	EXPECT_TRUE(nodes[1]->getNodebase()->getInput(0).isPluggedIn());

	addNodeToNodeEditor<WorkspaceOperator<EOperatorType::MatrixToMatrix>>();
	const auto mat = generateMat4();
	setValue_expectOk(nodes[2]->getNodebase(), mat);
	App::getModule<StateManager>().takeSnapshot();

	App::getModule<StateManager>().undo();
	App::getModule<StateManager>().redo();
	EXPECT_TRUE(Math::eq(mat, nodes[2]->getNodebase()->getData().getMat4()));

	destroyTestApplication();
}
