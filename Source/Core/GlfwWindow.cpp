#include "GlfwWindow.h"

#include <pgr.h>

#include "Commands/ApplicationCommands.h"
#include "Logger/LoggerInternal.h"

void glfwErrorCallback(int error, const char* description)
{
	pgr::dieWithError(description);
}

void GlfwWindow::init()
{
	// GLFW initialization. -----------------------------------------------------
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

	m_mainWindow =
			glfwCreateWindow(Config::WIN_WIDTH, Config::WIN_HEIGHT, Config::WIN_TITLE.c_str(), NULL, NULL); // c++ exception
	if (m_mainWindow == nullptr)
	{
		LOG_FATAL("GLFW could not create main window!");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_mainWindow);
	glfwSwapInterval(1); // Enable vsync.

	glfwSetWindowCloseCallback(m_mainWindow, [](GLFWwindow* window) { BeforeCloseCommand::dispatch(); });

	/*
	glfwSetKeyCallback(m_mainWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
  {

  });
	 */
}

GlfwWindow::~GlfwWindow()
{
	glfwTerminate();
}

GLFWwindow* GlfwWindow::get()
{
	return m_mainWindow;
}

void GlfwWindow::setTitle(const char* title)
{
	glfwSetWindowTitle(m_mainWindow, title);
}

void GlfwWindow::swapBuffers()
{
	glfwSwapBuffers(m_mainWindow);
}
