#include "SystemDialogs.h"

#include <codecvt>

#include "portable-file-dialogs.h"

#include "Logger/Logger.h"

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
#ifdef _WIN32
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wideFilename = converter.from_bytes(fileStr);
		result.push_back(fs::path(wideFilename));
#else
		result.push_back(fs::path(fileStr));
#endif
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
