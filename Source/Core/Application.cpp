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
#include "Scripting/Scripting.h"
#include "Utils/Color.h"
#include "Utils/TextureLoader.h"
#include "World2/World2.h"

#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include "GUI/Elements/Nodes/WorkspaceMatrixScale.h"
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
	testCycle->setTo(10.0f);
	testCycle->setMultiplier(0.10f);
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

		double current = glfwGetTime();
		double delta = current - lastFrameSeconds;
		Core::GraphManager::update(delta);
    lastFrameSeconds = current;

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
	delete m_world;

	/*
	ShaderProvider::dispose(); // delete red, base and alpha shaders

	Shaper::dispose(); // delete shaper shaders (local shaders in shaper.cpp)

	TextureLoader::endTextures();
	TextureLoader::endHCTextures();
	 */

	glfwTerminate();
}

bool Application::initI3T()
{
	// new scene scheme
	bool b = World2::initRender();
	m_world = World2::loadDefaultScene();

	// \todo DG testing
	WorkspaceMatrix4x4* mat =new WorkspaceMatrixScale((ImTextureID)0, "load free");
	glm::mat4 m=glm::mat4(1.0f);
	ValueSetResult result = mat->m_nodebase->setValue(glm::vec3(2.0f,2.0f,2.0f));
	printf("value set result %d\n",result.status);
	// ValueSetResult result = dynamic_cast<WorkspaceNodeWithCoreData*>(_workspace->back().get())->Nodebase.get()->setValue(node.data);
	m_world->handlesSetMatrix(mat);

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

World2* Application::world2()
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
