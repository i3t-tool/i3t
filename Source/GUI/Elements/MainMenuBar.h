/**
 * \file	GUI/Elements/MainMenuBar.h
 * \author Martin Herich <hericmar@fel.cvut.cz>
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
	virtual ~MainMenuBar() = default;

	void render();

private:
	bool m_showDemoWindow = false;

	void showFileMenu();
	void showEditMenu();
	void showWindowsMenu();
	void showHelpMenu();
};
