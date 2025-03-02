#include "gtest/gtest.h"

#include <algorithm>
#include <functional>

#include "I3T.h"
#include "I3TUtil.h"

#include "DIWNE/Basic/BasicNode.h"
#include "DIWNE/Basic/BasicNodeContainer.h"
#include "DIWNE/diwne_include.h"

using namespace DIWNE;

TEST(IteratorsTest, NodeRangeTest)
{
	auto app = initI3T(spdlog::level::warn);

	DIWNE::SettingsDiwne settings;
	NodeEditor editor(&settings);
	auto node1 = std::make_shared<DIWNE::BasicNode>(editor, "Node 1");
	editor.addNode(node1, ImVec2(0, 0));
	auto node2 = std::make_shared<DIWNE::BasicNode>(editor, "Node 2");
	editor.addNode(node2, ImVec2(100, 0));
	auto node3 = std::make_shared<DIWNE::BasicNode>(editor, "Node 3");
	editor.addNode(node3, ImVec2(200, 0));
	auto node4 = std::make_shared<DIWNE::BasicNode>(editor, "Node 4");
	editor.addNode(node4, ImVec2(0, 100));

	const int nodeCount = 4;

	ASSERT_TRUE(editor.getNodeList().size() == nodeCount);

	{
		std::vector<std::reference_wrapper<Node>> nodeRefs;
		std::vector<Node*> nodePtrs;
		std::vector<std::shared_ptr<Node>> nodeSharedPtrs;

		for (auto& node : editor.getNodes())
		{
			nodeRefs.push_back(node);
		}
		ASSERT_TRUE(nodeRefs.size() == nodeCount);
		nodeRefs.clear();

		NodeRange nodeRange = editor.getNodes();
		for (auto node = nodeRange.begin(); node != nodeRange.end(); node++)
		{
			ASSERT_TRUE(node.ptr() == node.operator->());
			nodePtrs.push_back(node.ptr());
			ASSERT_TRUE(&node.dereference() == &node.operator*());
			nodeRefs.push_back(node.dereference());
			nodeSharedPtrs.push_back(node.sharedPtr());
		}
		ASSERT_TRUE(nodeRefs.size() == nodePtrs.size());
		ASSERT_TRUE(nodeRefs.size() == nodeSharedPtrs.size());
		ASSERT_TRUE(nodeRefs.size() == nodeCount);

		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node1.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node4.get()) != nodePtrs.end());

		int counter = 0;
		for (auto node = nodeRange.begin(); node != nodeRange.end(); ++node)
			counter++;
		ASSERT_TRUE(counter == nodeCount);

		std::vector<Node*> nodePtrs2;
		std::vector<std::shared_ptr<Node>> nodeSharedPtrs2;

		nodeSharedPtrs2 = nodeRange.collect();
		ASSERT_TRUE(nodeSharedPtrs.size() == nodeCount);
		nodePtrs2 = editor.getNodes().collectRaw();
		ASSERT_TRUE(nodePtrs.size() == nodeCount);

		ASSERT_TRUE(nodePtrs2 == nodePtrs);
		ASSERT_TRUE(nodeSharedPtrs2 == nodeSharedPtrs);
	}
}

TEST(IteratorsTest, RecursiveNodeRangeTest)
{
	auto app = initI3T(spdlog::level::warn);

	DIWNE::SettingsDiwne settings;
	NodeEditor editor(&settings);
	auto node1 = std::make_shared<DIWNE::BasicNode>(editor, "Node 1");
	editor.addNode(node1, ImVec2(0, 0));
	auto node2 = std::make_shared<DIWNE::BasicNode>(editor, "Node 2");
	editor.addNode(node2, ImVec2(100, 0));
	auto node3 = std::make_shared<DIWNE::BasicNode>(editor, "Node 3");
	editor.addNode(node3, ImVec2(200, 0));
	auto node4 = std::make_shared<DIWNE::BasicNode>(editor, "Node 4");
	editor.addNode(node4, ImVec2(0, 100));

	auto node5 = std::make_shared<DIWNE::BasicNode>(editor, "Node 5");
	auto node6 = std::make_shared<DIWNE::BasicNode>(editor, "Node 6");

	auto container1 = std::make_shared<DIWNE::BasicNodeContainer>(editor, "Container 1");
	container1->addNode(node5);
	editor.addNode(container1, ImVec2(100, 100));

	auto container2 = std::make_shared<DIWNE::BasicNodeContainer>(editor, "Container 2");
	container2->addNode(node6);
	editor.addNode(container2, ImVec2(200, 100));

	const int nodeCount = 6;
	const int allNodeCount = 8;

	ASSERT_TRUE(editor.getNodeList().size() == nodeCount);
	ASSERT_TRUE(container1->getNodes().collectRaw().size() == 1);
	ASSERT_TRUE(container2->getNodes().collect().size() == 1);
	{
		std::vector<std::reference_wrapper<Node>> nodeRefs;
		std::vector<Node*> nodePtrs;
		std::vector<std::shared_ptr<Node>> nodeSharedPtrs;

		for (auto& node : editor.getNodes())
		{
			nodeRefs.push_back(node);
		}
		ASSERT_TRUE(nodeRefs.size() == nodeCount);
		nodeRefs.clear();

		for (auto& node : editor.getAllNodesInnerIncluded())
		{
			nodeRefs.push_back(node);
		}
		ASSERT_TRUE(nodeRefs.size() == allNodeCount);
		nodeRefs.clear();

		RecursiveNodeRange nodeRange = editor.getAllNodesInnerIncluded();
		for (auto node = nodeRange.begin(); node != nodeRange.end(); node++)
		{
			ASSERT_TRUE(node.ptr() == node.operator->());
			nodePtrs.push_back(node.ptr());
			ASSERT_TRUE(&node.dereference() == &node.operator*());
			nodeRefs.push_back(node.dereference());
			nodeSharedPtrs.push_back(node.sharedPtr());
		}
		ASSERT_TRUE(nodeRefs.size() == nodePtrs.size());
		ASSERT_TRUE(nodeRefs.size() == nodeSharedPtrs.size());
		ASSERT_TRUE(nodeRefs.size() == allNodeCount);

		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node5.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node6.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), container1.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), container2.get()) != nodePtrs.end());

		int counter = 0;
		for (auto node = nodeRange.begin(); node != nodeRange.end(); ++node)
			counter++;
		ASSERT_TRUE(counter == allNodeCount);

		std::vector<Node*> nodePtrs2;
		std::vector<std::shared_ptr<Node>> nodeSharedPtrs2;

		nodeSharedPtrs2 = nodeRange.collect();
		ASSERT_TRUE(nodeSharedPtrs.size() == allNodeCount);
		nodePtrs2 = editor.getAllNodesInnerIncluded().collectRaw();
		ASSERT_TRUE(nodePtrs.size() == allNodeCount);

		ASSERT_TRUE(nodePtrs2 == nodePtrs);
		ASSERT_TRUE(nodeSharedPtrs2 == nodeSharedPtrs);
	}
}

TEST(IteratorsTest, FilteredRecursiveNodeRangeTest)
{
	auto app = initI3T(spdlog::level::warn);

	DIWNE::SettingsDiwne settings;
	NodeEditor editor(&settings);
	auto node1 = std::make_shared<DIWNE::BasicNode>(editor, "Node 1");
	editor.addNode(node1, ImVec2(0, 0));
	auto node2 = std::make_shared<DIWNE::BasicNode>(editor, "Node 2");
	editor.addNode(node2, ImVec2(100, 0));
	auto node3 = std::make_shared<DIWNE::BasicNode>(editor, "Node 3");
	editor.addNode(node3, ImVec2(200, 0));
	auto node4 = std::make_shared<DIWNE::BasicNode>(editor, "Node 4");
	editor.addNode(node4, ImVec2(0, 100));

	auto node5 = std::make_shared<DIWNE::BasicNode>(editor, "Node 5");
	auto node6 = std::make_shared<DIWNE::BasicNode>(editor, "Node 6");

	auto container1 = std::make_shared<DIWNE::BasicNodeContainer>(editor, "Container 1");
	container1->addNode(node5);
	editor.addNode(container1, ImVec2(100, 100));

	auto container2 = std::make_shared<DIWNE::BasicNodeContainer>(editor, "Container 2");
	container2->addNode(node6);
	editor.addNode(container2, ImVec2(200, 100));

	const int nodeCount = 6;
	const int allNodeCount = 8;

	ASSERT_TRUE(editor.getNodeList().size() == nodeCount);
	ASSERT_TRUE(container1->getNodes().collectRaw().size() == 1);
	ASSERT_TRUE(container2->getNodes().collect().size() == 1);
	{
		std::vector<std::reference_wrapper<Node>> nodeRefs;
		std::vector<Node*> nodePtrs;
		std::vector<std::shared_ptr<Node>> nodeSharedPtrs;

		for (auto& node : editor.getNodes())
		{
			nodeRefs.push_back(node);
		}
		ASSERT_TRUE(nodeRefs.size() == nodeCount);
		nodeRefs.clear();

		for (auto& node : editor.getAllNodesInnerIncluded())
		{
			nodeRefs.push_back(node);
		}
		ASSERT_TRUE(nodeRefs.size() == allNodeCount);
		nodeRefs.clear();

		for (auto& node : editor.getSelectedNodesInnerIncluded())
		{
			GTEST_FAIL(); // No nodes are selected, expecting no iteration
		}

		FilteredRecursiveNodeRange nodeRange = editor.getSelectedNodesInnerIncluded();
		for (auto node = nodeRange.begin(); node != nodeRange.end(); node++)
		{
			GTEST_FAIL(); // No nodes are selected, expecting no iteration
		}

		node5->setSelected(true);

		for (auto node = nodeRange.begin(); node != nodeRange.end(); node++)
		{
			ASSERT_TRUE(node.ptr() == node.operator->());
			nodePtrs.push_back(node.ptr());
			ASSERT_TRUE(&node.dereference() == &node.operator*());
			nodeRefs.push_back(node.dereference());
			nodeSharedPtrs.push_back(node.sharedPtr());
		}
		ASSERT_TRUE(nodeRefs.size() == nodePtrs.size());
		ASSERT_TRUE(nodeRefs.size() == nodeSharedPtrs.size());
		ASSERT_TRUE(nodeRefs.size() == 1);
		nodeRefs.clear();
		nodePtrs.clear();
		nodeSharedPtrs.clear();

		node2->setSelected(true);
		node6->setSelected(true);

		for (auto node = nodeRange.begin(); node != nodeRange.end(); ++node)
		{
			ASSERT_TRUE(node.ptr() == node.operator->());
			nodePtrs.push_back(node.ptr());
			ASSERT_TRUE(&node.dereference() == &node.operator*());
			nodeRefs.push_back(node.dereference());
			nodeSharedPtrs.push_back(node.sharedPtr());
		}
		ASSERT_TRUE(nodeRefs.size() == nodePtrs.size());
		ASSERT_TRUE(nodeRefs.size() == nodeSharedPtrs.size());
		ASSERT_TRUE(nodeRefs.size() == 3);
		nodeRefs.clear();
		nodePtrs.clear();
		nodeSharedPtrs.clear();

		auto container3 = std::make_shared<DIWNE::BasicNodeContainer>(editor, "Container 3");
		editor.addNode(container3, ImVec2(200, 200));

		auto node7 = std::make_shared<DIWNE::BasicNode>(editor, "Node 7");
		auto node8 = std::make_shared<DIWNE::BasicNode>(editor, "Node 8");
		node8->setSelected(true);
		auto node9 = std::make_shared<DIWNE::BasicNode>(editor, "Node 9");
		container3->addNode(node7);
		container3->addNode(node8);
		container3->addNode(node9);

		node1->setSelected(true);
		container3->setSelected(true);
		container1->setSelected(true);
		node5->setSelected(false);

		for (auto node = nodeRange.begin(); node != nodeRange.end(); ++node)
		{
			ASSERT_TRUE(node.ptr() == node.operator->());
			nodePtrs.push_back(node.ptr());
			ASSERT_TRUE(&node.dereference() == &node.operator*());
			nodeRefs.push_back(node.dereference());
			nodeSharedPtrs.push_back(node.sharedPtr());
		}
		ASSERT_TRUE(nodeRefs.size() == nodePtrs.size());
		ASSERT_TRUE(nodeRefs.size() == nodeSharedPtrs.size());
		ASSERT_TRUE(nodeRefs.size() == 6);

		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node1.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node2.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node6.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), container1.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), container3.get()) != nodePtrs.end());
		ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node8.get()) != nodePtrs.end());

		int counter = 0;
		for (auto& node : editor.getSelectedNodesInnerIncluded())
			counter++;
		ASSERT_TRUE(counter == 6);

		std::vector<Node*> nodePtrs2;
		std::vector<std::shared_ptr<Node>> nodeSharedPtrs2;

		nodeSharedPtrs2 = nodeRange.collect();
		ASSERT_TRUE(nodeSharedPtrs.size() == 6);
		nodePtrs2 = editor.getSelectedNodesInnerIncluded().collectRaw();
		ASSERT_TRUE(nodePtrs.size() == 6);

		ASSERT_TRUE(nodePtrs2 == nodePtrs);
		ASSERT_TRUE(nodeSharedPtrs2 == nodeSharedPtrs);
	}
}

// TODO: Test recursed containers
// S2 (N3, S1 (N2), N1)
// S2 (S1 (N2))

// TODO: Improvements
// ASSERT_EQ vs ASSERT_TRUE
// Wrap node editor init in a method
// ASSERT_NE(std::find(nodePtrs.begin(), nodePtrs.end(), node1.get()), nodePtrs.end())
//    << "Node 1 should be present in the nodePtrs list.";

// TODO: FilteredNodeRange test
// TEST(IteratorsTest, FilteredNodeRangeTest)
//{
//	auto app = initI3T(spdlog::level::warn);
//
//	DIWNE::SettingsDiwne settings;
//	NodeEditor editor(&settings);
//	auto node1 = std::make_shared<DIWNE::BasicNode>(editor, "Node 1");
//	editor.addNode(node1, ImVec2(0, 0));
//	auto node2 = std::make_shared<DIWNE::BasicNode>(editor, "Node 2");
//	editor.addNode(node2, ImVec2(100, 0));
//	auto node3 = std::make_shared<DIWNE::BasicNode>(editor, "Node 3");
//	editor.addNode(node3, ImVec2(200, 0));
//	auto node4 = std::make_shared<DIWNE::BasicNode>(editor, "Node 4");
//	editor.addNode(node4, ImVec2(0, 100));
//
//	// Define a predicate: filter only nodes whose names contain "2" or "3"
//	auto predicate = [](const Node& node) {
//		return node.getName().find("2") != std::string::npos ||
//		       node.getName().find("3") != std::string::npos;
//	};
//
//	FilteredNodeRange nodeRange(predicate, &editor.getNodeList());
//
//	std::vector<std::reference_wrapper<Node>> nodeRefs;
//	std::vector<Node*> nodePtrs;
//	std::vector<std::shared_ptr<Node>> nodeSharedPtrs;
//
//	// Ensure only the expected nodes are in the filtered range
//	for (auto node = nodeRange.begin(); node != nodeRange.end(); ++node)
//	{
//		ASSERT_TRUE(node.ptr() == node.operator->());
//		nodePtrs.push_back(node.ptr());
//		ASSERT_TRUE(&node.dereference() == &node.operator*());
//		nodeRefs.push_back(node.dereference());
//		nodeSharedPtrs.push_back(node.sharedPtr());
//	}
//
//	// Expect node2 and node3 in the filtered range
//	ASSERT_EQ(nodeRefs.size(), 2);
//	ASSERT_EQ(nodePtrs.size(), 2);
//	ASSERT_EQ(nodeSharedPtrs.size(), 2);
//	ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node2.get()) != nodePtrs.end());
//	ASSERT_TRUE(std::find(nodePtrs.begin(), nodePtrs.end(), node3.get()) != nodePtrs.end());
//
//	// Ensure iteration count is correct
//	int counter = 0;
//	for (auto node = nodeRange.begin(); node != nodeRange.end(); ++node)
//		counter++;
//	ASSERT_EQ(counter, 2);
//
//	// Test collect()
//	auto collectedPtrs = nodeRange.collect();
//	ASSERT_EQ(collectedPtrs.size(), 2);
//	ASSERT_EQ(collectedPtrs, nodeSharedPtrs);
//
//	// Modify selection and test again
//	node4->setSelected(true);
//	auto newPredicate = [](const Node& node) { return node.isSelected(); };
//	FilteredNodeRange updatedRange(newPredicate, &editor.getNodeList());
//
//	std::vector<std::shared_ptr<Node>> updatedNodes = updatedRange.collect();
//	ASSERT_EQ(updatedNodes.size(), 1);
//	ASSERT_EQ(updatedNodes.front().get(), node4.get());
//}


// TODO: Edge case test
// auto falsePredicate = [](const Node&) { return false; };
// FilteredNodeRange emptyRange(falsePredicate, &editor.getNodeList());
// ASSERT_TRUE(emptyRange.collect().empty());

// auto truePredicate = [](const Node&) { return true; };
// FilteredNodeRange allNodesRange(truePredicate, &editor.getNodeList());
// ASSERT_EQ(allNodesRange.collect().size(), editor.getNodeList().size());
