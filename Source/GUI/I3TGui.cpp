/**
 * \file
 * \brief Implementation of I3TGui wrappers for ImGui widgets with optional logging.
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016–2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "I3TGui.h"
#include <imgui.h>

/**
 * \brief Wrapper around ImGui::MenuItem.
 */
bool I3TGui::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled)
{
	return ImGui::MenuItem(label, shortcut, selected, enabled);
}

/**
 * \brief Wrapper around ImGui::MenuItem with optional logging on selection.
 *
 * \param label The menu item label.
 * \param shortcut The shortcut key shown with the item (optional).
 * \param selected Whether the item is currently selected.
 * \param enabled Whether the item is enabled.
 * \param log Optional logging callback. If not provided, default logging will be used.
 * \return true if the menu item was clicked.
 */
bool I3TGui::MenuItemWithLog(const char* label, const char* shortcut, bool selected, bool enabled,
                             const std::function<void()>& log)
{
	if (ImGui::MenuItem(label, shortcut, selected, enabled))
	{
		if (log)
			log();
		else
			LOG_EVENT_SELECT(label);

		return true;
	}
	return false;
}

/**
 * \brief Wrapper around MenuItemWithLog that also modifies a selection flag.
 *
 * \param label The menu item label.
 * \param shortcut The shortcut key.
 * \param p_selected Pointer to the selection state flag (can be nullptr).
 * \param enabled Whether the item is enabled.
 * \param log Optional logging callback. If not provided, default logging will be used.
 * \return true if the menu item was clicked.
 */
bool I3TGui::MenuItemWithLog(const char* label, const char* shortcut, bool* p_selected, bool enabled,
                             const std::function<void()>& log)
{
	if (MenuItemWithLog(label, shortcut, p_selected ? *p_selected : false, enabled, log))
	{
		if (p_selected)
			*p_selected = !*p_selected;
		return true;
	}
	return false;
}

/**
 * \brief Wrapper around ImGui::BeginMenu.
 *
 * \param label The menu title.
 * \param enabled Whether the menu is enabled.
 * \return true if the menu is open and should render its contents.
 */
bool I3TGui::BeginMenu(const char* label, bool enabled)
{
	return ImGui::BeginMenu(label, enabled);
}

/**
 * \brief Wrapper around ImGui::BeginMenu with logging on menu activation.
 *
 * \param label The menu title.
 * \param enabled Whether the menu is enabled.
 * \return true if the menu is open and should render its contents.
 */
bool I3TGui::BeginMenuWithLog(const char* label, bool enabled)
{
	if (ImGui::BeginMenu(label, enabled))
	{
		if (ImGui::IsItemClicked())
		{
			LOG_EVENT_MENU_CLICK(std::string(label));
		}
		return true;
	}
	return false;
}

/**
 * \brief Wrapper around ImGui::Button.
 *
 * \param label The button label.
 * \param size The size of the button.
 * \return true if the button was pressed.
 */
bool I3TGui::Button(const char* label, const ImVec2& size)
{
	return ImGui::Button(label, size);
}

/**
 * \brief Wrapper around ImGui::Button with logging on click.
 *
 * \param label The button label.
 * \param size The size of the button.
 * \return true if the button was pressed.
 */
bool I3TGui::ButtonWithLog(const char* label, const ImVec2& size)
{
	if (ImGui::Button(label, size))
	{
		LOG_EVENT_BUTTON_CLICK(std::string(label));
		return true;
	}
	return false;
}
