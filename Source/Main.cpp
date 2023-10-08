/**
 * \brief	\mainpage Documentation before refactoring
 *
 *  I3T is an educational application which enables the study of 3D
 * transformations and their hierarchy in an illustrative way. This tool is a
 * result of master's thesis by Michal Folta done in the year 2016 at the <A
 * href="http://dcgi.fel.cvut.cz/">Department of Computer Graphics and
 * Interaction</a>, FEL, CTU in Prague. The development continues... The tool
 * homepage: http://www.i3t-tool.org/.
 *
 *  \section Layout The layout of the GUI
 *  The scene is divided into two parts - the 3D scene preview and a workspace
 * with the scene graph boxes.
 *  - 3D scene is defined in the class World and rendered using the class Scene.
 *  - The workspace is stored in tabGroup.layers[1].tabs.
 *  TabGroup is the root of all GUI objects.
 *
 *  \section Engine Engine
 *
 *
 *  \section Logic Logic
 *  \section GUI GUI
 *  - base components: button, form, textTab, checkBox,... They are created
 * independently on the rest of the program
 *  - specialized components for I3T tool:
 *  The base class is the class \a Tab. This class serves as a base class for
 * all GUI elements, such as boxes on the screen, such as Forms (boxes),
 * operators, labels, check-boxes, numerical edit fields, ... The colors, sizes,
 * and state of GUI components (and also the colors of the wires) and the key
 * bindings are defined in a static structure \a TabsConfig.
 *
 *  \section main main.cpp
 *  Main defines the World - update() handles all interaction.
 *
 *  \section Documentation External documentation
 *  Files with models can be either in Wavefront <A
 * href="https://en.wikipedia.org/wiki/Wavefront_.obj_file">.obj</a> format or
 * in a proprietary .tmsh format.
 *
 *  \section Notes Notes not to forget about
 *  - <A href="./todo.html">todolist </a>
 *  - Properties
 *    - REMOVED: C++/Advanced: Disable specific warnings:
 * 4482;%(DisableSpecificWarnings) Qualifying an enumeration value with the name
 * of the enumeration is not valid C++03, it is valid C++11 - it was a problem
 * for VS2010
 *    - Enable incremental linking: YES
 *    - Edit and continue <A
 * href="https://docs.microsoft.com/en-us/visualstudio/debugger/edit-and-continue?view=vs-2019>Documentation</a>
 * <br> Tools > Options > (or Debug > Options) > Debugging > General, select
 * Edit and Continue in the right pane.
 *      - Apply changes on continue (Native only)
 *        If selected, Visual Studio automatically compiles and applies code
 * changes when you continue debugging from a break state. Otherwise, you can
 * choose to apply changes using Debug > Apply Code Changes.
 *      - Warn about stale code (Native only)
 *        If selected, gives warnings about stale code.
 */

#include <filesystem>
#include <string>

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "I3T.h"
#include "Logger/Logger.h"

#include "Utils/Other.h"

static const std::string DIE_SEND_MAIL = "If it does not help, send me an email to felkepet@fel.cvut.cz with the "
                                         "snapshot of the program messages "
                                         "as they appear in the program console.\n";

static const std::string DIE_TEXT = "The I3T tool did not start. "
                                    "Please try the following steps to solve the problem and if it does not "
                                    "help, "
                                    "send me an email to felkepet@fel.cvut.cz with the snapshot of the program "
                                    "messages "
                                    "as they appear in the program console.\n"
                                    " \n"
                                    "The steps to try:\n"
                                    " \n"
                                    " - The program is written in Visual Studio 2010 - you may need to install "
                                    "the Visual studio 2010 runtime "
                                    "libraries"
                                    " from https://www.microsoft.com/en-us/download/details.aspx?id=5555 \n"
                                    " \n"
                                    " - the program is targeted to a Graphics card with OpenGL 3.1. If you "
                                    "have a internal and external GPU,"
                                    " switch to the external GPU card. You may need to add transform.exe in "
                                    "the GPU control panel/3D settings"
                                    " to the list of progams using the dedicated GPU card.\n";

static const std::string DIE_TEXT_OPENGL_VERSION =
    "The I3T tool did not start. \n\n"
    "The program is targeted to a Graphics card with OpenGL 3.1. or higher. If "
    "you have a system with two graphics "
    "cards, internal and external, "
    " switch to the external GPU card please. You may need to add "
    "transform.exe in the GPU control panel/3D settings"
    " to the list of progams using the dedicated GPU card.\n"
    " \n" +
    DIE_SEND_MAIL;

static const std::string DIE_TEXT_PROGRAM_INIT =
    "The I3T tool did not start. \n\n"
    "The program is written in Visual Studio 2010 - you may need to install "
    "the Visual studio 2010 runtime libraries "
    "from https://www.microsoft.com/en-us/download/details.aspx?id=5555 .\n"
    " \n" +
    DIE_SEND_MAIL;

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

	// Get application instance.
	{
		I3TApplication app;

		// Initialize all modules.
		app.init();

		app.run();
	}

	LOG_INFO("I3T is now exited.");

	END_LOGGER;

	return 0;
}
