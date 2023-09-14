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
