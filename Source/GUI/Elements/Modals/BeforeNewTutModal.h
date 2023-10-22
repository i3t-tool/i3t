/**
 * \file
 * \brief
 * \author Adam Loucký
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "GUI/Elements/IWindow.h"
#include "Tutorial/Tutorial.h"

class BeforeNewTutModal : public ModalWindow
{
public:
	BeforeNewTutModal() : ModalWindow("Opening a new tutorial") {}

	void onImGui() override;

private:
	void submit();
};