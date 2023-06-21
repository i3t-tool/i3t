/**
 * \file	GUI\inputController.h
 *
 * Declares the input controller class.
 * Header for representing keyboard/mouse tracker
 *
 * \date:	2014/11/16, 2020/09/09, 2020/12/07
 * \author: Michal Folta, CTU Prague, Martin Herich <hericmar@fel.cvut.cz>
 */
#pragma once

#include <map>
#include <vector>

#include "glm/vec2.hpp"

#include "Core/Defs.h"
#include "GUI/Elements/IWindow.h"
#include "InputBindings.h"
#include "InputController.h"
#include "KeyCodes.h"

struct GLFWwindow;

// TODO: (DR) For InputManager to remain in Core, it should be completely separated from ImGui,
//  that likely means it could only function as an Interface with a ImGuiInputManager implementation or so located in
//  the GUI package

// TODO: (DR) The class probably also shouldn't be static, could be a singleton module like others
//  This kind of brings up the issue of how would code access input manager, right now its a simple static call but
//  if it was a module it requires a way to get the Application instance, which can be done statically via App::get()
//  Is that a good approach? Or would it be better to pass around instance of InputManager for querying input?

// TODO: (DR) Refactoring and doc

//===----------------------------------------------------------------------===//

/**
 * \brief Class handling all GLUT interaction events (mouse and keyboard) and
 * starting the label editor.
 *
 * This class stores state of all keys and mouse buttons in a map \a keyMap -
 * used for controlling all interaction. It also updates the stored statuses of
 * the keys from JUST pressed/released to pressed/release. The update() is
 * called regularly from the end of main.cpp: logicUpdate()
 */
class InputManager final
{
public:
	enum KeyState
	{
		UP,
		JUST_UP,
		DOWN,
		JUST_DOWN
	}; ///< state of the key used in keyMap - enum { 0 - up, 1 - just up, 2 -
	   ///< down, 3 - just down}

	/// States of all keys, modifiers, and mouse buttons - enum value KeyState.
	/// changed in update()
	static std::map<Keys::Code, KeyState> m_keyMap;

	///< \todo passed to all handlers, but probably not used

	// TODO: (DR) Never accessed, unused? Related
	static bool m_ignoreImGuiEvents;

	static float m_mouseX,
	    m_mouseY; ///< Current mouse cursor position in screen coordinates (0 is corner of the monitor)
	static float m_mouseXPrev, m_mouseYPrev;   ///< Previous frame's cursor position
	static float m_mouseXDelta, m_mouseYDelta; ///< Change of the cursor position across last two frames
	static float m_mouseXDragDelta,
	    m_mouseYDragDelta;           ///< Same as mouse delta but only non-zero during some mouse press
	static float m_mouseWheelOffset; ///< Immediate mouse scroll change

private:
	static std::vector<InputController*> m_inputControllers;

public:
	static void init();

	/**
	 * Updates mouse position and buttons
	 */
	static void beginFrame();

	/**
	 * Updates the stored statuses of the keys in the \a keyMap array (JUST_UP -> UP, JUST_DOWN -> DOWN).
	 * Processes events.
	 */
	static void endFrame();

	static glm::vec2 getMouseDelta()
	{
		return {m_mouseXDelta, m_mouseYDelta};
	}
	static glm::vec2 getMousePos()
	{
		return {m_mouseX, m_mouseY};
	}

	static void bindGlobalAction(const char* action, EKeyState state, KeyCallback fn);

	static void triggerAction(const char* action, EKeyState state);

	static void setInputAction(const char* action, Keys::Code code, ModifiersList mods = ModifiersList());
	static void setInputAxis(const char* action, float scale, Keys::Code code, ModifiersList mods = ModifiersList());

	static void addInputController(InputController* controller)
	{
		m_inputControllers.push_back(controller);
	}

	static bool areModifiersActive(Modifiers mods);

	/**
	 * Set active input controller (for focused window).
	 */
	static void setActiveInput(InputController* input);

	/**
	 * Check if the specified input controller is active.
	 * Window input controller will be active when the window is focused.
	 */
	static bool isInputActive(InputController* input);

public:
	/**
	 * Sets an entry in the keyMap to justPressed (JUST_DOWN)
	 *
	 * \param	code The enum code of the key.
	 */
	static void setPressed(const Keys::Code code)
	{
		m_keyMap[code] = JUST_DOWN;
	}

	//===----------------------------------------------------------------------===//
	static bool isActionTriggered(const char* name, EKeyState state);
	static bool isAxisActive(const char* name);
	//===----------------------------------------------------------------------===//

	static void setUnpressed(const Keys::Code code)
	{
		m_keyMap[code] = JUST_UP;
	}
	static bool isKeyPressed(const Keys::Code code)
	{
		return (m_keyMap[code] == DOWN || m_keyMap[code] == JUST_DOWN);
	}
	static bool isKeyJustPressed(const Keys::Code code)
	{
		return (m_keyMap[code] == JUST_DOWN);
	}
	static bool isKeyJustUp(const Keys::Code code)
	{
		return (m_keyMap[code] == JUST_UP);
	}

	/**
	 * @return Whether any of the mouse buttons was clicked
	 */
	static bool isMouseClicked();

	/**
	 * @return Whether any of the mouse buttons is pressed
	 */
	static bool isMouseDown();

	/**
	 * \brief Helper function for handling pressed keys.
	 * \param	keyPressed	The key pressed.
	 */
	static void keyDown(int keyPressed);

	/**
	 * \brief Helper function for handling released keys.
	 * \param	keyReleased	The key released.
	 */
	static void keyUp(int keyReleased);
	//@}

	/**
	 * Get active GLFW viewport (window).
	 * \return GLFW window.
	 */
	static GLFWwindow* getCurrentViewport();

private:
	/**
	 * Locks cursor before camera drag or rotation.
	 */
	static void beginCameraControl();

	/** Unlock the cursor after camera interaction. */
	static void endCameraControl();

	/**
	 * Process action and axis events.
	 */
	static void processEvents(InputController& controller);

	/**
	 * Check ImGui mouse input for current window and set mouse cursor position
	 * and mouse button map. Use this function after ImGui::Begin(...).
	 */
	static void processViewportEvents();

private:
	static InputController s_globalInputController;
	static InputController* s_activeInput;
};
