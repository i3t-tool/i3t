#include "BasicNode.h"

#include "DIWNE/Core/Layout/Gravity.h"
#include "DIWNE/Core/NodeEditor.h"
#include "DIWNE/Core/Style/DiwneStyle.h"
#include "DIWNE/Core/Style/StyleOverride.h"
#include "DIWNE/Core/diwne_utils.h"

namespace DIWNE
{
BasicNode::BasicNode(NodeEditor& editor, std::string label)
    : Node(editor, label), m_top(diwne, label + "_top"), m_left(diwne, label + "_left"),
      m_center(diwne, label + "_center"), m_right(diwne, label + "_right"), m_middle(diwne, label + "_middle"),
      m_topLabel(label), m_style(std::make_shared<StyleOverride>(diwne))
{}

void BasicNode::begin(DrawInfo& context)
{
	Node::begin(context);
	drawBody();
}

void BasicNode::content(DrawInfo& context)
{
	m_top.begin();
	topContent(context);
	m_top.end();

	m_middle.begin();
	m_left.begin();
	leftContent(context);
	m_left.end(&m_middle);

	m_middle.sameLine();

	//	if (m_centerDummySpace > 0)
	//	{
	//		ImGui::Indent((m_left.getRect().GetWidth() + m_centerDummySpace * diwne.mp_settingsDiwne->middleAlign) *
	//		                  diwne.getZoom() +
	//		              ImGui::GetStyle().ItemSpacing.x);
	//	} /* spacing is already zoomed in Diwne */
	m_middle.spring(0.5);

	m_center.begin();
	centerContent(context);
	m_center.end(&m_middle);

	m_middle.sameLine();

	//	if (m_centerDummySpace > 0)
	//	{
	//		ImGui::Indent((m_middle.getRect().GetWidth() + m_centerDummySpace * (1 -
	// diwne.mp_settingsDiwne->middleAlign))
	//* 		                  diwne.getZoom() + 		              ImGui::GetStyle().ItemSpacing.x);
	//	}
	m_middle.spring(0.5);

	m_right.begin();
	rightContent(context);
	m_right.end(&m_middle);

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
	ImVec2 padding = m_style->size(DiwneStyle::nodePadding);
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
	ImGuiStyle& style = ImGui::GetStyle();

	//	LOG_INFO("top {}:{} {}:{} ({}, {})", m_top.getMinX(), m_top.getMinY(), m_top.getMaxX(), m_top.getMaxY(),
	// m_top.getWidth(), m_top.getHeight()); 	LOG_INFO("left {}:{} {}:{} ({}, {})", m_left.getMinX(),
	// m_left.getMinY(), m_left.getMaxX(), m_left.getMaxY(), m_left.getWidth(), m_left.getHeight()); 	LOG_INFO("center
	// {}:{} {}:{} ({},
	//{})", m_center.getMinX(), m_center.getMinY(), m_center.getMaxX(), m_center.getMaxY(), m_center.getWidth(),
	// m_center.getHeight()); 	LOG_INFO("right {}:{} {}:{} ({}, {})", m_right.getMinX(), m_right.getMinY(),
	// m_right.getMaxX(), m_right.getMaxY(), m_right.getWidth(), m_right.getHeight()); 	LOG_INFO("middle {}:{} {}:{}
	// ({},
	//{})", m_middle.getMinX(), m_middle.getMinY(), m_middle.getMaxX(), m_middle.getMaxY(), m_middle.getWidth(),
	// m_middle.getHeight());

	if (ImGui::IsKeyDown(ImGuiKey_E))
		int x = 5;

	/* ItemSpacing is already scaled, node rects are using unscaled coordinates, thus the divison */
	ImVec2 spacing = ImGui::GetStyle().ItemSpacing / diwne.canvas().getZoom();

	float maxX = std::max(m_top.getMaxX(), m_middle.getMinX() + m_middle.getMinimumWidth());
	m_top.setMaxX(maxX);
	m_middle.setMaxX(maxX);

	//	Gravity::alignRightEdge({m_top, m_right});
	Gravity::alignBottom({m_left, m_center, m_right});

	//	float maxX = std::max(m_top.getMaxX(), m_right.getMaxX());
	//	float maxY = std::max({m_left.getMaxY(), m_center.getMaxY(), m_right.getMaxY()});
	//	m_top.setMaxX(maxX);
	//	m_right.setMaxX(maxX);
	//	m_right.setMaxY(maxY);
	//	m_middle.setMaxX(maxX);

	//	float rightWidth = m_right.getRect().GetWidth();
	//	/* space is between left-middle and middle-right */
	//	float centerWidth = m_left.getRect().GetWidth() + m_center.getWidth() + rightWidth + (spacing.x * 2);
	//	float maxWidth = std::max(m_top.getRect().GetWidth(), std::max(centerWidth, m_bottom.getWidth()));
	//
	//	float bottomYOfCentre = m_bottom.getRect().Min.y - spacing.y;
	//
	//	m_top.setMaxX(m_top.getMinX() + maxWidth); /* top.Max.x is most right point now */
	//
	//	m_left.setMaxY(bottomYOfCentre);
	//
	//	0 = maxWidth - centerWidth; /* how much shift middle or right content for
	//	                                                right-alignation */
	//	/* \todo span graphic of middle background to fill middle of node or left it
	//	  just around middle content? m_middleRectDiwne.Min.x = m_leftRectDiwne.Max.x
	//	  + spacing.x; m_middleRectDiwne.Max.x = m_top.getMax().x - rightWidth -
	//	  spacing.x;*/ /* space between middle-right */
	//	//	m_middle.setMaxY(bottomYOfCentre);
	//
	//	m_right.setMaxY(bottomYOfCentre);

	// Update DIWNE rect
	m_rect.Min = m_top.getMin();
	m_rect.Max = m_middle.getMax();

	m_left.layout();
	m_top.layout();
	m_center.layout();
	m_right.layout();
	m_middle.layout();
	//	LOG_INFO("leftX: {}, cenX: {}, rightX: {}", m_left.getRect().Min.x, m_center.getRect().Min.x,
	//	         m_right.getRect().Min.x);
	//	LOG_INFO("leftW: {}, cenW: {}, rightW: {}, topW: {}", m_left.getRect().GetWidth(),
	// m_center.getRect().GetWidth(), 	         m_right.getRect().GetWidth(), m_top.getRect().GetWidth());
	// LOG_INFO("middle width:
	//{}, minW: {}, avail: {}", m_middle.getRect().GetWidth(), m_middle.getMinimumWidth(),
	// m_middle.getAvailableSpringWidth())
}

void BasicNode::translate(const ImVec2& vec)
{
	DiwneObject::translate(vec);
	m_top.translate(vec);
	m_left.translate(vec);
	m_middle.translate(vec);
	m_center.translate(vec);
	m_right.translate(vec);
}

void BasicNode::drawHeader()
{
	diwne.canvas().AddRectFilledDiwne(m_top.getMin(), m_top.getMax(), m_style->color(DiwneStyle::nodeHeaderBg),
	                                  m_style->decimal(DiwneStyle::nodeRounding), ImDrawFlags_RoundCornersTop);
}

void BasicNode::drawBody()
{
	diwne.canvas().AddRectFilledDiwne(m_rect.Min, m_rect.Max, m_style->color(DiwneStyle::nodeBg),
	                                  m_style->decimal(DiwneStyle::nodeRounding), ImDrawFlags_RoundCornersAll);
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