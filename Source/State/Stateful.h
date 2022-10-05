#pragma once

#include "rapidjson/document.h"

using Memento = rapidjson::Document;

class IStateful
{
public:
	virtual Memento getState()
	{
		auto memento = Memento{};
		memento.SetObject();

		return memento;
	};

	virtual void setState(const Memento&) {}

	virtual void onStateChange() {}
};
