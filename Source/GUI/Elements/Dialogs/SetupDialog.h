/**
 * \file GUI/Elements/Dialogs/setupDialog.h
 */
#pragma once

#include "GUI/Elements/IWindow.h"

class SetupDialog : public IWindow
{
public:
	I3T_WINDOW(SetupDialog)

	virtual void render() override;
};
