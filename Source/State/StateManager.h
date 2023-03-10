#pragma once

#include <deque>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "State/Stateful.h"

/**
 * Holds global state and takes state snapshots.
 */
class StateManager : public Module
{
public:
	/// Implicitly creates the initial state (Memento with index 0).
	///
	/// \warning Class is statically initialized, originators are not set!
	StateManager();

	void takeSnapshot();
	void undo();
	void redo();

	//

	void setOriginator(IStateful* originator) { m_originators.push_back(originator); }

	//

	[[nodiscard]] bool canUndo() const;
	[[nodiscard]] bool canRedo() const;

	int getMementosCount() const;
	int getPossibleUndosCount() const;
	int getPossibleRedosCount() const;

	[[nodiscard]] const Memento& getCurrentState() const;

	bool isDirty() const { return m_savedSceneHash != m_hashes[m_currentStateIdx] && m_dirty; }

	/// \pre m_originator is set.
	void createEmptyScene();

	//===-- Files manipulation functions ------------------------------------===//

	bool loadScene(const fs::path& scene);

	bool saveScene();
	bool saveScene(const fs::path& scene);

	void setScene(const fs::path& scene);

	bool hasScene() { return !m_currentScene.empty(); }
	auto scenePath() { return m_currentScene; }

	//===-- Recent files ----------------------------------------------------===//

	const std::vector<std::string>& getRecentFiles() { return m_recentFiles; }

	//===--------------------------------------------------------------------===//

	void clear();

private:
	/// Resets counters, set clean state and takes initial snapshot.
	void reset();

	friend class Application;

	Memento createMemento();

	fs::path m_currentScene;
	long m_savedSceneHash{};

	bool m_dirty = false;

	std::vector<IStateful*> m_originators;

	int m_currentStateIdx;
	std::deque<Memento> m_mementos;
	std::deque<long> m_hashes;

	std::vector<std::string> m_recentFiles;
};
