/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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

} // namespace State
