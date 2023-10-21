/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Petr Felkel
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <memory>

#include "GUI/Elements/IWindow.h"

class TextureRegion;

class AboutWindow : public IWindow
{
public:
	I3T_WINDOW(AboutWindow)
	std::shared_ptr<TextureRegion> dcgiReg;

	void render() override;
};
