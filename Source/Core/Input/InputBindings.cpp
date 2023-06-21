#include "InputBindings.h"

#include "InputManager.h"

std::vector<InputBindings::ActionMapping> g_defaultAction;
std::vector<InputBindings::AxisMapping> g_defaultAxis;

InputBindings::ActionsMap InputBindings::m_inputActions;
InputBindings::AxesMap InputBindings::m_inputAxis;

void InputBindings::init()
{
	// Default input bindings.
	InputManager::setInputAction("copy", Keys::c, {Keys::ctrll});
	InputManager::setInputAction("paste", Keys::v, {Keys::ctrll});
	InputManager::setInputAction("cut", Keys::x, {Keys::ctrll});
	InputManager::setInputAction("duplicateSelected", Keys::d, {Keys::ctrll});
	InputManager::setInputAction("duplicate", Keys::mouseLeft, {Keys::ctrll});
	InputManager::setInputAction("delete", Keys::del);
	// InputManager::setInputAction("delete", Keys::backspace);

	InputManager::setInputAction("undo", Keys::b, {Keys::ctrll});
	InputManager::setInputAction("undo", Keys::z, {Keys::ctrll});
	InputManager::setInputAction("redo", Keys::n, {Keys::ctrll});
	InputManager::setInputAction("redo", Keys::y, {Keys::ctrll});
	InputManager::setInputAction("save", Keys::s, {Keys::ctrll});
	InputManager::setInputAction("saveAs", Keys::s, {Keys::ctrll, Keys::shiftl});

	InputManager::setInputAction("scrollUp", Keys::mouseScrlUp);
	InputManager::setInputAction("scrollDown", Keys::mouseScrlDown);

	InputManager::setInputAxis("scroll", 1.0f, Keys::mouseScrlUp);
	InputManager::setInputAxis("scroll", -1.0f, Keys::mouseScrlDown);
	InputManager::setInputAxis("pan", 1.0f, Keys::mouseMiddle);
	InputManager::setInputAxis("rotate", 1.0f, Keys::mouseRight);
	InputManager::setInputAxis("rotateAround", 1.0f, Keys::mouseRight, {Keys::altl});

	InputManager::setInputAxis("moveForward", 1.0f, Keys::w);
	InputManager::setInputAxis("moveForward", -1.0f, Keys::s);
	InputManager::setInputAxis("moveRight", 1.0f, Keys::d);
	InputManager::setInputAxis("moveRight", -1.0f, Keys::a);

#ifdef I3T_DEBUG
	InputManager::setInputAction("test", Keys::t);                                       /// \todo Will be removed
	InputManager::setInputAction("TestMouseCtrlAction", Keys::mouseLeft, {Keys::ctrll}); /// \todo Will be removed
	// InputManager::setInputAction("MyTestAction", Keys::t, {Keys::ctrll, Keys::altl});    /// \todo Will be removed
	InputManager::setInputAxis("MyTestAxis", 1.0f, Keys::p, {Keys::ctrll}); /// \todo Will be removed
	setAxisKey("MyTestAxis", -1.0f, Keys::mouseRight, {Keys::ctrll});       /// \todo Will be removed
#endif

	/*
	InputManager::setInputAxis("MouseScroll", 1.0f, Keys::mouseScrlUp);
	setAxisKey("MouseScroll", -1.0f, Keys::mouseScrlDown);
	 */

	// Workspace window actions.
	InputManager::setInputAction("selectAll", Keys::a, {Keys::ctrll});
	InputManager::setInputAction("invertSelection", Keys::i, {Keys::ctrll});
	// InputManager::setInputAction("center", Keys::d); /* \todo JH what is
	// center? */

	/* tracking */
	InputManager::setInputAxis("trackingSmoothLeft", 1.0f, Keys::left);
	InputManager::setInputAxis("trackingSmoothRight", 1.0f, Keys::right);
	InputManager::setInputAction("trackingJaggedLeft", Keys::left);
	InputManager::setInputAction("trackingJaggedRight", Keys::right);
	InputManager::setInputAction("trackingEscOff", Keys::esc);
	// InputManager::setInputAction("trackingSwitch", Keys::t, {Keys::ctrll});
	// InputManager::setInputAction("trackingModeSwitch", Keys::p);
	// InputManager::setInputAction("trackingSwitchOn", Keys::p);
	// InputManager::setInputAction("trackingSwitchOff", Keys::o); /* \todo JH maybe some different key when more than 2
	//  																																tracking
	//  direction? */


	InputManager::setInputAxis("selectionRectangle", 1.0f, Keys::mouseLeft);

	InputManager::setInputAction("unplugInputPin", Keys::mouseLeft);
	InputManager::setInputAction("hold", Keys::mouseLeft);
	InputManager::setInputAxis("drag", 1.0f, Keys::mouseLeft);
	InputManager::setInputAction("zoomToAll", Keys::a, {Keys::shiftl});
	InputManager::setInputAction("zoomToSelected", Keys::x, {Keys::shiftl});

	// InputManager::setInputAction("unselectAll", Keys::mouseLeft);
	InputManager::setInputAction("unselectAll", Keys::esc);
	InputManager::setInputAxis("NOTunselectAll", 1.0f, Keys::ctrll);
	InputManager::setInputAxis("NOTunselectAll", 1.0f, Keys::shiftl);
	InputManager::setInputAxis("DONTselect", 1.0f, Keys::ctrll);
	InputManager::setInputAction("select", Keys::mouseLeft);

	InputManager::setInputAction("createAndPlugConstructor", Keys::mouseLeft, {Keys::ctrll});
	InputManager::setInputAction("unplugInput", Keys::mouseLeft);

	InputManager::setInputAction("touch", Keys::mouseLeft);

	InputManager::setInputAction("raisePopup", Keys::mouseRight);

	InputManager::setInputAction("viewport_select", Keys::mouseLeft);

	InputManager::setInputAction("toggleNodeWorkspaceVisibility", Keys::i, {Keys::ctrlr});
}

const std::vector<InputBindings::ActionMapping>& InputBindings::getActionMapping(const char* name)
{
	if (isActionCreated(name))
	{
		return m_inputActions[name];
	}
	return g_defaultAction;
}

bool InputBindings::isActionCreated(const char* name)
{
	return m_inputActions.contains(name);
}

void InputBindings::setAction(const char* name)
{
	m_inputActions[name];
}

void InputBindings::setActionKey(const char* name, Keys::Code code, ModifiersList modifiers)
{
	/// \todo MH Check for conflicts.
	if (isActionCreated(name))
	{
		m_inputActions[name].push_back({code, createModifiers(modifiers)});
	}
}

void InputBindings::removeActionKey(const char* name, Keys::Code code)
{
	if (isActionCreated(name))
	{
		std::erase_if(m_inputActions[name], [&code](ActionMapping action) {
			return code == action.code;
		});
	}
}

std::vector<InputBindings::AxisMapping> InputBindings::getAxisMapping(const char* name)
{
	if (isAxisCreated(name))
	{
		return m_inputAxis[name];
	}
	return g_defaultAxis;
}

bool InputBindings::isAxisCreated(const char* name)
{
	return m_inputAxis.contains(name);
}

void InputBindings::setAxis(const char* name)
{
	m_inputAxis[name];
}

void InputBindings::setAxisKey(const char* name, float scale, Keys::Code code, ModifiersList modifiers)
{
	if (InputBindings::isAxisCreated(name))
	{
		m_inputAxis[name].push_back({code, scale, createModifiers(modifiers)});
	}
}

void InputBindings::removeAxisKey(const char* name, Keys::Code code)
{
	if (isAxisCreated(name))
	{
		std::erase_if(m_inputAxis[name], [&code](AxisMapping axis) {
			return axis.code == code;
		});
	}
}
