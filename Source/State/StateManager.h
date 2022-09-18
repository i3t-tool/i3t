#pragma once

#include <deque>

#include "Core/Defs.h"

#include "Memento.h"
#include "SceneData.h"

constexpr int MAX_STATES = 32;

class IStateful;

/**
 * Holds global state and takes state snapshots.
 */
class StateManager : public Singleton<StateManager>
{
public:
	StateManager();
	void setOriginator(IStateful* originator) { m_originator = originator; }
	void takeSnapshot();
	void undo();
	void redo();

	[[nodiscard]] bool canUndo() const;
	[[nodiscard]] bool canRedo() const;

	[[nodiscard]] Memento getCurrentState() const;

	bool isDirty() const { return m_dirty; }

	/// \pre m_originator is set.
	void createEmptyScene();

	//===-- Files manipulation functions
	//--------------------------------------===//

	bool loadScene(const fs::path& scene);
	// SceneData buildScene(const std::string& rawScene, GuiNodes&
	// workspaceNodes);

	bool saveScene();
	bool saveScene(const fs::path& scene);

	void setScene(const fs::path& scene);

	bool hasScene() { return !m_currentScene.empty(); }
	auto scenePath() { return m_currentScene; }

	//===----------------------------------------------------------------------===//

	//===-- Recent files
	//------------------------------------------------------===//

	const std::vector<std::string>& getRecentFiles() { return m_recentFiles; }

	//===----------------------------------------------------------------------===//

private:
	void setUnsavedWindowTitle();
	void setSavedWindowTitle();

	void resetState();

	[[nodiscard]] bool hasNewestState() const;

	fs::path m_currentScene;
	bool m_dirty = false;

	IStateful* m_originator;

	int m_currentStateIdx;
	std::deque<Memento> m_mementos;

	std::vector<std::string> m_recentFiles;
};
