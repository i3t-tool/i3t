#include "StateManager.h"

#include "yaml-cpp/yaml.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "State/Stateful.h"
#include "Utils/JSON.h"
#include "Utils/Random.h"
#include "Utils/Text.h"

static std::vector<std::string> readRecentFiles()
{
	return {};

	/// \todo Implement!
}

StateManager::StateManager() { m_recentFiles = readRecentFiles(); }

void StateManager::takeSnapshot()
{
	Memento state;
	state.SetObject();

	if (m_originators.empty())
	{
		LOG_WARN("You have no originators set for the StateManager, is it correct?");
	}

	for (const auto& originator : m_originators)
	{
		auto memento = originator->getState();
		JSON::merge(state, memento, state.GetAllocator());
	}

	m_mementos.push_back(std::move(state));
	m_hashes.push_back(randLong());
	m_currentStateIdx = m_mementos.size() - 1;

	if (m_mementos.size() != 1)
	{
		m_dirty = true;
	}

	for (const auto& originator : m_originators)
	{
		originator->onStateChange();
	}
}

void StateManager::undo()
{
	if (!canUndo())
	{
		return;
	}

	auto& memento = m_mementos[--m_currentStateIdx];

	for (auto& originator : m_originators)
	{
		originator->setState(memento);
	}

	for (auto& originator : m_originators)
	{
		originator->onStateChange();
	}
}

void StateManager::redo()
{
	if (!canRedo())
	{
		return;
	}

	auto& memento = m_mementos[++m_currentStateIdx];

	memento.GetAllocator();

	for (auto& originator : m_originators)
	{
		originator->setState(memento);
	}

	for (auto& originator : m_originators)
	{
		originator->onStateChange();
	}
}

bool StateManager::canUndo() const { return m_currentStateIdx > 0; }

bool StateManager::canRedo() const { return m_mementos.size() != 1 && m_mementos.size() - 1 != m_currentStateIdx; }

int StateManager::getMementosCount() const { return m_mementos.size(); }

int StateManager::getPossibleUndosCount() const { return m_mementos.size() - 1; }

int StateManager::getPossibleRedosCount() const
{
	/// \todo Test me!
	return m_mementos.size() - m_currentStateIdx;
}

//

const Memento& StateManager::getCurrentState() const { return m_mementos[m_currentStateIdx]; }

void StateManager::createEmptyScene() { reset(); }

//===-- Files manipulation functions --------------------------------------===//

bool StateManager::loadScene(const fs::path& scene)
{
	const auto maybeScene = JSON::parse(scene, "Data/Schemas/Scene.schema.json");

	if (!maybeScene.has_value())
		return false;

	for (auto* originator : m_originators)
	{
		originator->setState(maybeScene.value());
	}

	m_currentScene = scene;

	reset();

	return true;
}

bool StateManager::saveScene() { return saveScene(m_currentScene); }

bool StateManager::saveScene(const fs::path& target)
{
	const auto result = JSON::save(target, m_mementos[m_currentStateIdx]);

	m_savedSceneHash = m_hashes[m_currentStateIdx];

	return result;
}

void StateManager::setScene(const fs::path& scene)
{
	m_currentScene = scene;
	const auto newTitle = std::string(g_baseTitle) + ": " + scene.filename().string();

	App::get().setTitle(newTitle);
}

void StateManager::clear()
{
	for (const auto& originator : m_originators)
	{
		originator->clear();
	}

	reset();
}

//===----------------------------------------------------------------------===//

void StateManager::reset()
{
	m_currentStateIdx = -1;
	m_dirty = false;

	m_mementos.clear();

	takeSnapshot();
}

void StateManager::finalize() {}
