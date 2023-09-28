#include "Config.h"

#include <fstream>
#include <istream>

#include "Core/Resources/ResourceManager.h"
#include "Logger/Logger.h"
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

bool Config::getValue(std::istream& is, const std::string& input, const std::string& attribName, float& val)
{
	if (input == attribName)
	{
		is >> val;
		if (is.good())
		{
			// cout << attribName << " : " << val << std::endl;
			return true;
		}
	}
	return false;
}

bool Config::getBool(std::istream& is, const std::string& input, const std::string& attribName, bool& val)
{
	if (input == attribName)
	{
		float f;
		is >> f;
		if (is.good())
		{
			// cout << attribName << " : " << val << std::endl;
			f != 0 ? val = true : val = false;
			return true;
		}
	}
	return false;
}

bool Config::getWord(std::istream& is, const std::string& input, const std::string& attribName, std::string& val)
{
	if (input == attribName)
	{
		is >> val;
		if (is.good())
		{
			// cout << attribName << " : " << val << std::endl;
			return true;
		}
	}
	return false;
}

bool Config::getVec2(std::istream& is, const std::string& input, const std::string& attribName, glm::vec2& val)
{
	if (input == attribName)
	{
		is >> val.x >> val.y;
		if (is.good())
		{
			// cout << attribName << " : " << val.x << ", " << val.y << ", " << val.z
			// << std::endl;
			return true;
		}
	}
	return false;
}

bool Config::getVec3(std::istream& is, const std::string& input, const std::string& attribName, glm::vec3& val)
{
	if (input == attribName)
	{
		is >> val.x >> val.y >> val.z;
		if (is.good())
		{
			// cout << attribName << " : " << val.x << ", " << val.y << ", " << val.z
			// << std::endl;
			return true;
		}
	}
	return false;
}

bool Config::getVec4(std::istream& is, const std::string& input, const std::string& attribName, glm::vec4& val)
{
	if (input == attribName)
	{
		is >> val.x >> val.y >> val.z >> val.w;
		if (is.good())
		{
			// cout << attribName << " : " << val.x << ", " << val.y << ", " << val.z
			// << std::endl;
			return true;
		}
	}
	return false;
}

bool Config::getMat4(std::istream& is, const std::string& input, const std::string& attribName, glm::mat4& val)
{
	if (input == attribName)
	{
		is >> val[0][0] >> val[1][0] >> val[2][0] >> val[3][0] >> val[0][1] >> val[1][1] >> val[2][1] >> val[3][1] >>
		    val[0][2] >> val[1][2] >> val[2][2] >> val[3][2] >> val[0][3] >> val[1][3] >> val[2][3] >> val[3][3];
		if (is.good())
		{
			// cout << key << " : " << std::endl;
			// cout << val[0][0] << ", " << val[1][0] << ", " << val[2][0] << ", " <<
			// val[3][0] << std::endl; cout << val[0][1] << ", " << val[1][1] << ", "
			// << val[2][1] << ", " << val[3][1] << std::endl; cout << val[0][2] << ",
			// " << val[1][2] << ", " << val[2][2] << ", " << val[3][2] << std::endl;
			// cout << val[0][3] << ", " << val[1][3] << ", " << val[2][3] << ", " <<
			// val[3][3] << std::endl;
			return true;
		}
	}
	return false;
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

std::string Config::TUTORIALS_FOLDER = "Data/tutorials/";
std::string Config::TEXTURE_FOLDER = "Data/textures/";