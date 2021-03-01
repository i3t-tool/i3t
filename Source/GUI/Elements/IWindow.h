/**
 * \file	GUI/Elements/IWindow.h
 *
 * GUI element class.
 */
#pragma once

#include <string>

#define I3T_WINDOW(WindowType)                                                                                       \
public:                                                                                                              \
	static constexpr const char* ID = #WindowType;                                                                     \
	[[nodiscard]] const char* getID() const override { return ID; }

/**
 * ImGui GUI element abstract class.
 */
class IWindow
{
public:
	IWindow(bool show = false) : m_show(show) {}

	virtual ~IWindow() = default;
	virtual void render() = 0;
	virtual const char* getID() const = 0;
	void hide() { m_show = false; }
	void show() { m_show = true; }
	bool isVisible() const { return m_show; }
	bool* getShowPtr() { return &m_show; }

protected:
	friend class Application;
	bool m_show;
};
