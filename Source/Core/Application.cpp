#include "Application.h"

#include <functional>

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
#include "GUI/UIModule.h" /// \todo Remove this dependency
#include "Logger/Logger.h"
#include "State/StateManager.h"
#include "Viewport/Viewport.h"

using namespace Core;

double lastFrameSeconds = 0.0; // PF changed to double

static Configuration* g_Config = nullptr;

Application::Application()
{
	s_instance = this;
}

Application::~Application()
{
	for (auto& [_, m] : m_modules)
	{
		m->onClose();
	}

	delete m_viewport; // TODO: (DR) Maybe turn into a smart pointer

	m_window->finalize();
}

void Application::init()
{
	initWindow();

	Core::GraphManager::init();

	//

	CloseCommand::addListener([this] {
		onClose();
	});

	InputManager::init();

	InputManager::bindGlobalAction("undo", EKeyState::Pressed, [&]() {
		LOG_INFO("undo triggered");
		App::getModule<StateManager>().undo();
	});
	InputManager::bindGlobalAction("redo", EKeyState::Pressed, [&]() {
		LOG_INFO("redo triggered");
		App::getModule<StateManager>().redo();
	});

	// Former initI3T member function
	const auto conf = loadConfig("Config.json");
	createModule<Core::ResourceManager>();
	App::getModule<ResourceManager>().createDefaultResources(conf->Resources);

	m_viewport = new Vp::Viewport();
	m_viewport->init(Vp::ViewportSettings());

	// Call implementation of init() in derived class
	onInit();
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

		InputManager::beginFrame(); // Must be called *before* rendering.

		Logger::getInstance().update(); // (DR) I'm assuming this should update after input manager since it uses it?

		// Update and display.
		onDisplay();
		logicUpdate(delta);

		InputManager::endFrame(); // Must be called *after* rendering.

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

void Application::logicUpdate(double delta)
{
	viewport()->update(delta);
}

Application& Application::get()
{
	return *s_instance;
}

/// \todo Remove this dependency
UIModule* Application::getUI()
{
	return &getModule<UIModule>();
}

Vp::Viewport* Application::viewport()
{
	return m_viewport;
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
