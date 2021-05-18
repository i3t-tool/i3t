/**
 * \file Commands/ApplicationCommands.h
 * \author Martin Herich
 * \date 23.10.2020
 *
 * All application control commands.
 */
#pragma once

#include "Command.h"
#include "Tutorial/Tutorial.h"

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

class SetTutorialCommand : public Command<SetTutorialCommand, std::shared_ptr<Tutorial>>
{
};

