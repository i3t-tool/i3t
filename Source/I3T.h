/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Core/Application.h"
#include "GUI/Theme/Theme.h"
#include "GUI/UIModule.h"


class I3TApplication : public Application
{
public:
	bool m_debugWindowManager = false; ///< Debug switch for WindowManager, toggled in Help > Debug window manager
	bool m_debugTrackball = false;     ///< Debug switch for WindowManager, toggled in Help > Debug trackball

protected:
	void onInit() override;
	void onBeginFrame() override {};
	void onEndFrame() override {};
	void onUpdate(double delta) override;
	void onClose() override;
};

// Forward declarations
namespace Vp
{
class Viewport;
}
namespace Core
{
class ResourceManager;
}
class UIModule;
class WorkspaceModule;
class ViewportModule;

// Static util methods
namespace I3T
{
// Global app accessor
I3TApplication& app();

// Modules
UIModule* getUI();
Vp::Viewport* getViewport();
ViewportModule& getViewportModule();
Core::ResourceManager& getResourceManager();
WorkspaceModule& getWorkspace();

// Theme
std::vector<Theme>& getThemes();
Theme& getTheme();
ImFont* getFont(EFont font);
const ImVec4& getColor(EColor color);
float getSize(ESize size);
ImVec2 getSize(ESizeVec2 size);

/**
 * Change properties of the given theme.
 * @param theme
 */
template <typename Theme_>
inline void emplaceTheme(Theme_&& theme)
{
	auto& allThemes = I3T::getThemes();
	auto it = std::find_if(allThemes.begin(), allThemes.end(), [](Theme& theme) {
		return theme.getName() == I3T::getTheme().getName();
	});
	*it = theme;
	I3T::getUI()->setTheme(*it);
}

// Window
/**
 * Get pointer to dockable (unique) window.
 * \tparam T window type
 * \return shared pointer to window of type T.
 */
template <typename T>
Ptr<T> getWindowPtr()
{
	return getUI()->getWindowManager().getWindowPtr<T>();
}
} // namespace I3T