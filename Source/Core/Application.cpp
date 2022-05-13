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
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"
#include "Utils/Color.h"
#include "Utils/TextureLoader.h"
#include "World/World.h"

double lastFrameSeconds = 0.0;  //PF changed to double

Application::Application()
{
	if (s_instance == nullptr)
		s_instance = this;

	m_isPaused = false;

	m_modules.push_back(new UIModule());

	m_scriptInterpreter = new Scripting();

	Core::GraphManager::init();
}

Application::~Application()
{
	for (auto* module : m_modules)
	{
		delete module;
	}
}

void Application::init()
{
	BeforeCloseCommand::addListener(std::bind(&App::onBeforeClose, this));
	CloseCommand::addListener([this] { onClose(); });
	ConsoleCommand::addListener([this](std::string c) { m_scriptInterpreter->runCommand(c); });

  InputManager::init();
	StateManager::instance().setOriginator(this);

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

	if (!StateManager::instance().hasScene())
		StateManager::instance().createEmptyScene();
}

//===----------------------------------------------------------------------===//

void Application::initWindow()
{
	m_window = new Window();
	m_window->init();
}

GLFWwindow* Application::mainWindow()
{
	return m_window->get();
}

const std::string& Application::getTitle()
{
	return m_window->getTitle();
}

void Application::setTitle(const std::string& title)
{
	m_window->setTitle(title.c_str());
}

//===----------------------------------------------------------------------===//

void Application::run()
{
	while (!m_bShouldClose)
	{
    //glfwWaitEvents();
    glfwPollEvents();

    // Process commands.
		for (auto& command : m_commands)
		{
			command->execute();
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
	for (auto& module : m_modules)
		module->onClose();

	World::end();
	delete m_world;
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

//===-- State functions ---------------------------------------------------===//

Memento Application::getState()
{
	auto& nodes = getUI()->getWindowPtr<WorkspaceWindow>()->getNodeEditor().m_workspaceCoreNodes;

	SerializationVisitor visitor;
	std::string rawState = visitor.dump(nodes);

	return Memento({ rawState });
}

void Application::setState(const Memento& memento)
{
	auto& nodes = getUI()->getWindowPtr<WorkspaceWindow>()->getNodeEditor().m_workspaceCoreNodes;
	nodes.clear();

	auto& rawScene = memento.getSnapshot().front();

	buildScene(rawScene, nodes);
}

void Application::onStateChange(const std::string& winTitlePostfix)
{
	std::string newTitle;
	if (StateManager::instance().hasScene())
		newTitle = std::string(g_baseTitle) + " - " + StateManager::instance().scenePath().string() + winTitlePostfix;
	else
		newTitle = std::string(g_baseTitle) + winTitlePostfix;

	m_window->setTitle(newTitle.c_str());
}

//===----------------------------------------------------------------------===//

Application& Application::get()
{
	return *s_instance;
}

UIModule* Application::getUI()
{
	return (UIModule*) m_modules[0];
}

World* Application::world()
{
	return m_world;
}

void Application::onBeforeClose()
{
	getUI()->showUniqueWindow<BeforeCloseModal>();
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
Application* Application::s_instance = nullptr;
