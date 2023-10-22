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

#include "Core/Resources/ResourceFiles.h"
#include "Mesh.h"

namespace Core
{

/**
 * A set a of 3D file format files located somewhere in the filesystem.
 * The main identifier of such a resource is the primary 3D file format file like a .gltf, .obj or .fbx file.
 *
 * Officially supported file formats:
 * - .GLTF (.bin)
 * - .GLB
 * - .OBJ (.mtl)
 */
class ModelResourceFiles : public ResourceFiles
{
public:
	ModelResourceFiles(fs::path path, std::string name);

	bool fetchFiles() override;

	/**
	 * @param data A Core::Mesh object
	 */
	bool fetchFiles(void* data) override;
};

} // namespace Core