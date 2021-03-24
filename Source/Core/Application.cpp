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
#include "GUI/UIModule.h"
#include "Logger/Logger.h"
#include "Rendering/ShaderProvider.h"
#include "Rendering/Shaper.h"
#include "Scripting/Scripting.h"
#include "Utils/Color.h"
#include "Utils/TextureLoader.h"
#include "World2/World2.h"


#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixScale.h"

Application::Application()
{
	m_isPaused = false;

	m_ui = new UIModule();
	m_modules.push_back(m_ui);

	m_scriptInterpreter = new Scripting();
}

void Application::init()
{
	BeforeCloseCommand::addListener(std::bind(&App::onBeforeClose, this));
	CloseCommand::addListener([this] { onClose(); });
	ConsoleCommand::addListener(
			[this](std::string rawCommand) { Log::info("Application got command '{}'.", rawCommand); });
	ConsoleCommand::addListener([this](std::string c) { m_scriptInterpreter->runCommand(c); });
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
	InputManager::preUpdate(); ///< update the mouse button state

	InputManager::update(); ///< Update mouseDelta, mousePrev, and the stored statuses of the keys in the \a keyMap
	                        ///< array (JUST_UP -> UP, JUST_DOWN -> DOWN).
}

void Application::finalize()
{

	delete m_world2;
	// world = nullptr; //PF problem during glutExit...

	/// \todo Write recent files.
	// RecentFiles::writeRecent();

	ShaderProvider::dispose(); // delete red, base and alpha shaders

	Shaper::dispose(); // delete shaper shaders (local shaders in shaper.cpp)

	TextureLoader::endTextures();
	TextureLoader::endHCTextures();

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

	// read content files -- see 6 lines below
	// if (Reader::readContentCFG(Config::getAbsolutePath(Config::CONTENT_FILE.c_str()).c_str())) return 1;

	// new scene scheme
	bool b = World2::initRender();
	m_world2 = World2::loadDefaultScene();

	//testing
	WorkspaceMatrix4x4* mat =new WorkspaceMatrixScale((ImTextureID)0, "load free");
	glm::mat4 m=glm::mat4(1.0f);
	ValueSetResult result = mat->m_nodebase->setValue(glm::vec3(2.0f,2.0f,2.0f));
	printf("value set result %d\n",result.status);
	//ValueSetResult result = dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->back().get())->Nodebase.get()->setValue(node.data);
	m_world2->handlesSetMatrix(mat);

	return 0;
}

Application& Application::get()
{
	return s_instance;
}

UIModule* Application::getUI()
{
	return m_ui;
}

World2* Application::world2()
{
	return m_world2;
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
