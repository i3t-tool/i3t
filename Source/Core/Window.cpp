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
#include "Window.h"

#include "stb_image.h"
#include <pgr.h>

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"

void glfwErrorCallback(int error, const char* description)
{
	pgr::dieWithError(description);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		InputManager::keyDown(key);
	}
	if (action == GLFW_RELEASE)
	{
		InputManager::keyUp(key);
	}
}

void Window::init(const int oglVersionMajor, const int oglVersionMinor, bool oglDebug, bool oglForwardCompat)
{
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit())
	{
		LOG_FATAL("Can't initialize GLFW.");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, oglVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, oglVersionMinor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, oglForwardCompat ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, oglDebug ? GLFW_TRUE : GLFW_FALSE);

	m_mainWindow = glfwCreateWindow(Config::WIN_WIDTH, Config::WIN_HEIGHT, BASE_WINDOW_TITLE.c_str(), nullptr, nullptr);

	if (m_mainWindow == nullptr)
	{
		LOG_FATAL("GLFW could not create main window!");
		exit(EXIT_FAILURE);
	}

	// Note that the key callback has to be set BEFORE initializing ImGui GLFW backend as the callbacks get overwritten.
	// The ImGui backend upon init will notice this existing callback and call it itself.
	glfwSetKeyCallback(m_mainWindow, glfwKeyCallback);
	glfwSetWindowUserPointer(m_mainWindow, (void*) this);

	setTitle(BASE_WINDOW_TITLE.c_str());

	int x, y, channels;
	constexpr int desiredChannels = 4;
	auto* pixels = stbi_load("Data/Textures/logoi3t.png", &x, &y, &channels, desiredChannels);
	if (pixels)
	{
		GLFWimage image{x, y, pixels};
		glfwSetWindowIcon(m_mainWindow, 1, &image);
	}
	stbi_image_free(pixels);

	glfwMakeContextCurrent(m_mainWindow);
	glfwSwapInterval(1); // Enable vsync.

	glfwSetWindowCloseCallback(m_mainWindow, [](GLFWwindow* window) {
		BeforeCloseCommand::dispatch();
	});
}

GLFWwindow* Window::get()
{
	return m_mainWindow;
}

const std::string& Window::getTitle()
{
	return m_title;
}

void Window::setTitle(const char* title)
{
	m_title = title;
	glfwSetWindowTitle(m_mainWindow, title);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_mainWindow);
}

void Window::finalize()
{
	glfwTerminate();
}
