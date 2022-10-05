#include "Resource.h"

#include "Config.h"

namespace Core
{
Resource::Resource(std::string id, fs::path path, EResourceType type)
    : ID(std::move(id)), Path(Config::getAbsolutePath(path)), Type(type)
{
	I3T_ASSERT(doesFileExists(Path) && "File at path must exists!");
}
} // namespace Core
