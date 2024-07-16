/**
 * \file
 * \brief
 * \authors Michal Folta <michal.fol@gmail.com>, Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Core/Resources/Resource.h"
#include "Utils/FilesystemUtils.h"

// TODO: (DR) This stuff should probably be moved to StateManager, especially the root and sceneRoot
struct Configuration
{
	static inline const fs::path root = fs::current_path();
	static inline const fs::path sceneRoot = root / "Data" / "Scenes";

	static inline const fs::path configFile = root / "Data" / "Config.json";
	static inline const fs::path logEventsDefinition = root / "Data" / "LogEventsDefinition.json";
	static inline const fs::path appLog = root / "Logs" / "App.log";
	static inline const fs::path userInteractionLog = root / "Logs" / "UserInteraction.log";
	static inline const fs::path mouseLog = root / "Logs" / "Mouse.log";
};

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
	static std::string VERSION;              ///< The I3T version
	static std::string AUTHOR;               ///< The author (Michal Folta)
	static std::string MAIL;                 ///< The author's mail
	static std::string SUPERVISOR;           ///< The supervisor (PF)
	static std::string SUPERVISOR_MAIL;      ///< The supervisor's mail
	static std::string WEB_LINK;             ///< The web link to the thesis page
	static std::string OFFICIAL_SITE;        ///< The official site
	static std::string INFO_TEXT;            ///< The information text \todo Add year and
	                                         ///< note of the continual development.
	static std::string COPYRIGHT_TEXT;       ///< printed on the console
	static std::string COPYRIGHT_TEXT_SHORT; ///< printed on the console
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
};
