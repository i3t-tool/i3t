/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <vector>

class ICommand;

class CommandDispatcher
{
public:
	static CommandDispatcher& get();

	void execute();

	/// CommandDispatcher takes ownership of the \p command.
	void enqueueCommand(ICommand* command);

private:
	std::vector<ICommand*> m_commandsToExecute;
	std::vector<ICommand*> m_commandsQueue;
};
