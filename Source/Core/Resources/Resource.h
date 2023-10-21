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

#include <string>

#include "Core/Defs.h"
#include "ResourceFiles.h"
#include "Utils/FilesystemUtils.h"

namespace Core
{
enum class ResourceType
{
	Model,
	Shader,
	Texture
};

// TODO: (DR) Maybe switch back to filesystem::path
// TODO: (DR) Absolute / Relative path handling

/**
 * Object holding information about a resource.
 */
class Resource
{
public:
	friend class ResourceManager;

	std::string alias; ///< Resource alias
	std::string path;  ///< Arbitrary resource file path, doesn't necessary need to be valid

	ResourceType resourceType = ResourceType::Model;

	std::vector<std::string> alternativeAliases; ///< Any additional aliases that got registered after the first one
	                                             ///< which is not included in this list

	Ptr<ResourceFiles>
	    resourceFiles; ///< The resource CAN have an assigned ResourceFiles instance tracking any associated files

private:
	size_t hashId;              ///< Resource hash, unique identifier
	std::shared_ptr<void> data; ///< Arbitrary data pointer

public:
	Resource(std::string alias, std::string path, ResourceType type);

private:
	Resource(std::string alias, size_t hashId, std::string path, ResourceType type, std::shared_ptr<void> data);
};
} // namespace Core
