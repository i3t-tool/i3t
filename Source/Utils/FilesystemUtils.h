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

#include <filesystem>

#include "Core/Result.h"
#include "Logger/Logger.h"

namespace fs = std::filesystem;

namespace FilesystemUtils
{

inline void reportFilesystemException(const fs::filesystem_error& e)
{
	LOG_ERROR("Filesystem error: {}, path1: {}, path2: {}, code: {}", e.what(), e.path1().string(), e.path2().string(),
	          e.code().value());
}

/**
 * Copy file or directory to a location.
 * The target location directory is created if necessary.
 * Filesystem exceptions are handled and reported by this method.
 * @return Whether the copy operation succeeded or not
 */
inline bool copy(const fs::path& from, const fs::path& to,
                 const fs::copy_options options = fs::copy_options::overwrite_existing | fs::copy_options::recursive)
{
	try
	{
		// Ensure the target's directory exists
		if (fs::is_directory(to))
		{
			fs::create_directories(to);
		}
		else
		{
			fs::create_directories(to.parent_path());
		}
		fs::copy(from, to, options);
		return true;
	}
	catch (const fs::filesystem_error& e)
	{
		reportFilesystemException(e);
		return false;
	}
}

/**
 * Copy file or directory to another directory.
 * The target location directory is created if necessary.
 * Filesystem exceptions are handled and reported by this method.
 * @return Whether the copy operation succeeded or not
 */
inline bool copyToDir(const fs::path& from, const fs::path& toDir,
                      const fs::copy_options options = fs::copy_options::overwrite_existing |
                                                       fs::copy_options::recursive)
{
	return FilesystemUtils::copy(from, toDir / from.filename(), options);
}

/**
 * Copy file to a location.
 * The target location directory is created if necessary.
 * Filesystem exceptions are handled and reported by this method.
 * @return Whether the copy operation succeeded or not
 */
inline bool copyFile(const fs::path& from, const fs::path& to,
                     const fs::copy_options options = fs::copy_options::overwrite_existing)
{
	try
	{
		// Ensure the target's directory exists
		if (fs::is_directory(to))
		{
			fs::create_directories(to);
		}
		else
		{
			fs::create_directories(to.parent_path());
		}
		fs::copy_file(from, to, options);
		return true;
	}
	catch (const fs::filesystem_error& e)
	{
		reportFilesystemException(e);
		return false;
	}
}

/**
 * Remove the specified file or directories.
 * Directories are deleted recursively.
 * @param path The file or directory to delete.
 * @param deleteEmptyFolders If true, any empty parent directories of the specified file or dir will be deleted as well
 * up recursively until the root directory.
 * @return Whether the delete operation succeeded.
 */
inline bool deleteFileOrDir(const fs::path& path, bool deleteEmptyFolders = false)
{
	try
	{
		fs::remove_all(path);

		if (!deleteEmptyFolders)
			return true;

		// Delete recursively any EMPTY parent directories up until the root dir
		fs::path parentDir = path.parent_path();
		while (fs::is_directory(parentDir) && fs::is_empty(parentDir))
		{
			fs::remove(parentDir);
			parentDir = parentDir.parent_path();
		}
		return true;
	}
	catch (const fs::filesystem_error& e)
	{
		reportFilesystemException(e);
		return false;
	}
}

/**
 * Checks if a path is located "downstream" from a base location.
 * This check if performed by inspecting the relative path between these two locations.
 * The relative path must not start with going "up" a directory.
 */
inline bool isSubpath(const fs::path path, const fs::path base)
{
	fs::path relPath = path.lexically_relative(base);
	return !relPath.empty() && !relPath.string().starts_with("..");
}

/**
 * Compare two paths with exception handling.
 * Calls the fs::equivalent method directly which checks if two paths are pointing to the same EXISTING file.
 * Thus both paths MUST exist for this method to not throw an exception.
 * Use the weaklyEquivalent() method if any of the paths don't exist.
 */
inline bool equivalent(const fs::path& path1, const fs::path& path2)
{
	try
	{
		return fs::equivalent(path1, path2);
	}
	catch (const fs::filesystem_error& e)
	{
		reportFilesystemException(e);
		return false;
	}
}

inline fs::path normalizePath(const fs::path& originalPath)
{
	fs::path currentPath;
	if (originalPath.is_absolute())
		currentPath = originalPath;
	else
		currentPath = fs::current_path() / originalPath;
	while (true)
	{
		fs::path newPath = weakly_canonical(currentPath);
		if (newPath != currentPath)
			currentPath = newPath;
		else
			break;
	}
	return currentPath;
}

/**
 * Compare two paths with exception handling.
 * Uses mostly string comparison trickery.
 * The paths do NOT have to exist.
 */
inline bool weaklyEquivalent(const fs::path& path1, const fs::path& path2)
{
	// TODO: (DR) Write some tests
	try
	{
		// Normalize paths and compare their canonical versions
		fs::path path1N = normalizePath(path1);
		fs::path path2N = normalizePath(path2);
		return path1N == path2N;
	}
	catch (const fs::filesystem_error& e)
	{
		reportFilesystemException(e);
		return false;
	}
}

/**
 * Compares two paths either using equivalent() or weaklyEquivalent().
 * The weak equivalence is used when either of the two paths don't exist.
 */
inline bool anyEquivalent(const fs::path& path1, const fs::path& path2)
{
	if (fs::exists(path1) && fs::exists(path2))
	{
		return FilesystemUtils::equivalent(path1, path2);
	}
	else
	{
		return weaklyEquivalent(path1, path2);
	}
}

Result<std::string, Error> readFile(const fs::path& path);

} // namespace FilesystemUtils