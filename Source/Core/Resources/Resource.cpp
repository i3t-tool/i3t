#include "Resource.h"

#include "Config.h"

using namespace Core;

Resource::Resource(std::string alias, std::string path, ResourceType type)
    : alias(std::move(alias)), path(path), resourceType(type)
{
	// Empty
}

Resource::Resource(std::string alias, size_t hashId, std::string path, ResourceType type, std::shared_ptr<void> data)
    : alias(std::move(alias)), hashId(hashId), path(path), resourceType(type), data(data)
{
	// Empty
}
