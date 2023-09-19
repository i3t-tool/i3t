#include "SystemDialogs.h"

#include <codecvt>

#include "portable-file-dialogs.h"

#include "Logger/Logger.h"

bool SystemDialogs::OpenSingleFileDialog(std::filesystem::path& result, const std::string& title, fs::path root,
                                         const std::vector<std::string>& filter)
{
	const auto pwd = fs::current_path();

	auto files = pfd::open_file(title, root.make_preferred().string(), filter).result();

	if (files.empty())
	{
		return false;
	}

	const auto resultStr = files[0];
#ifdef _WIN32
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wideFilename = converter.from_bytes(resultStr);
	result = wideFilename;
#else
	result = resultStr;
#endif

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
