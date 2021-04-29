#include "InputManager.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Core/Application.h"
#include "Core/GlfwWindow.h"
#include "InputBindings.h"
#include "Logger/LoggerInternal.h"

#include "GUI/Elements/Windows/ViewportWindow.h"

#define IM_MOUSE_KEYS_COUNT 3

constexpr Keys::Code imGuiMouseKeys[] = {Keys::mouseLeft, Keys::mouseRight, Keys::mouseMiddle};

ImGuiConfigFlags g_mousedFlags;

void InputManager::canSetInputAction(const char* action, Keys::Code code)
{

}

void InputManager::setInputAction(const char* action, Keys::Code code)
{
	if (!InputBindings::isActionCreated(action))
		InputBindings::m_inputActions.insert({action, code});
}

void InputManager::setInputAxis(const char* action, float scale, Keys::Code code)
{
	if (!InputBindings::isAxisCreated(action))
		InputBindings::m_inputAxis[action];

	InputBindings::m_inputAxis[action].push_back({code, scale});
}

bool InputManager::isMouseClicked()
{
	for (int i = 0; i < IM_MOUSE_KEYS_COUNT; i++)
	{
		if (ImGui::IsMouseClicked(i))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::isMouseDown()
{
	for (int i = 0; i < IM_MOUSE_KEYS_COUNT; i++)
	{
		if (ImGui::IsMouseDown(i))
		{
			return true;
		}
	}
	return false;
}

void InputManager::processViewportEvents()
{
	ImGuiIO& io = ImGui::GetIO();

	m_mouseOffset.x = ImGui::GetWindowPos().x;
	m_mouseOffset.y = ImGui::GetWindowPos().y + ImGui::GetFrameHeight();

	// Get cursor position, consider window offset and tab height.
	float cursorXPos = io.MousePos.x - m_mouseOffset.x;
	float cursorYPos = io.MousePos.y - m_mouseOffset.y;

	double x = 0;
	double y = 0;
	glfwGetCursorPos(Application::get().mainWindow(), &x, &y);

	if (ImGui::IsMousePosValid())
	{
		m_mouseX = cursorXPos;
		m_mouseY = cursorYPos;
	}

	/// \todo MH

	/*
	// Avoid catching events outside of window.
	if (!ImGui::IsWindowFocused() || !ImGui::IsWindowHovered())
	  return;
	*/
	if (!isActive<UI::Viewport>())
		return;

	// Check left, right and middle button.
	for (int i = 0; i < IM_MOUSE_KEYS_COUNT; i++)
	{
		if (ImGui::IsMouseClicked(i))
		{
			LOG_EVENT_MOUSE_CLICK(Keys::getKeyString(imGuiMouseKeys[i]), std::to_string(m_mouseX),
			                      std::to_string(m_mouseY));
			setPressed(imGuiMouseKeys[i]);
		}

		if (ImGui::IsMouseReleased(i))
		{
			LOG_EVENT_MOUSE_RELEASE(Keys::getKeyString(imGuiMouseKeys[i]), std::to_string(m_mouseX),
			                        std::to_string(m_mouseY));
			setUnpressed(imGuiMouseKeys[i]);
		}
	}

	/*
	// Check scrolling.
	if (io.MouseWheel < -0.1f)
		setPressed(Keys::mouseScrlUp);
	else
		setUnpressed(Keys::mouseScrlUp);

	if (io.MouseWheel > 0.1f)
		setPressed(Keys::mouseScrlDown);
	else
		setUnpressed(Keys::mouseScrlDown);
	 */

	// Handle keys.
	/*
	for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
	{
	  if (ImGui::IsKeyPressed(i))
	    keyDown(i);

	  if (ImGui::IsKeyReleased(i))
	    keyUp(i);
	}
	 */
}

bool InputManager::isActionZoomToAll()
{
	return isKeyPressed(Keys::ctrll) && isKeyJustPressed(Keys::a);
}

void InputManager::beginCameraControl()
{
	// Disable system cursor. The cursor will be hidden and at the endCameraControl the cursor will
	// be at the same position.
	glfwSetInputMode(getCurrentViewport(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	m_ignoreImGuiEvents = true;

	// Tell ImGui to don't capture mouse position. No widgets will be set to the hovered state.
	auto& io = ImGui::GetIO();
	g_mousedFlags = io.ConfigFlags; // Store current IO configuration flags.
	io.ConfigFlags |= ImGuiConfigFlags_NoMouse | ImGuiConfigFlags_NoMouseCursorChange;
}

void InputManager::endCameraControl()
{
	glfwSetInputMode(getCurrentViewport(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	m_ignoreImGuiEvents = false;

	auto& io = ImGui::GetIO();
	io.ConfigFlags = g_mousedFlags;
}

void InputManager::preUpdate()
{
	m_mouseButtonState.left = isKeyPressed(Keys::mouseLeft);
	m_mouseButtonState.right = isKeyPressed(Keys::mouseRight);
	m_mouseButtonState.middle = isKeyPressed(Keys::mouseMiddle);
}

void InputManager::update()
{
	m_mouseXDelta = m_mouseX - m_mouseXPrev;
	m_mouseYDelta = m_mouseY - m_mouseYPrev;

	m_mouseXPrev = m_mouseX;
	m_mouseYPrev = m_mouseY;

	double x = 0, y = 0;
	glfwGetCursorPos(Application::get().mainWindow(), &x, &y);

	LOG_EVENT_MOUSE_POS(std::to_string(m_mouseX), std::to_string(m_mouseY));

	// mouseXDelta = 0;
	// mouseYDelta = 0;

	if (m_focusedWindow)
	{
		for (const auto& [key, fn] : m_focusedWindow->Input.m_keyCallbacks)
		{
			if (m_keyMap[key] == KeyState::DOWN)
				fn();
		}

		for (const auto& [key, fn] : m_focusedWindow->Input.m_keyDownCallbacks)
		{
			if (m_keyMap[key] == KeyState::JUST_DOWN)
				fn();
		}

    for (const auto& [action, state, fn] : m_focusedWindow->Input.m_actions)
    {
			auto key = InputBindings::m_inputActions[action];
			bool shouldProcess = m_keyMap[key] == KeyState::JUST_UP || m_keyMap[key] == KeyState::JUST_DOWN;
			if (shouldProcess)
      {
        if (m_keyMap[key] == KeyState::JUST_DOWN && state == EKeyState::Pressed)
        {
          fn();
				}
        if (m_keyMap[key] == KeyState::JUST_UP && state == EKeyState::Released)
        {
          fn();
        }
			}
    }

		for (const auto& [action, fn] : m_focusedWindow->Input.m_axis)
    {
      auto keys = InputBindings::m_inputAxis[action];
			for (const auto& [key, scale] : keys)
      {
				if (m_keyMap[key] == KeyState::DOWN || m_keyMap[key] == KeyState::JUST_DOWN)
        {
					fn(scale);
				}
			}
		}
	}

	for (std::map<Keys::Code, KeyState>::const_iterator it = m_keyMap.begin(); it != m_keyMap.end(); ++it)
	{
		if (it->second == JUST_UP)
		{
			m_keyMap[it->first] = UP;
		}
		else if (it->second == JUST_DOWN)
		{
			m_keyMap[it->first] = DOWN;
		}
	}
}

void InputManager::keyDown(int keyPressed)
{
	switch (keyPressed)
	{
		// alphabet a-z
	case GLFW_KEY_A:
		setPressed(Keys::a);
		break;
	case GLFW_KEY_B:
		setPressed(Keys::b);
		break;
	case GLFW_KEY_C:
		setPressed(Keys::c);
		break;
	case GLFW_KEY_D:
		setPressed(Keys::d);
		break;
	case GLFW_KEY_E:
		setPressed(Keys::e);
		break;
	case GLFW_KEY_F:
		setPressed(Keys::f);
		break;
	case GLFW_KEY_G:
		setPressed(Keys::g);
		break;
	case GLFW_KEY_H:
		setPressed(Keys::h);
		break;
	case GLFW_KEY_I:
		setPressed(Keys::i);
		break;
	case GLFW_KEY_J:
		setPressed(Keys::j);
		break;
	case GLFW_KEY_K:
		setPressed(Keys::k);
		break;
	case GLFW_KEY_L:
		setPressed(Keys::l);
		break;
	case GLFW_KEY_M:
		setPressed(Keys::m);
		break;
	case GLFW_KEY_N:
		setPressed(Keys::n);
		break;
	case GLFW_KEY_O:
		setPressed(Keys::o);
		break;
	case GLFW_KEY_P:
		setPressed(Keys::p);
		break;
	case GLFW_KEY_Q:
		setPressed(Keys::q);
		break;
	case GLFW_KEY_R:
		setPressed(Keys::r);
		break;
	case GLFW_KEY_S:
		setPressed(Keys::s);
		break;
	case GLFW_KEY_T:
		setPressed(Keys::t);
		break;
	case GLFW_KEY_U:
		setPressed(Keys::u);
		break;
	case GLFW_KEY_V:
		setPressed(Keys::v);
		break;
	case GLFW_KEY_W:
		setPressed(Keys::w);
		break;
	case GLFW_KEY_X:
		setPressed(Keys::x);
		break;
	case GLFW_KEY_Y:
		setPressed(Keys::y);
		break;
	case GLFW_KEY_Z:
		setPressed(Keys::z);
		break;

		// numbers
	case GLFW_KEY_KP_0:
		setPressed(Keys::n0);
		break;
	case GLFW_KEY_KP_1:
		setPressed(Keys::n1);
		break;
	case GLFW_KEY_KP_2:
		setPressed(Keys::n2);
		break;
	case GLFW_KEY_KP_3:
		setPressed(Keys::n3);
		break;
	case GLFW_KEY_KP_4:
		setPressed(Keys::n4);
		break;
	case GLFW_KEY_KP_5:
		setPressed(Keys::n5);
		break;
	case GLFW_KEY_KP_6:
		setPressed(Keys::n6);
		break;
	case GLFW_KEY_KP_7:
		setPressed(Keys::n7);
		break;
	case GLFW_KEY_KP_8:
		setPressed(Keys::n8);
		break;
	case GLFW_KEY_KP_9:
		setPressed(Keys::n9);
		break;

		// specials
	case GLFW_KEY_ESCAPE:
		setPressed(Keys::esc);
		break;

	case GLFW_KEY_LEFT_ALT:
		setPressed(Keys::altl);
		break;
	case GLFW_KEY_RIGHT_ALT:
		setPressed(Keys::altr);
		break;
	case GLFW_KEY_LEFT_CONTROL:
		setPressed(Keys::ctrll);
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		setPressed(Keys::ctrlr);
		break;
	case GLFW_KEY_LEFT_SHIFT:
		setPressed(Keys::shiftl);
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		setPressed(Keys::shiftr);
		break;

	case GLFW_KEY_UP:
		setPressed(Keys::up);
		break;
	case GLFW_KEY_DOWN:
		setPressed(Keys::down);
		break;
	case GLFW_KEY_LEFT:
		setPressed(Keys::left);
		break;
	case GLFW_KEY_RIGHT:
		setPressed(Keys::right);
		break;

	case GLFW_KEY_F1:
		setPressed(Keys::f1);
		break;
	case GLFW_KEY_F2:
		setPressed(Keys::f2);
		break;
	case GLFW_KEY_F3:
		setPressed(Keys::f3);
		break;
	case GLFW_KEY_F4:
		setPressed(Keys::f4);
		break;
	case GLFW_KEY_F5:
		setPressed(Keys::f5);
		break;
	case GLFW_KEY_F6:
		setPressed(Keys::f6);
		break;
	case GLFW_KEY_F7:
		setPressed(Keys::f7);
		break;
	case GLFW_KEY_F8:
		setPressed(Keys::f8);
		break;
	case GLFW_KEY_F9:
		setPressed(Keys::f9);
		break;
	case GLFW_KEY_F10:
		setPressed(Keys::f10);
		break;
	case GLFW_KEY_F11:
		setPressed(Keys::f11);
		break;
	case GLFW_KEY_F12:
		setPressed(Keys::f12);
		break;

	case GLFW_KEY_HOME:
		setPressed(Keys::home);
		break;
	case GLFW_KEY_INSERT:
		setPressed(Keys::insert);
		break;
	case GLFW_KEY_DELETE:
		setPressed(Keys::del);
		break;
	case GLFW_KEY_END:
		setPressed(Keys::end);
		break;
	case GLFW_KEY_PAGE_DOWN:
		setPressed(Keys::pageDown);
		break;
	case GLFW_KEY_PAGE_UP:
		setPressed(Keys::pageUp);
		break;

	default:; // printf("Unrecognized key pressed\n");
	}
}

void InputManager::keyUp(int keyReleased)
{
	// LOG_DEBUG("keyup: {}", keyReleased);

	switch (keyReleased)
	{
		/// \todo Will this be used?
		// alphabet a-z
	case GLFW_KEY_A:
		setUnpressed(Keys::a);
		break;
	case GLFW_KEY_B:
		setUnpressed(Keys::b);
		break;
	case GLFW_KEY_C:
		setUnpressed(Keys::c);
		break;
	case GLFW_KEY_D:
		setUnpressed(Keys::d);
		break;
	case GLFW_KEY_E:
		setUnpressed(Keys::e);
		break;
	case GLFW_KEY_F:
		setUnpressed(Keys::f);
		break;
	case GLFW_KEY_G:
		setUnpressed(Keys::g);
		break;
	case GLFW_KEY_H:
		setUnpressed(Keys::h);
		break;
	case GLFW_KEY_I:
		setUnpressed(Keys::i);
		break;
	case GLFW_KEY_J:
		setUnpressed(Keys::j);
		break;
	case GLFW_KEY_K:
		setUnpressed(Keys::k);
		break;
	case GLFW_KEY_L:
		setUnpressed(Keys::l);
		break;
	case GLFW_KEY_M:
		setUnpressed(Keys::m);
		break;
	case GLFW_KEY_N:
		setUnpressed(Keys::n);
		break;
	case GLFW_KEY_O:
		setUnpressed(Keys::o);
		break;
	case GLFW_KEY_P:
		setUnpressed(Keys::p);
		break;
	case GLFW_KEY_Q:
		setUnpressed(Keys::q);
		break;
	case GLFW_KEY_R:
		setUnpressed(Keys::r);
		break;
	case GLFW_KEY_S:
		setUnpressed(Keys::s);
		break;
	case GLFW_KEY_T:
		setUnpressed(Keys::t);
		break;
	case GLFW_KEY_U:
		setUnpressed(Keys::u);
		break;
	case GLFW_KEY_V:
		setUnpressed(Keys::v);
		break;
	case GLFW_KEY_W:
		setUnpressed(Keys::w);
		break;
	case GLFW_KEY_X:
		setUnpressed(Keys::x);
		break;
	case GLFW_KEY_Y:
		setUnpressed(Keys::y);
		break;
	case GLFW_KEY_Z:
		setUnpressed(Keys::z);
		break;
		/// -------------------------

		// numbers
	case GLFW_KEY_KP_0:
		setUnpressed(Keys::n0);
		break;
	case GLFW_KEY_KP_1:
		setUnpressed(Keys::n1);
		break;
	case GLFW_KEY_KP_2:
		setUnpressed(Keys::n2);
		break;
	case GLFW_KEY_KP_3:
		setUnpressed(Keys::n3);
		break;
	case GLFW_KEY_KP_4:
		setUnpressed(Keys::n4);
		break;
	case GLFW_KEY_KP_5:
		setUnpressed(Keys::n5);
		break;
	case GLFW_KEY_KP_6:
		setUnpressed(Keys::n6);
		break;
	case GLFW_KEY_KP_7:
		setUnpressed(Keys::n7);
		break;
	case GLFW_KEY_KP_8:
		setUnpressed(Keys::n8);
		break;
	case GLFW_KEY_KP_9:
		setUnpressed(Keys::n9);
		break;

		// specials
	case GLFW_KEY_ESCAPE:
		setUnpressed(Keys::esc);
		break;

	case GLFW_KEY_LEFT_ALT:
		setUnpressed(Keys::altl);
		break;
	case GLFW_KEY_RIGHT_ALT:
		setUnpressed(Keys::altr);
		break;
	case GLFW_KEY_LEFT_CONTROL:
		setUnpressed(Keys::ctrll);
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		setUnpressed(Keys::ctrlr);
		break;
	case GLFW_KEY_LEFT_SHIFT:
		setUnpressed(Keys::shiftl);
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		setUnpressed(Keys::shiftr);
		break;

	case GLFW_KEY_UP:
		setUnpressed(Keys::up);
		break;
	case GLFW_KEY_DOWN:
		setUnpressed(Keys::down);
		break;
	case GLFW_KEY_LEFT:
		setUnpressed(Keys::left);
		break;
	case GLFW_KEY_RIGHT:
		setUnpressed(Keys::right);
		break;

	case GLFW_KEY_F1:
		setUnpressed(Keys::f1);
		break;
	case GLFW_KEY_F2:
		setUnpressed(Keys::f2);
		break;
	case GLFW_KEY_F3:
		setUnpressed(Keys::f3);
		break;
	case GLFW_KEY_F4:
		setUnpressed(Keys::f4);
		break;
	case GLFW_KEY_F5:
		setUnpressed(Keys::f5);
		break;
	case GLFW_KEY_F6:
		setUnpressed(Keys::f6);
		break;
	case GLFW_KEY_F7:
		setUnpressed(Keys::f7);
		break;
	case GLFW_KEY_F8:
		setUnpressed(Keys::f8);
		break;
	case GLFW_KEY_F9:
		setUnpressed(Keys::f9);
		break;
	case GLFW_KEY_F10:
		setUnpressed(Keys::f10);
		break;
	case GLFW_KEY_F11:
		setUnpressed(Keys::f11);
		break;
	case GLFW_KEY_F12:
		setUnpressed(Keys::f12);
		break;

	case GLFW_KEY_HOME:
		setUnpressed(Keys::home);
		break;
	case GLFW_KEY_INSERT:
		setUnpressed(Keys::insert);
		break;
	case GLFW_KEY_DELETE:
		setUnpressed(Keys::del);
		break;
	case GLFW_KEY_END:
		setUnpressed(Keys::end);
		break;
	case GLFW_KEY_PAGE_DOWN:
		setUnpressed(Keys::pageDown);
		break;
	case GLFW_KEY_PAGE_UP:
		setUnpressed(Keys::pageUp);
		break;

	default:
		LOG_ERROR("Unrecognized key pressed!");
	}
}

GLFWwindow* InputManager::getCurrentViewport()
{
	ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
	ImGuiContext* g = ImGui::GetCurrentContext(); // Get current/last moused viewport.

	GLFWwindow* window = nullptr;
	for (int n = 0; n < platformIO.Viewports.Size; n++)
	{
		if (platformIO.Viewports[n]->ID == g->MouseViewport->ID)
		{
			window = (GLFWwindow*)platformIO.Viewports[n]->PlatformHandle;
		}
	}

	return window;
}

//===-- Statics -----------------------------------------------------------===//
MouseButtonState InputManager::m_mouseButtonState;

std::map<Keys::Code, InputManager::KeyState> InputManager::m_keyMap;

std::vector<InputController*> InputManager::m_inputControllers;
Ptr<IWindow> InputManager::m_focusedWindow;
Ptr<IWindow> InputManager::m_hoveredWindow;

bool InputManager::m_ignoreImGuiEvents = false;
glm::vec2 InputManager::m_mouseOffset;
float InputManager::m_mouseX = 0;
float InputManager::m_mouseY = 0;
float InputManager::m_mouseXPrev = 0;
float InputManager::m_mouseYPrev = 0;

float InputManager::m_mouseXDelta = 0;
float InputManager::m_mouseYDelta = 0;

int InputManager::m_winWidth = 0;
int InputManager::m_winHeight = 0;

