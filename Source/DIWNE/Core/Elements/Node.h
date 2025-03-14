/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "DiwneObject.h"

namespace DIWNE
{
class NodeEditor;

using NodeFlag = uint64_t;

/**
 * TODO: Docs
 */
class Node : public DiwneObject
{
protected:
	NodeFlag m_flag{0}; ///< The node flag bit field @see Node::getFlag()

public:
	Node(NodeEditor& diwne, std::string labelDiwne = "DiwneNode");

	~Node() override;

	// TODO: Why was copy constuctor commented out? Is it ever used?
	//        /** Copy constructor
	//         *  \param other Object to copy from
	//         */
	//        Node(const Node& other);

	// TODO: Does this operator make sense? <<<<<<<<<<<<<<<<<<<<<<<<
	/** Assignment operator
	 *  \param other Object to assign from
	 *  \return A reference to this
	 */
	Node& operator=(const Node& other);

	// Lifecycle
	// =============================================================================================================
	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;
	void updateLayout(DrawInfo& context) override;

protected:
	void afterDrawDiwne(DrawInfo& context) override;

public:
	bool allowDrawing() override;


	// Interaction
	// =============================================================================================================
	void onSelection(bool selected) override;
	void onDrag(DrawInfo& context, bool dragStart, bool dragEnd) override;
	void onDestroy(bool logEvent) override;

protected:
	bool processSelectDiwne(DrawInfo& context) override;

public:
	// Getters and setters
	// =============================================================================================================
	/**
	 * Get the node bit flag value at index.
	 *
	 * The node flag is a simple 64 bit integer used as a bit field to hold 64 user defined boolean flags.
	 * As of right now DIWNE does not use any of the flags by itself, so their meaning and usage is entirely arbitrary.
	 * The intended purpose is to use flags to identify particular node subclasses and avoid any dynamic_casts when
	 * casting to a derived type for specific functionality.
	 * @see setFlag()
	 */
	bool getFlag(char index) const;
	void setFlag(char index, bool value); ///< Set the node bit flag value at index @see getFlag()

	// =============================================================================================================

	virtual void drawSelectionIndicator(DrawInfo& context);

	bool willBeRemovedFromContainer(const DiwneObject* container);
};

} /* namespace DIWNE */
