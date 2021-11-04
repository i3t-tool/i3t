#include "Application.h"

#include <functional>

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Module.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Modals/BeforeCloseModal.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Loader/ConfigLoader.h"
#include "Logger/Logger.h"
#include "Scripting/Scripting.h"
#include "State/DumpVisitor.h"
#include "State/Manager.h"
#include "Utils/Color.h"
#include "Utils/TextureLoader.h"
#include "World/World.h"

double lastFrameSeconds = 0.0;  //PF changed to double

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
	ConsoleCommand::addListener([this](std::string c) { m_scriptInterpreter->runCommand(c); });

  InputManager::init();
	StateManager::instance().setOriginator(m_ui);

	InputManager::bindGlobalAction("save", EKeyState::Pressed, [&](){
		onSave();
	});
	InputManager::bindGlobalAction("undo", EKeyState::Pressed, [&](){
		Log::info("undo triggered");
		StateManager::instance().undo();
	});
	InputManager::bindGlobalAction("redo", EKeyState::Pressed, [&](){
		Log::info("redo triggered");
		StateManager::instance().redo();
	});
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

	/// \todo after pressing quit, it still updates the logic and fails on nonexistent camera in Scene::keyUpdate()
	/// \todo move the logic Update to the timer
	// TIME_STEP_ACU -= TIME_STEP;

	for (auto* m : m_modules)
		m->beginFrame();

	for (auto* m : m_modules)
		m->endFrame();

	// Input update must be called after rendering.
	logicUpdate();

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
	//getchar();printf("a\n");
	loadConfig();

	// new scene scheme
	bool b = World::init();//getchar(); printf("b\n");
	m_world = new World();
	m_world->onStart();
	//getchar(); printf("c\n");
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

World* Application::world()
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

void Application::onSave()
{
	Log::info("Save state!");
	auto workspace = getUI()->getWindowPtr<WorkspaceWindow>();
	auto& nodes = workspace->m_workspaceCoreNodes;

	DumpVisitor visitor;
	std::string rawState = visitor.dump(nodes);

	Log::info(rawState);
}

void Application::enqueueCommand(ICommand* command)
{
	m_commands.push_back(command);
}

// Statics
Application Application::s_instance;
