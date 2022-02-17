#pragma once

#include <optional>

#include "Core/Defs.h"

#include "Memento.h"
#include "SceneData.h"

class IStateful;

/**
 * Holds global state and takes state snapshots.
 */
class StateManager : public Singleton<StateManager>
{
public:
	void setOriginator(IStateful* originator) { m_originator = originator; }
	void takeSnapshot();
	void undo();
	void redo();

	[[nodiscard]] bool canUndo() const;
	[[nodiscard]] bool canRedo() const;

	[[nodiscard]] std::optional<Memento> getCurrentState() const;

	bool isDirty() const { return m_dirty; }

	//===-- Files manipulation functions --------------------------------------===//

	bool loadScene(const fs::path& scene);

	bool saveScene();
	bool saveScene(const fs::path& scene);

	void setScene(const fs::path& scene);
	bool hasScene() { return !m_currentScene.empty(); }

	//===----------------------------------------------------------------------===//

private:
	void setUnsavedWindowTitle();
	void setSavedWindowTitle();

	[[nodiscard]] bool hasNewestState() const;

	fs::path m_currentScene;
	bool m_dirty = false;

	int m_currentStateIdx = -1;
	IStateful* m_originator;
	std::vector<Memento> m_mementos;
};
