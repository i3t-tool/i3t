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

#include "imgui.h"

#include "I3T.h"

#include "Core/Application.h"
#include "GUI/Theme.h"
#include "GUI/UIModule.h"

// TODO: (DR) Maybe merge this with the I3TApplication (I3T.h) file?
namespace I3T
{
inline std::vector<Theme>& getThemes()
{
	return getUI()->getThemes();
}

inline Theme& getTheme()
{
	return getUI()->getTheme();
}

/**
 * Change properties of the given theme.
 * @param theme
 */
template <typename Theme_> inline void emplaceTheme(Theme_&& theme)
{
	auto& allThemes = I3T::getThemes();
	auto it = std::find_if(allThemes.begin(), allThemes.end(), [](Theme& theme) {
		return theme.getName() == I3T::getTheme().getName();
	});
	*it = theme;
	I3T::getUI()->setTheme(*it);
}

inline ImFont* getFont(EFont font)
{
	return getTheme().get(font);
}

inline const ImVec4& getColor(EColor color)
{
	return getTheme().get(color);
}

inline float getSize(ESize size)
{
	return getTheme().get(size);
}

inline const ImVec2& getSize(ESizeVec2 size)
{
	return getTheme().get(size);
}

// TODO: (DR) Possibly shouldn't be here, works just like a util
/**
 * Get pointer to dockable (unique) window.
 * \tparam T window type
 * \return shared pointer to window of type T.
 */
template <typename T> Ptr<T> getWindowPtr()
{
	return getUI()->getWindowManager().getWindowPtr<T>();
}
} // namespace I3T
