#include "API.h"

#include "imgui.h"

#include "Application.h"

using namespace I3T;

ImFont* I3T::getFont(EFont font)
{
	auto* ui = App::get().getUI();
	auto& theme = ui->getTheme();
	return theme.get(font);
}

UI* I3T::getUI()
{
	return App::get().getUI();
}
