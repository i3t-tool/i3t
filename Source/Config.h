/**
 * \file	config.h
 *
 * Declares the configuration structure
 *
 * \date  	2014/11/16
 * \author 	Michal Folta, CTU Prague
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Core/Resources/Resource.h"
#include "Utils/FilesystemUtils.h"

struct Configuration
{
	std::vector<Core::Resource> Resources;

	static inline const fs::path root = fs::current_path();
	static inline const fs::path sceneRoot = root / "Data/Scenes";

	static inline const fs::path logEventsDefinition = root / "Data/LogEventsDefinition.json";
	static inline const fs::path appLog = root / "Logs/App.log";
	static inline const fs::path userInteractionLog = root / "Logs/UserInteraction.log";
	static inline const fs::path mouseLog = root / "Logs/Mouse.log";
};

Ptr<Configuration> loadConfig(const fs::path& filename);

// TODO: (DR) Pretty sure this is all obsolete at this point and some parts of it should be moved somewhere else
/**
 * A configuration structure.
 *
 * it is read by loadFromFile(), default is in the file \\cfg_default.dcfg
 */
struct Config
{
	//@{
	/** \name Text for the about window */
	static std::string VERSION;         ///< The I3T version
	static std::string AUTHOR;          ///< The author (Michla Folta)
	static std::string MAIL;            ///< The author's mail
	static std::string SUPERVISOR;      ///< The supervisor (PF)
	static std::string SUPERVISOR_MAIL; ///< The supervisor's mail
	static std::string WEB_LINK;        ///< The web link to the thesis page
	static std::string OFFICIAL_SITE;   ///< The official site
	static std::string INFO_TEXT;       ///< The information text \todo Add year and
	                                    ///< note of the continual development.
	static glm::vec3 LINK_COLOR;
	///< The url link (mailTo: and http:) color used in aboutForm.h \todo Move to
	///< the config structure
	static std::string DEF_DESC;
	///< Information describing the definition - initial state of the scene
	///< description \todo Update to contain all key bindings from tabsConfig.cpp
	//@}

	static bool SHOW_CONSOLE; ///< True to show, false to hide the console
	static bool FULLSCREEN;   ///< True to fullscreen

	static int WIN_WIDTH;  ///< Width of the window - this is not a constant - set
	                       ///< on onReshape in main.cpp
	static int WIN_HEIGHT; ///< Height of the window - this is not a constant -
	                       ///< set on onReshape in main.cpp

	static int REFRESHTIME; ///< The refreshtime

	static glm::vec3 BACKGROUND_COLOR; ///< The background color

	static float CAM_NEAR; ///< The camera near plane
	static float CAM_FAR;  ///< The camera far plane

	static float MOUSE_SENSITIVITY;        ///< The mouse sensitivity
	static float CAM_MOTION_SENSITIVITY;   ///< The camera motion sensitivity
	static float CAM_SCROLL_SENSITIVITY;   ///< The camera scroll sensitivity
	static float CAM_ROLL_SENSITIVITY;     ///< The camera roll sensitivity
	static float CAM_LERP_DISTTOSTEP_KOEF; ///< The camera linearly interpolate
	                                       ///< disttostep koef

	// content
	static std::string CONTENT_FILE;
	///< The content file, such as \\data\\content.cnt, enumerating the
	///< \\data\\*.cfg files (textures, materials (Phong parameters + textures),
	///< geometries (models), and objects (combines names, geometries and textures
	///< to groups of named objects))
	static std::string LOAD_SCENE; ///< The load scene
	static std::string TUTORIALS_FOLDER;
	static std::string TEXTURE_FOLDER;

	//@{

	/**
	    \name Readers of standard data types.

	    The caller provides the \a attribName and the \a variable to be
	    read to; and also the std::string just read from the file \a input.
	    When the std::string \a input equals to the \a attribName, the
	    routine reads the value(s) of the attribute from the stream
	    consuming the bytes following \a attribName text. Otherwise
	    it reads nothing leaving the input stream \a is untouched.
	    These routines are all called on each read attribute name,
	    serving similarly as a switch(input).
	 */

	/**
	 * Read a float of given name from the input stream
	 *
	 * \param [in]		is   		The input stream.
	 * \param 		  	input		The std::string just read from the stream containing
	 * the name of the attribute which follows. Is provided by the file reader.
	 * \param attribName 	The std::string with the attribute name to be read from
	 * the stream. This value is being compared with the input \param [out]		val
	 * The read value is stored to the given variable.
	 *
	 * \return	True if it succeeds to read the attribute value, false if it
	 * fails.
	 */
	static bool getValue(std::istream& is, const std::string& input, const std::string& attribName, float& val);

	/**
	 * Read a bool of given name from the input stream
	 *
	 * \param [in]		is   		The input stream.
	 * \param 		  	input		The std::string just read from the stream containing
	 * the name of the attribute which follows. Is provided by the file reader.
	 * \param attribName 	The std::string with the attribute name to be read from
	 * the stream. This value is being compared with the input \param [out]		val
	 * The read value is stored to the given variable.
	 *
	 * \return	True if it succeeds to read the attribute value, false if it
	 * fails.
	 */

	static bool getBool(std::istream& is, const std::string& input, const std::string& attribName, bool& val);

	/**
	 * Read a word (string) of given name from the input stream
	 *
	 * \param [in]		is   		The input stream.
	 * \param 		  	input		The std::string just read from the stream containing
	 * the name of the attribute which follows. Is provided by the file reader.
	 * \param attribName 	The std::string with the attribute name to be read from
	 * the stream. This value is being compared with the input \param [out]		val
	 * The read value is stored to the given variable.
	 *
	 * \return	True if it succeeds to read the attribute value, false if it
	 * fails.
	 */

	static bool getWord(std::istream& is, const std::string& input, const std::string& attribName, std::string& val);

	/**
	 * Read vector 2 of given name from the input stream
	 *
	 * \param [in]		is   		The input stream.
	 * \param 		  	input		The std::string just read from the stream containing
	 * the name of the attribute which follows. Is provided by the file reader.
	 * \param attribName 	The std::string with the attribute name to be read from
	 * the stream. This value is being compared with the input \param [out]		val
	 * The read value is stored to the given variable.
	 *
	 * \return	True if it succeeds to read the attribute value, false if it
	 * fails.
	 */

	static bool getVec2(std::istream& is, const std::string& input, const std::string& attribName, glm::vec2& val);

	/**
	 * Read vector 3 of given name from the input stream
	 *
	 * \param [in]		is   		The input stream.
	 * \param 		  	input		The std::string just read from the stream containing
	 * the name of the attribute which follows. Is provided by the file reader.
	 * \param attribName 	The std::string with the attribute name to be read from
	 * the stream. This value is being compared with the input \param [out]		val
	 * The read value is stored to the given variable.
	 *
	 * \return	True if it succeeds to read the attribute value, false if it
	 * fails.
	 */

	static bool getVec3(std::istream& is, const std::string& input, const std::string& attribName, glm::vec3& val);

	/**
	 * Read a vector 4 of given name from the input stream
	 *
	 * \param [in]		is   		The input stream.
	 * \param 		  	input		The std::string just read from the stream containing
	 * the name of the attribute which follows. Is provided by the file reader.
	 * \param attribName 	The std::string with the attribute name to be read from
	 * the stream. This value is being compared with the input \param [out]		val
	 * The read value is stored to the given variable.
	 *
	 * \return	True if it succeeds to read the attribute value, false if it
	 * fails.
	 */

	static bool getVec4(std::istream& is, const std::string& input, const std::string& attribName, glm::vec4& val);

	/**
	 * Read a matrix 4 of given name from the input stream
	 *
	 * \param [in]		is   		The input stream.
	 * \param 		  	input		The std::string just read from the stream containing
	 * the name of the attribute which follows. Is provided by the file reader.
	 * \param attribName 	The std::string with the attribute name to be read from
	 * the stream. This value is being compared with the input \param [out]		val
	 * The read value is stored to the given variable.
	 *
	 * \return	True if it succeeds to read the attribute value, false if it
	 * fails.
	 */

	static bool getMat4(std::istream& is, const std::string& input, const std::string& attribName, glm::mat4& val);

	/**
	 * Skip comments. This routine consumes either a single line comment (starting
	 * with # or //) or a multiline comment
	 * - Does not allow nested comments! \todo the only method changing the value
	 * of input and only for multiline comments - change to blank...
	 *
	 * \param [in]	is   	The input stream.
	 * \param 	  	input	The std::string just read from the stream.
	 */

	static void skipComments(std::istream& is, std::string input);
};

#endif
