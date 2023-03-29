#pragma once

namespace Core
{
/**
 * Base class for objects that are the actual data managed by a ResourceManager.
 * This is meant to be used with multiple inheritance if necessary.
 */
class ManagedResource
{
public:
	size_t m_resourceId{0}; ///< Resource ID of an object
	                        ///< An identifier used by a ResourceManager
};
} // namespace Core