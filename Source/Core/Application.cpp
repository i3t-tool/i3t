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
#include "Loader/ConfigLoader.h"
#include "Logger/Logger.h"
#include "Scripting/Scripting.h"
#include "Utils/Color.h"
#include "Utils/TextureLoader.h"
#include "World/World.h"

#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixFree.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixScale.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixTranslation.h"
#include "Nodes/GraphManager.h"


double lastFrameSeconds = 0.0f;
Ptr<Core::Cycle> testCycle;

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

  /// \todo MH remove test code.
  testCycle = Core::GraphManager::createCycle();
	testCycle->setFrom(1.25f);
	testCycle->setTo(-1.25f);
	testCycle->setMultiplier(-0.10f);
  testCycle->setMode(Core::Cycle::EMode::Once);


  InputManager::init();
}

void Application::initModules()
{
	for (auto* m : m_modules)
		m->init();
}

void Application::initWindow()
{
	m_window = new GlfwWindow();
	m_window->init();
}

void Application::run()
{
	while (!m_bShouldClose)
	{
    glfwWaitEvents();

    // Process commands.
		for (auto& command : m_commands)
		{
			command->execute();
			delete command;
		}
		m_commands.clear();

		double current = glfwGetTime();
		double delta = current - lastFrameSeconds;
		Core::GraphManager::update(delta);
    lastFrameSeconds = current;

		// Update and display.
		onDisplay();
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

	for (auto* m : m_modules)
		m->beginFrame();

	for (auto* m : m_modules)
		m->endFrame();

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
	delete m_world;

	World::end();

	glfwTerminate();
}

bool Application::initI3T()
{
	loadConfig();

		// new scene scheme
		bool b = World::init();
		m_world = World::loadDefaultScene();
		m_world->tmpSetNode();//tmp - to be removed
		m_world->onUpdate();
		GLuint renderTexture;

		ImVec2 textureSize = ImVec2(
				5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * 20.0f,
				5* I3T::getSize(ESizeVec2::Nodes_ItemsSpacing).y + 6 * 20.0f
		);

		//rend = new RenderTexture(&renderTexture, textureSize.x, textureSize.y);
		//screen = new GameObject(unitcubeMesh, &World::shader0, World::cGridTexture);
		//screen->addComponent(new Renderer());
		//cam = new Camera(60.0f, screen, rend);

		glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 1.0f);

		RenderTexture* rend = new RenderTexture(&renderTexture, 256, 256);
		GameObject* screen = new GameObject(unitcubeMesh, &World::shader0, World::cGridTexture);

		screen->translate(glm::vec3(1000,100,-3));

		screen->addComponent(new Renderer());
		Camera* cam = new Camera(60.0f, screen, rend);
		cam->update();
		return b;

}

Application& Application::get()
{
	return s_instance;
}

UIModule* Application::getUI()
{
	return m_ui;
}

World* Application::world2()
{
	return m_world;
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

// Statics
Application Application::s_instance;
