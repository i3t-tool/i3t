#include "SystemDialogs.h"

#include "portable-file-dialogs.h"

bool SystemDialogs::OpenSingleFileDialog(std::string& result, const std::string& title, const std::string& root,
                                         const std::vector<std::string>& filter)
{
  auto dialog = pfd::open_file(title, root, filter);

  while (!dialog.ready(40) && !result.empty())
  {
    result = dialog.result()[0];
    return true;
  }

  return false;
}

bool SystemDialogs::SaveSingleFileDialog(std::string& result, const std::string& title, const std::string& root,
                                         const std::vector<std::string>& filter)
{
  auto destination = pfd::save_file(title, root, filter).result();
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
