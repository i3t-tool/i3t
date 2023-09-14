#pragma once

#include <set>

#include "Core/Defs.h"

namespace Core
{

/**
 * A collection of file paths in the filesystem that are together required to load a single resource like a model.
 * An instance of this class helps keep track of actual files that a resource was loaded from and allows them to be
 * deleted when not used anymore.
 */
class ResourceFiles
{
public:
	fs::path m_path;    ///< Path to the "primary" file, from which other paths are derived
	fs::path m_rootDir; ///< Root directory of the resource. Derived from m_path.

	std::string m_name; ///< Arbitrary name, can be used as a name for the root subfolder

	std::set<fs::path> m_files;

	ResourceFiles(fs::path path, std::string name);
	virtual ~ResourceFiles() = default;

	void addFile(fs::path filePath);
	bool changeLocation(fs::path newDir, bool deleteOriginals);
	bool deleteFiles();

	/**
	 * Derive and fetch all required files from the primary file path.
	 */
	virtual bool fetchFiles() = 0;

	/**
	 * Derive and fetch all required files from the primary file path and supplied arbitrary data.
	 */
	virtual bool fetchFiles(void* data) = 0;

	bool operator<(const ResourceFiles& rhs) const
	{
		return m_path < rhs.m_path;
	}
	bool operator==(const ResourceFiles& rhs) const
	{
		return !(*this < rhs) && !(rhs < *this);
	}
	bool operator!=(const ResourceFiles& rhs) const
	{
		return !(rhs == *this);
	}
};

} // namespace Core
