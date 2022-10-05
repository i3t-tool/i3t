/**
 * \file Core/GlfwWindow
 * \author Martin Herich
 * \date 30.10.2020
 *
 * Wrapper for GLFW window.
 */
#pragma once

#include <string>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class Window
{
public:
	~Window() = default;

	/**
	 * Initialize GLFW window.
	 *
	 * Exit the program if a window cannot be created.
	 */
	void init();

	const std::string& getTitle();
	void setTitle(const char* title);

	void swapBuffers();
	GLFWwindow* get();

private:
	friend class Application;

	void finalize();

	std::string m_title;
	GLFWwindow* m_mainWindow;
};
