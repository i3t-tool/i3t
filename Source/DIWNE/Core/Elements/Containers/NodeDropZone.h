/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "DIWNE/Core/Elements/Containers/NodeContainer.h"
#include "DIWNE/Core/Elements/DiwneObject.h"

namespace DIWNE
{
class NodeDropZone : public DiwneObject, public NodeContainer
{
	using Super = DiwneObject;

protected:
	int m_dropIndex = -1; ///< Insertion index of a dragged node that might get dropped in (invalid if negative)

public:
	NodeDropZone(DIWNE::NodeEditor& diwne, DiwneObject* parent);

	/**
	 * Decides whether the passed node should be allowed to be added to this drop zone.
	 */
	virtual bool acceptNode(Node* node)
	{
		return true;
	}

	void onNodeAdd(Node* node, int index) override;
	// TODO: Docs, this moves the node back to the editor
	void onNodeRemove(std::shared_ptr<Node> node, int index) override;

	void initialize(DrawInfo& context) override;
	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;
	void updateLayout(DrawInfo& context) override;

	void processInteractions(DrawInfo& context) override;

	void afterDraw(DrawInfo& context) override;

	void onDestroy(bool logEvent) override;

	bool allowDragStart() const override;
	bool allowHover() const override;

protected:
	bool tryAddNode(DrawInfo& context);
	bool tryRemoveNode(DrawInfo& context);

	/// Draws the drop indicator for the passed node about to be dropped at specified index.
	virtual void drawDropIndicator(Node* newNode, int index);

	/// Returns at which index a the new node should be added, -1 for invalid position.
	virtual int isNodeAboveDropZone(Node* newNode);

	// TODO: Maybe different name, isAcceptable?
	/**
	 * Decide whether the node should be placed in the drop zone if at all.
	 * Calls the user defined acceptNode() function.
	 * @return The index at which the new node should be added, -1 for failure.
	 */
	virtual int acceptNodeDiwne(Node* node);

	/// Content drawn where there are no nodes in the drop zone
	virtual void drawEmptyContent(DrawInfo& context);
};

} // namespace DIWNE
