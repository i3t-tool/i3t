#pragma once

#include <filesystem>

class StateManager;

namespace fs = std::filesystem;

namespace State
{

constexpr auto I3T_SCENE_SCHEMA = "Data/Schemas/Scene.schema.json";

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

	const bool m_readOnly;

	Scene(StateManager* stateManager, bool readOnly) : m_stateManager(stateManager), m_readOnly(readOnly) {}

	bool isSaved() const
	{
		return !m_path.empty();
	}
};

/// Returns true if target scene is an I3T scene and is read-only.
bool isReadOnly(const fs::path& scene);

} // namespace State
