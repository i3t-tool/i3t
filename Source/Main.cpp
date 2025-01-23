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

#include <string>

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "I3T.h"
#include "Logger/Logger.h"

#include "Utils/Other.h"

/**
 * \brief	Main entry-point for this application
 *
 * 			If the preprocessor definition RELEASE_STANDALONE exists, it defines the
 * application directory to the current directory. If not, it sets the
 * application directory either to Release, or to a Debug subdirectory of the
 * project directory.
 *
 * 			Then, it loads the config and screen files, if given as an command line
 * argument. \param	argc	The number of command-line arguments provided. \param
 * argv	An array of command-line argument strings. \return	Exit-code for the
 * process - 0 for success, else an error code.
 */
int main(int argc, char* argv[])
{
	std::cout << (Config::COPYRIGHT_TEXT);

	// init the logging library
	INIT_LOGGER(argc, argv);

	LOG_INFO("Working directory is {}.", Configuration::root.string());

	// Destroy app before logger
	int exitCode = 0;
	{
		I3TApplication app;

		// Initialize all modules.
		if (!app.init())
		{
			LOG_FATAL("Failed to initialize I3T!");
			exit(EXIT_FAILURE);
		}

		exitCode = app.run();
	}

	LOG_INFO("I3T is now exited.");

	END_LOGGER;

	return exitCode;
}
