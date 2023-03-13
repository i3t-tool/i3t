#include "Application.h"

#include <functional>

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Modals/BeforeCloseModal.h"
#include "GUI/Elements/Modals/BeforeNewModal.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Logger/Logger.h"
#include "Scripting/ScriptingModule.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"
#include "World/World.h"

using namespace Core;

double lastFrameSeconds = 0.0; // PF changed to double

static Configuration* g_Config = nullptr;

void Application::init()
{
	if (s_instance == nullptr)
		s_instance = this;

	m_isPaused = false;

	createModule<ResourceManager>();
	createModule<StateManager>();
	createModule<UIModule>();
	createModule<ScriptingModule>().init();

	Core::GraphManager::init();

	//

	BeforeNewProjectCommand::addListener([this]() { getUI()->getWindowManager().showUniqueWindow<BeforeNewModal>(); });
	NewProjectCommand::addListener([]() { App::getModule<StateManager>().newScene(); });

	BeforeCloseCommand::addListener(std::bind(&App::onBeforeClose, this));
	CloseCommand::addListener([this] { onClose(); });
	ConsoleCommand::addListener([this](std::string c) { getModule<ScriptingModule>().runScript(c.c_str()); });

	InputManager::init();

	InputManager::bindGlobalAction("undo", EKeyState::Pressed,
	                               [&]()
	                               {
		                               LOG_INFO("undo triggered");
		                               App::getModule<StateManager>().undo();
	                               });
	InputManager::bindGlobalAction("redo", EKeyState::Pressed,
	                               [&]()
	                               {
		                               LOG_INFO("redo triggered");
		                               App::getModule<StateManager>().redo();
	                               });
}

void Application::initModules()
{
	for (auto& [_, m] : m_modules)
	{
		m->init();
	}

	App::getModule<StateManager>().createEmptyScene();
}

//===----------------------------------------------------------------------===//

static const std::string DIE_SEND_MAIL = "If it does not help, send me an email to felkepet@fel.cvut.cz with the "
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
	pgr::DebugLevel debugLevel = pgr::DEBUG_OFF; // < Set debug level here!

	m_window = new Window();
	m_window->init(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR, debugLevel != pgr::DEBUG_OFF, true);

	// PGR and OpenGL initializing.
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR, debugLevel))
	{
		SystemDialogs::FireErrorMessageDialog("I3T", DIE_TEXT_OPENGL_VERSION);
	}

	// Extra opengl 4.3 severity level which pgr framework doesn't account for
	if (debugLevel != pgr::DEBUG_OFF)
	{
		GLuint severity = GL_DEBUG_SEVERITY_NOTIFICATION;
		GLboolean toggle = debugLevel <= 1 ? GL_TRUE : GL_FALSE;
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DONT_CARE, severity, 0, NULL, toggle);
		glDebugMessageControl(GL_DEBUG_SOURCE_WINDOW_SYSTEM, GL_DONT_CARE, severity, 0, NULL, toggle);
		glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, severity, 0, NULL, toggle);
		glDebugMessageControl(GL_DEBUG_SOURCE_THIRD_PARTY, GL_DONT_CARE, severity, 0, NULL, toggle);
		glDebugMessageControl(GL_DEBUG_SOURCE_APPLICATION, GL_DONT_CARE, severity, 0, NULL, toggle);
		glDebugMessageControl(GL_DEBUG_SOURCE_OTHER, GL_DONT_CARE, severity, 0, NULL, toggle);
	}
}

GLFWwindow* Application::mainWindow() { return m_window->get(); }

const std::string& Application::getTitle() { return m_window->getTitle(); }

void Application::setTitle(const std::string& title) { m_window->setTitle(title.c_str()); }

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

		Logger::getInstance().update();

		// Update and display.
		onDisplay();
		logicUpdate(delta);

		// Input update must be called after rendering. (Due to some values being zero-ed out too early)
		// Might be better to update inputs before rendering to reduce input lag and retain consistency with ImGui
		// That would require slight InputManager modifications and testing
		// TODO: (DR) Enabling window manager debug and setting glfwSwapInterval(10) shows the input lag I mentioned
		//  (Watch the yellow and purple cursors on the mouse. Purple InputManager position is lagging behind the yellow
		//  one,
		//	which is also kinda lagging, but thats probably normal)
		//  Not critical but might be worth fixing.
		InputManager::update();

		// glfwSwapBuffers(m_window);
		m_window->swapBuffers();
	}
}

void Application::onDisplay()
{
	// TODO: (DR) Figure out what this was about, probably just delete these old comments
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

	for (auto& [_, m] : m_modules)
		m->beginFrame();

	for (auto& [_, m] : m_modules)
		m->endFrame();
}

void Application::logicUpdate(double delta) { viewport()->update(delta); }

void Application::finalize()
{
	for (auto& [_, m] : m_modules)
		m->onClose();

	World::end();      // TODO: (DR) Remove
	delete m_world;    // TODO: (DR) Remove
	delete m_viewport; // TODO: (DR) Maybe turn into a smart pointer

	m_window->finalize();

	// TODO: Investigate why this was here. Was causing an assertion error on
	// exit. delete s_instance; s_instance == nullptr;
}

bool Application::initI3T()
{
	// getchar();printf("a\n");
	// loadConfig();
	const auto conf = loadConfig("Config.json");
	App::getModule<ResourceManager>().createDefaultResources(conf->Resources);

	// new scene scheme
	bool b = World::init(); // getchar(); printf("b\n");
	m_world = new World();
	m_world->onStart();
	// getchar(); printf("c\n");

	m_viewport = new Vp::Viewport();
	m_viewport->init(Vp::ViewportSettings());

	return b;
}

Application& Application::get() { return *s_instance; }

UIModule* Application::getUI() { return &getModule<UIModule>(); }

World* Application::world() { return m_world; }

Vp::Viewport* Application::viewport() { return m_viewport; }

void Application::onBeforeClose() { getUI()->getWindowManager().showUniqueWindow<BeforeCloseModal>(); }

void Application::onClose() { m_bShouldClose = true; }

void Application::enqueueCommand(ICommand* command) { m_commands.push_back(command); }

// Statics
Application* Application::s_instance = nullptr;
