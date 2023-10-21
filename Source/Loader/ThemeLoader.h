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
#pragma once

#include <filesystem>

#include "Core/Result.h"

namespace fs = std::filesystem;

class Theme;

/**
 *
 * \return True on success.
 */
void saveTheme(const fs::path& path, Theme& theme);

std::expected<Theme, Error> loadTheme(const fs::path& path);
