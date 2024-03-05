/**
 * \file
 * \brief Base class for all commands that can application process.
 * \author Martin Herich <martin.herich@phire.cz>
 * \date 24.10.2020
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <functional>

#include "Core/Application.h"
#include "ICommand.h"

/**
 * Base class for all commands.
 *
 * Any derived command can be accessed by calling
 * DerivedCommandName::addListener(fptr); For more information see <a
 * href="https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern">
 * Curiously recurring template pattern (static m_listeners).</a>
 *
 * \tparam Type command derived class (CloseCommand for example)-
 * \tparam Args command argument for callback call.
 */
template <typename Type, typename... Args> class Command : public ICommand
{
public:
	typedef std::function<void(Args&...)> Callback;

	Command() = default;
	virtual ~Command() = default;

	/**
	 * Add a listener for a command.
	 *
	 * <b>NEVER</b> use the function from constructor of statically instantiated
	 * object.
	 *
	 * \param function callback to call when the command is issued. Use std::bind
	 * for member function of instantiated class or raw function pointer for
	 * static member function or plain function.
	 *
	 * \todo Check how std::bind handle `this` reference.
	 */
	static void addListener(Callback function)
	{
		s_listeners.push_back(function);
	};

	/**
	 * Call all callbacks.
	 *
	 * Expand m_params tuple as function arguments, <a
	 * href="https://stackoverflow.com/a/37100646">link</a>.
	 */
	void execute() override
	{
		for (Callback callback : s_listeners)
		{
			std::apply(
			    [callback](auto... args) {
				    callback(args...);
			    },
			    m_args);
		}
	};

	/**
	 * Tell application that command was issued.
	 *
	 * Pass a copy of command and its arguments to the Application.
	 */
	static void dispatch(Args... args)
	{
		std::tuple<Args...> m_args(args...);

		App::get().enqueueCommand(new Command<Type, Args...>(m_args));
	}

	static void dispatchImmediate(Args... args)
	{
		std::tuple<Args...> m_args(args...);

		Command<Type, Args...> cmd(m_args);
		cmd.execute();
	}

protected:
	/**
	 * Copy a command and its arguments.
	 *
	 * \param args command arguments.
	 */
	explicit Command(const std::tuple<Args...>& args)
	{
		m_args = args;
	}

private:
	static std::vector<Callback> s_listeners;

	std::tuple<Args...> m_args;
};

template <typename Type, typename... Args>
std::vector<std::function<void(Args&...)>> Command<Type, Args...>::s_listeners;
