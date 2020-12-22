#include "GlfwWindow.h"

#include <pgr.h>

#include "Commands/ApplicationCommands.h"
#include "Logger/Logger.h"

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
  /// \todo GLFW antialiasing. Antialiasing in scene window.
  // glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing custom thingy

  m_mainWindow = glfwCreateWindow(Config::WIN_WIDTH, Config::WIN_HEIGHT, Config::WIN_TITLE.c_str(), NULL, NULL);
  if (m_mainWindow == nullptr)
  {
    LOG_FATAL("GLFW could not create main window!");
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(m_mainWindow);
  glfwSwapInterval(1); // Enable vsync.

  glfwSetWindowCloseCallback(m_mainWindow, [](GLFWwindow* window) { BeforeCloseCommand::dispatch(); });
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
