#include "StateManager.h"

#include "Commands/ApplicationCommands.h"
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

StateManager::StateManager()
{
	m_recentFiles = readRecentFiles();
}

void StateManager::init()
{
	NewProjectCommand::addListener([]() {
		App::getModule<StateManager>().newScene();
	});

	createEmptyScene();
}

void StateManager::takeSnapshot()
{
	auto state = createMemento();

	m_mementos.push_back(std::move(state));
	m_hashes.push_back(randLong());
	m_currentStateIdx = m_mementos.size() - 1;

	if (m_mementos.size() != 1)
	{
		m_dirty = true;
	}

	setWindowTitle();
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
		originator->setState(memento, false);
	}

	setWindowTitle();
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
		originator->setState(memento, false);
	}

	setWindowTitle();
}

bool StateManager::canUndo() const
{
	return m_currentStateIdx > 0;
}

bool StateManager::canRedo() const
{
	return m_mementos.size() != 1 && m_mementos.size() - 1 != m_currentStateIdx;
}

int StateManager::getMementosCount() const
{
	return m_mementos.size();
}

int StateManager::getPossibleUndosCount() const
{
	return m_mementos.size() - 1;
}

int StateManager::getPossibleRedosCount() const
{
	/// \todo Test me!
	return m_mementos.size() - m_currentStateIdx;
}

//

const Memento& StateManager::getCurrentState() const
{
	return m_mementos[m_currentStateIdx];
}

void StateManager::createEmptyScene()
{
	reset();
}

//===-- Files manipulation functions --------------------------------------===//

bool StateManager::loadScene(const fs::path& scene)
{
	const auto maybeScene = JSON::parse(scene, "Data/Schemas/Scene.schema.json");

	if (!maybeScene.has_value())
	{
		return false;
	}

	for (auto* originator : m_originators)
	{
		originator->setState(maybeScene.value(), true);
	}

	m_currentScene = scene;

	reset();

	return true;
}

bool StateManager::saveScene()
{
	return saveScene(m_currentScene);
}

bool StateManager::saveScene(const fs::path& target)
{
	const auto result = JSON::save(target, createMemento());

	m_savedSceneHash = m_hashes[m_currentStateIdx];
	m_currentScene = target;

	setWindowTitle();

	return result;
}

void StateManager::newScene()
{
	for (const auto& originator : m_originators)
	{
		originator->clear();
	}

	m_currentScene = "";

	reset();
}

void StateManager::setWindowTitle()
{
	std::string sceneName = "New scene";
	if (hasScene())
	{
		sceneName = scenePath().filename().string();

		if (isDirty())
		{
			sceneName += " - Unsaved";
		}
	}

	const auto newTitle = std::string(BASE_WINDOW_TITLE) + " - " + sceneName;

	App::get().setTitle(newTitle);
}

//===----------------------------------------------------------------------===//

void StateManager::reset()
{
	m_currentStateIdx = -1;
	m_dirty = false;

	m_mementos.clear();

	setWindowTitle();

	takeSnapshot();
}

Memento StateManager::createMemento()
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

	return state;
}
