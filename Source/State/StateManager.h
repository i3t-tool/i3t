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

	void onInit() override;
	void onBeginFrame() override;
	void onEndFrame() override;
	void onClose() override;

	void addOriginator(IStateful* originator)
	{
		m_originators.push_back(originator);
	}

	// Undo/Redo _______________________________________________________________________________________________________
private:
	// The snapshot will be taken in the next frame, after it has been requested
	void tryTakeSnapshot();
	// Creates a workspace snapshot for undo/redo.
	void takeSnapshot();
	// Creates a workspace snapshot for undo/redo. However, overwrites the last snapshot made if it was made with this function.
	void takeRewritableSnapshot();
public:
	// Request a snapshot to be taken in the next frame.
	void requestSnapshot()
	{
		m_snapshotRequested = true;
	}
	// Request a snapshot to be taken in the next frame.
	// If a request for a snapshot to be overwritten was also made in the previous frame, the snapshot will be overwritten.
	void requestRewritableSnapshot()
	{
		m_rewritableSnapshotRequested = true;
	}
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

	bool saveScene();
	bool saveScene(const fs::path& scene);

	bool loadScene(const fs::path& path);

	void newScene(bool firstRun = false);

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

	static bool isValidScenePath(const fs::path path);

	// Global save/load ________________________________________________________________________________________________

	bool saveGlobal();
	bool loadGlobal();
	void resetGlobal();

	// User data _______________________________________________________________________________________________________

	void saveUserData();
	void loadUserData();

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

	/// Resets counters, set clean state and takes initial snapshot.
	void reset();

	void setWindowTitle();
	void pushRecentFile(const fs::path& file);

	// Scene save/load _________________________________________________________________________________________________

	const fs::path m_sceneDataFolderName = fs::path("scene_data");

	Ptr<State::Scene> m_currentScene;

	/**
	 * Set the passed scene as the current scene.
	 * @param newScene The new current scene.
	 * @param newScenePath If a non-empty path is passed, it is set as the path of the new current scene.
	 * @return True if the current scene actually changed, false if the passed scene was already the current scene.
	 */
	bool setCurrentScene(Ptr<Scene> newScene, fs::path newScenePath = "");

	std::optional<Memento> createSceneMemento(Scene* scene);

	// Create a memento from WorkspaceWindow without "zoom" and "workArea" properties
	std::optional<Memento> createSnapshotMemento(Scene* scene);

	// Global save/load ________________________________________________________________________________________________

	const fs::path m_globalFilePath = fs::path("Global.json");

	std::optional<Memento> createGlobalMemento();

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

	bool m_snapshotRequested{false};
	bool m_rewritableSnapshotRequested{false};
	int m_currentStateIdx{-1};
	std::vector<Memento> m_mementos;
	std::vector<long> m_hashes;

	bool m_dirty = false;
};
