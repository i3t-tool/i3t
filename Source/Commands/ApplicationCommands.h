/**
 * \file Commands/ApplicationCommands.h
 * \author Martin Herich
 * \date 23.10.2020
 *
 * All application control commands.
 */
#pragma once

#include "Command.h"

class BeforeCloseCommand : public Command<BeforeCloseCommand>
{
};

class CloseCommand : public Command<CloseCommand>
{
};

class HideWindowCommand : public Command<HideWindowCommand, std::string>
{
};

class ConsoleCommand : public Command<ConsoleCommand, std::string>
{
};
