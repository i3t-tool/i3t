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
	/// Get state of a stateful object, the result will be merged
	/// into one global memento.
	virtual Memento getState()
	{
		auto memento = Memento{};
		memento.SetObject();

		return memento;
	};

	/// Called on scene load, undo and redo.
	/// @param memento New global state.
	virtual void setState(const Memento& memento) {}

	virtual void clear() {}
};
