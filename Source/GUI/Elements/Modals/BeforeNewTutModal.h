#pragma once

#include "GUI/Elements/IWindow.h"
#include "Tutorial/Tutorial.h"

class BeforeNewTutModal : public IWindow
{
public:
	I3T_WINDOW(BeforeNewTutModal)

	BeforeNewTutModal() = default;

	void render() override;

private:
	void submit();
};