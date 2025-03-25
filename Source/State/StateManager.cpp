/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "StateManager.h"

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "State/Stateful.h"
#include "Utils/JSON.h"
#include "Utils/Random.h"
#include "Utils/TimeUtils.h"

void StateManager::onInit()
{
	NewProjectCommand::addListener([]() {
		App::getModule<StateManager>().newScene();
	});

	// Create UserData folder if it doesn't exist.
	fs::create_directory(USER_DATA_DIR);

	// Create UserData/Global.json if it doesn't exist.
	if (!fs::exists(Configuration::root / USER_DATA_FILE))
	{
		saveUserData();
	}
	loadUserData();

	LOG_INFO("[STATE MANAGER] Checking for unused tmp directories (timeout {}) ...", m_tmpDirectoryLockFileTimeout);
	purgeTmpDirectories();
}

void StateManager::onBeginFrame()
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
					LOG_ERROR("[STATE MANAGER] Failed to open tmp directory LOCK file for writing! Path: {}",
					          lockFile.string());
				}
			}
			catch (std::exception& e)
			{
				LOG_ERROR(
				    "[STATE MANAGER] An exception occurred while writing to the tmp directory LOCK file: {}, code: {}",
				    e.what(), strerror(errno));
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
	saveGlobal();
	deleteTmpDirectory();
}

void StateManager::takeSnapshot()
{
	if (auto state = createSceneMemento(nullptr))
	{
		if (!m_mementos.empty())
		{
			if (state == m_mementos[m_currentStateIdx])
			{
				return;
			}
		}

		if (canRedo())
		{
			m_mementos.erase(m_mementos.begin() + m_currentStateIdx + 1, m_mementos.end());
			m_hashes.erase(m_hashes.begin() + m_currentStateIdx + 1, m_hashes.end());
		}

		m_mementos.push_back(std::move(*state));
		m_hashes.push_back(randLong());
		if (m_hashes.size() == 1)
		{
			m_hashes[0] = m_currentScene->m_hash;
		}
		m_currentStateIdx++;

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
		originator->loadScene(memento, nullptr);
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
		originator->loadScene(memento, nullptr);
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

bool StateManager::isValidScenePath(const fs::path path)
{
	if (fs::is_directory(path))
	{
		LOG_ERROR("Invalid scene save location! Path cannot be a directory: {}.", path.string());
		return false;
	}
	if (path.extension().string() != I3T_SCENE_EXTENSION)
	{
		LOG_ERROR("Invalid scene save location! Scene files need to end with {}.", I3T_SCENE_EXTENSION);
		return false;
	}
	return true;
}

std::optional<Memento> StateManager::createSceneMemento(Scene* scene)
{
	Memento state;
	state.SetObject();

	if (m_originators.empty())
	{
		LOG_WARN("You have no originators set for the StateManager. Cannot take scene snapshot.");
		return std::nullopt;
	}

	for (const auto& originator : m_originators)
	{
		auto memento = originator->saveScene(scene);
		JSON::merge(state, memento, state.GetAllocator());
	}

	return state;
}

std::optional<Memento> StateManager::createGlobalMemento()
{
	Memento state;
	state.SetObject();

	if (m_originators.empty())
	{
		LOG_WARN("You have no originators set for the StateManager. Cannot create global state memento.");
		return std::nullopt;
	}

	for (const auto& originator : m_originators)
	{
		auto memento = originator->saveGlobal();
		// TODO: (DR) No need to deep copy merge docs if we simply pass a Document ref to saveScene/saveGlobal methods.
		JSON::merge(state, memento, state.GetAllocator());
	}

	return state;
}

bool StateManager::saveScene()
{
	return saveScene(m_currentScene->m_path);
}

bool StateManager::saveScene(const fs::path& target)
{
	if (!isValidScenePath(target))
	{
		return false;
	}

	setCurrentScene(std::make_shared<Scene>(this, false), target);

	if (m_currentScene->m_readOnly)
	{
		LOG_ERROR("[SCENE SAVE] No save performed, target scene {} is read-only!", target.string());
		return false;
	}
	LOG_INFO("[SCENE SAVE] Saving scene to '{}'.", target.string());

	createTmpDirectory();

	// Create scene .json file
	const auto result = JSON::save(target, *createSceneMemento(m_currentScene.get()));

	m_currentScene->m_hash = m_hashes[m_currentStateIdx];
	if (m_currentScene->m_prevPath != target)
	{
		// save as with different name, add original scene to recent files
		pushRecentFile(target);
	}

	setWindowTitle();

	return result;
}

bool StateManager::loadScene(const fs::path& path)
{
	LOG_INFO("");
	LOG_INFO("[SCENE LOAD] Loading scene from '{}'.", path.string());

	if (!fs::exists(path))
	{
		LOG_ERROR("[SCENE LOAD] Scene file at '{}' does not exist!", path.string());
		return false;
	}

	rapidjson::Document doc;
	if (!JSON::parse(path, doc, I3T_SCENE_SCHEMA))
	{
		LOG_ERROR("[SCENE LOAD] Failed to parse the scene file!");
		return false;
	}

	bool readOnly = false;
	if (doc.HasMember("readOnly"))
	{
		readOnly = doc["readOnly"].GetBool();
	}

	Ptr<Scene> newScene = std::make_shared<Scene>(this, readOnly);

	setCurrentScene(newScene, path);

	createTmpDirectory();
	wipeTmpDirectory();

	for (auto* originator : m_originators)
	{
		originator->loadScene(doc, m_currentScene.get());
	}

	pushRecentFile(path);

	reset();

	return true;
}

bool StateManager::setCurrentScene(Ptr<Scene> newScene, fs::path newScenePath)
{
	if (m_currentScene && m_currentScene->m_path == newScenePath)
	{
		m_currentScene->m_sceneLocationChanged = false;
		return false;
	}

	if (!newScenePath.empty())
	{
		// Gather information about previous scene so that ResourceManager saveScene can copy over all files properly
		if (m_currentScene->isSaved() && !FilesystemUtils::weaklyEquivalent(m_currentScene->m_path, newScenePath))
		{
			newScene->m_sceneLocationChanged = true;
			newScene->m_prevPath = m_currentScene->m_path;
			newScene->m_prevDataPath = m_currentScene->m_dataPath;
		}
		else
		{
			newScene->m_sceneLocationChanged = false;
			newScene->m_prevPath.clear();
			newScene->m_prevDataPath.clear();
		}
		newScene->m_path = newScenePath;
		newScene->m_dataPath = newScenePath.parent_path() / m_sceneDataFolderName;
	}
	m_currentScene = newScene;

	return true;
}

void StateManager::newScene(bool firstRun)
{
	for (const auto& originator : m_originators)
	{
		originator->clearScene();
	}

	setCurrentScene(std::make_shared<Scene>(this, false));

	createTmpDirectory();
	wipeTmpDirectory();

	reset();
}

bool StateManager::saveGlobal()
{
	fs::path target = m_globalFilePath;

	std::string absolutePath;
	try
	{
		absolutePath = fs::absolute(target).string();
	}
	catch (const fs::filesystem_error& e)
	{
		FilesystemUtils::reportFilesystemException(e);
		absolutePath = target.string() + " (Failed to resolve absolute path)";
	}

	LOG_INFO("[STATE MANAGER] Saving global data to '{}'.", absolutePath);

	// Create global data .json file
	const auto result = JSON::save(target, *createGlobalMemento());

	return result;
}

bool StateManager::loadGlobal()
{
	fs::path target = m_globalFilePath;

	std::string absolutePath;
	try
	{
		absolutePath = fs::absolute(target).string();
	}
	catch (const fs::filesystem_error& e)
	{
		FilesystemUtils::reportFilesystemException(e);
		absolutePath = target.string() + " (Failed to resolve absolute path)";
	}

	LOG_INFO("[STATE MANAGER] Loading global data from '{}'.", absolutePath);

	if (!fs::exists(target))
	{
		LOG_WARN("[STATE MANAGER] Global data file at '{}' does not exist!", target.string());
		return false;
	}

	rapidjson::Document doc;
	if (!JSON::parse(target, doc, I3T_SCENE_SCHEMA))
	{
		LOG_WARN("[STATE MANAGER] Failed to parse global data file!");
		return false;
	}

	for (auto* originator : m_originators)
	{
		originator->loadGlobal(doc);
	};

	return true;
}

void StateManager::resetGlobal()
{
	for (const auto& originator : m_originators)
	{
		originator->clearGlobal();
	}
}

void StateManager::saveUserData()
{
	auto& data = getUserData();

	auto result = JSON::serializeToFile(data, Configuration::root / USER_DATA_FILE);
	if (!result)
	{
		LOG_ERROR("[STATE MANAGER] Failed to create initial UserData/Global.json: {}", result.error());
	}
}

void StateManager::loadUserData()
{
	auto& data = getUserData();

	// Load UserData/Global.json.
	auto result = JSON::deserializeFile(Configuration::root / USER_DATA_FILE, data);
	if (!result)
	{
		LOG_ERROR("[STATE MANAGER] Failed to load UserData/Global.json: {}", result.error());
	}

	// Prune non-existing recent files.
	for (auto it = data.recentFiles.begin(); it != data.recentFiles.end();)
	{
		auto path = fs::path(*it);
		if (!fs::exists(path))
		{
			LOG_INFO("[STATE MANAGER] Pruning non-existing recent file: {}", path.string())
			it = data.recentFiles.erase(it);
		}
		else
		{
			++it;
		}
	}

	data.trimRecentFiles();
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
	}

	if (isDirty())
	{
		sceneName += " - Unsaved";
	}

	const auto newTitle = std::string(BASE_WINDOW_TITLE) + " - " + sceneName;

	App::get().setTitle(newTitle);
}

void StateManager::pushRecentFile(const fs::path& file)
{
	getUserData().pushRecentFile(file.string());
	saveUserData();
	saveGlobal();
}

void StateManager::createTmpDirectory()
{
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
		LOG_ERROR("[STATE MANAGER] Failed to create tmp directory! Path: '{}'", m_tmpDirectory.string());
		FilesystemUtils::reportFilesystemException(e);
	}
}

void StateManager::deleteTmpDirectory()
{
	if (m_tmpDirectory.empty())
	{
		return;
	}

	// Delete tmp directory
	if (!FilesystemUtils::deleteFileOrDir(m_tmpDirectory, false))
	{
		LOG_ERROR("[STATE MANAGER] Failed to delete tmp directory! Path: '{}'", m_tmpDirectory.string());
	}
}

void StateManager::wipeTmpDirectory()
{
	if (m_tmpDirectory.empty())
	{
		LOG_WARN("[STATE MANAGER] Cannot wipe tmp directory before creating it!");
		return;
	}

	// Delete content of tmp directory
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(m_tmpDirectory))
		{
			if (entry.path().filename() != m_tmpDirectoryLockFileName) // Ignore lock file
				fs::remove_all(entry.path());
		}
	}
	catch (const fs::filesystem_error& e)
	{
		FilesystemUtils::reportFilesystemException(e);
		LOG_ERROR("[STATE MANAGER] Failed to wipe tmp directory! Path: '{}'", m_tmpDirectory.string());
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
					LOG_ERROR("[STATE MANAGER] Failed to open lock file at '{}'. Cannot purge tmp dir.",
					          lockFilePath.string());
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
					LOG_ERROR("[STATE MANAGER] Failed to parse lock file at '{}'. Cannot purge tmp dir. Lock file "
					          "contents:\n'{}'",
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
					LOG_INFO("[STATE MANAGER] Parallel tmp directory '{}' lock time: {}", lockFilePath.string(),
					         timeDiff);
				}
			}
		}

		if (purge)
		{
			// Delete tmp dir
			LOG_INFO("[STATE MANAGER] Purging tmp dir '{}': {}", maybeTmpDir.path().string(), purgeReason);
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
