/**
 * \file	GUI/Elements/introWindow.h
 *
 * Intro window.
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
	StartWindow(bool show);

	/**
   * \brief searches through the tutorial directory and adds all found tutorial files to the window
   */
	void reloadTutorials();

	/**
   * \brief Renders the Start window using ImGui.
   */
	void render() override;

private:
	std::shared_ptr<GUIImage>                    m_dummyImage;
	std::shared_ptr<GUIImage>                    m_folderImage;
	std::shared_ptr<GUIImage>                    m_logoImage;
	std::shared_ptr<GUIImage>                    m_cvutImage;
	std::shared_ptr<GUIImage>                    m_i3tImage;
	std::vector<std::shared_ptr<TutorialHeader>> m_tutorial_headers;
	void                                         renderTutorials();
};
