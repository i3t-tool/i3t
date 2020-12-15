#pragma once

#include <memory>

#include "GUI/Elements/IWindow.h"

class TextureRegion;


class AboutDialog : public IWindow
{
public:
	static constexpr const char id[] = "AboutDialog";
  std::shared_ptr<TextureRegion> dcgiReg;

  AboutDialog();
  void render() override;
};
