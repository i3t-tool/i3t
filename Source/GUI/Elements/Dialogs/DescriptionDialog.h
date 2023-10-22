/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 29.09.2020
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

/**
 * Scene description dialog.
 * \todo Create scene description here.
 * \todo Make text resizable.
 * \todo Make open scene unsaved after text editing.
 */
class DescriptionDialog : public IWindow
{
public:
	I3T_WINDOW(DescriptionDialog)

	DescriptionDialog();
	virtual void render() override;

private:
	bool m_isEditState;

	/// Description text has fixed length.
	char m_descriptionText[8096] = "";
};
