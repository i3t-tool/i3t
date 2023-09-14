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