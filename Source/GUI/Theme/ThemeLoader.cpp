/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "ThemeLoader.h"

#include <fstream>
#include <map>

#include "yaml-cpp/yaml.h"

#include "GUI/Theme/Theme.h"
#include "Utils/FilesystemUtils.h"

template <typename T>
std::optional<T> strToEnum(std::map<T, const char*>& map, std::string&& name)
{
	for (const auto& [key, val] : map)
	{
		if (strcmp(val, name.c_str()) == 0)
		{
			return key;
		}
	}
	return std::nullopt;
}

void dumpVec2Seq(YAML::Emitter& out, const float* vec)
{
	out << YAML::Flow << YAML::BeginSeq;
	out << vec[0];
	out << vec[1];
	out << YAML::EndSeq << YAML::Block;
}

void dumpVec4SeqRound(YAML::Emitter& out, const float* vec, int precision)
{
	out << YAML::Flow << YAML::BeginSeq;
	out << fmt::format("{:.{}f}", vec[0], precision);
	out << fmt::format("{:.{}f}", vec[1], precision);
	out << fmt::format("{:.{}f}", vec[2], precision);
	out << fmt::format("{:.{}f}", vec[3], precision);
	out << YAML::EndSeq << YAML::Block;
}

Result<ImVec2, Error> parseVec2Seq(YAML::Node& node)
{
	if (!node.IsSequence() || node.size() != 2)
	{
		return Err("invalid sequence");
	}

	ImVec2 vec;

	vec.x = node[0].as<float>();
	vec.y = node[1].as<float>();

	return vec;
}

Result<ImVec4, Error> parseVec4Seq(YAML::Node& node)
{
	if (!node.IsSequence() || node.size() != 4)
	{
		return Err("invalid sequence");
	}

	ImVec4 vec;

	vec.x = node[0].as<float>();
	vec.y = node[1].as<float>();
	vec.z = node[2].as<float>();
	vec.w = node[3].as<float>();

	return vec;
}

template <typename T>
bool compareThemeEntry(const T& a, const T& b)
{
	return a.first < b.first;
}

void saveTheme(const fs::path& path, Theme& theme)
{
	if (path.empty())
	{
		return;
	}

	// Sort the style keys alphabetically first
	std::vector<std::pair<EColor, ImVec4>> colors;
	for (const auto& [key, val] : theme.getColorsRef())
		colors.emplace_back(key, val);
	std::vector<std::pair<ESize, float>> sizes1;
	for (const auto& [key, val] : theme.getSizesRef())
		sizes1.emplace_back(key, val.first);
	std::vector<std::pair<ESizeVec2, ImVec2>> sizes2;
	for (const auto& [key, val] : theme.getSizesVecRef())
		sizes2.emplace_back(key, val.first);

	std::sort(colors.begin(), colors.end(), compareThemeEntry<std::pair<EColor, ImVec4>>);
	std::sort(sizes1.begin(), sizes1.end(), compareThemeEntry<std::pair<ESize, float>>);
	std::sort(sizes2.begin(), sizes2.end(), compareThemeEntry<std::pair<ESizeVec2, ImVec2>>);

	// Generate yml file

	YAML::Emitter out;
	out << YAML::BeginMap;

	out << YAML::Key << "mode";
	out << YAML::Value << (theme.isDark() ? "dark" : "light");

	out << YAML::Key << "colors";
	out << YAML::Value << YAML::BeginMap;
	for (const auto& [key, val] : colors)
	{
		if (auto str = enumToStr(Theme::getColorNames(), key))
		{
			out << YAML::Key << str;
			out << YAML::Value;
			dumpVec4SeqRound(out, (const float*) &val, 3);
		}
	}
	out << YAML::EndMap;

	out << YAML::Key << "sizes";
	out << YAML::Value << YAML::BeginMap;
	for (const auto& [key, val] : sizes1)
	{
		if (auto* str = enumToStr(Theme::getSizeNames(), key))
		{
			out << YAML::Key << str;
			out << YAML::Value << val;
		}
	}
	out << YAML::EndMap;

	out << YAML::Key << "size vectors";
	out << YAML::Value << YAML::BeginMap;
	for (const auto& [key, val] : sizes2)
	{
		if (auto* str = enumToStr(Theme::getSizeVecNames(), key))
		{
			out << YAML::Key << str;
			out << YAML::Value;
			dumpVec2Seq(out, (const float*) &val);
		}
	}
	out << YAML::EndMap;

	out << YAML::EndMap;

	std::ofstream outfile(path);
	outfile << out.c_str() << "\n";
	outfile.close();
}

//--------------------------------------------------------------------------------------------------------------------//

static bool isDark(YAML::Node& yaml)
{
	if (!yaml["mode"] || !yaml["mode"].IsScalar())
	{
		return true;
	}

	auto shouldSetLight = yaml["mode"].as<std::string>() == "light";

	return !shouldSetLight;
}

static Theme::Colors loadColors(YAML::Node& yaml)
{
	Theme::Colors colors;

	if (!yaml["colors"])
	{
		return colors;
	}

	auto yamlColors = yaml["colors"];
	for (YAML::const_iterator it = yamlColors.begin(); it != yamlColors.end(); ++it)
	{
		auto node = it->second.as<YAML::Node>();

		if (auto en = strToEnum(Theme::getColorNames(), it->first.as<std::string>()))
		{
			auto result = parseVec4Seq(node);
			if (result)
			{
				colors[*en] = *result;
			}
			else
			{
				LOG_ERROR("[loadTheme] Invalid value in file: {}", it->first.as<std::string>());
			}
		}
		else
		{
			LOG_ERROR("[loadTheme] Invalid name {}", it->first.as<std::string>());
		}
	}

	return colors;
}

static Theme::Sizes loadSizes(YAML::Node& yaml)
{
	Theme::Sizes sizes;

	if (!yaml["sizes"])
	{
		return sizes;
	}

	auto yamlSizes = yaml["sizes"];
	for (YAML::const_iterator it = yamlSizes.begin(); it != yamlSizes.end(); ++it)
	{
		if (auto en = strToEnum(Theme::getSizeNames(), it->first.as<std::string>()))
		{
			sizes[*en] = {it->second.as<float>(), true}; // We set dpi scaling to true, it will get changed later
		}
		else
		{
			LOG_ERROR("[loadTheme] Invalid name {}", it->first.as<std::string>());
		}
	}

	return sizes;
}

static Theme::SizesVec loadSizeVectors(YAML::Node& yaml)
{
	Theme::SizesVec sizesVec;

	if (!yaml["size vectors"])
	{
		return sizesVec;
	}

	auto yamlSizeVec = yaml["size vectors"];
	for (YAML::const_iterator it = yamlSizeVec.begin(); it != yamlSizeVec.end(); ++it)
	{
		auto node = it->second.as<YAML::Node>();

		if (auto en = strToEnum(Theme::getSizeVecNames(), it->first.as<std::string>()))
		{
			auto result = parseVec2Seq(node);
			if (result)
			{
				sizesVec[*en] = {*result, true}; // We set dpi scaling to true, it will get changed later
			}
			else
			{
				LOG_ERROR("[loadTheme] Invalid value in file: {}", it->first.as<std::string>());
			}
		}
		else
		{
			LOG_ERROR("[loadTheme] Invalid name {}", it->first.as<std::string>());
		}
	}

	return sizesVec;
}

std::expected<Theme, Error> loadTheme(const fs::path& path)
{
	LOG_INFO("Loading theme from file: {}", path.string());

	if (!fs::exists(path))
	{
		return Err("File does not exist");
	}

	YAML::Node yaml;

	auto name = path.stem().string();

	try
	{
		yaml = YAML::LoadFile(path.string());

		auto theme = Theme(name, isDark(yaml), loadColors(yaml), loadSizes(yaml), loadSizeVectors(yaml));
		theme.initFonts();

		return theme;
	}
	catch (const std::exception& e)
	{
		return Err(fmt::format("Failed to parse theme file {}: {}", path.string(), e.what()));
	}
}
