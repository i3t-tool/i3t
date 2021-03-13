#pragma once

#include "imgui.h"

#include "Core/Application.h"
#include "GUI/Theme.h"
#include "GUI/UI.h"

namespace I3T
{
inline UI* getUI()
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

template <typename T> IWindow* getWindowPtr()
{
	return getUI()->getWindowPtr<T>();
}
} // namespace I3T
