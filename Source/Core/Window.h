/**
 * \file
 * \brief Wrapper for GLFW window.
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 30.10.2020
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <string>

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
	void init(const int oglVersionMajor, const int oglVersionMinor, bool oglDebug, bool oglForwardCompat);

	void show() const;

	const std::string& getTitle();
	void setTitle(const char* title);

	void swapBuffers();
	GLFWwindow* get();

	void finalize();

	void setVSync(bool enable);

	void setWindowPosAndSize(int posX, int posY, int sizeX, int sizeY, bool maximized);

	void getWindowPosAndSize(int& posX, int& posY, int& sizeX, int& sizeY, bool& maximized) const;

private:
	bool m_closeDisabled = false;
	std::string m_title;
	GLFWwindow* m_mainWindow;
};
