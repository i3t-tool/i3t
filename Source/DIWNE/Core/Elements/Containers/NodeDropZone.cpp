#include "NodeDropZone.h"

#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/diwne_utils.h"
#include "Utils/Color.h"

namespace DIWNE
{
NodeDropZone::NodeDropZone(NodeEditor& diwne, DiwneObject* parent) : DiwneObject(diwne, "DropZone"), NodeContainer(this)
{
	setParentObject(parent);
	setSelectable(false);
	setFixed(true);
}

void NodeDropZone::initialize(DrawInfo& context)
{
	purgeNodes(); // Erase objects marked for destruction or removal in the previous frame
}

void NodeDropZone::begin(DrawInfo& context)
{
	diwne.canvas().AddRectFilledDiwne(m_rect.Min, m_rect.Max, diwne.style().color(DiwneStyle::dropZoneBg));
	ImGui::PushID(m_labelDiwne.c_str());
	ImGui::BeginGroup();
}

void NodeDropZone::content(DrawInfo& context)
{
	float zoom = diwne.getZoom();
	if (m_nodes.empty())
	{
		drawEmptyContent(context);
	}
	else
	{
		DGui::SameLineDummy(ImVec2(diwne.style().dropZoneMarginWidth * zoom, 0.0f));
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			if (i == m_dropIndex)
				DGui::SameLineDummy(diwne.style().size(DiwneStyle::dropZoneDropGap) * ImVec2(zoom, zoom));

			Node& node = *m_nodes[i];
			node.drawDiwne(context, m_drawMode);

			if (i == m_nodes.size() - 1)
			{
				if (m_dropIndex == m_nodes.size())
				{
					ImGui::SameLine(0, 0);
					ImGui::Dummy(diwne.style().size(DiwneStyle::dropZoneDropGap) * ImVec2(zoom, zoom));
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

void NodeDropZone::drawEmptyContent(DrawInfo& context)
{
	float zoom = diwne.getZoom();
	const ImVec2 defaultSize = ImVec2(130 * zoom, 70 * zoom);
	ImVec2 origin = ImGui::GetCursorScreenPos();
	const char* emptyLabel = "Drag and drop\n nodes here";
	ImGui::SetCursorScreenPos(origin + (defaultSize / 2.0f) - (ImGui::CalcTextSize(emptyLabel) / 2.0f) -
	                          ImVec2(0, ImGui::GetStyle().ItemSpacing.y / 2.0f));
	ImGui::TextDisabled("%s", emptyLabel);
	ImGui::SetCursorScreenPos(origin);
	ImGui::Dummy(defaultSize);
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

	std::shared_ptr<Node>& dropNode = action->nodes[0];
	m_dropIndex = acceptNodeDiwne(dropNode.get());
	if (m_dropIndex >= 0)
	{
		context.consumeInput(); // Prevents other drop zones from trying to accept this node

		// Draw the drop indicator
		drawDropIndicator(dropNode.get(), m_dropIndex);

		if (context.state.dragEnd)
		{
			addNodeAt(dropNode, m_dropIndex);
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
	std::shared_ptr<Node>* dropNodePtr = nullptr;
	if (action->nodes.size() != 1)
	{
		// When multiple nodes are dragged we want to only remove a node from the drop zone that is right under
		// the cursor (that is the source node of the drag action).
		// This way we don't have to deal with nodes outside of the screen for example.
		for (auto& node : action->nodes)
		{
			if (*node == *action->source.lock())
			{
				dropNodePtr = &node;
				break;
			}
		}
	}
	else
	{
		dropNodePtr = &action->nodes[0];
	}
	std::shared_ptr<Node>& dropNode = *dropNodePtr;

	// If node that being dragged is in this drop zone, we remove it from it
	if (dropNode != nullptr && dropNode->getParentObject() == this)
	{
		// The drop zone being the nodes parent should guarantee the node was contained in the drop zone
		if (!removeNode(dropNode->sharedPtr<Node>()))
		{
			DIWNE_ERROR("Attempted to remove a node from a drop zone it wasn't contained in!");
			assert(false);
			return false;
		}
		return true;
	}
	return false;
}
void NodeDropZone::onNodeAdd(Node* node, int index)
{
	node->setFixed(true); // Nodes are marked as fixed while inside the drop zone
	m_dropIndex = -1;     // Reset drop target index
}
void NodeDropZone::onNodeRemove(std::shared_ptr<Node> node, int index)
{
	node->setFixed(false); // Make the node free to be dragged again
	if (!node->isDestroyed())
		diwne.addNode(node); // Reassigns parent to the node editor
}

int NodeDropZone::acceptNodeDiwne(Node* node)
{
	if (node->isDestroyed()) // Don't accept nodes marked for destruction
		return -1;
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
		diwne.canvas().AddRectFilledDiwne(m_rect.Min, m_rect.Max, diwne.style().color(DiwneStyle::dropIndicatorColor));
		return;
	}

	const ImGuiStyle& style = ImGui::GetStyle();
	const DiwneStyle& dstyle = diwne.style();
	if (index == 0)
	{
		diwne.canvas().AddRectFilledDiwne(m_rect.Min,
		                                  m_rect.Min + ImVec2(dstyle.dropZoneMarginWidth, m_rect.GetHeight()),
		                                  dstyle.color(DiwneStyle::dropIndicatorColor));
	}
	else if (index == m_nodes.size())
	{
		diwne.canvas().AddRectFilledDiwne(m_rect.Max - ImVec2(dstyle.dropZoneMarginWidth, m_rect.GetHeight()),
		                                  m_rect.Max, dstyle.color(DiwneStyle::dropIndicatorColor));
	}
	else
	{
		ImRect rect1 = m_nodes[index - 1]->getRectDiwne();
		ImRect rect2 = m_nodes[index]->getRectDiwne();
		ImVec2 min = rect1.GetTR();
		ImVec2 max = rect2.GetBL();
		diwne.canvas().AddRectFilledDiwne(min, max, dstyle.color(DiwneStyle::dropIndicatorColor));
	}
}

bool NodeDropZone::allowDragStart() const
{
	return false;
}
bool NodeDropZone::allowHover() const
{
	return false;
}
void NodeDropZone::onDestroy(bool logEvent)
{
	for (auto& node : m_nodes)
	{
		node->destroy(logEvent);
	}
	DiwneObject::onDestroy(logEvent);
}

} // namespace DIWNE