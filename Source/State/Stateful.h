/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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
