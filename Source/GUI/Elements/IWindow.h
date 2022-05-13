/**
 * \file	GUI/Elements/IWindow.h
 *
 * GUI element class.
 */
#pragma once

#include <string>

#include "spdlog/fmt/fmt.h"

#include "Core/Input/InputController.h"

#define I3T_WINDOW(WindowType)                                                                                       \
public:                                                                                                              \
	static constexpr const char* ID = #WindowType;                                                                     \
	[[nodiscard]] const char* getID() const override { return ID; }

/**
 * ImGui GUI Window abstract class.
 *
 * \see ~IWindow for lifecycle information.
 */
class IWindow
{
public:
	explicit IWindow(bool show = false) : m_show(show) {}

	/**
	 * \pre Window cannot be destroyed at runtime. It may cause crash.
	 */
	virtual ~IWindow() = default;
	virtual void render() = 0;
	virtual const char* getID() const = 0;
	void hide() { m_show = false; }
	void show() { m_show = true; }
	bool isVisible() const { return m_show; }
	bool* getShowPtr() { return &m_show; }
  std::string getName(const char* name) const { return fmt::format("{}###{}", name, getID()); };

	/**
	 * Returns window input controller.
	 */
	InputController& getInput() { return Input; }
	InputController* getInputPtr() { return &Input; }

protected:
	friend class Application;
	friend class InputManager;
	bool m_show;
	InputController Input;
};
