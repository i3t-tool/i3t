/**
 * \file	GUI/Elements/MainMenuBar.h
 *
 * ImGui menu bar.
 */
#pragma once

/**
 * Top menu bar.
 * The bar also contains dialogs and other windows associated with menu items.
 */
class MainMenuBar
{
public:
  MainMenuBar();
  ~MainMenuBar() {};
  void render();

private:
  bool m_showDemoWindow;

  //AboutDialog m_aboutDialog;
  //DescriptionDialog m_descriptionDialog;
  // SetupDialog m_setupDialog;

  void showFileMenu();
  void showEditMenu();
  void showWindowsMenu();
  void showViewportsMenu();
  void showHelpMenu();
};
