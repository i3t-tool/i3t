#pragma once

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"

#include "Scene.h"

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

using namespace State;

class IStateful
{
public:
	virtual ~IStateful(){};

	/// Get state of a stateful object, the result will be merged
	/// into one global memento.

	/**
	 * Get state of a stateful object, the result will be merged into one global memento.
	 */
	virtual Memento saveState(State::Scene* scene) = 0;

	/**
	 * Called on scene load, undo and redo.
	 * @param memento New global state
	 */
	virtual void loadState(const Memento& memento, State::Scene* scene) = 0;

	virtual void clearState() = 0;
};
