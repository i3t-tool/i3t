#include "StateManager.h"

#include "Stateful.h"
#include "SerializationVisitor.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Utils/Text.h"

constexpr const char* g_unsavedPostfix = " - Unsaved";
constexpr const char* g_savedPostfix   = " - Saved";

StateManager::StateManager()
{
	resetState();
}

void StateManager::takeSnapshot()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	m_mementos.push_back(m_originator->getState());

	if (m_currentStateIdx < m_mementos.size() - 1)
	{
		// Override existing state: undo, undo, takeSnapshot.
		m_mementos[++m_currentStateIdx] = m_originator->getState();
		m_mementos.resize(m_currentStateIdx + 1);
	}
	else
	{
		// Push new state.
		m_mementos.push_back(m_originator->getState());
		m_currentStateIdx = m_mementos.size() - 1;
	}

	setUnsavedWindowTitle();
}

void StateManager::undo()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	if (!canUndo()) return;

	auto memento = m_mementos[--m_currentStateIdx];

	m_originator->setState(memento);

	setUnsavedWindowTitle();
}

void StateManager::redo()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	if (!canRedo()) return;

	auto memento = m_mementos[++m_currentStateIdx];
	m_originator->setState(memento);

	setUnsavedWindowTitle();
}

bool StateManager::canUndo() const
{
	return m_currentStateIdx > 0;
}

bool StateManager::canRedo() const
{
	return m_mementos.size() != 1 && m_mementos.size() - 1 != m_currentStateIdx;
}

Memento StateManager::getCurrentState() const
{
	return m_mementos[m_currentStateIdx];
}

void StateManager::createEmptyScene()
{
	resetState();
	takeSnapshot();
}

//===-- Files manipulation functions --------------------------------------===//

bool StateManager::loadScene(const fs::path& scene)
{
	resetState();
	takeSnapshot();

	auto& workspaceNodes =
			I3T::getWindowPtr<WorkspaceWindow>()->getNodeEditor().m_workspaceCoreNodes;
	workspaceNodes.clear();

	std::ifstream f(scene);
	std::string rawScene;
	if (f)
	{
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		rawScene = ss.str();
	}
	buildScene(rawScene, workspaceNodes);

	m_currentScene = scene;

	setSavedWindowTitle();

	return true;
}

bool StateManager::saveScene()
{
	return saveScene(m_currentScene);
}

bool StateManager::saveScene(const fs::path& target)
{
	auto& workspaceNodes =
			I3T::getWindowPtr<WorkspaceWindow>()->getNodeEditor().m_workspaceCoreNodes;

	SerializationVisitor visitor;
	std::string rawState = visitor.dump(workspaceNodes);
	std::ofstream f(target);
	f << rawState;

	setSavedWindowTitle();

	return true;
}

void StateManager::setScene(const fs::path& scene)
{
	m_currentScene = scene;
	const auto newTitle = std::string(g_baseTitle) + ": " + scene.filename().string();

	App::get().setTitle(newTitle);
}

//===----------------------------------------------------------------------===//

void StateManager::setUnsavedWindowTitle()
{
	if (!m_dirty)
	{
		m_originator->onStateChange(g_unsavedPostfix);
	}

	m_dirty = true;
}

void StateManager::setSavedWindowTitle()
{
	if (m_dirty)
	{
		m_originator->onStateChange(g_savedPostfix);
	}
	else
	{
		m_originator->onStateChange("");
	}

	m_dirty = false;
}

bool StateManager::hasNewestState() const
{
	return m_currentStateIdx == m_mementos.size() - 1;
}

void StateManager::resetState()
{
	m_currentStateIdx = -1;
	m_dirty           = false;

	m_mementos.clear();
}
