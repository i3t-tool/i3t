/**
 * \file
 * \brief Native dialogs.
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 30.10.2020
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
#include <string>
#include <vector>

namespace fs = std::filesystem;

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
bool OpenSingleFileDialog(fs::path& result, const std::string& title, fs::path root = "",
                          const std::vector<std::string>& filter = {"All Files", "*"});

/**
 * Get multiple file names via native file dialog.
 *
 * Note that the file <b>dialog blocks the execution of application</b>.
 *
 * \param result Reference to a list which should contain selected file names.
 * \param title Dialog title.
 * \param root Specifies default path for dialog to open.
 * \param filter Specifies which files should be showed in the dialog according
 * \param singleSelect Whether only a single file can be chosen.
 *to its types. Pass parameter as a key, value vector {Audio Files", "*.wav
 **.mp3", "All Files", "*"}.
 *
 * \return Whether at least one file was selected or not.
 */
bool OpenFilesDialog(std::vector<fs::path>& result, const std::string& title, fs::path root = "",
                     const std::vector<std::string>& filter = {"All Files", "*"}, bool singleSelect = false);

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
bool SaveSingleFileDialog(std::filesystem::path& filename, const std::string& title, fs::path root = "",
                          const std::vector<std::string>& filter = {"All Files", "*"});

/**
 * Show fatal error message
 *
 * Dialog will block execution of application.
 */
void FireErrorMessageDialog(const std::string& title, const std::string& message);
}; // namespace SystemDialogs
