#include "BasicNode.h"

#include "DIWNE/Core/Layout/Gravity.h"
#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/diwne_utils.h"

namespace DIWNE
{
BasicNode::BasicNode(NodeEditor& editor, std::string label)
    : Node(editor, label), m_top(diwne, label + "_top"), m_left(diwne, label + "_left"),
      m_center(diwne, label + "_center"), m_right(diwne, label + "_right"), m_middle(diwne, label + "_middle"),
      m_topLabel(label)
{}

void BasicNode::begin(DrawInfo& context)
{
	Super::begin(context);
	drawBody();
}

void BasicNode::content(DrawInfo& context)
{
	ImGuiStyle& style = ImGui::GetStyle();

	m_top.begin();
	topContent(context);
	m_top.end();
	if (!m_headerSpacing)
		DGui::NewLine(); // No vertical spacing

	m_middle.begin();
	m_left.begin();
	leftContent(context);
	m_left.end();

	ImGui::SameLine(0.0f,
	                m_contentSpacing == -1 ? style.ItemSpacing.x : diwne.canvas().diwne2screenSize(m_contentSpacing));

	m_middle.spring(0.5f);

	m_center.begin();
	centerContent(context);
	m_center.end();

	ImGui::SameLine(0.0f,
	                m_contentSpacing == -1 ? style.ItemSpacing.x : diwne.canvas().diwne2screenSize(m_contentSpacing));

	m_middle.spring(0.5f);

	m_right.begin();
	rightContent(context);
	m_right.end();

	if (m_bottomSpacing)
		ImGui::Spacing();

	m_middle.end();
}


void BasicNode::end(DrawInfo& context)
{
	Node::end(context);
}

void BasicNode::topContent(DrawInfo& context)
{
	drawHeader();
	float zoom = diwne.getZoom();
	ImVec2 padding = style().size(Style::NODE_PADDING);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding.y * zoom);
	ImGui::Dummy(ImVec2(padding.x * zoom, 0));
	ImGui::SameLine(0, 0);
	ImGui::TextUnformatted(m_topLabel.c_str());
	ImGui::SameLine();
	DGui::NewLine();
	ImGui::Dummy(ImVec2(padding.x * zoom, padding.y * zoom));
}

void BasicNode::leftContent(DrawInfo& context) {}
void BasicNode::rightContent(DrawInfo& context) {}
void BasicNode::centerContent(DrawInfo& context)
{
	ImGui::Text("Content");
	ImGui::Spacing();
}
void BasicNode::bottomContent(DrawInfo& context) {}

void BasicNode::updateLayout(DrawInfo& context)
{
	Gravity::alignRightEdge({m_top, m_middle});
	Gravity::alignBottom({m_left, m_center, m_right});

	// Update DIWNE rect
	ImRect panelRect = ImRect(m_top.getMin(), m_middle.getMax());
	setSize(panelRect.GetSize()); // m_rect is just resized, not moved, see updateLayout() docs

	m_left.layout();
	m_top.layout();
	m_center.layout();
	m_right.layout();
	m_middle.layout();
}

void BasicNode::translate(const ImVec2& vec)
{
	Super::translate(vec);
	m_top.translate(vec);
	m_left.translate(vec);
	m_middle.translate(vec);
	m_center.translate(vec);
	m_right.translate(vec);
}

void BasicNode::drawHeader(ImDrawFlags corners)
{
	diwne.canvas().AddRectFilledDiwne(m_top.getMin(), m_top.getMax(), style().color(Style::NODE_HEADER_BG),
	                                  style().decimal(Style::NODE_ROUNDING), corners);
}

void BasicNode::drawBody()
{
	diwne.canvas().AddRectFilledDiwne(m_displayRect.Min, m_displayRect.Max, style().color(Style::NODE_BG),
	                                  style().decimal(Style::NODE_ROUNDING), ImDrawFlags_RoundCornersAll);
}

const std::string& BasicNode::getTopLabel() const
{
	return m_topLabel;
}

void BasicNode::setTopLabel(const std::string& str)
{
	m_topLabel = str;
}


} // namespace DIWNE