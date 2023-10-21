/**
 * \file
 * \brief ImGui GUI Window abstract class
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
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
	explicit IWindow(bool show = false) : m_show(show) {}

	/**
	 * \pre Window cannot be destroyed at runtime. It may cause crash.
	 */
	virtual ~IWindow() = default;

	virtual void render() = 0;

public:
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

	/**
	 * @return ImGui window name.
	 */
	const char* getName() const;
	const std::string& setName(const char* name);

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

public:
	// Window info (updated by updateWindowInfo())
	glm::vec2 m_windowPos;   ///< Top-left corner of the window in screen coordinates
	glm::ivec2 m_windowSize; ///< Window width and height dimensions

	glm::vec2 m_windowMin; ///< Top left corner of the window, same as m_windowPos (separate variable for clarity)
	glm::vec2 m_windowMax; ///< Bottom right corner of the window

protected:
	bool m_show;
	std::string imGuiName;
	InputController Input;

	WindowManager* m_windowManager = nullptr; ///< Weak reference to a WindowManager set when this window is added to it
};


class ModalWindow : public IWindow
{
public:
	ModalWindow(const std::string& title);

protected:
	virtual void onImGui() = 0;

public:
	void open();

	/**
	 * Hides this dialog and all its children.
	 */
	void close();

private:
	void render() override;

	const char* getID() const override
	{
		return "ModalWindow";
	}
};
