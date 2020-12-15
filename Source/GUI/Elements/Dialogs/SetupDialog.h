/**
 * \file GUI/Elements/Dialogs/setupDialog.h
 */
#pragma once

#include "GUI/Elements/IWindow.h"

class SetupDialog : public IWindow
{
public:
  static constexpr const char id[] = "SetupDialog";

  virtual void render() override;
};
