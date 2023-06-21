#include "InputManager.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Core/Application.h"
#include "Core/Window.h"
#include "InputBindings.h"
#include "Logger/Logger.h"

#define IM_MOUSE_KEYS_COUNT 3

// TODO: (DR) This class is still somewhat confusing.
//  Yes it handles stuff like InputControllers, useful for custom keybinds later.
//  However it also handles key presses and mouse. But in an odd way, some stuff like mouse button and key presses are
//  updated directly from glfw callbacks in the glfw backend but then other stuff (mouse position) is updated from ImGui
//  That's a little odd. It works as a nice facade for the ImGui IO, which is fine, but maybe we should take all info
//  directly from ImGui and avoid customizing the ImGui backend to directly intercept glfw callbacks.

constexpr Keys::Code imGuiMouseKeys[] = {Keys::mouseLeft, Keys::mouseRight, Keys::mouseMiddle};

ImGuiConfigFlags g_mousedFlags;

InputController InputManager::s_globalInputController;
std::vector<InputController*> InputManager::m_inputControllers;
InputController* InputManager::s_activeInput = nullptr;

std::map<Keys::Code, InputManager::KeyState> InputManager::m_keyMap;

// TODO: (DR) Never accessed, unused?
bool InputManager::m_ignoreImGuiEvents = false;

float InputManager::m_mouseX = 0;
float InputManager::m_mouseY = 0;
float InputManager::m_mouseXPrev = 0;
float InputManager::m_mouseYPrev = 0;
float InputManager::m_mouseXDelta = 0;
float InputManager::m_mouseYDelta = 0;
float InputManager::m_mouseXDragDelta = 0;
float InputManager::m_mouseYDragDelta = 0;
float InputManager::m_mouseWheelOffset = 0;

void InputManager::init()
{
	InputBindings::init();
}

void InputManager::bindGlobalAction(const char* action, EKeyState state, KeyCallback fn)
{
	s_globalInputController.bindAction(action, state, fn);
}

void InputManager::triggerAction(const char* action, EKeyState state)
{
	s_globalInputController.triggerAction(action, state);

	if (s_activeInput)
		s_activeInput->triggerAction(action, state);
}

void InputManager::setInputAction(const char* name, Keys::Code code, ModifiersList mods)
{
	if (!InputBindings::isActionCreated(name))
	{
		InputBindings::m_inputActions.insert({name, {}});
	}
	InputBindings::m_inputActions[name].push_back({code, createModifiers(mods)});
}

void InputManager::setInputAxis(const char* action, float scale, Keys::Code code, ModifiersList mods)
{
	if (!InputBindings::isAxisCreated(action))
	{
		InputBindings::m_inputAxis.insert({action, {}});
	}
	InputBindings::m_inputAxis[action].push_back({code, scale, createModifiers(mods)});
}

bool InputManager::areModifiersActive(Modifiers mods)
{
	bool active = mods[0] == isKeyPressed(Keys::Code::ctrll);
	active &= mods[1] == isKeyPressed(Keys::Code::altl);
	active &= mods[2] == isKeyPressed(Keys::Code::shiftl);

	return active;
}

void InputManager::setActiveInput(InputController* input)
{
	I3T_ASSERT(input != nullptr, "Input parameter is null");
	s_activeInput = input;
}

bool InputManager::isInputActive(InputController* input)
{
	I3T_ASSERT(input != nullptr, "Input parameter is null");
	return s_activeInput == input;
}

bool InputManager::isActionTriggered(const char* name, EKeyState state)
{
	if (!InputBindings::m_inputActions.contains(name))
		return false;

	auto& keys = InputBindings::m_inputActions[name];

	bool result = false;
	if (state == EKeyState::Released)
	{
		for (auto action : keys)
		{
			result |= isKeyJustUp(action.code) && areModifiersActive(action.modifiers);
		}
	}
	if (state == EKeyState::Pressed)
	{
		for (auto action : keys)
		{
			result |= isKeyJustPressed(action.code) && areModifiersActive(action.modifiers);
		}
	}

	return result;
}

bool InputManager::isAxisActive(const char* name)
{
	if (!InputBindings::m_inputAxis.contains(name))
		return false;

	auto& axes = InputBindings::m_inputAxis[name];
	for (const auto& a : axes)
	{
		if (isKeyPressed(a.code))
		{
			return true;
		}
	}
	return false;
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

// TODO: (DR) Unused, different mouse drag handling strategy was used, might be handy in the future
void InputManager::beginCameraControl()
{
	// Disable system cursor. The cursor will be hidden and at the
	// endCameraControl the cursor will be at the same position.
	glfwSetInputMode(getCurrentViewport(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	m_ignoreImGuiEvents = true;

	// Tell ImGui to don't capture mouse position. No widgets will be set to the
	// hovered state.
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

void InputManager::processEvents(InputController& controller)
{
	for (const auto& [action, state, callback] : controller.m_actions)
	{
		auto& keys = InputBindings::m_inputActions[action];
		for (const auto& [key, mods] : keys)
		{
			// Check if key is in desired state.
			bool shouldProcess = (m_keyMap[key] == KeyState::JUST_DOWN && state == EKeyState::Pressed ||
			                      m_keyMap[key] == KeyState::JUST_UP && state == EKeyState::Released) &&
			                     areModifiersActive(mods);

			if (shouldProcess)
				callback();
		}
	}

	for (const auto& [action, callback] : controller.m_axis)
	{
		auto keys = InputBindings::m_inputAxis[action];
		for (const auto& [key, scale, mods] : keys)
		{
			bool shouldProcess =
			    (m_keyMap[key] == KeyState::DOWN || m_keyMap[key] == KeyState::JUST_DOWN) && areModifiersActive(mods);

			if (shouldProcess)
			{
				callback(scale);
			}
		}
	}
}

void InputManager::beginFrame()
{
	ImGuiIO& io = ImGui::GetIO();

	// Update mouse position
	if (ImGui::IsMousePosValid())
	{
		m_mouseX = io.MousePos.x;
		m_mouseY = io.MousePos.y;
	}

	m_mouseXDelta = io.MouseDelta.x;
	m_mouseYDelta = io.MouseDelta.y;

	m_mouseXPrev = io.MousePosPrev.x;
	m_mouseYPrev = io.MousePosPrev.y;

	// TODO: (DR) drag delta is unused, haven't tested if it works properly after changes, should be
	/* \todo JH \todo MH probably very naive */
	if (isMouseDown())
	{
		m_mouseXDragDelta += m_mouseXDelta;
		m_mouseYDragDelta += m_mouseYDelta;
	}
	else
	{
		m_mouseXDragDelta = m_mouseYDragDelta = 0;
	}

	LOG_EVENT_MOUSE_POS(std::to_string(m_mouseX), std::to_string(m_mouseY));

	// Update left, right and middle button.
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
}

void InputManager::endFrame()
{
	// TODO: (DR) REWRITE NEEDED
	//  The InputManager should be taking input FROM IMGUI ONLY
	//  We should not be changing the glfw backend whatsoever, its not necessary
	//  The code updating the InputManager values with ImGui should run at the BEGINNING of a frame, however
	//  currently its written to partially update at the beginning in glfw backend and then again at the end with this
	//  code. A relic of the old codebase.
	//  Also the logging of events needs to be changed and tested.
	//  I've added beginFrame and endFrame methods (similar to Module) that get called accordingly and started moving
	//  stuff from this method to beginFrame, starting with mouse button updates which were broken.
	//  I also moved the mouse position update, hoping that doesn't break something.
	//  Note that this fixed an issue with actions that caused InputManager::isActionTriggered(pressed) to always return
	//  false if the action was bound to one of the mouse buttons.

	// Process events (Note: Dispatches callbacks)
	processEvents(s_globalInputController);
	if (s_activeInput)
		processEvents(*s_activeInput);

	// Process keys
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

	/// \todo MH mouse scroll release.
	m_keyMap[Keys::Code::mouseScrlUp] = KeyState::UP;
	m_keyMap[Keys::Code::mouseScrlDown] = KeyState::UP;
	m_mouseWheelOffset = 0;
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
			window = (GLFWwindow*) platformIO.Viewports[n]->PlatformHandle;
		}
	}

	return window;
}
