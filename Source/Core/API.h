#pragma once

#include "GUI/Theme.h"
#include "GUI/UI.h"

namespace I3T
{
ImFont* getFont(EFont font);
const ImVec4& getColor(EColor color);
Theme& getTheme();
UI* getUI();

template <typename T> IWindow* getWindowPtr()
{
	return getUI()->getWindowPtr<T>();
}
} // namespace I3T
