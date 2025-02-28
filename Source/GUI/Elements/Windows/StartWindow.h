/**
 * \file
 * \brief
 * \authors Miroslav Müller, Adam Loucký
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "TutorialWindow.h"

#include <string>

#include "GUI/Elements/IWindow.h"
#include "Tutorial/Tutorial.h"

class StartWindow : public IWindow
{
public:
	I3T_WINDOW(StartWindow)
	explicit StartWindow(bool show);

	void renderLeftPanel() const;
	void renderRightPanel();

	/// \brief Renders the Start window using ImGui.
	void render() override;

private:
	std::shared_ptr<GUIImage> m_dummyImage;
	std::shared_ptr<GUIImage> m_folderImage;
	std::shared_ptr<GUIImage> m_logoImage;
	std::shared_ptr<GUIImage> m_cvutImage;
	std::shared_ptr<GUIImage> m_i3tImage;

	void loadTutorialAndShowWindow(const Ptr<TutorialHeader>& header, Ptr<Tutorial> tut);

	// for now leave it true, later it will be tested and decide what window to show according to persona
	bool m_finishedAllTutorials = true;
};
