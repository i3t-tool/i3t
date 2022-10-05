#pragma once

#include <string>

#include "Core/Defs.h"
#include "Utils/Filesystem.h"

namespace Core
{
enum class EResourceType
{
	Model,
	Shader,
};

struct Resource
{
	std::string ID;
	fs::path Path;
	EResourceType Type = EResourceType::Model; //

	/**
	 * @pre File at @p path must exists!
	 *
	 * @param id
	 * @param path
	 * @param type
	 */
	Resource(std::string id, fs::path path, EResourceType type);
};
} // namespace Core
