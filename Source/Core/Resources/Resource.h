#pragma once

#include <string>

#include "Core/Defs.h"
#include "Utils/Filesystem.h"

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
 * Object representing a resource.
 */
class Resource
{
public:
	friend class ResourceManager;

	std::string alias; ///< Resource alias
	std::string path;  ///< Resource path, used for display only, doesn't need to
	                   ///< be a valid path
	ResourceType resourceType = ResourceType::Model;

private:
	size_t hashId;              ///< Resource hash, unique identifier
	std::shared_ptr<void> data; ///< Arbitrary data pointer

public:
	Resource(std::string alias, std::string path, ResourceType type);

private:
	Resource(std::string alias, size_t hashId, std::string path, ResourceType type, std::shared_ptr<void> data);
};
} // namespace Core
