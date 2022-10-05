#include "Window.h"

#include "stb_image.h"
#include <pgr.h>

#include "Commands/ApplicationCommands.h"
#include "Logger/LoggerInternal.h"

void glfwErrorCallback(int error, const char* description)
{
	pgr::dieWithError(description);
}

void Window::init()
{
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit())
	{
		LOG_FATAL("Can't initialize GLFW.");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

	m_mainWindow = glfwCreateWindow(Config::WIN_WIDTH, Config::WIN_HEIGHT,
	                                g_baseTitle.c_str(), nullptr, nullptr);

	if (m_mainWindow == nullptr)
	{
		LOG_FATAL("GLFW could not create main window!");
		exit(EXIT_FAILURE);
	}

	setTitle(g_baseTitle.c_str());

	int x, y, channels;
	constexpr int desiredChannels = 4;
	auto* pixels =
	    stbi_load(Config::getAbsolutePath("Data/textures/logoi3t.png").c_str(),
	              &x, &y, &channels, desiredChannels);
	if (pixels)
	{
		GLFWimage image{x, y, pixels};
		glfwSetWindowIcon(m_mainWindow, 1, &image);
	}
	stbi_image_free(pixels);

	glfwMakeContextCurrent(m_mainWindow);
	glfwSwapInterval(1); // Enable vsync.

	glfwSetWindowCloseCallback(m_mainWindow, [](GLFWwindow* window)
	                           { BeforeCloseCommand::dispatch(); });
}

GLFWwindow* Window::get() { return m_mainWindow; }

const std::string& Window::getTitle() { return m_title; }

void Window::setTitle(const char* title)
{
	m_title = title;
	glfwSetWindowTitle(m_mainWindow, title);
}

void Window::swapBuffers() { glfwSwapBuffers(m_mainWindow); }

void Window::finalize() { glfwTerminate(); }
