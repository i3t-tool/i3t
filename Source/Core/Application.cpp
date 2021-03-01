#include "Application.h"

#include <functional>

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Module.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Modals/BeforeCloseModal.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/ImGui/imgui_impl_glfw.h"
#include "GUI/ImGui/imgui_impl_opengl3.h"
#include "GUI/Theme.h"
#include "GUI/UI.h"
#include "Rendering/ShaderProvider.h"
#include "Rendering/Shaper.h"
#include "Utils/Color.h"
#include "Utils/TextureLoader.h"

Application::Application()
{
	m_isPaused = false;
	m_world = nullptr;

	m_ui = new UI();
	m_modules.push_back(m_ui);
}

void Application::init()
{
  BeforeCloseCommand::addListener(std::bind(&App::onBeforeClose, this));
  CloseCommand::addListener([this] { onClose(); });
}

void Application::initModules()
{
  for (auto* module : m_modules)
    module->init();
}

void Application::initWindow()
{
	m_window = new GlfwWindow();
	m_window->init();

	/// \todo Show console -> ShowWindow() function is from Win32 API.
	// showConsole(Config::SHOW_CONSOLE);

	/// \todo GLFW does not support timer directly, timer callback behaviour must be implemented
	/// by yourself.
	// glutTimerFunc(Config::REFRESHTIME, onTimer, 0);
}

void Application::run()
{
	while (!m_bShouldClose)
	{
		// Process commands.
		for (auto& command : m_commands)
		{
			command->execute();
			delete command;
		}
		m_commands.clear();

		// Update and display.
		onDisplay();

		// Poll events.
		glfwWaitEvents();
	}
}

void Application::onDisplay()
{
	/*
	 \todo Pause scene update.
	if (m_isPaused)
	  return; // True to stop display update. Set by main.setPause(), not used now.
	 */

	/// \todo Window title change on scene or workspace change.
	std::stringstream ss;
	// ss << Config::WIN_TITLE;
	// PF XX for video capture
	ss << Config::WIN_TITLE;

	// glfwSetWindowTitle(m_window->get(), ss.str().c_str());
	m_window->setTitle(ss.str().c_str());

	logicUpdate();
	/// \todo after pressing quit, it still updates the logic and fails on nonexistent camera in Scene::keyUpdate()
	/// \todo move the logic Update to the timer
	// TIME_STEP_ACU -= TIME_STEP;

  for (auto* module : m_modules)
    module->beginFrame();

  for (auto* module : m_modules)
    module->endFrame();

	// glfwSwapBuffers(m_window);
	m_window->swapBuffers();
}

void Application::logicUpdate()
{
	InputController::preUpdate(); ///< update the mouse button state

	if (m_world)
		m_world->update(); ///< update World = handle 2D and 3D interaction
	// PF glutExit called during this update - crash after window close. Solved in TabSpace::closeMessageExitEvent

	InputController::update(); ///< Update mouseDelta, mousePrev, and the stored statuses of the keys in the \a keyMap
	                           ///< array (JUST_UP -> UP, JUST_DOWN -> DOWN).
}

void Application::finalize()
{
	delete ObjectDefinitionTree::tree;

	delete m_world;
	// world = nullptr; //PF problem during glutExit...

	/// \todo Write recent files.
	// RecentFiles::writeRecent();

	ShaderProvider::dispose(); // delete red, base and alpha shaders

	Shaper::dispose(); // delete shaper shaders (local shaders in shaper.cpp)

	TextureLoader::endTextures();
	TextureLoader::endHCTextures();

	GeometryMap::dispose();
	GeometryMap::disposeHC();

	glfwTerminate();
}

int Application::initI3T()
{
	int err;

	///   - load hard coded textures to TextureLoader
	if (!TextureLoader::loadHCTexture("defAxisTex", Config::getAbsolutePath("/Data/textures/axis.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("cGrid", Config::getAbsolutePath("/Data/textures/cGrid.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("abCube", Config::getAbsolutePath("/Data/textures/cube.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("cube_color", Config::getAbsolutePath("/Data/textures/cube_color.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("dcgiLogo", Config::getAbsolutePath("/Data/textures/dcgi-logo-60.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("icons", Config::getAbsolutePath("/Data/textures/icons.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("white", Config::getAbsolutePath("/Data/textures/white.png")))
		return 1;

	/// \todo the font texture name should be defined as config parameter. Now hard coded also in TabSpace::init
	if (!TextureLoader::loadHCTexture("tahoma_16_0.png",
	                                  Config::getAbsolutePath("/Data/internal/font/tahoma_16_0.png")))
		return 1;

	///   - init Shaper, probably the line debugger
	err = Shaper::initShaders();
	if (err)
		return err;

	///    - Init shaders in ShaderProvider <br> Note: Other shaders are in: shaper.cpp(162), GUI\glyphShader.cpp(26),
	///    and GUI\tabShader.cpp(24)
	err = ShaderProvider::init(Config::getAbsolutePath("/Data/internal/shaders/red.vert").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/red.frag").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/base.vert").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/base.frag").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/alpha.vert").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/alpha.frag").c_str());
	if (err)
		return err;

	///    - load geometry to GeometryMap
	if (GeometryMap::addHCGeometry("testCube", Config::getAbsolutePath("/Data/models/testCube.tmsh")))
		return 1;
	if (GeometryMap::addHCGeometry("orbit", Config::getAbsolutePath("/Data/models/orbit.tmsh")))
		return 1;
	if (GeometryMap::addHCGeometry("abCube", Config::getAbsolutePath("/Data/models/cube.tmsh")))
		return 1;
	if (GeometryMap::addHCGeometry("frustrum", Config::getAbsolutePath("/Data/models/testCube.tmsh"), 2.0f))
		return 1;
	if (GeometryMap::addHCGeometry("camera", Config::getAbsolutePath("/Data/models/camModel.tmsh"), 1.5f))
		return 1;
	if (GeometryMap::addHCGeometry("defAxis", Config::getAbsolutePath("/Data/models/axis.tmsh"), 1.5f))
		return 1;

	///     - load materials to MaterialMap
	MaterialMap::addHCMaterial(new Material("testMaterial", TextureLoader::getHCId("cGrid")));
	MaterialMap::addHCMaterial(new Material("abCube", TextureLoader::getHCId("abCube")));
	MaterialMap::addHCMaterial(new Material("cube_color", TextureLoader::getHCId("cube_color")));
	// MaterialMap::addHCMaterial(new Material("camera", -1, Color::GRAY, Color::WHITE, 1.0f, 1.0f, 1.0f, 1.0f));
	MaterialMap::addHCMaterial(
			new Material("camera", TextureLoader::getHCId("white"), Color::GRAY, Color::WHITE, 1.0f, 1.0f, 1.0f, 1.0f));
	MaterialMap::addHCMaterial(
			new Material("frustrum", TextureLoader::getHCId("cGrid"), Color::YELLOW, Color::WHITE, 1.0f, 1.0f, 1.0f, 1.0f));
	MaterialMap::addHCMaterial(new Material("defAxisMat", TextureLoader::getHCId("defAxisTex"), Color::WHITE,
	                                        Color::WHITE, 1.0f, 1.0f, 1.0f, 1.0f));

	GeometryMap::connectAllHCToShader(); /// \todo Connection of shader should take into account the materials too...
	/// \bug problem - dle dat se neda urcit shader - byl spatne shader pro kameru s texturou -1 a je spatne shader pro
	/// frustrum, ktery ma byt alphaShader a je baseShader

	///   - objects definitions data structure ObjectDefinitionTree
	ObjectDefinitionTree::tree = new ObjectDefinitionTree(nullptr);

	// read content files -- see 6 lines below
	// if (Reader::readContentCFG(Config::getAbsolutePath(Config::CONTENT_FILE.c_str()).c_str())) return 1;

	///   - create the \ref world World
	m_world = new World();

	// connect users geometries
	// GeometryMap::connectAllToShader();

	// world->onReshape(Config::WIN_WIDTH, Config::WIN_HEIGHT);

	///   - setup camera control
	m_world->setConfigCameraControl();

	return 0;
}

Application& Application::get()
{
	return s_instance;
}

UI* Application::getUI()
{
	return m_ui;
}

World* Application::world()
{
	return m_world;
}

ImFont* Application::getFont(int fontId)
{
	return m_ui->m_currentTheme.get(static_cast<size_t>(fontId));
}

GLFWwindow* Application::mainWindow()
{
	return m_window->get();
}

void Application::onBeforeClose()
{
	m_ui->showUniqueWindow<BeforeCloseModal>();
}

void Application::onClose()
{
	m_bShouldClose = true;
}

void Application::enqueueCommand(ICommand* command)
{
	m_commands.push_back(command);
}

Application Application::s_instance;
