#include "gtest/gtest.h"

#include "State/Manager.h"
#include "State/Stateful.h"

class Workspace : public IStateful
{
public:
	void setManager(StateManager* manager) { m_manager = manager; }

	Memento getState() override
	{
		return Memento(m_state);
	}

	void setState(const Memento& memento) override
	{
		m_state = memento.getSnapshot();
	}

	void save()
	{
		m_manager->takeSnapshot();
	}

	void setValue(const std::string& val)
	{
		m_state.push_back(val);
		save();
	}

private:
	StateManager* m_manager;
	State m_state;
};

TEST(StateHandlingTest, OperationsAreSupported)
{
	Workspace workspace;
	StateManager manager(&workspace);
	workspace.setManager(&manager);

	std::vector<std::string> state;

	for (char i = 0; i < 10; ++i)
	{
		char c = 0x61 + i;
		auto data = std::string(1, c);
		state.push_back(data);

		workspace.setValue(data);
	}
	EXPECT_TRUE(workspace.getState().getSnapshot().size() == 10);
}
