#include "ConfigLoader.h"

#include "yaml-cpp/yaml.h"

#include "Config.h"
#include "Logger/Logger.h"
#include "Utils.h"
#include "Utils/Filesystem.h"

void loadConfig()
{
	auto configFile = "config.yml";

	if (!doesFileExists(configFile))
		LOG_FATAL("Unable to load 'config.yml' file.");

	auto yaml = YAML::LoadFile(configFile);

	getValue(yaml["theme"], Config::DEFAULT_THEME);
}

void saveConfig()
{
	YAML::Emitter out;
	out << YAML::BeginMap;

	out << YAML::Key << "theme" << YAML::Value << Config::DEFAULT_THEME;

	out << YAML::EndMap;

	std::ofstream outfile("config.yml");
	outfile << out.c_str() << "\n";
	outfile.close();
}
