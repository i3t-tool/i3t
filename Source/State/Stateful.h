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

	/**
	 * Save state for the current scene.
	 * The scene parameter should contain a scene object for full filesystem saves
	 * and nullptr for quick undo/redo operations.
	 * \param scene The current scene or nullptr (in case of undo/redo)
	 */
	virtual Memento saveScene(State::Scene* scene) = 0;

	/**
	 * Called on scene load, undo and redo.
	 * \param scene The current scene or nullptr (in case of undo/redo)
	 * @param memento New scene state
	 */
	virtual void loadScene(const Memento& memento, State::Scene* scene) = 0;

	/**
	 * Set the scene state to some default empty state.
	 */
	virtual void clearScene() = 0;

	/**
	 * Save shared global state.
	 */
	virtual Memento saveGlobal() = 0;

	/**
	 * Load global state from a memento.
	 * @param memento New global state
	 */
	virtual void loadGlobal(const Memento& memento) = 0;

	/**
	 * Set the global state to some default empty state.
	 */
	virtual void clearGlobal() = 0;

	/**
	 * Create an empty memento (rapidjson::Document)
	 */
	Memento emptyMemento()
	{
		rapidjson::Document doc;
		doc.SetObject();
		return doc;
	}
};
