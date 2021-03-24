#pragma once

#include "imgui.h"

#include "Core/Application.h"
#include "GUI/Theme.h"
#include "GUI/UIModule.h"

namespace I3T
{
inline UIModule* getUI()
{
	return App::get().getUI();
}

inline Theme& getTheme()
{
	return getUI()->getTheme();
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

/**
 * Get pointer to dockable (unique) window.
 * \tparam T window type
 * \return shared pointer to window of type T.
 */
template <typename T> Ptr<T> getWindowPtr()
{
	Ptr<IWindow> ptr;
	if ((ptr = getUI()->getWindowPtr<T>()) == nullptr)
	    return nullptr;
  return std::dynamic_pointer_cast<T>(ptr);
}
} // namespace I3T
