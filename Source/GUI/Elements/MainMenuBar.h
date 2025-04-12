/**
 * \file
 * \brief ImGui GUI Window abstract class
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Core/Result.h"

namespace MenuBarDialogs
{
Result<Void, Error> open();
Result<Void, Error> append();
}

/**
 * Top menu bar.
 * The bar also contains dialogs and other windows associated with menu items.
 */
class MainMenuBar
{
public:
	MainMenuBar();
	virtual ~MainMenuBar() = default;

	void render();

private:
	bool m_showDemoWindow = false;

	void showFileMenu();
	void showEditMenu();
	void showWindowsMenu();
	void showTutorialMenu();
	void showHelpMenu();
};
