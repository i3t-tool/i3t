#pragma once

class StateManager;

namespace State
{

class Scene
{
public:
	// Current info
	fs::path m_path;     ///< Path to the scene json file
	fs::path m_dataPath; ///< Path to the scene data directory

	long m_hash{};

	// Previous save info
	bool m_sceneLocationChanged{false}; ///< Whether the scene path is a new location compared to the previous save
	fs::path m_prevPath;                ///< m_path of the previous saved scene, valid if m_sceneLocationChanged is true
	fs::path m_prevDataPath; ///< m_dataPath of the previous saved scene, valid if m_sceneLocationChanged is true

	StateManager* m_stateManager; ///< Reference to the state manager this scene is managed by

	Scene(StateManager* stateManager) : m_stateManager(stateManager) {}

	bool isSaved() const
	{
		return !m_path.empty();
	}
};

} // namespace State
