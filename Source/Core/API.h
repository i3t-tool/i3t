#pragma once

#include "imgui.h"

#include "Core/Application.h"
#include "GUI/Theme.h"
#include "GUI/UIModule.h"

// TODO: (DR) This probably shouldn't be in the Core (As its basically a global utility namespace)
namespace I3T
{
inline UIModule* getUI()
{
	return App::get().getUI();
}

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
