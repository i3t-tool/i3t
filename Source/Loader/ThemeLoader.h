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
