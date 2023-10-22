/**
 * \file
 * \brief
 * \authors Michal Folta, Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Config.h"

#include <istream>

#include "Core/Resources/ResourceManager.h"
#include "Utils/JSON.h"

Ptr<Configuration> loadConfig(const fs::path& filename)
{
	auto conf = std::make_shared<Configuration>();

	rapidjson::Document doc;
	JSON::parse(filename, doc, "Data/Schemas/Config.schema.json");

	if (auto resources = Core::ResourceManager::readResources(doc["resources"]))
	{
		conf->Resources = *resources;
	}

	return conf;
}

// statics

std::string Config::VERSION = "v006";
std::string Config::AUTHOR = "Michal Folta";
std::string Config::MAIL = "michal.fol@gmail.com";
std::string Config::SUPERVISOR = "Petr Felkel";
std::string Config::SUPERVISOR_MAIL = "felkel@fel.cvut.cz";
std::string Config::WEB_LINK = "http://dcgi.felk.cvut.cz/theses/2016/foltamic";
std::string Config::OFFICIAL_SITE = "http://dcgi.fel.cvut.cz/";
std::string Config::INFO_TEXT = "Learn geometric transformations.\n\n"
                                "I3T is an educational application which "
                                "enables the study of 3D transformations and "
                                "their hierarchy in an illustrative way.\n\n"
                                "This tool is a result of master's thesis done "
                                "at the Department of Computer "
                                "Graphics and Interaction, FEL, CTU in Prague";
std::string Config::COPYRIGHT_TEXT = "I3T - The Interactive Tool for Teaching Transformations\n\n"

                                     "  Copyright (C) 2016-2023 I3T team, Department of Computer Graphics\n"
                                     "  and Interaction, FEE, Czech Technical University in Prague, Czech Republic.\n\n"

                                     "I3T - The Interactive Tool for Teaching Transformations\n"
                                     "  is free software: you can redistribute it and/or modify\n"
                                     "  it under the terms of the GNU General Public License as published by\n"
                                     "  the Free Software Foundation, either version 3 of the License, or\n"
                                     "  (at your option) any later version.\n\n"

                                     "I3T - The Interactive Tool for Teaching Transformations\n"
                                     "  is distributed in the hope that it will be useful,\n"
                                     "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                                     "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
                                     "  See the GNU General Public License for more details.\n\n"

                                     "You should have received a copy of the GNU General Public License.\n"
                                     "  with this program in the attached file LICENSE.txt. \n"
                                     "  If not, see <https://www.gnu.org/licenses/gpl-3.0.txt>.\n\n"

                                     "Project home: http://www.i3t-tool.org\n\n\n";
std::string Config::COPYRIGHT_TEXT_SHORT =
    "I3T - The Interactive Tool for Teaching Transformations\n\n"

    "  Copyright (C) 2016-2023 I3T team, Department of Computer Graphics\n"
    "  and Interaction, FEE, Czech Technical University in Prague, Czech Republic.\n\n"

    "  This program comes with ABSOLUTELY NO WARRANTY. This is free software,\n"
    "  and you are welcome to redistribute it under certain conditions.\n"
    "  For details, read the attached file LICENSE.txt.\n";

glm::vec3 Config::LINK_COLOR = glm::vec3(0.55f, 0.8f, 1.0f);

std::string Config::DEF_DESC =

    "Describe your scene here.\n"
    "\n"
    "Controls:\n"
    "-------------------------\n"
    "\n"
    "Camera:\n"
    "\n"
    "mouse right  - rotate\n"
    "mouse middle - pan\n"
    "mouse scroll - zoom\n"
    "\n"
    "work space:\n"
    "\n"
    "mouse left   - components movement\n"
    "mouse right  - menu\n"
    "mouse middle - pan\n"
    "mouse scroll - zoom\n";
///< Text of the help - \todo Complete the help: Fill in all missing key
///< combinations into the help window in config.h

bool Config::SHOW_CONSOLE = false;
bool Config::FULLSCREEN = false;

int Config::WIN_WIDTH = 1400;
int Config::WIN_HEIGHT = 900;

glm::vec3 Config::BACKGROUND_COLOR = glm::vec3(0.3f, 0.3f, 0.35f);

int Config::REFRESHTIME = 17;

float Config::CAM_NEAR = 1.0f;
float Config::CAM_FAR = 1000.0f;

float Config::MOUSE_SENSITIVITY = 0.2f;
float Config::CAM_MOTION_SENSITIVITY = 0.1f;
float Config::CAM_ROLL_SENSITIVITY = 0.001f;
float Config::CAM_SCROLL_SENSITIVITY = 0.02f; //
float Config::CAM_LERP_DISTTOSTEP_KOEF = 0.05f;

std::string Config::CONTENT_FILE = "Data/content.cfg";
std::string Config::LOAD_SCENE = "NONE";

std::string Config::TUTORIALS_FOLDER = "Data/Tutorials/";
std::string Config::TEXTURE_FOLDER = "Data/Textures/";
