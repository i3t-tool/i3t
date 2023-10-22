/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \date 14.09.2023
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <functional>

#include "GUI/Elements/IWindow.h"

class RemoveModelModal : public ModalWindow
{
private:
	std::string m_modelAlias;
	int m_usedCount;

	std::function<void()> m_onRemove;

public:
	RemoveModelModal(std::string modelAlias, int usedCount, std::function<void()> onRemove);

	void onImGui() override;
};
