#include "gtest/gtest.h"

#include "Config.h"
#include "Core/API.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "State/StateManager.h"
#include "State/Stateful.h"

#include "Common.h"
#include "Generator.h"

using namespace Core;

const auto& getNodes(Ptr<WorkspaceWindow> workspaceWindow) { return workspaceWindow->getNodeEditor().getAllNodes(); }

/// \todo This test may require the OpenGL context!
TEST(StateTest, SceneCanBeSavedAndLoaded)
{
	createTestApplication();

	const auto scenePath = "Test/State/TestScene.json";
	const auto emptyScenePath = "Test/State/EmptyScene.json";

	std::vector<Ptr<WorkspaceNodeWithCoreDataWithPins>> nodes;

	// Requires the application to be initialized!
	auto* ui = I3T::getUI();
	ASSERT_TRUE(ui != nullptr);
	const auto workspace = ui->getWindowPtr<WorkspaceWindow>();
	ASSERT_TRUE(workspace != nullptr);

	// create empty scene
	App::getModule<StateManager>().saveScene("Test/State/EmptyScene.json");

	{
		ASSERT_TRUE(getNodes(workspace).empty());

		EXPECT_FALSE(App::getModule<StateManager>().canUndo());
		EXPECT_FALSE(App::getModule<StateManager>().canRedo());

		// StateManager contains the initial state only.
		EXPECT_TRUE(App::getModule<StateManager>().getMementosCount() == 1);

		nodes.push_back(addNodeToNodeEditor<WorkspaceOperator<ENodeType::FloatToFloat>>(ImVec2(1.0f, 0.0f)));
		EXPECT_TRUE(App::getModule<StateManager>().canUndo());
		EXPECT_TRUE(App::getModule<StateManager>().getPossibleUndosCount() == 1);

		nodes.push_back(addNodeToNodeEditor<WorkspaceOperator<ENodeType::FloatToFloat>>(ImVec2(-1.0f, 1.0f)));
		EXPECT_TRUE(App::getModule<StateManager>().canUndo());
		EXPECT_TRUE(App::getModule<StateManager>().getPossibleUndosCount() == 2);

		nodes.push_back(addNodeToNodeEditor<WorkspaceOperator<ENodeType::FloatToFloat>>(ImVec2(0.0f, -1.0f)));
		EXPECT_TRUE(App::getModule<StateManager>().canUndo());
		EXPECT_TRUE(App::getModule<StateManager>().getPossibleUndosCount() == 3);

		// workspace contains 3 operators.
		ASSERT_TRUE(getNodes(workspace).size() == nodes.size());

		connectNodes(nodes[0], nodes[1], 0, 0);
		EXPECT_TRUE(App::getModule<StateManager>().getPossibleUndosCount() == 4);

		connectNodes(nodes[1], nodes[2], 0, 0);
		EXPECT_TRUE(App::getModule<StateManager>().getPossibleUndosCount() == 5);

		const bool result = App::getModule<StateManager>().saveScene(scenePath);
		ASSERT_TRUE(result);
	}

	{
		// clear all nodes with empty scene file

		const bool result = App::getModule<StateManager>().loadScene(emptyScenePath);
		ASSERT_TRUE(result);
		EXPECT_FALSE(App::getModule<StateManager>().canUndo());
		EXPECT_FALSE(App::getModule<StateManager>().canRedo());

		ASSERT_TRUE(getNodes(workspace).empty());
	}

	{
		const bool result = App::getModule<StateManager>().loadScene(scenePath);
		ASSERT_TRUE(result);
		EXPECT_FALSE(App::getModule<StateManager>().canUndo());
		EXPECT_FALSE(App::getModule<StateManager>().canRedo());

		// ASSERT_TRUE(getNodes(workspace).size() == nodes.size());
	}

	destroyTestApplication();
}

TEST(StateTest, TransformsAreSavedAndLoadedProperly)
{
	createTestApplication();

	const auto scenePath = "Test/State/TestScene.json";

	const auto workspace = I3T::getUI()->getWindowPtr<WorkspaceWindow>();

	ASSERT_TRUE(getNodes(workspace).empty());

	// create empty scene
	App::getModule<StateManager>().saveScene("Test/State/EmptyScene.json");

	std::vector<Ptr<WorkspaceNodeWithCoreData>> nodes;

	nodes.push_back(addNodeToNodeEditor<WorkspaceTransformation_s<ETransformType::Scale>>());
	nodes.push_back(addNodeToNodeEditor<WorkspaceTransformation_s<ETransformType::Free>>());
	nodes.push_back(addNodeToNodeEditor<WorkspaceSequence>());

	const auto randomVec3 = generateVec3();

	{
		const auto scaleNode = getNodes(workspace)[0]->getNodebase()->as<Core::Transformation>();
		const auto freeNode = getNodes(workspace)[1]->getNodebase()->as<Core::Transformation>();
		const auto sequenceNode = getNodes(workspace)[2]->getNodebase()->as<Core::Sequence>();

		scaleNode->disableSynergies();
		scaleNode->setDefaultValue("scale", randomVec3);
		scaleNode->saveValue();
		scaleNode->setDefaultValue("scale", glm::vec3(1.0f, 1.0f, 1.0f));
		App::getModule<StateManager>().takeSnapshot();

		std::static_pointer_cast<WorkspaceSequence>(nodes[2])->moveNodeToSequence(nodes[1]);

		/// \todo Sequence now contains a transform but the workspace also contains
		/// it. \todo When to take a snapshot?
		App::getModule<StateManager>().takeSnapshot();
	}

	// due to WorkspaceSequence::setRemoveFromWorkspace
	for (int i = 0; i < 2; ++i)
		Application::get().onDisplay();

	App::getModule<StateManager>().takeSnapshot();

	{
		const bool result = App::getModule<StateManager>().saveScene(scenePath);
		ASSERT_TRUE(result);
	}

	{
		const bool result = App::getModule<StateManager>().loadScene(scenePath);
		ASSERT_TRUE(result);
	}

	// due to WorkspaceSequence::setRemoveFromWorkspace
	for (int i = 0; i < 2; ++i)
		Application::get().onDisplay();

	{
		const auto sequenceNode = getNodes(workspace)[0]->getNodebase()->as<Core::Sequence>();

		EXPECT_TRUE(sequenceNode->getMatrices().size() == 1);

		//

		const auto scaleNode = getNodes(workspace)[1]->getNodebase()->as<Core::Transformation>();

		EXPECT_TRUE(scaleNode->hasSavedValue());
		EXPECT_TRUE(Math::eq(scaleNode->getData().getMat4(), glm::scale(glm::vec3(1.0f, 1.0f, 1.0f))));
		EXPECT_TRUE(!scaleNode->hasSynergies());
		EXPECT_TRUE(Math::eq(scaleNode->getSavedValue(), glm::scale(randomVec3)));
	}

	destroyTestApplication();
}
