#include "gtest/gtest.h"

#include "GUI/Elements/Nodes/Tools.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "I3T.h"

// #include "Common.h"

using namespace Core;

TEST(NodeToolsTest, CopyNodes)
{
	I3TApplication app;
	app.init();

	const auto f1 = addNodeToNodeEditor<WorkspaceOperator<EOperatorType::FloatToFloat>>();
	const auto f2 = addNodeToNodeEditor<WorkspaceOperator<EOperatorType::FloatToFloat>>();
	const auto f3 = addNodeToNodeEditor<WorkspaceOperator<EOperatorType::FloatAddFloat>>();
	const auto f4 = addNodeToNodeEditor<WorkspaceOperator<EOperatorType::FloatToFloat>>();

	connectNodes(f1, f2, 0, 0);
	connectNodes(f2, f3, 0, 0);
	connectNodes(f3, f4, 0, 0);

	const auto snapshot = copyNodes({f2, f3});
	pasteNodes(*snapshot);

	const auto workspace = App::getModule<UIModule>().getWindowManager().getWindowPtr<WorkspaceWindow>();
	const auto& workspaceNodes = workspace->getNodeEditor().m_workspaceCoreNodes;
	ASSERT_TRUE(workspaceNodes.size() == 6);

	const auto f5 = workspaceNodes[4];
	const auto f6 = workspaceNodes[5];

	Ptr<GuiNode> valueNode;
	Ptr<GuiNode> addNode;

	if (f5->getNodebase()->getOperation()->isConstructor)
	{
		valueNode = f5;
		addNode = f6;
	}
	else
	{
		valueNode = f6;
		addNode = f5;
	}

	const auto& connectedToFloat = valueNode->getNodebase()->getOutput(0).getOutComponents();
	EXPECT_TRUE(connectedToFloat.size() == 1);
	const auto addNodeInput = addNode->getNodebase()->getInput(0);
	EXPECT_TRUE(addNodeInput.isPluggedIn());
	EXPECT_EQ(addNodeInput.getParentPin()->getOwner()->getId(), valueNode->getNodebase()->getId());
}
