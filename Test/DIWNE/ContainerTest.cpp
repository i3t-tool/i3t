#include "gtest/gtest.h"

#include "DIWNE/diwne_include.h"

#include "DIWNE/Basic/BasicNode.h"
#include "DIWNE/Basic/SequenceNodeContainer.h"

#include "Util/DiwneTestApp.h"

using namespace DIWNE;

class ContainerTest : public ::testing::Test
{
public:
	DIWNE::SettingsDiwne settings;
	std::shared_ptr<NodeEditor> editor;

	std::unique_ptr<DiwneTestApp> app;

	void SetUp() override
	{
		editor = std::make_shared<NodeEditor>("NodeEditor", &settings);
		app = std::make_unique<DiwneTestApp>();
		app->create(editor.get());
	}

	void TearDown() override
	{
		app->destroy();
	}
};

TEST_F(ContainerTest, BasicTest)
{
	// TODO: Node being added to mulitple containers at the same time (multiple drop zones)
	//  Only one drop zone (the last one) must end up with the node

	// TODO: Deletion of a node that was just inserted into a drop zone
	//  Eg. we drop a node and immediately press delete, the node is marked for removal from node editor
	//  by the drop zone but is also marked for destruction by the delete key.
	//  In the end the node must be removed from the editor and drop zone

	// TODO: Removal of node (->remove()) NEVER cause it to deallocate UNLESS ->destroy() was called as well

	// TODO: Deletion of nodes while dragging
	//  The nodes that are being dragged must be valid objects, right now drag action keeps raw pointers that can
	//  become invalid, thats unacceptable -> use shared or weak ptrs and test for destruction

	// TODO: Deletion of drag and action source
	//  The drag source cannot be deleted or become invisible (handed already just have to test) without ending
	//  the drag / action.

	// TODO: I3T core node lifecycle, CoreNode destructor calls finalize() on core node, it should be done in onDestroy!
}

TEST_F(ContainerTest, BasicDropZoneTest)
{
	auto node1 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 1");
	editor->addNode(node1, ImVec2(0, 0));
	auto node2 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 2");
	editor->addNode(node2, ImVec2(100, 0));
	auto node3 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 3");
	editor->addNode(node3, ImVec2(200, 0));
	auto node4 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 4");
	editor->addNode(node4, ImVec2(0, 100));

	ASSERT_EQ(editor->getNodeList().size(), 4);

	auto node5 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 5");
	auto node6 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 6");

	auto sequence1 = std::make_shared<DIWNE::SequenceNodeContainer>(*editor);
	sequence1->getDropZone().addNode(node5);
	sequence1->getDropZone().addNode(node6);

	ASSERT_EQ(editor->getNodeList().size(), 4);

	editor->addNode(sequence1, ImVec2(100, 100));

	ASSERT_EQ(editor->getNodeList().size(), 5);
	ASSERT_EQ(sequence1->getNodeList().size(), 2);

	app->frame();

	sequence1->getDropZone().addNode(node4);

	ASSERT_EQ(editor->getNodeList().size(), 5);
	ASSERT_EQ(sequence1->getNodeList().size(), 3);

	app->frame(); // Auto purge node4

	ASSERT_EQ(editor->getNodeList().size(), 4);
	ASSERT_EQ(sequence1->getNodeList().size(), 3);

	node4->destroy();

	ASSERT_EQ(editor->getNodeList().size(), 4);
	ASSERT_EQ(sequence1->getNodeList().size(), 3);

	app->frame();

	ASSERT_EQ(editor->getNodeList().size(), 4);
	ASSERT_EQ(sequence1->getNodeList().size(), 2);

	sequence1->getDropZone().removeNodeAt(0);
	ASSERT_TRUE(sequence1->getNodeList()[0] == node6);
	sequence1->getDropZone().removeNode(node6);
	ASSERT_EQ(sequence1->getNodeList().size(), 0);

	ASSERT_EQ(editor->getNodeList().size(), 6); // Node 5 and 6 get immediately moved back to editor
	ASSERT_EQ(editor->getAllNodesInnerIncluded().collectRaw().size(), 6);

	app->frame();

	ASSERT_EQ(editor->getNodeList().size(), 6);
	ASSERT_EQ(editor->getAllNodesInnerIncluded().collectRaw().size(), 6);

	editor->removeNode(node6);
	editor->removeNodeAt(0);

	ASSERT_EQ(editor->getNodeList().size(), 4);
	ASSERT_EQ(editor->getAllNodesInnerIncluded().collectRaw().size(), 4);
}

TEST_F(ContainerTest, DestroyNodeWhileAddingItToDropZone)
{
	auto node1 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 1");
	editor->addNode(node1, ImVec2(0, 0));
	auto node2 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 2");
	editor->addNode(node2, ImVec2(100, 0));
	auto node3 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 3");
	editor->addNode(node3, ImVec2(200, 0));
	auto node4 = std::make_shared<DIWNE::BasicNode>(*editor, "Node 4");
	editor->addNode(node4, ImVec2(0, 100));

	ASSERT_EQ(editor->getNodeList().size(), 4);

	auto sequence1 = std::make_shared<DIWNE::SequenceNodeContainer>(*editor);
	editor->addNode(sequence1, ImVec2(100, 100));

	ASSERT_EQ(editor->getNodeList().size(), 5);
	ASSERT_EQ(sequence1->getNodeList().size(), 0);

	sequence1->getDropZone().addNode(node4);

	ASSERT_EQ(editor->getNodeList().size(), 5);
	ASSERT_EQ(sequence1->getNodeList().size(), 1);

	app->frame(); // Simulate one frame to allow automatic object purge

	ASSERT_EQ(editor->getNodeList().size(), 4);
	ASSERT_EQ(sequence1->getNodeList().size(), 1);
	ASSERT_TRUE(sequence1->getNodeList()[0] == node4);

	node3->destroy();
	sequence1->getDropZone().addNode(node3);
	ASSERT_EQ(sequence1->getNodeList().size(), 2);
	ASSERT_TRUE(sequence1->getNodeList()[0] == node4);
	ASSERT_TRUE(sequence1->getNodeList()[1] == node3);

	ASSERT_EQ(editor->getNodeList().size(), 4);

	sequence1->getDropZone().addNode(node2);
	node2->destroy();
	ASSERT_EQ(sequence1->getNodeList().size(), 3);
	ASSERT_TRUE(sequence1->getNodeList()[0] == node4);
	ASSERT_TRUE(sequence1->getNodeList()[1] == node3);
	ASSERT_TRUE(sequence1->getNodeList()[2] == node2);
	ASSERT_EQ(editor->getNodeList().size(), 4);

	app->frame(); // Nodes 2 and 3 get purged

	ASSERT_EQ(editor->getNodeList().size(), 2);
	ASSERT_EQ(sequence1->getNodeList().size(), 1); // Only n4 remains
	ASSERT_TRUE(sequence1->getNodeList()[0] == node4);

	ASSERT_EQ(editor->getAllNodesInnerIncluded().collectRaw().size(), 3);
}
