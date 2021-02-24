/**
 * \file	GUI\inputController.h
 *
 * Declares the input controller class.
 * Header for representing keyboard/mouse tracker
 *
 * \date:	2014/11/16, 2020/09/09, 2020/12/07
 * \author: Michal Folta, CTU Prague
 */
#pragma once

#include <map>

#include <glm/vec2.hpp>

#include "GUI/Elements/IWindow.h"

struct GLFWwindow;

/** State of the three mouse buttons (does not store the state of the modificators */
struct MouseButtonState final
{
	bool left, right, middle; ///< mouse buttons
	// bool shift, alt, ctrl;	///< keyboard modifiers - NOT USED

	MouseButtonState()
	{
		left = false;
		right = false;
		middle = false;
	}

	/**
	 * \brief Is any mouse button pressed?
	 * The inner state depends on the Keys::mouseLeft, right, middle. It must be one step behind the reality...
	 * The inner state is set by InputController::preUpdate(); in  main.cpp: logicUpdate()
	 * \todo Check - never used
	 * \return true if any button pressed.
	 */
	bool isPressed() const { return (left || right || middle); }
};
//===----------------------------------------------------------------------===//

/** Status of the keyboard keys and also mouse keys. */
class Keys final
{
public:
	/** Codes representing keys on the keyboard and mouse (letters, numbers, functional keys, arrows, ... )
	  \todo Add handling of Ctrl-C, Ctrl-V, ...
	  */
	enum Code
	{
		a = 0,
		b,
		c,
		d,
		e,
		f,
		g,
		h,
		i,
		j,
		k,
		l,
		m,
		n,
		o,
		p,
		q,
		r,
		s,
		t,
		u,
		v,
		w,
		x,
		y,
		z,
		n1,
		n2,
		n3,
		n4,
		n5,
		n6,
		n7,
		n8,
		n9,
		n0,
		altr,
		altl,
		ctrll,
		ctrlr,
		shiftl,
		shiftr,
		left,
		right,
		up,
		down,
		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
		f7,
		f8,
		f9,
		f10,
		f11,
		f12,
		home,
		end,
		insert,
		del,
		pageUp,
		pageDown,
		esc,
		mouseLeft,
		mouseRight,
		mouseMiddle,
		mouseScrlUp,
		mouseScrlDown
	};

	static const char* getKeyString(int enumVal) { return keyStrings[enumVal]; }

	static const char* keyStrings[];
};
//===----------------------------------------------------------------------===//

/**
 * \brief Class handling all GLUT interaction events (mouse and keyboard) and starting the label editor.
 *
 * This class stores state of all keys and mouse buttons in a map \a keyMap - used for controlling all
 * interaction. It also updates the stored statuses of the keys from JUST pressed/released to pressed/release.
 * The update() is called regularly from the end of main.cpp: logicUpdate()
 */
class InputController final
{
public:
	enum KeyState
	{
		UP,
		JUST_UP,
		DOWN,
		JUST_DOWN
	}; ///< state of the key used in keyMap - enum { 0 - up, 1 - just up, 2 - down, 3 - just down}

	static std::map<Keys::Code, KeyState>
			m_keyMap; ///< states of all keys, modifiers, and mouse buttons - enum value KeyState. changed in update()

	static int m_winWidth, m_winHeight; ///< Window size

	static MouseButtonState m_mouseButtonState; ///< status of L,M,R mouse buttons (true for pressed) without modifiers.
	                                            ///< \todo passed to all handlers, but probably not used

	static bool m_ignoreImGuiEvents;
	static glm::vec2 m_mouseOffset;
	static float m_mouseX, m_mouseY, m_mouseXPrev, m_mouseYPrev; ///< mouse cursor position
	static float m_mouseXDelta, m_mouseYDelta;

public:
	//@{
	/** \name Handling of the keyboard keys */

	/**
	 * Sets an entry in the keyMap to justPressed (JUST_DOWN)
	 *
	 * \param	code The enum code of the key.
	 */
	static void setPressed(const Keys::Code code)
	{
		// if (keyMap[code] != DOWN)
		m_keyMap[code] = JUST_DOWN;
	}

	static void setUnpressed(const Keys::Code code) { m_keyMap[code] = JUST_UP; }

	static bool isKeyPressed(const Keys::Code code) { return (m_keyMap[code] == DOWN || m_keyMap[code] == JUST_DOWN); }

	static bool isKeyJustPressed(const Keys::Code code) { return (m_keyMap[code] == JUST_DOWN); }

	static bool isKeyJustUp(const Keys::Code code) { return (m_keyMap[code] == JUST_UP); }
	//@}

	/**
	 * Locks cursor before camera drag or rotation.
	 */
	static void beginCameraControl();

	/** Unlock the cursor after camera interaction. */
	static void endCameraControl();

	/**
	 * Sets screen size - called from GUI::resize(), which is called by main.onReshape()
	 *
	 * \param width The screen width.
	 * \param	height The screen height.
	 */
	static void setScreenSize(const int width, const int height)
	{
		m_winWidth = width;
		m_winHeight = height;
	}

	//@{
	/** \name Handling the mouse */
	/**
	 * \brief Copies state of the mouse buttons from keyMap into bool variables of the mouseButtonState
	 */
	static void preUpdate();
	//@}

	/**
	 * \brief Updates \a mouseDelta and \a mousePrev, and updates the stored statuses of the keys in the \a keyMap array
	 * (JUST_UP -> UP, JUST_DOWN -> DOWN).
	 */
	static void update();

	/**
	 * Check ImGui mouse input for current window and set mouse cursor position and mouse button map.
	 * Use this function after ImGui::Begin(...).
	 */
	static void processViewportEvents();

	static bool isActionZoomToAll();

	//@{
	/** \name Key callbacks */
private:
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
};
