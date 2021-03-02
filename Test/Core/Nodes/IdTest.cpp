#include "gtest/gtest.h"

#include "imgui_node_editor.h"

#include "Core/Nodes/Id.h"

using namespace Core;

void TestId(ax::NodeEditor::NodeId id) {
  // It compiles!
}

TEST(CoreIDTest, IDCanBeConvertedToNodeEditorIDs)
{
  auto id = IdGenerator::next();

  TestId(id);

  ax::NodeEditor::NodeId nodeId = id;

  EXPECT_EQ(nodeId.Get(), id);
}
