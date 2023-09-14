#include "ThemeLoader.h"

#include <fstream>
#include <map>

#include "yaml-cpp/yaml.h"

#include "GUI/Theme.h"
#include "Utils/FilesystemUtils.h"

template <typename T> std::optional<T> strToEnum(std::map<T, const char*>& map, std::string&& name)
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

void dumpVec4(YAML::Emitter& out, const float* vec)
{
	out << YAML::BeginMap;
	out << YAML::Key << "x" << YAML::Value << vec[0];
	out << YAML::Key << "y" << YAML::Value << vec[1];
	out << YAML::Key << "z" << YAML::Value << vec[2];
	out << YAML::Key << "w" << YAML::Value << vec[3];
	out << YAML::EndMap;
}

ImVec2 parseVec2(YAML::Node& node)
{
	ImVec2 vec;
	vec.x = node["x"].as<float>();
	vec.y = node["y"].as<float>();

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

void saveTheme(const fs::path& path, Theme& theme)
{
	if (path.empty())
	{
		return;
	}

	YAML::Emitter out;
	out << YAML::BeginMap;

	out << YAML::Key << "colors";
	out << YAML::Value << YAML::BeginMap;
	for (const auto& [key, val] : theme.getColorsRef())
	{
		if (auto str = enumToStr(Theme::getColorNames(), key))
		{
			out << YAML::Key << str;
			out << YAML::Value;
			dumpVec4(out, (const float*) &val);
		}
	}
	out << YAML::EndMap;

	out << YAML::Key << "sizes";
	out << YAML::Value << YAML::BeginMap;
	for (const auto& [key, val] : theme.getSizesRef())
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
	for (const auto& [key, val] : theme.getSizesVecRef())
	{
		if (auto* str = enumToStr(Theme::getSizeVecNames(), key))
		{
			out << YAML::Key << str;
			out << YAML::Value;
			dumpVec2(out, (const float*) &val);
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
	if (!FilesystemUtils::doesFileExists(path.string().c_str()))
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
				colors[*en] = parseVec4(node);
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
				sizesVec[*en] = parseVec2(node);
			}
		}
	}

	auto theme = Theme(name, colors, sizes, sizesVec);
	theme.initFonts();

	return theme;
}
