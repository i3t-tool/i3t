#include "StateManager.h"

#include "Stateful.h"
#include "SerializationVisitor.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Utils/Text.h"

constexpr const char* g_unsavedPostfix = " - Unsaved";
constexpr const char* g_savedPostfix   = " - Saved";

void StateManager::takeSnapshot()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	m_mementos.push_back(m_originator->getState());

	m_currentStateIdx = m_mementos.size() - 1;

	setUnsavedWindowTitle();
}

void StateManager::undo()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	if (!canUndo()) return;

	auto memento = m_mementos[m_currentStateIdx];

	m_originator->setState(memento);
	--m_currentStateIdx;
}

void StateManager::redo()
{
	I3T_ASSERT(m_originator != nullptr && "Originator is unset!");

	if (!canRedo()) return;

	++m_currentStateIdx;  // move cursor to the next state
	auto memento = m_mementos[m_currentStateIdx];
	m_originator->setState(memento);
}

bool StateManager::canUndo() const
{
	return !m_mementos.empty() && m_currentStateIdx >= 0;
}

bool StateManager::canRedo() const
{
	return !m_mementos.empty() && m_currentStateIdx != m_mementos.size() - 1;
}

std::optional<Memento> StateManager::getCurrentState() const
{
	if (m_currentStateIdx == -1) return std::nullopt;
	return m_mementos[m_currentStateIdx];
}

//===-- Files manipulation functions --------------------------------------===//

bool StateManager::loadScene(const fs::path& scene)
{
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
		const auto& title = App::get().getTitle();
		std::string currentTitle = title;
		if (ends_with(currentTitle, g_savedPostfix))
			currentTitle = std::string(title.begin(), title.end() - strlen(g_savedPostfix));

		App::get().setTitle(currentTitle + g_unsavedPostfix);
	}

	m_dirty = true;
}

void StateManager::setSavedWindowTitle()
{
	if (m_dirty)
	{
		const auto& title = App::get().getTitle();
		auto currentTitle = title;
		if (ends_with(currentTitle, g_unsavedPostfix))
			currentTitle = std::string(title.begin(), title.end() - strlen(g_unsavedPostfix));

		App::get().setTitle(currentTitle + g_savedPostfix);
	}
	else
	{
		App::get().setTitle(std::string(g_baseTitle) + " - " + m_currentScene.filename().string());
	}

	m_dirty = false;
}

bool StateManager::hasNewestState() const
{
	return m_currentStateIdx == m_mementos.size() - 1;
}
