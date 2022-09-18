#pragma once

#include "Core/Application.h"

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void execute() = 0;
};
