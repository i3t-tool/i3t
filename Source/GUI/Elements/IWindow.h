/**
 * \file	GUI/Elements/IWindow.h
 *
 * GUI element class.
 */
#pragma once

/**
 * ImGui GUI element abstract class.
 */
class IWindow
{
public:
  IWindow(bool show = false) { m_show = show; };

  virtual ~IWindow() = default;
  virtual void render() = 0;
  void hide() { m_show = false; }

protected:
  friend class Application;
  bool m_show;
};
