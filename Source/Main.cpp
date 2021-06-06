/**
 * \file       main.cpp
 * \author     Michal Folta
 * \date
 * \brief      Here is the main procedure.
 */

/**
 * \brief	\mainpage Documentation before refactoring
 *
 *  I3T is an educational application which enables the study of 3D transformations and their hierarchy in an
 * illustrative way. This tool is a result of master's thesis by Michal Folta done in the year 2016 at the <A
 * href="http://dcgi.fel.cvut.cz/">Department of Computer Graphics and Interaction</a>, FEL, CTU in Prague. The
 * development continues... The tool homepage: http://www.i3t-tool.org/.
 *
 * 	\section Layout The layout of the GUI
 * 	The scene is divided into two parts - the 3D scene preview and a workspace with the scene graph boxes.
 * 	- 3D scene is defined in the class World and rendered using the class Scene.
 * 	- The workspace is stored in tabGroup.layers[1].tabs.
 * 	TabGroup is the root of all GUI objects.
 *
 * 	\section Engine Engine
 *
 *
 *  \section Logic Logic
 *  \section GUI GUI
 *  - base components: button, form, textTab, checkBox,... They are created independently on the rest of the program
 *  - specialized components for I3T tool:
 *  The base class is the class \a Tab. This class serves as a base class for all GUI elements, such as boxes on the
 *  screen, such as Forms (boxes), operators, labels, check-boxes, numerical edit fields, ...
 *  The colors, sizes, and state of GUI components (and also the colors of the wires) and the key bindings are defined
 *  in a static structure \a TabsConfig.
 *
 *  \section main main.cpp
 *  Main defines the World - update() handles all interaction.
 *
 *  \section Documentation External documentation
 *  Files with models can be either in Wavefront <A href="https://en.wikipedia.org/wiki/Wavefront_.obj_file">.obj</a>
 * format or in a proprietary .tmsh format.
 *
 *  \section Notes Notes not to forget about
 *  - <A href="./todo.html">todolist </a>
 *  - Properties
 *    - REMOVED: C++/Advanced: Disable specific warnings: 4482;%(DisableSpecificWarnings)
 *      Qualifying an enumeration value with the name of the enumeration is not valid C++03, it is valid C++11 - it
 * was a problem for VS2010
 *    - Enable incremental linking: YES
 *    - Edit and continue <A
 * href="https://docs.microsoft.com/en-us/visualstudio/debugger/edit-and-continue?view=vs-2019>Documentation</a> <br>
 *       Tools > Options > (or Debug > Options) > Debugging > General, select Edit and Continue in the right pane.
 *      - Apply changes on continue (Native only)
 *        If selected, Visual Studio automatically compiles and applies code changes when you continue debugging from
 * a break state. Otherwise, you can choose to apply changes using Debug > Apply Code Changes.
 *      - Warn about stale code (Native only)
 *        If selected, gives warnings about stale code.
 */

// Uncomment this to create a standalone release version
//#define RELEASE_STANDALONE     //PF Nepouzivat!!! Je definovan target Release Standalone a v nem je toto definov�no
// - outdir je ../release
// - hleda data v miste, kde je spusten
// - negeneruje pdb

#include <sstream>
#include <string>

#include "pgr.h"

#include "Config.h"
#include "Core/Defs.h"
#include "Logger/LoggerInternal.h"
#include "Utils/Other.h"

#include "Commands/ApplicationCommands.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"

int window; ///< current main window

static const std::string DIE_SEND_MAIL =
		"If it does not help, send me an email to felkepet@fel.cvut.cz with the snapshot of the program messages "
		"as they appear in the program console.\n";

static const std::string DIE_TEXT =
		"The I3T tool did not start. "
		"Please try the following steps to solve the problem and if it does not help, "
		"send me an email to felkepet@fel.cvut.cz with the snapshot of the program messages "
		"as they appear in the program console.\n"
		" \n"
		"The steps to try:\n"
		" \n"
		" - The program is written in Visual Studio 2010 - you may need to install the Visual studio 2010 runtime "
		"libraries"
		" from https://www.microsoft.com/en-us/download/details.aspx?id=5555 \n"
		" \n"
		" - the program is targeted to a Graphics card with OpenGL 3.1. If you have a internal and external GPU,"
		" switch to the external GPU card. You may need to add transform.exe in the GPU control panel/3D settings"
		" to the list of progams using the dedicated GPU card.\n";

static const std::string DIE_TEXT_OPENGL_VERSION =
		"The I3T tool did not start. \n\n"
		"The program is targeted to a Graphics card with OpenGL 3.1. or higher. If you have a system with two graphics "
		"cards, internal and external, "
		" switch to the external GPU card please. You may need to add transform.exe in the GPU control panel/3D settings"
		" to the list of progams using the dedicated GPU card.\n"
		" \n" +
		DIE_SEND_MAIL;

static const std::string DIE_TEXT_PROGRAM_INIT =
		"The I3T tool did not start. \n\n"
		"The program is written in Visual Studio 2010 - you may need to install the Visual studio 2010 runtime libraries "
		"from https://www.microsoft.com/en-us/download/details.aspx?id=5555 .\n"
		" \n" +
		DIE_SEND_MAIL;

// Dal�� sd�len�
//- jako v�ukov� sc�ny pou�ijte 01xxx ... 0nxxx
//- ostatn� sc�ny jsou taky u�ite�n� a demonstruj� dal�� transformace a �lohy z grafiky a schopnosti n�stroje I3T, ale
// nejsou sou��st� testov�n�
//- vydr�te se pros�m s nik�m z jin�ch cvi�en� n�sleduj�c� 2 t�dny nebavit o testov�n� a o n�stroji I3T, zkreslilo by
// to v�sledek testov�n�
//"
/**
 * \brief	Main entry-point for this application
 *
 * 			If the preprocessor definition RELEASE_STANDALONE exists, it defines the application
 * 			directory to the current directory. If not, it sets the application directory either
 * 			to Release, or to a Debug subdirectory of the project directory.
 *
 * 			Then, it loads the config and screen files, if given as an command line argument.
 * \param	argc	The number of command-line arguments provided.
 * \param	argv	An array of command-line argument strings.
 * \return	Exit-code for the process - 0 for success, else an error code.
 */
int main(int argc, char* argv[])
{
#ifdef I3T_RELEASE_STANDALONE // variant for standalone release
	std::string root = "";
#else // special settings for usage in Visual Studio devenv
  auto root = I3T_PROJECT_ROOT;
#endif

	Config::WORKING_DIRECTORY = root;

	// init the logging library
	INIT_LOGGER(argc, argv);

	LOG_INFO("Working directory is {}.", Config::WORKING_DIRECTORY);

	///   - seek command line parameters for config files
	/* True if the .dcgf file was given as a command line parameter and must be loaded. */
	bool dcfgFlag = false;
	std::string dcfg;

	/* True to load the screen file, the .scn file was given as a command line parameter and must be loaded.*/
	bool scnFlag = false;
	std::string scn;

	for (int i = 1; i < argc; i++)
	{
		if (Config::getFileExtension(argv[i]) == "scn")
		{
			scn = argv[i];
			scnFlag = true;
		}
		else if (Config::getFileExtension(argv[i]) == "dcfg")
		{
			dcfg = argv[i];
			dcfgFlag = true;
		}
	}

	///   - load the configuration file .dcfg
	if (dcfgFlag)
		Config::loadFromFile(dcfg);
	else
	{
		Config::loadFromFile(Config::getAbsolutePath("cfg_default.dcfg"));
	}
	///   - load the scene
	/// \todo Load scene in App::initI3T().
	if (scnFlag)
		Config::LOAD_SCENE = scn;

	/// \todo Window icon!

	/// \todo Run app in fullscreen mode.
	// if (Config::FULLSCREEN)
	//  glutFullScreen();
	//---------------------------------------------------------------------------

	// Get application instance.
	Application& app = Application::get();

	// I. Create GLFW window.
	app.initWindow();

	// II. PGR and OpenGL initializing.
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR, pgr::DEBUG_OFF))
	{
		SystemDialogs::FireErrorMessageDialog("I3T", DIE_TEXT_OPENGL_VERSION);
	}

	// Initialize all modules.
	app.init();

	// III. Initialize I3T stuff,
	// read other config files.
	// Load objects(geometry and textures), create the world and load the initial scene.
	// Loads the shaders, updates the camera mode.
	if (!app.initI3T())
	{
		SystemDialogs::FireErrorMessageDialog("I3T", DIE_TEXT_PROGRAM_INIT);
		LOG_FATAL("Cannot initialize I3T stuffs.");
		exit(-1);
	}

	app.initModules();

	app.run();

	app.finalize();

	LOG_INFO("I3T is now exited.");

	END_LOGGER;

	return 0;
}
