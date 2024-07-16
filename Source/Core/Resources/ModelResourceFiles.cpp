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
#include "ModelResourceFiles.h"
#include "ResourceManager.h"

Core::ModelResourceFiles::ModelResourceFiles(fs::path path, std::string name) : ResourceFiles(path, name) {}

bool Core::ModelResourceFiles::fetchFiles()
{
	// Load the mesh in order get texture info
	LOG_INFO("[IMPORT] Temporarily loading model from the specified location.")
	RMI.m_forceMinimumLoad = true;
	Mesh* importedMesh = RMI.mesh(m_path.string());
	RMI.m_forceMinimumLoad = false;
	if (!importedMesh)
	{
		LOG_ERROR("[IMPORT] Failed to load model at '{}'!", m_path.string());
		return false;
	}

	fetchFiles(importedMesh);

	// Remove the model loaded initially
	LOG_INFO("[IMPORT] Removing original temporarily loaded model");
	RMI.removeResource(importedMesh);

	return true;
}

bool Core::ModelResourceFiles::fetchFiles(void* data)
{
	Core::Mesh* mesh = static_cast<Mesh*>(data);

	// The primary file (.gltf, .glb, .obj, ...)
	if (!fs::exists(m_path))
	{
		LOG_ERROR("[IMPORT] The model file '{}' does not exist!", m_path.string());
		return false;
	}
	addFile(m_path);

	int auxFileCount = 0;
	int texFileCount = 0;

	// Auxiliary files (.bin, .mtl, ...)
	fs::path ext = m_path.extension();
	if (!ext.empty())
	{
		if (ext.string() == ".gltf")
		{
			// Parse the .gltf file for .bin, .glbin, or .glbuf files
			rapidjson::Document doc;
			if (!JSON::parse(m_path, doc))
			{
				LOG_ERROR("Failed to parse .gltf file at '{}'!", m_path.string());
			}
			else
			{
				if (doc.HasMember("buffers"))
				{
					for (const auto& entry : doc["buffers"].GetArray())
					{
						if (!entry.HasMember("uri"))
							continue;
						fs::path binFilePath = entry["uri"].GetString();
						addFile(m_rootDir / binFilePath);
						auxFileCount++;
					}
				}
			}
		}
		else if (ext.string() == ".glb")
		{
			// No need to copy anything
		}
		else if (ext.string() == ".obj")
		{
			// Need to also copy matching .mtl file
			fs::path maybeMtlFile = fs::path(m_path).replace_extension(fs::path("mtl"));
			if (fs::exists(maybeMtlFile))
			{
				addFile(m_rootDir / maybeMtlFile);
				auxFileCount++;
			}

			else
			{
				LOG_INFO("[IMPORT] No matching .mtl file found for .obj file '{}'.", m_path.string());
			}
		}
		else
		{
			LOG_WARN("[IMPORT] Note: The model extension '{}' is not officially supported.\nThis may cause issues with "
			         "some formats that use multiple files.",
			         ext.string())
		}
	}
	else
	{
		LOG_WARN("The model file has no extension! Cannot determine file format. This may cause issues with some "
		         "formats that use multiple files!");
	}

	// Copy texture files to the tmp scene location
	for (const auto& texturePath : mesh->m_textureFileList)
	{
		if (!fs::exists(texturePath))
		{
			LOG_ERROR("[IMPORT] Referenced texture file does not exist! Texture file '{}' for model at '{}'!",
			          texturePath, m_path.string());
		}
		else
		{
			addFile(texturePath);
			texFileCount++;
		}
	}

	LOG_INFO("[IMPORT] Found {} auxiliary and {} texture files.", auxFileCount, texFileCount);
	LOG_DEBUG("[IMPORT] Full resource file list:");
	int i = 0;
	for (const auto& entry : m_files)
	{
		LOG_DEBUG("[IMPORT] {}: {}", std::to_string(i), entry.string());
		i++;
	}
}
