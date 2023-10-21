/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Application.h"

#include <functional>

#include "GUI/ImGui/imgui_impl_glfw.h"
#include "GUI/ImGui/imgui_impl_opengl3.h"

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"

using namespace Core;

constexpr const char* ImGui_GLSLVersion = "#version 140";

// Statics
Application* Application::s_instance = nullptr;

Application::Application()
{
	s_instance = this;
}

Application::~Application()
{
	if (!m_shouldClose)
	{
		close();
	}

	m_window->finalize();
}

Application& Application::get()
{
	return *s_instance;
}

void Application::init()
{
	initWindow();

	// Setup Dear ImGui context after OpenGL context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void) io;

	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
	                                                    // Platform Windows
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Allocate path to the imgui ini file on heap.
	io.IniFilename = "Data/imgui.ini";

	// Call implementation of init() in derived class
	onInit();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window->get(), true);
	ImGui_ImplOpenGL3_Init(ImGui_GLSLVersion);
}

void Application::run()
{
	// TOOD: (DR) m_modules: I'm thinking it might be a good idea to use a data structure that would retain the module
	// insertion order. That way there would be a consistent order in which modules are iterated/updated in. I suppose
	// individual operations of module calls should be independent of order but having such consistency in such a major
	// "loop" of the application might save us some issues in case another C++ implementation decides to iterate in a
	// different order.

	while (!m_shouldClose)
	{
		glfwPollEvents();

		// Process commands.
		for (auto& command : m_commands)
		{
			command->execute();
		}
		m_commands.clear();

		beginFrame();

		// Logic update
		update();

		// Render stuff
		display();

		endFrame();
	}
}

void Application::beginFrame()
{
	// Update input (before logic update and rendering)
	InputManager::beginFrame();

	// Start the Dear ImGui frame ----------------------
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	onBeginFrame();
}

void Application::endFrame()
{
	// Must be called *after* rendering *before* ending the frame.
	InputManager::endFrame();

	onEndFrame();

	// ImGui rendering ----------------------------------------------------------
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we
	// save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call
	//  glfwMakeContextCurrent(window) directly)
	// ImGuiIO& io = ImGui::GetIO(); (void)io;
	auto& io = ImGui::GetIO();
	(void) io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	m_window->swapBuffers();
}

void Application::update()
{
	// Get delta
	double current = glfwGetTime();
	double delta = current - m_lastFrameSeconds;
	m_lastFrameSeconds = current;

	for (auto& [_, m] : m_modules)
	{
		m->onUpdate(delta);
	}

	onUpdate(delta);
}

void Application::display()
{
	for (auto& [_, m] : m_modules)
		m->onBeginFrame();

	for (auto& [_, m] : m_modules)
		m->onEndFrame();
}

void Application::close()
{
	m_shouldClose = true;

	for (auto& [_, m] : m_modules)
	{
		m->onClose();
	}

	onClose();
}

void Application::enqueueCommand(ICommand* command)
{
	m_commands.push_back(command);
}

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

GLFWwindow* Application::getWindow()
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
