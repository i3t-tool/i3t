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

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/Workspace/Tools.h"
#include "I3T.h"

#include "I3TUtil.h"

using namespace Core;

TEST(NodeToolsTest, CopyNodes)
{
	auto app = initI3T();

	const auto f1 = Workspace::addNodeToNodeEditor<Workspace::Operator<EOperatorType::FloatToFloat>>();
	const auto f2 = Workspace::addNodeToNodeEditor<Workspace::Operator<EOperatorType::FloatToFloat>>();
	const auto f3 = Workspace::addNodeToNodeEditor<Workspace::Operator<EOperatorType::FloatAddFloat>>();
	const auto f4 = Workspace::addNodeToNodeEditor<Workspace::Operator<EOperatorType::FloatToFloat>>();

	connectNodes(f1, f2, 0, 0);
	connectNodes(f2, f3, 0, 0);
	connectNodes(f3, f4, 0, 0);

	const auto snapshot = Workspace::Tools::copyNodes({f2, f3});
	Workspace::Tools::pasteNodes(*snapshot);

	const auto workspace = App::getModule<UIModule>().getWindowManager().getWindowPtr<WorkspaceWindow>();
	const auto& workspaceNodes = workspace->getNodeEditor().getAllCoreNodes().collect();
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
