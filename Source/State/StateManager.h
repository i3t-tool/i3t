#pragma once

#include <deque>
#include <filesystem>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "State/Stateful.h"
#include "UserData.h"

/// Handles app state management.
class StateManager : public Module
{
public:
	StateManager() = default;

	void init() override;

	void takeSnapshot();
	void undo();
	void redo();

	//

	void setOriginator(IStateful* originator)
	{
		m_originators.push_back(originator);
	}

	//

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

		return m_savedSceneHash != m_hashes[m_currentStateIdx] && m_dirty;
	}

	/// \pre m_originator is set.
	void createEmptyScene();

	//===-- Files manipulation functions ------------------------------------===//

	bool loadScene(const fs::path& path);

	bool saveScene();
	bool saveScene(const fs::path& scene);

	bool hasScene()
	{
		return !m_currentScene.empty();
	}
	auto scenePath()
	{
		return m_currentScene;
	}

	void newScene();

	void loadUserData();
	void saveUserData();

private:
	void setWindowTitle();

	/// Resets counters, set clean state and takes initial snapshot.
	void reset();

	friend class Application;

	std::optional<Memento> createMemento();

	fs::path m_currentScene;
	long m_savedSceneHash{};

	bool m_dirty = false;

	std::vector<IStateful*> m_originators;

	int m_currentStateIdx;
	std::deque<Memento> m_mementos;
	std::deque<long> m_hashes;
};
