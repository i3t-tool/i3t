/**
 * \file Commands/ApplicationCommands.h
 * \author Martin Herich
 * \date 23.10.2020
 *
 * All application control commands.
 */
#pragma once

#include "Command.h"

#include "Core/Defs.h"
#include "GUI/Elements/IWindow.h"

struct Tutorial;

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

class SetFocusedWindowCommand : public Command<SetFocusedWindowCommand, Ptr<IWindow>>
{
};

class SetTutorialCommand : public Command<SetTutorialCommand, std::shared_ptr<Tutorial>>
{
};

