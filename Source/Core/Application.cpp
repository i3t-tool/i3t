/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
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

#include "imgui.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Commands/ApplicationCommands.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/Dialogs/SystemDialogs.h"
#include "Utils/Statistics.h"
#include "Utils/Timers.h"

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

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	m_window->finalize();
}

Application& Application::get()
{
	return *s_instance;
}

bool Application::init()
{
	// Using en_US UTF8 locale, could also use the default C locale with utf8.
	// Not explicitly setting a locale uses the OS default which can introduce inconsistencies with ImGui etc.
	// Think it will be best to assume the default C/en_US locale and "." as a decimal separator.
	// On that note, ImGui has an option to override the platform decimal separator io.PlatformLocaleDecimalPoint
	//  but that does not override the components using printf-like format (which most do).
	std::setlocale(LC_ALL, "en_US.UTF8");
	LOG_INFO("Using locale: {}", std::string(setlocale(LC_ALL, NULL)));

	initWindow();

	// Setup Dear ImGui context after OpenGL context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
	                                                    // Platform Windows
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Allocate path to the imgui ini file on heap.
	io.IniFilename = "Data/I3T.ini";

	// Setup Platform/Renderer bindings
	if (!ImGui_ImplGlfw_InitForOpenGL(m_window->get(), true))
		return false;
	if (!ImGui_ImplOpenGL3_Init(ImGui_GLSLVersion))
		return false;

	// Call implementation of init() in derived class
	onInit();

	return true;
}

int Application::run()
{
	// TODO: (DR) m_modules: I'm thinking it might be a good idea to use a data structure that would retain the module
	// insertion order. That way there would be a consistent order in which modules are iterated/updated in. I suppose
	// individual operations of module calls should be independent of order but having such consistency in such a major
	// "loop" of the application might save us some issues in case another C++ implementation decides to iterate in a
	// different order.

	int exitCode = 0;

	CloseCommand::addListener([this, &exitCode](int result) {
		exitCode = result;
		close();
	});

	while (!m_shouldClose)
	{
		if (!frame())
			break;
	}
	return exitCode;
}

bool Application::frame()
{
	glfwPollEvents();

	CommandDispatcher::get().execute();
	if (m_shouldClose)
	{
		// Do not run the rest of the loop if the application is closing.
		return false;
	}
	Statistics::update();

	beginFrame();

	// Logic update
	update();

	// Render stuff
	display();

	endFrame();
	return true;
}

void Application::beginFrame()
{
	for (auto& m : m_modules)
		m->onBeforeFrame();

	// Start the Dear ImGui frame ----------------------
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Update input (before logic update and rendering)
	InputManager::beginFrame();

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

	for (auto& m : m_modules)
	{
		m->onUpdate(delta);
	}

	onUpdate(delta);
}

void Application::display()
{
	for (auto& m : m_modules)
		m->onBeginFrame();

	for (auto& m : m_modules)
		m->onEndFrame();
}

void Application::close()
{
	m_shouldClose = true;

	for (auto& m : m_modules)
	{
		m->onClose();
	}

	onClose();
}

void Application::enqueueCommand(ICommand* command)
{
	m_commands.push_back(command);
}

static const std::string DIE_SEND_MAIL = "If it does not help, send me an email to felkel@fel.cvut.cz with the "
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
}

GLFWwindow* Application::getWindow()
{
	return m_window->get();
}

GLFWwindow* Application::getCurrentWindow()
{
	ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
	ImGuiContext* g = ImGui::GetCurrentContext(); // Get current/last moused viewport.

	GLFWwindow* window = nullptr;
	for (int n = 0; n < platformIO.Viewports.Size; n++)
	{
		if (platformIO.Viewports[n]->ID == g->MouseViewport->ID)
		{
			window = (GLFWwindow*) platformIO.Viewports[n]->PlatformHandle;
		}
	}

	return window;
}

const std::string& Application::getTitle()
{
	return m_window->getTitle();
}

void Application::setTitle(const std::string& title)
{
	m_window->setTitle(title.c_str());
}
