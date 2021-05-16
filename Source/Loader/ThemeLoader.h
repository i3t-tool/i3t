#pragma once

#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

class Theme;

/**
 *
 * \return True on success.
 */
void saveTheme(const fs::path& path, Theme& theme);

/**
 *
 * \return True on success, false if file does not exit.
 */
std::optional<Theme> loadTheme(const fs::path& path);
