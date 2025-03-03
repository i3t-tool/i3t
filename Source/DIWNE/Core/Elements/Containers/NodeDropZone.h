#pragma once

#include "DIWNE/Core/Elements/Containers/NodeContainer.h"
#include "DIWNE/Core/Elements/DiwneObject.h"

namespace DIWNE
{
class NodeDropZone : public DiwneObject, public NodeContainer
{
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

	void addNodeAt(const std::shared_ptr<Node>& node, int index = 0) override;
	bool removeNodeAt(int index) override;

	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;
	void updateLayout(DrawInfo& context) override;

	void processInteractions(DrawInfo& context) override;

	void afterDraw(DrawInfo& context) override;

	bool allowDragStart() const override;

protected:
	bool tryAddNode(DrawInfo& context);
	bool tryRemoveNode(DrawInfo& context);

	/// Draws the drop indicator for the passed node about to be dropped at specified index.
	virtual void drawDropIndicator(Node* newNode, int index);

	/// Returns at which index a the new node should be added, -1 for invalid position.
	virtual int isNodeAboveDropZone(Node* newNode);

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
