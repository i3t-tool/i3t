#include "gtest/gtest.h"

#include "imgui_node_editor.h"

#include "Core/Nodes/Id.h"

using namespace Core;

void testId(ax::NodeEditor::NodeId id) {
  // It compiles!
}

TEST(NodeIdTest, IDCanBeConvertedToNodeEditorIDs)
{
  auto id = IdGenerator::next();

  testId(id);

  ax::NodeEditor::NodeId nodeId = id;

  EXPECT_EQ(nodeId.Get(), id);
}
