#pragma once

#include "Memento.h"

class IStateful
{
public:
	virtual Memento getState()               = 0;
	virtual void    setState(const Memento&) = 0;

	virtual void onStateChange(const std::string& winTitlePostfix) = 0;
};
