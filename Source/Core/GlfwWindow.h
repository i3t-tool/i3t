/**
 * \file Core/GlfwWindow
 * \author Martin Herich
 * \date 30.10.2020
 *
 * Wrapper for GLFW window.
 */
#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class GlfwWindow
{
public:
	/**
	 * Initialize GLFW window.
	 *
	 * Exit the program if a window cannot be created.
	 */
	void init();
	void setTitle(const char* title);
	void swapBuffers();
	GLFWwindow* get();

private:
	GLFWwindow* m_mainWindow;
};
