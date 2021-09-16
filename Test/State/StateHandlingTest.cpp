#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "State/DumpVisitor.h"
#include "State/Manager.h"
#include "State/Stateful.h"

using namespace Core;

class Workspace : public IStateful
{
public:
	Workspace()
	{
		m_nodes = {
			GraphManager::createNode<ENodeType::Vector3>(),
		};
	}

	Memento getState() override
	{
		DumpVisitor visitor;

		State state;
		for (const auto& node : m_nodes)
		{
			state.push_back(visitor.dump(node));
		}

		return Memento(state);
	}

	void setState(const Memento& memento) override
	{
		// m_state = memento.getSnapshot();
	}

	void save()
	{
		StateManager::instance().takeSnapshot();
	}

private:
	std::vector<Core::NodePtr> m_nodes;
};

TEST(StateHandlingTest, OperationsAreSupported)
{
	Workspace workspace;
	StateManager::instance().setOriginator(&workspace);

	std::vector<std::string> state;

	for (char i = 0; i < 10; ++i)
	{
		char c = 0x61 + i;
		auto data = std::string(1, c);
		state.push_back(data);

		// workspace.setValue(data);
	}
	EXPECT_TRUE(workspace.getState().getSnapshot().size() == 10);
}
