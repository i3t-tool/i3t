/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Command.h"

#include "Core/Defs.h"
#include "GUI/Elements/IWindow.h"
#include "Viewport/entity/Entity.h"

struct Tutorial;

class NewProjectCommand : public Command<NewProjectCommand>
{};

class BeforeCloseCommand : public Command<BeforeCloseCommand>
{};

/// int - exit code
class CloseCommand : public Command<CloseCommand, int>
{};

class HideWindowCommand : public Command<HideWindowCommand, std::string>
{};

class ConsoleCommand : public Command<ConsoleCommand, std::string>
{};

class SetFocusedWindowCommand : public Command<SetFocusedWindowCommand, Ptr<IWindow>>
{};

class SetTutorialCommand : public Command<SetTutorialCommand, std::shared_ptr<Tutorial>>
{};

class SetTutorialStepCommand : public Command<SetTutorialStepCommand, int>
{};

class LoadWindowLayoutFromFileCommand : public Command<LoadWindowLayoutFromFileCommand, std::string>
{};

class LoadWindowLayoutFromStringCommand : public Command<LoadWindowLayoutFromStringCommand, std::string>
{};