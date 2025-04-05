/**
 * \file
 * \brief GUI wrappers for ImGui menu items and buttons with optional logging functionality.
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#pragma once
#include "Logger/Logger.h"

#include <imgui.h>

namespace I3TGui
{

/**
 * \brief Creates a standard menu item.
 *
 * \param label The display text for the menu item.
 * \param shortcut Optional shortcut key displayed next to the item.
 * \param selected Whether the item is currently selected.
 * \param enabled Whether the item is enabled and clickable.
 * \return true if the menu item was activated by the user.
 */
bool MenuItem(const char* label, const char* shortcut = nullptr, bool selected = false, bool enabled = true);

/**
 * \brief Creates a menu item with optional logging on activation.
 *
 * \param label The display text for the menu item.
 * \param shortcut Optional shortcut key displayed next to the item.
 * \param selected Whether the item is currently selected.
 * \param enabled Whether the item is enabled and clickable.
 * \param log Optional logging callback. If not provided, default logging will be used.
 * \return true if the menu item was activated by the user.
 */
bool MenuItemWithLog(const char* label, const char* shortcut = nullptr, bool selected = false, bool enabled = true,
                     const std::function<void()>& log = nullptr);

/**
 * \brief Creates a menu item with a selection pointer and optional logging.
 *
 * \param label The display text for the menu item.
 * \param shortcut Shortcut key displayed next to the item.
 * \param p_selected Pointer to the selection state of the item.
 * \param enabled Whether the item is enabled and clickable.
 * \param log Optional logging callback. If not provided, default logging will be used.
 * \return true if the menu item was activated by the user.
 */
bool MenuItemWithLog(const char* label, const char* shortcut, bool* p_selected, bool enabled = true,
                     const std::function<void()>& log = nullptr);

/**
 * \brief Begins a new submenu.
 *
 * \param label The display text for the menu.
 * \param enabled Whether the menu is enabled and can be interacted with.
 * \return true if the menu is open and its contents should be rendered.
 */
bool BeginMenu(const char* label, bool enabled = true);

/**
 * \brief Begins a new submenu with optional logging on opening.
 *
 * \param label The display text for the menu.
 * \param enabled Whether the menu is enabled and can be interacted with.
 * \return true if the menu is open and its contents should be rendered.
 */
bool BeginMenuWithLog(const char* label, bool enabled = true);

/**
 * \brief Creates a button.
 *
 * \param label The text displayed on the button.
 * \param size The size of the button (default is auto-sized).
 * \return true if the button was pressed.
 */
bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));

/**
 * \brief Creates a button with optional logging on press.
 *
 * \param label The text displayed on the button.
 * \param size The size of the button (default is auto-sized).
 * \return true if the button was pressed.
 */
bool ButtonWithLog(const char* label, const ImVec2& size = ImVec2(0, 0));

}; // namespace I3TGui
