#include "StateManager.h"

#include "Commands/ApplicationCommands.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "State/Stateful.h"
#include "Utils/JSON.h"
#include "Utils/Random.h"
#include "Utils/Text.h"
#include "Utils/TimeUtils.h"

void StateManager::init()
{
	NewProjectCommand::addListener([]() {
		App::getModule<StateManager>().newScene();
	});

	// Create UserData folder if it doesn't exist.
	fs::create_directory(USER_DATA_DIR);

	// Create UserData/Global.json if it doesn't exist.
	if (!fs::exists(USER_DATA_FILE))
	{
		saveUserData();
	}

	loadUserData();
	LOG_INFO("Checking for unused tmp directories (timeout {}) ...", m_tmpDirectoryLockFileTimeout);
	purgeTmpDirectories();
}

void StateManager::beginFrame()
{
	// Tmp directory lock
	// If the tmp directory exists, a file containing the current system time is written to it every 5 minutes.
	// This is so that tmp directories that previous application runs failed to delete on exit can be purged.

	if (!m_tmpDirectory.empty())
	{
		bool success = false;
		long long minutesSinceEpoch = TimeUtils::minutesSinceEpoch();
		long long timeDiff = abs(m_lastTmpDirectoryLockTimestamp - minutesSinceEpoch);
		if (timeDiff >= m_tmpDirectoryLockFileInterval)
		{
			// Write timestamp to lock file
			fs::path lockFile = m_tmpDirectory / m_tmpDirectoryLockFileName;
			try
			{
				std::ofstream ofs;
				ofs.exceptions(std::ios::badbit | std::ios::failbit);
				ofs.open(lockFile, std::ofstream::out | std::ofstream::trunc);
				if (ofs.is_open())
				{
					ofs << std::to_string(minutesSinceEpoch);
					ofs.close();
					success = true;
				}
				else
				{
					LOG_ERROR("Failed to open tmp directory LOCK file for writing! Path: {}", lockFile.string());
				}
			}
			catch (std::exception& e)
			{
				LOG_ERROR("An exception occurred while writing to the tmp directory LOCK file: {}, code: {}", e.what(),
				          strerror(errno));
			}

			// Schedule next write
			if (success)
			{
				m_lastTmpDirectoryLockTimestamp = minutesSinceEpoch; // Next write in lock interval
			}
			else
			{
				// Try again soon
				m_lastTmpDirectoryLockTimestamp =
				    minutesSinceEpoch - std::max(1LL, ((long long) (m_tmpDirectoryLockFileInterval * 0.9))) + 1;
			}
		}
	}
}

void StateManager::onClose()
{
	deleteTmpDirectory();
}

void StateManager::takeSnapshot()
{
	if (auto state = createMemento(nullptr))
	{
		m_mementos.push_back(std::move(*state));
		m_hashes.push_back(randLong());
		m_currentStateIdx = m_mementos.size() - 1;

		if (m_mementos.size() != 1)
		{
			m_dirty = true;
		}

		setWindowTitle();
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
		originator->loadState(memento, nullptr);
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
		originator->loadState(memento, nullptr);
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

bool StateManager::loadScene(const fs::path& path)
{
	LOG_INFO("");
	LOG_INFO("Loading scene from '{}'.", path.string());

	if (!fs::exists(path))
	{
		LOG_ERROR("Scene file at '{}' does not exist!", path.string());
		return false;
	}

	const auto maybeScene = JSON::parse(path, I3T_SCENE_SCHEMA);
	if (!maybeScene.has_value())
	{
		return false;
	}

	bool readOnly = false;
	if (maybeScene.value().HasMember("readOnly"))
	{
		readOnly = maybeScene.value()["readOnly"].GetBool();
	}

	Ptr<Scene> newScene = std::make_shared<Scene>(this, readOnly);

	setCurrentScene(newScene, path);

	createTmpDirectory();
	wipeTmpDirectory();

	for (auto* originator : m_originators)
	{
		originator->loadState(maybeScene.value(), m_currentScene.get());
	}

	pushRecentFile(path);

	reset();

	return true;
}

bool StateManager::saveScene()
{
	return saveScene(m_currentScene->m_path);
}

bool StateManager::saveScene(const fs::path& target)
{
	if (isReadOnly(target))
	{
		LOG_ERROR("No save performed, target scene {} is read-only!", target.string());
		return false;
	}

	LOG_INFO("");
	LOG_INFO("Saving scene to '{}'.", target.string());

	createTmpDirectory();

	setCurrentScene(std::make_shared<Scene>(this, false), target);

	// Create scene .json file
	const auto result = JSON::save(target, *createMemento(m_currentScene.get()));

	m_currentScene->m_hash = m_hashes[m_currentStateIdx];
	if (m_currentScene->m_prevPath != target)
	{
		// save as with different name, add original scene to recent files
		pushRecentFile(target);
	}

	setWindowTitle();

	return result;
}

bool StateManager::setCurrentScene(Ptr<Scene> newScene, fs::path newScenePath)
{
	if (m_currentScene && m_currentScene->m_path == newScenePath)
	{
		m_currentScene->m_sceneLocationChanged = false;
		return true;
	}
	if (fs::is_directory(newScenePath))
	{
		LOG_ERROR("Scene path cannot be a directory!");
		return false;
	}

	if (!newScenePath.empty())
	{
		// Gather information about previous scene so that ResourceManager saveState can copy over all files properly
		if (m_currentScene->isSaved() && !FilesystemUtils::weaklyEquivalent(m_currentScene->m_path, newScenePath))
		{
			newScene->m_sceneLocationChanged = true;
			newScene->m_prevPath = m_currentScene->m_path;
			newScene->m_prevDataPath = m_currentScene->m_dataPath;
		}
		newScene->m_path = newScenePath;
		newScene->m_dataPath = newScenePath.parent_path() / m_sceneDataFolderName;
	}
	m_currentScene = newScene;

	return true;
}

void StateManager::newScene()
{
	for (const auto& originator : m_originators)
	{
		originator->clearState();
	}

	setCurrentScene(std::make_shared<Scene>(this, false));

	createTmpDirectory();
	wipeTmpDirectory();

	reset();
}

void StateManager::loadUserData()
{
	auto& data = getUserData();

	// Load UserData/Global.json.
	auto result = JSON::deserialize(fs::path(USER_DATA_FILE), data);
	if (!result)
	{
		LOG_ERROR("Failed to load UserData/Global.json: {}", result.error());
	}

	// Prune non-existing recent files.
	for (auto it = data.recentFiles.begin(); it != data.recentFiles.end();)
	{
		auto path = fs::path(*it);
		if (!fs::exists(path))
		{
			LOG_INFO("Pruning non-existing recent file: {}", path.string())
			it = data.recentFiles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void StateManager::saveUserData()
{
	auto& data = getUserData();

	auto result = JSON::serialize(data, USER_DATA_FILE);
	if (!result)
	{
		LOG_ERROR("Failed to create initial UserData/Global.json: {}", result.error());
	}
}

std::optional<Memento> StateManager::createMemento(Scene* scene)
{
	Memento state;
	state.SetObject();

	if (m_originators.empty())
	{
		LOG_WARN("You have no originators set for the StateManager, cannot take snapshot.");
		return std::nullopt;
	}

	for (const auto& originator : m_originators)
	{
		auto memento = originator->saveState(scene);
		JSON::merge(state, memento, state.GetAllocator());
	}

	return state;
}

void StateManager::reset()
{
	m_currentStateIdx = -1;
	m_dirty = false;

	m_mementos.clear();

	setWindowTitle();

	takeSnapshot();
}

void StateManager::setWindowTitle()
{
	std::string sceneName = "New scene";
	if (hasScene())
	{
		sceneName = m_currentScene->m_path.filename().string();

		if (getCurrentScene()->m_readOnly)
		{
			sceneName += " (Read-only)";
		}
		else
		{
			if (isDirty())
			{
				sceneName += " - Unsaved";
			}
		}
	}

	const auto newTitle = std::string(BASE_WINDOW_TITLE) + " - " + sceneName;

	App::get().setTitle(newTitle);
}

void StateManager::pushRecentFile(const fs::path& file)
{
	getUserData().pushRecentFile(file.string());
	saveUserData();
}

void StateManager::createTmpDirectory()
{
	// TODO: (DR) Perhaps some error handling
	if (!m_tmpDirectory.empty())
	{
		return;
	}

	// Tmp directory is created once per application run, it is reused until the application closes, at which point it
	// is also deleted.
	// The tmp directory is ALWAYS created anew by the application and must NOT be an existing
	// directory, this is to prevent accidents deleting existing unrelated files (unlikely but it's a failsafe)

	// No tmp directory created yet
	// Get tmp directory path that points to a folder that doesn't exist yet
	unsigned int index = 0;
	while (true)
	{
		const fs::path tmpDirectoryPath =
		    fs::path(m_tmpDirectoryRootName / m_tmpDirectoryDefaultName).concat(std::to_string(index));
		try
		{
			if (!fs::exists(tmpDirectoryPath))
			{
				m_tmpDirectory = tmpDirectoryPath;
				break;
			}
		}
		catch (std::filesystem::filesystem_error& e)
		{
			FilesystemUtils::reportFilesystemException(e);
			break;
		}
		index++;
	}

	// Create tmp directory
	try
	{
		fs::create_directories(m_tmpDirectory);
	}
	catch (std::filesystem::filesystem_error& e)
	{
		LOG_ERROR("Failed to create tmp directory!");
		FilesystemUtils::reportFilesystemException(e);
	}
}

void StateManager::deleteTmpDirectory()
{
	// TODO: (DR) Perhaps some error handling
	if (m_tmpDirectory.empty())
	{
		return;
	}

	// Delete tmp directory
	fs::remove_all(m_tmpDirectory);
}

void StateManager::wipeTmpDirectory()
{
	if (m_tmpDirectory.empty())
	{
		LOG_WARN("Cannot wipe tmp directory before creating it!");
		return;
	}

	// Delete content of tmp directory
	for (const auto& entry : std::filesystem::directory_iterator(m_tmpDirectory))
	{
		if (entry.path().filename() != m_tmpDirectoryLockFileName) // Ignore lock file
			fs::remove_all(entry.path());
	}
}

void StateManager::purgeTmpDirectories()
{
	// If tmp root exists
	// -> Iterate over valid tmp folders = folders starting with tmp default name
	// -> find lock file, if it doesn't exist, delete folder
	// -> if it does, read it
	// -> compare its content to the current minutesSinceEpoch
	// -> if diff is bigger than threshold -> delete that tmp folder

	if (!fs::exists(m_tmpDirectoryRootName))
	{
		return;
	}

	for (auto const& maybeTmpDir : std::filesystem::directory_iterator{m_tmpDirectoryRootName})
	{
		bool purge = false;
		std::string purgeReason;
		if (maybeTmpDir.path().compare(m_tmpDirectory) == 0)
			continue; // Not interested in deleting our own tmp dir

		if (maybeTmpDir.path().filename().string().starts_with(m_tmpDirectoryDefaultName.string()))
		{
			const fs::path lockFilePath = maybeTmpDir.path() / m_tmpDirectoryLockFileName;
			if (!fs::exists(lockFilePath))
			{
				purge = true;
				purgeReason = "No lock file";
			}
			else
			{
				// Read lock file
				std::string data;
				std::ifstream ifs;
				ifs.open(lockFilePath);

				if (!ifs.is_open())
				{
					LOG_ERROR("Failed to open lock file at '{}'. Cannot purge tmp dir.", lockFilePath.string());
					continue;
				}

				std::getline(ifs, data);
				ifs.close();

				// Compare lock timestamp to current time
				const long long minutesSinceEpoch = TimeUtils::minutesSinceEpoch();
				long long lockMinutesSinceEpoch = 0;
				try
				{
					lockMinutesSinceEpoch = std::stoll(data);
				}
				catch (std::exception& e)
				{
					LOG_ERROR("Failed to parse lock file at '{}'. Cannot purge tmp dir. Lock file contents:\n'{}'",
					          lockFilePath.string(), data);
					continue;
				}
				const long long timeDiff = abs(lockMinutesSinceEpoch - minutesSinceEpoch);
				if (timeDiff >= m_tmpDirectoryLockFileTimeout)
				{
					// Lock file has timed out, purge
					purge = true;
					purgeReason = "Lock file timed out";
				}
				else
				{
					LOG_INFO("Parallel tmp directory '{}' lock time: {}", lockFilePath.string(), timeDiff);
				}
			}
		}

		if (purge)
		{
			// Delete tmp dir
			LOG_INFO("Purging tmp dir '{}': {}", maybeTmpDir.path().string(), purgeReason);
			try
			{
				fs::remove_all(maybeTmpDir.path());
			}
			catch (std::filesystem::filesystem_error& e)
			{
				FilesystemUtils::reportFilesystemException(e);
			}
		}
	}
}

const fs::path StateManager::getTmpDirectory() const
{
	return m_tmpDirectory;
}

const fs::path StateManager::fetchTmpDirectory()
{
	createTmpDirectory();
	return m_tmpDirectory;
}
