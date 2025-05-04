/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
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

namespace Vp
{
struct GridSettings;
}
// TODO: (DR) Rename to SetupWindow, or better PreferencesWindow
class PreferencesWindow : public IWindow
{
	float m_uiScaleTmp;

public:
	I3T_WINDOW(PreferencesWindow)

	PreferencesWindow();

	virtual void render() override;

	void showUISettings();
	void showViewportSettings();
	void showWorkspaceSettings();

	void showGridSettings(Vp::GridSettings& grid);

	void showHelpTip(const char* text);
};