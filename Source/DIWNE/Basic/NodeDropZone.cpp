#include "NodeDropZone.h"

#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/diwne_utils.h"
#include "Utils/Color.h"

namespace DIWNE
{
NodeDropZone::NodeDropZone(NodeEditor& diwne, DiwneObject* parent) : DiwneObject(diwne, "DropZone")
{
	setParentObject(parent);
	setSelectable(false);
	setFixed(true);
}

NodeRange<> NodeDropZone::getNodes() const
{
	return NodeRange<>(&m_nodes);
}

void NodeDropZone::begin(DrawInfo& context)
{
	diwne.canvas().AddRectFilledDiwne(m_rect.Min, m_rect.Max, diwne.style().dropZoneBackground);
	ImGui::PushID(m_labelDiwne.c_str());
	ImGui::BeginGroup();
}

void NodeDropZone::content(DrawInfo& context)
{
	float zoom = diwne.getZoom();
	if (m_nodes.empty())
	{
		const ImVec2 defaultSize = ImVec2(130 * zoom, 70 * zoom);
		ImVec2 origin = ImGui::GetCursorScreenPos();
		const char* emptyLabel = "Drag and drop\n nodes here";
		ImGui::SetCursorScreenPos(origin + (defaultSize / 2.0f) - (ImGui::CalcTextSize(emptyLabel) / 2.0f) -
		                          ImVec2(0, ImGui::GetStyle().ItemSpacing.y / 2.0f));
		ImGui::TextDisabled("%s", emptyLabel);
		ImGui::SetCursorScreenPos(origin);
		ImGui::Dummy(defaultSize);
	}
	else
	{
		DGui::SameLineDummy(ImVec2(diwne.style().dropZoneMarginWidth * zoom, 0.0f));
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			if (i == m_dropIndex)
				DGui::SameLineDummy(diwne.style().dropZoneDropGap * zoom);

			Node& node = *m_nodes[i];
			node.drawDiwne(context, m_drawMode2);

			if (i == m_nodes.size() - 1)
			{
				if (m_dropIndex == m_nodes.size())
				{
					ImGui::SameLine(0, 0);
					ImGui::Dummy(diwne.style().dropZoneDropGap * zoom);
				}
				ImGui::SameLine(0, 0);
			}
			else
			{
				ImGui::SameLine();
			}
		}
		DGui::SameLineDummy(ImVec2(diwne.style().dropZoneMarginWidth * zoom, 0.0f)); // Right margin
	}
}

void NodeDropZone::end(DrawInfo& context)
{
	ImGui::EndGroup();
	ImGui::PopID();
	m_internalHover = ImGui::IsItemHovered();
}
void NodeDropZone::updateLayout(DrawInfo& context)
{
	m_rect.Min = diwne.canvas().screen2diwne(ImGui::GetItemRectMin());
	m_rect.Max = diwne.canvas().screen2diwne(ImGui::GetItemRectMax());
}

void NodeDropZone::afterDraw(DrawInfo& context)
{
	DiwneObject::afterDraw(context);
}

void NodeDropZone::processInteractions(DrawInfo& context)
{
	DiwneObject::processInteractions(context);

	// Check if a node was dragged over the drop zone area
	tryAddNode(context);
	tryRemoveNode(context);
}

bool NodeDropZone::tryAddNode(DrawInfo& context)
{
	if (!context.state.dragging || context.inputConsumed || ImGui::IsKeyDown(ImGuiKey_ModShift))
		return false;

	// Currently only one node at a time can be added or removed
	Actions::DragNodeAction* action = context.state.getActiveAction<Actions::DragNodeAction>();
	if (!action || action->nodes.size() != 1)
		return false;

	Node* dropNode = action->nodes[0];
	m_dropIndex = acceptNodeDiwne(dropNode);
	if (m_dropIndex >= 0)
	{
		context.consumeInput(); // Prevents other drop zones from trying to accept this node

		// Draw the drop indicator
		drawDropIndicator(dropNode, m_dropIndex);

		if (context.state.dragEnd)
		{
			// We only accept nodes that are not already in another container
			dropNode->remove(); // Mark the node for removal from the node editor
			dropNode->setParentObject(this);
			dropNode->setFixed(true); // Nodes are marked as fixed while inside the drop zone
			m_nodes.insert(m_nodes.begin() + m_dropIndex, dropNode->sharedPtr<Node>());
			m_dropIndex = -1; // Reset drop target index
			return true;
		}
	}
	return false;
}

bool NodeDropZone::tryRemoveNode(DrawInfo& context)
{
	if (!context.state.dragging || !context.state.dragStart || context.inputConsumed)
		return false;

	Actions::DragNodeAction* action = context.state.getActiveAction<Actions::DragNodeAction>();
	if (!action)
		return false;

	// Currently only one node at a time can be added or removed
	Node* dropNode = nullptr;
	if (action->nodes.size() != 1)
	{
		// When multiple nodes are dragged we want to only remove a node from the drop zone that is right under
		// the cursor (that is the source node of the drag action).
		// This way we don't have to deal with nodes outside of the screen for example.
		for (auto& node : action->nodes)
		{
			if (node->m_labelDiwne == action->source)
			{
				dropNode = node;
				break;
			}
		}
	}
	else
	{
		dropNode = action->nodes[0];
	}

	// If node that being dragged is in this drop zone, we remove it from it
	if (dropNode != nullptr && dropNode->getParentObject() == this)
	{
		// The drop zone being the nodes parent should guarantee the node was contained in the drop zone
		std::shared_ptr<Node> dropNodePtr = dropNode->sharedPtr<Node>();
		auto it = std::find(m_nodes.begin(), m_nodes.end(), dropNodePtr);
		if (it != m_nodes.end())
		{
			m_nodes.erase(it);
			dropNodePtr->setFixed(false); // Make the node free to be dragged again
			diwne.addNode(dropNodePtr);   // Reassigns parent to the node editor
			return true;
		}
		else
		{
			DIWNE_ERROR("Attempted to remove a node from a drop zone it wasn't contained in!");
			assert(false);
		}
	}
	return false;
}

int NodeDropZone::acceptNodeDiwne(Node* node)
{
	if (this->isChildOfObject(node)) // Exclude any parent nodes
		return -1;
	if (node->isChildObject())
		return -1; // Nodes must be extracted from their old containers before being dropped into another
	int dropIndex = isNodeAboveDropZone(node);
	if (dropIndex < 0)
		return -1; // The node must be "above" the drop zone
	if (!acceptNode(node))
		return -1; // User defined filter
	return dropIndex;
}

int NodeDropZone::isNodeAboveDropZone(Node* newNode)
{
	ImRect newNodeRect = newNode->getRectDiwne();
	//	newNodeRect.Expand(ImVec2(-newNodeRect.GetWidth() / 2.0f * 0.8f, 0.0f));

	// Coarse test
	if (!this->getRectDiwne().Overlaps(newNodeRect))
		return -1;

	float centerX = newNodeRect.GetCenter().x;
	//	float width = newNodeRect.GetWidth() / 2.0f;
	//	float rightX = newNodeRect.Max.x;
	for (int i = 0; i < m_nodes.size(); ++i)
	{
		ImRect nodeRect = m_nodes[i]->getRectDiwne();
		if (centerX < nodeRect.GetCenter().x)
			return i;
	}
	return m_nodes.size();
}

void NodeDropZone::drawDropIndicator(Node* newNode, int index)
{
	if (m_nodes.empty())
	{
		diwne.canvas().AddRectFilledDiwne(m_rect.Min, m_rect.Max, diwne.style().dropIndicatorColor);
		return;
	}

	const ImGuiStyle& style = ImGui::GetStyle();
	const DiwneStyle& dstyle = diwne.style();
	if (index == 0)
	{
		diwne.canvas().AddRectFilledDiwne(
		    m_rect.Min, m_rect.Min + ImVec2(dstyle.dropZoneMarginWidth, m_rect.GetHeight()), dstyle.dropIndicatorColor);
	}
	else if (index == m_nodes.size())
	{
		diwne.canvas().AddRectFilledDiwne(m_rect.Max - ImVec2(dstyle.dropZoneMarginWidth, m_rect.GetHeight()),
		                                  m_rect.Max, dstyle.dropIndicatorColor);
	}
	else
	{
		ImRect rect1 = m_nodes[index - 1]->getRectDiwne();
		ImRect rect2 = m_nodes[index]->getRectDiwne();
		ImVec2 min = rect1.GetTR();
		ImVec2 max = rect2.GetBL();
		diwne.canvas().AddRectFilledDiwne(min, max, dstyle.dropIndicatorColor);
	}
}

bool NodeDropZone::allowDragStart() const
{
	return false;
}

} // namespace DIWNE