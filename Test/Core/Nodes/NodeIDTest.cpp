#include "gtest/gtest.h"

#include "imgui_node_editor.h"

#include "Core/Nodes/Id.h"

using namespace Core;

void testId(ax::NodeEditor::NodeId id)
{
	// It compiles!
}

TEST(NodeIDTest, IDCanBeConvertedToNodeEditorIDs)
{
	IdGenerator generator;

	auto id = generator.next();

	testId(id);

	ax::NodeEditor::NodeId nodeId = id;

	EXPECT_EQ(nodeId.Get(), id);
}
