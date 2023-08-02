#pragma once

#include "GUI/Elements/IWindow.h"
#include "Tutorial/Tutorial.h"

class BeforeNewTutModal : public ModalWindow
{
public:
	BeforeNewTutModal() : ModalWindow("Opening a new tutorial") {}

	void onImGui() override;

private:
	void submit();
};