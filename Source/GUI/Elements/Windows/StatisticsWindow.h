/**
 * \file
 * \brief Defines the StatisticsWindow class for rendering statistical information in the GUI.
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#pragma once

#include "GUI/Elements/IWindow.h"

/**
 * \class StatisticsWindow
 * \brief A class representing a GUI window for displaying statistics.
 *
 * This class inherits from the IWindow interface and provides an implementation
 * for rendering a statistics window in the I3T application.
 */
class StatisticsWindow : public IWindow
{
public:
	/**
	 * \brief Macro to define window-related behavior.
	 */
	I3T_WINDOW(StatisticsWindow);

	/**
	 * \brief Constructs a StatisticsWindow object.
	 */
	StatisticsWindow();

	/**
	 * \brief Renders the contents of the statistics window.
	 *
	 * This method overrides the render function in the IWindow interface.
	 */
	void render() override;

private:
	/**
	 * \brief Draws the specific content of the statistics window.
	 *
	 * This is a helper method used internally by the render function to display
	 * detailed statistical information.
	 */
	void drawContent();
};
