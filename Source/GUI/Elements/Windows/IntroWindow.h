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

class IntroWindow : public IWindow
{
public:
  I3T_WINDOW(IntroWindow) 
  IntroWindow(bool show);

  /**
   * \brief get all folders
   */
  void reloadTutorials();

  /**
   * \brief Renders the tutorial window using ImGui.
   */
  void render() override;

private:
  std::shared_ptr<GUIImage> dummyImage;
  std::shared_ptr<GUIImage> folderImage;
  std::shared_ptr<GUIImage> logoImage;
  std::shared_ptr<GUIImage> cvutImage;
  std::vector<std::shared_ptr<TutorialHeader>> m_tutorial_headers;
  void renderTutorials();
};
