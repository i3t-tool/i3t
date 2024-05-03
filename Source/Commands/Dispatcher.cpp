#include "Commands/Dispatcher.h"

#include "Commands/ICommand.h"

CommandDispatcher& CommandDispatcher::get()
{
	static CommandDispatcher instance;

	return instance;
}

void CommandDispatcher::execute()
{
	for (ICommand* command : m_commandsToExecute)
	{
		command->execute();
		delete command;
	}
	m_commandsToExecute.clear();

	m_commandsToExecute.swap(m_commandsQueue);
}

void CommandDispatcher::enqueueCommand(ICommand* command)
{
	m_commandsQueue.push_back(command);
}
