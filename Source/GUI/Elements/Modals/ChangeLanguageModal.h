/**
 * \file
 * \brief
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \date 04.11.2024
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#pragma once

#include "../IWindow.h"

class ChangeLanguageModal : public ModalWindow
{
public:
	I3T_WINDOW(ChangeLanguageWindow)

	ChangeLanguageModal();

	// void render() override;
	void onImGui() override;

	static void showChangeLanguageMenu();
};
