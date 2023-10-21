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
#include "ResourceFiles.h"

#include "Utils/FilesystemUtils.h"

namespace Core
{

ResourceFiles::ResourceFiles(fs::path path, std::string name) : m_path(path), m_name(name)
{
	m_rootDir = m_path.parent_path();
}

void ResourceFiles::addFile(fs::path filePath)
{
	m_files.insert(filePath);
}

bool ResourceFiles::changeLocation(fs::path newDir, bool deleteOriginals)
{
	std::set<fs::path> newFilePaths;

	// Copy files to new directory
	bool error = false;
	for (const auto& file : m_files)
	{
		fs::path relativePath = file.lexically_relative(m_rootDir);
		fs::path newPath = newDir / relativePath;
		if (!FilesystemUtils::copyFile(file, newPath, fs::copy_options::update_existing))
		{
			error = true;
		}
		else
		{
			newFilePaths.insert(newPath);
		}
	}

	if (error)
	{
		LOG_ERROR("[IMPORT] Failed to copy model files over to a different directory! Copy from: {} to {} failed.",
		          m_rootDir.string(), newDir.string());
		return false;
	}

	// Delete original files if requested and if the copy operation succeeded
	if (deleteOriginals)
	{
		if (!deleteFiles())
		{
			LOG_ERROR("[IMPORT] Failed to delete unused model files at '{}'!", m_rootDir.string());
		}
	}

	// Update variables

	fs::path primaryFileRelPath = m_path.lexically_relative(m_rootDir);
	fs::path newPrimaryFilePath = newDir / primaryFileRelPath;

	m_path = newPrimaryFilePath;
	m_rootDir = newDir;
	m_files.clear();
	m_files = newFilePaths;

	return true;
}

bool ResourceFiles::deleteFiles()
{
	bool error = false;
	for (const auto& file : m_files)
	{
		error |= FilesystemUtils::deleteFileOrDir(file, true);
	}
	return error;
}

} // namespace Core