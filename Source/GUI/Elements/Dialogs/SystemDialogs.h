/**
 * \file Utils/SystemDialogs.h
 * \author Martin Herich hericmar@fel.cvut.cz
 * \date 30.10.2020
 *
 * Native dialogs.
 */
#pragma once

#include <string>
#include <vector>

/**
 * A bunch of system dialogs.
 */
namespace SystemDialogs
{
/**
 * Get single file name via native file dialog.
 *
 * Note that the file <b>dialog blocks the execution of application</b>.
 *
 * \param result Reference to a string which should contain selected file name.
 * \param title Dialog title.
 * \param root Specifies default path for dialog to open.
 * \param filter Specifies which files should be showed in the dialog according
 *to its types. Pass parameter as a key, value vector {Audio Files", "*.wav
 **.mp3", "All Files", "*"}.
 *
 * \return Whether file was selected or not.
 */
bool OpenSingleFileDialog(std::string& result, const std::string& title, const std::string& root = "",
                          const std::vector<std::string>& filter = {"All Files", "*"});

/**
 * Save single file via system native file dialog.
 *
 * Saves a scene to new scene file (it overrides existing file).
 * Note that the file <b>dialog blocks the execution of application</b>.
 *
 * \param filename Absolute filename to scene file.
 *
 * \return Whether file was selected or not.
 */
bool SaveSingleFileDialog(std::string& filename, const std::string& title, const std::string& root = "",
                          const std::vector<std::string>& filter = {"All Files", "*"});

/**
 * Show fatal error message
 *
 * Dialog will block execution of application.
 */
void FireErrorMessageDialog(const std::string& title, const std::string& message);
}; // namespace SystemDialogs
