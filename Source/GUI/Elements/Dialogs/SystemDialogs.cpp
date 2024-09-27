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

#include <functional>

#include "portable-file-dialogs.h"

#include "Config.h"
#include "Logger/Logger.h"
#include "Utils/Text.h"

template <typename T>
T currentPathGuard(std::function<T(void)> fn)
{
	const auto pwd = fs::current_path();

	auto result = fn();

	if (fs::current_path() != pwd)
	{
		LOG_WARN("Current path changed by file dialog from {} to {}. Restoring.", pwd.string(),
		         fs::current_path().string());
		fs::current_path(Configuration::root);
	}

	return result;
}

bool SystemDialogs::OpenSingleFileDialog(std::filesystem::path& result, const std::string& title, fs::path root,
                                         const std::vector<std::string>& filter)
{
	std::vector<fs::path> results;

	if (!OpenFilesDialog(results, title, root, filter, true))
	{
		return false;
	}

	result = results.at(0);

	return true;
}

bool SystemDialogs::OpenFilesDialog(std::vector<fs::path>& result, const std::string& title, fs::path root,
                                    const std::vector<std::string>& filter, bool singleSelect)
{
	const auto pwd = fs::current_path();

	auto files = currentPathGuard<std::vector<std::string>>([&] {
		return pfd::open_file(title, root.make_preferred().string(), filter,
		                      (singleSelect ? pfd::opt::none : pfd::opt::multiselect))
		    .result();
	});

	if (files.empty())
	{
		return false;
	}

	result.clear();
	for (const auto& fileStr : files)
	{
		result.push_back(fs::path(fileStr));
	}

	return true;
}

bool SystemDialogs::SaveSingleFileDialog(std::filesystem::path& result, const std::string& title, fs::path root,
                                         const std::vector<std::string>& filter)
{
	auto destination = currentPathGuard<std::string>([&] {
		return pfd::save_file(title, root.make_preferred().string(), filter).result();
	});

	if (destination.empty())
	{
		return false;
	}

	result = destination;

	return true;
}

void SystemDialogs::FireErrorMessageDialog(const std::string& title, const std::string& message)
{
	pfd::message(title, message, pfd::choice::ok, pfd::icon::error);
}
