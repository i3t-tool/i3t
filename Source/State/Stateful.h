#pragma once

#include "rapidjson/document.h"

/// @note Nodes are referenced by their index.
/// ```json
/// {
///   "workspace": {
///     "operators": { ... },
///     "edges": [
///       [leftNodeID, pinIndex, righNodeID, pinIndex]
///     ]
///   }
/// }
/// ```
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

	virtual void clear() {}
};
