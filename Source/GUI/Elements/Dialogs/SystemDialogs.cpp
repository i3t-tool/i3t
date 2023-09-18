#include "SystemDialogs.h"

#include <codecvt>

#include "portable-file-dialogs.h"

bool SystemDialogs::OpenSingleFileDialog(std::filesystem::path& result, const std::string& title, fs::path root,
                                         const std::vector<std::string>& filter)
{
	auto dialog = pfd::open_file(title, root.make_preferred().string(), filter, pfd::opt::force_path);

	while (!dialog.ready(40) && result.empty())
	{
		if (dialog.result().size() > 0)
		{
			const auto resultStr = dialog.result()[0];
#ifdef _WIN32
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring wideFilename = converter.from_bytes(resultStr);
			result = wideFilename;
#else
			result = resultStr;
#endif

			return true;
		}
	}

	return false;
}

bool SystemDialogs::SaveSingleFileDialog(std::filesystem::path& result, const std::string& title, fs::path root,
                                         const std::vector<std::string>& filter)
{
	auto destination = pfd::save_file(title, root.make_preferred().string(), filter, pfd::opt::force_path).result();

	if (!destination.empty())
	{
		result = destination;
		return true;
	}

	return false;
}

void SystemDialogs::FireErrorMessageDialog(const std::string& title, const std::string& message)
{
	pfd::message(title, message, pfd::choice::ok, pfd::icon::error);
}
