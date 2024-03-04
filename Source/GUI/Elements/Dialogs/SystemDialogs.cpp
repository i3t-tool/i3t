/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "SystemDialogs.h"

#include "portable-file-dialogs.h"

#include "Logger/Logger.h"
#include "Utils/Text.h"

bool SystemDialogs::OpenSingleFileDialog(std::filesystem::path& result, const std::string& title, fs::path root,
                                         const std::vector<std::string>& filter)
{
	std::vector<fs::path> results;
	if (OpenFilesDialog(results, title, root, filter, true))
	{
		result = results.at(0);
		return true;
	}
	else
	{
		return false;
	}
}

bool SystemDialogs::OpenFilesDialog(std::vector<fs::path>& result, const std::string& title, fs::path root,
                                    const std::vector<std::string>& filter, bool singleSelect)
{
	const auto pwd = fs::current_path();

	auto files = pfd::open_file(title, root.make_preferred().string(), filter,
	                            (singleSelect ? pfd::opt::none : pfd::opt::multiselect))
	                 .result();

	if (files.empty())
	{
		return false;
	}

	result.clear();
	for (const auto& fileStr : files)
	{
		result.push_back(fs::path(fileStr));
	}

	if (fs::current_path() != pwd)
	{
		LOG_WARN("Current path changed by file dialog from {} to {}. Restoring.", pwd.string(),
		         fs::current_path().string());
		fs::current_path(pwd);
	}

	return true;
}

bool SystemDialogs::SaveSingleFileDialog(std::filesystem::path& result, const std::string& title, fs::path root,
                                         const std::vector<std::string>& filter)
{
	const auto pwd = fs::current_path();

	auto destination = pfd::save_file(title, root.make_preferred().string(), filter).result();

	if (destination.empty())
	{
		return false;
	}

	result = destination;

	if (fs::current_path() != pwd)
	{
		fs::current_path(pwd);
		LOG_WARN("Current path changed by file dialog from {} to {}. Restoring.", pwd.string(),
		         fs::current_path().string());
	}

	return true;
}

void SystemDialogs::FireErrorMessageDialog(const std::string& title, const std::string& message)
{
	pfd::message(title, message, pfd::choice::ok, pfd::icon::error);
}
