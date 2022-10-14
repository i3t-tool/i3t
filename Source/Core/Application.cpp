#include "Application.h"

#include <functional>

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Module.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
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

using namespace Core;

double lastFrameSeconds = 0.0; // PF changed to double

static Configuration* g_Config = nullptr;

void Application::init()
{
	if (s_instance == nullptr)
		s_instance = this;

	m_isPaused = false;

	m_modules.push_back(new UIModule());

	m_scriptInterpreter = new Scripting();

	Core::GraphManager::init();

	//

	BeforeCloseCommand::addListener(std::bind(&App::onBeforeClose, this));
	CloseCommand::addListener([this] { onClose(); });
	ConsoleCommand::addListener([this](std::string c)
	                            { m_scriptInterpreter->runCommand(c); });

	InputManager::init();

	InputManager::bindGlobalAction("undo", EKeyState::Pressed,
	                               [&]()
	                               {
		                               Log::info("undo triggered");
		                               StateManager::instance().undo();
	                               });
	InputManager::bindGlobalAction("redo", EKeyState::Pressed,
	                               [&]()
	                               {
		                               Log::info("redo triggered");
		                               StateManager::instance().redo();
	                               });

	StateManager::instance().setOriginator(this);
}

void Application::initModules()
{
	for (auto* m : m_modules)
	{
		m->init();
	}

	StateManager::instance().createEmptyScene();
}

//===----------------------------------------------------------------------===//

static const std::string DIE_SEND_MAIL =
    "If it does not help, send me an email to felkepet@fel.cvut.cz with the "
    "snapshot of the program messages "
    "as they appear in the program console.\n";

static const std::string DIE_TEXT_OPENGL_VERSION =
    "The I3T tool did not start. \n\n"
    "The program is targeted to a Graphics card with OpenGL 3.1. or higher. If "
    "you have a system with two graphics "
    "cards, internal and external, "
    " switch to the external GPU card please. You may need to add "
    "transform.exe in the GPU control panel/3D settings"
    " to the list of programs using the dedicated GPU card.\n"
    " \n" +
    DIE_SEND_MAIL;

void Application::initWindow()
{
	m_window = new Window();
	m_window->init();

	// PGR and OpenGL initializing.
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR, pgr::DEBUG_OFF))
	{
		SystemDialogs::FireErrorMessageDialog("I3T", DIE_TEXT_OPENGL_VERSION);
	}
}

GLFWwindow* Application::mainWindow() { return m_window->get(); }

const std::string& Application::getTitle() { return m_window->getTitle(); }

void Application::setTitle(const std::string& title)
{
	m_window->setTitle(title.c_str());
}

//===----------------------------------------------------------------------===//

void Application::run()
{
	while (!m_bShouldClose)
	{
		// glfwWaitEvents();
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
	  return; // True to stop display update. Set by main.setPause(), not used
	now.
	 */

	/// \todo after pressing quit, it still updates the logic and fails on
	/// nonexistent camera in Scene::keyUpdate() \todo move the logic Update to
	/// the timer
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

	InputManager::update(); ///< Update mouseDelta, mousePrev, and the stored
	                        ///< statuses of the keys in the \a keyMap array
	                        ///< (JUST_UP -> UP, JUST_DOWN -> DOWN).
}

void Application::finalize()
{
	for (auto& module : m_modules)
		module->onClose();

	World::end();
	delete m_world;

	m_window->finalize();
	StateManager::instance().finalize();

	/*
	delete s_instance;
	s_instance == nullptr;
	 */
}

bool Application::initI3T()
{
	// getchar();printf("a\n");
	// loadConfig();
	const auto conf = loadConfig(Config::getAbsolutePath("Config.json"));
	ResourceManager::instance().init(conf->Resources);

	// new scene scheme
	bool b = World::init(); // getchar(); printf("b\n");
	m_world = new World();
	m_world->onStart();
	// getchar(); printf("c\n");
	return b;
}

//===-- State functions ---------------------------------------------------===//

void Application::onStateChange()
{
	auto title = g_baseTitle;

	if (StateManager::instance().hasScene())
	{
		title += ": " + StateManager::instance().scenePath().filename().string();
	}
	else
	{
		title += ": New Scene";
	}

	if (StateManager::instance().isDirty())
	{
		title += "*";
	}

	setTitle(title);
}

//===----------------------------------------------------------------------===//

Application& Application::get() { return *s_instance; }

UIModule* Application::getUI()
{
	I3T_ASSERT(!m_modules.empty() && dynamic_cast<UIModule*>(m_modules[0]) &&
	           "Did you initialize the Application properly?");

	/// \todo Unsafe!
	return (UIModule*)m_modules[0];
}

World* Application::world() { return m_world; }

void Application::onBeforeClose()
{
	getUI()->showUniqueWindow<BeforeCloseModal>();
}

void Application::onClose() { m_bShouldClose = true; }

void Application::enqueueCommand(ICommand* command)
{
	m_commands.push_back(command);
}

// Statics
Application* Application::s_instance = nullptr;
