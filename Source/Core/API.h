#pragma once

#include "GUI/Theme.h"
#include "GUI/UI.h"

namespace I3T
{
ImFont* getFont(EFont font);
UI* getUI();

template <typename T>
IWindow* getWindowPtr()
{
	return getUI()->template getWindowPtr<T>();
}
}
