/**
 * \file GUI/Elements/Dialogs/setupDialog.h
 */
#pragma once

#include "GUI/Elements/IWindow.h"

// TODO: (DR) Remove, not used anymore

class SetupDialog : public IWindow
{
public:
	I3T_WINDOW(SetupDialog)

	virtual void render() override;
};
