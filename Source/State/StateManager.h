#pragma once

#include <deque>
#include <filesystem>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "Scene.h"
#include "State/Stateful.h"
#include "UserData.h"

using namespace std::literals;

constexpr auto I3T_SCENE_EXTENSION = ".scene";

/// Handles app state management.
class StateManager : public Module
{
public:
	StateManager() = default;

	void init() override;
	void beginFrame() override;
	void onClose() override;

	void addOriginator(IStateful* originator)
	{
		m_originators.push_back(originator);
	}

	// Undo/Redo _______________________________________________________________________________________________________

	void takeSnapshot();
	void undo();
	void redo();

	[[nodiscard]] bool canUndo() const;
	[[nodiscard]] bool canRedo() const;

	int getMementosCount() const;
	int getPossibleUndosCount() const;
	int getPossibleRedosCount() const;

	[[nodiscard]] const Memento& getCurrentState() const;

	bool isDirty() const
	{
		if (m_hashes.empty() || m_currentStateIdx == -1)
		{
			return m_dirty;
		}

		return m_currentScene->m_hash != m_hashes[m_currentStateIdx] && m_dirty;
	}

	// Scene save/load _________________________________________________________________________________________________

	bool loadScene(const fs::path& path);

	bool saveScene();
	bool saveScene(const fs::path& scene);

	void newScene();

	/**
	 * @return Whether a previously saved scene is currently loaded.
	 */
	bool hasScene()
	{
		return m_currentScene->isSaved();
	}

	Scene* getCurrentScene()
	{
		return m_currentScene.get();
	}

	// User data _______________________________________________________________________________________________________

	void loadUserData();
	void saveUserData();

	// Temporary directory _____________________________________________________________________________________________

	const fs::path getTmpDirectory() const;
	/**
	 * Returns the temporary directory path just like getTmpDirectory() but creates it if it doesn't exist yet.
	 */
	const fs::path fetchTmpDirectory();

	fs::path getTmpDirectoryLockFileName()
	{
		return m_tmpDirectoryLockFileName;
	}

private:
	friend class Application;

	std::vector<IStateful*> m_originators;

	std::optional<Memento> createMemento(Scene* scene);

	/// Resets counters, set clean state and takes initial snapshot.
	void reset();

	void setWindowTitle();
	void pushRecentFile(const fs::path& file);

	// Scene save/load _________________________________________________________________________________________________

	const fs::path m_sceneDataFolderName = fs::path("scene_data");

	Ptr<State::Scene> m_currentScene;

	bool setCurrentScene(Ptr<Scene> newScene, fs::path newScenePath = "");

	// Temporary directory _____________________________________________________________________________________________

	const fs::path m_tmpDirectoryRootName = fs::path("temp");
	const fs::path m_tmpDirectoryDefaultName = fs::path("tmp");
	const fs::path m_tmpDirectoryLockFileName = fs::path(".lock");
	const long long m_tmpDirectoryLockFileInterval = 5;
	const long long m_tmpDirectoryLockFileTimeout = m_tmpDirectoryLockFileInterval * 3;

	fs::path m_tmpDirectory;
	long long m_lastTmpDirectoryLockTimestamp{0};

	void createTmpDirectory();
	void deleteTmpDirectory();
	void wipeTmpDirectory();
	void purgeTmpDirectories();

	// Undo/Redo _______________________________________________________________________________________________________

	int m_currentStateIdx;
	std::deque<Memento> m_mementos;
	std::deque<long> m_hashes;

	bool m_dirty = false;
};
