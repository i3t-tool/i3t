/**
 * \file	GUI/Elements/IWindow.h
 *
 * ImGui GUI Window abstract class
 */
#pragma once

#include <string>

#include "glm/vec2.hpp"
#include "spdlog/fmt/fmt.h"

#include "Core/Input/InputController.h"

#define I3T_WINDOW(WindowType)                                                                                         \
public:                                                                                                                \
	static constexpr const char* ID = #WindowType;                                                                     \
	[[nodiscard]] const char* getID() const override                                                                   \
	{                                                                                                                  \
		return ID;                                                                                                     \
	}

class WindowManager;

/**
 * ImGui GUI Window abstract class.
 * Every window should call its IWindow::updateWindowInfo() method after ImGui::Begin() call.
 *
 * \see WindowManager for lifecycle information.
 */
class IWindow : public std::enable_shared_from_this<IWindow>
{
	friend class WindowManager;

public:
	// Window info (updated by updateWindowInfo())
	glm::vec2 m_windowPos;   ///< Top-left corner of the window in screen coordinates
	glm::ivec2 m_windowSize; ///< Window width and height dimensions

	glm::vec2 m_windowMin; ///< Top left corner of the window, same as m_windowPos (separate variable for clarity)
	glm::vec2 m_windowMax; ///< Bottom right corner of the window

protected:
	bool m_show;
	std::string m_name;
	InputController Input;

	WindowManager* m_windowManager = nullptr; ///< Weak reference to a WindowManager set when this window is added to it
public:
	friend class Application;
	friend class InputManager;

	explicit IWindow(bool show = false) : m_show(show) {}

	/**
	 * \pre Window cannot be destroyed at runtime. It may cause crash.
	 */
	virtual ~IWindow() = default;
	virtual void render() = 0;

	virtual const char* getID() const = 0;

	void hide()
	{
		m_show = false;
	}
	void show()
	{
		m_show = true;
	}
	bool isVisible() const
	{
		return m_show;
	}
	bool* getShowPtr()
	{
		return &m_show;
	}

	std::string getName() const;
	std::string setName(const char* name);

	/**
	 * Returns window input controller.
	 */
	InputController& getInput()
	{
		return Input;
	}
	InputController* getInputPtr()
	{
		return &Input;
	}

protected:
	/**
	 * Collect information about the current window. Should be called right after ImGui's Begin() function
	 * to store and supply window's screen position and size to any code being called until another window starts being
	 * constructed. If this call is omitted after beginning a window anything querying the WindowManager for window
	 * position and dimensions may receive invalid information.
	 */
	void updateWindowInfo();
};
