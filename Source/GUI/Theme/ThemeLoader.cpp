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

void dumpVec2(YAML::Emitter& out, const float* vec)
{
	out << YAML::BeginMap;
	out << YAML::Key << "x" << YAML::Value << vec[0];
	out << YAML::Key << "y" << YAML::Value << vec[1];
	out << YAML::EndMap;
}

void dumpVec2Seq(YAML::Emitter& out, const float* vec)
{
	out << YAML::Flow << YAML::BeginSeq;
	out << vec[0];
	out << vec[1];
	out << YAML::EndSeq << YAML::Block;
}

void dumpVec4(YAML::Emitter& out, const float* vec)
{
	out << YAML::BeginMap;
	out << YAML::Key << "x" << YAML::Value << vec[0];
	out << YAML::Key << "y" << YAML::Value << vec[1];
	out << YAML::Key << "z" << YAML::Value << vec[2];
	out << YAML::Key << "w" << YAML::Value << vec[3];
	out << YAML::EndMap;
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

ImVec2 parseVec2(YAML::Node& node)
{
	ImVec2 vec;
	vec.x = node["x"].as<float>();
	vec.y = node["y"].as<float>();
	return vec;
}

ImVec2 parseVec2Seq(YAML::Node& node)
{
	ImVec2 vec;
	vec.x = node[0].as<float>();
	vec.y = node[1].as<float>();
	return vec;
}

ImVec4 parseVec4(YAML::Node& node)
{
	ImVec4 vec;
	vec.x = node["x"].as<float>();
	vec.y = node["y"].as<float>();
	vec.z = node["z"].as<float>();
	vec.w = node["w"].as<float>();
	return vec;
}

ImVec4 parseVec4Seq(YAML::Node& node)
{
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
		sizes1.emplace_back(key, val);
	std::vector<std::pair<ESizeVec2, ImVec2>> sizes2;
	for (const auto& [key, val] : theme.getSizesVecRef())
		sizes2.emplace_back(key, val);

	std::sort(colors.begin(), colors.end(), compareThemeEntry<std::pair<EColor, ImVec4>>);
	std::sort(sizes1.begin(), sizes1.end(), compareThemeEntry<std::pair<ESize, float>>);
	std::sort(sizes2.begin(), sizes2.end(), compareThemeEntry<std::pair<ESizeVec2, ImVec2>>);

	// Generate yml file

	YAML::Emitter out;
	out << YAML::BeginMap;

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

std::expected<Theme, Error> loadTheme(const fs::path& path)
{
	if (!fs::exists(path))
	{
		return Err("File does not exist");
	}

	YAML::Node yaml;
	try
	{
		yaml = YAML::LoadFile(path.string());
	}
	catch (const std::exception& e)
	{
		return Err(std::string(e.what()));
	}

	auto name = path.stem().string();

	Theme::Colors colors;
	Theme::Sizes sizes;
	Theme::SizesVec sizesVec;

	if (yaml["colors"])
	{
		auto yamlColors = yaml["colors"];
		for (YAML::const_iterator it = yamlColors.begin(); it != yamlColors.end(); ++it)
		{
			auto node = it->second.as<YAML::Node>();

			if (auto en = strToEnum(Theme::getColorNames(), it->first.as<std::string>()))
			{
				colors[*en] = parseVec4Seq(node);
			}
			else
				LOG_ERROR("[loadTheme] Invalid name {} in file: {}", it->first.as<std::string>(), name);
		}
	}
	if (yaml["sizes"])
	{
		auto yamlSizes = yaml["sizes"];
		for (YAML::const_iterator it = yamlSizes.begin(); it != yamlSizes.end(); ++it)
		{
			if (auto en = strToEnum(Theme::getSizeNames(), it->first.as<std::string>()))
			{
				sizes[*en] = it->second.as<float>();
			}
		}
	}
	if (yaml["size vectors"])
	{
		auto yamlSizeVec = yaml["size vectors"];
		for (YAML::const_iterator it = yamlSizeVec.begin(); it != yamlSizeVec.end(); ++it)
		{
			auto node = it->second.as<YAML::Node>();

			if (auto en = strToEnum(Theme::getSizeVecNames(), it->first.as<std::string>()))
			{
				sizesVec[*en] = parseVec2Seq(node);
			}
		}
	}

	auto theme = Theme(name, colors, sizes, sizesVec);
	theme.initFonts();

	return theme;
}
