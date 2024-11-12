#include "BasicNode.h"

namespace DIWNE
{
// TODO:\ CONTINUE HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//  For now I suppose we can make IDs be statically generated in DiwneObject
//  As far as I can tell they aren't really used anywhere meaninfully (aside from unique identifer)
//  All CoreNodes use the underlying Core::Node id which is also a simple num sequence
//  But the fact its the Core::Node id is never really used, and that id can be retrieved
//  anyway at any time using getNodebase()->getId().
//  Also Node and Pin have their own virtual getId methods instead of inherited getIdDiwne (???) (fix that)

BasicNode::BasicNode(NodeEditor& editor, std::string label)
    : Node(editor, label), m_topLabel(label), m_top(diwne, label + "_top"), m_left(diwne, label + "_left"),
      m_center(diwne, label + "_center"), m_right(diwne, label + "_right"), m_bottom(diwne, label + "_bottom"),
      m_middle(diwne, label + "_middle")
{
	DiwneStyle& style = diwne.m_style;
	m_background = style.nodeBackground;
	m_headerBackground = style.nodeHeaderBackground;
	m_rounding = style.nodeRounding;
}

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
	m_left.end();

	ImGui::SameLine();

	if (m_centerDummySpace > 0)
	{
		ImGui::Indent((m_left.getRect().GetWidth() + m_centerDummySpace * diwne.mp_settingsDiwne->middleAlign) *
		                  diwne.getWorkAreaZoom() +
		              ImGui::GetStyle().ItemSpacing.x);
	} /* spacing is already zoomed in Diwne */
	m_center.begin();
	centerContent(context);
	m_center.end();
	ImGui::SameLine();

	if (m_centerDummySpace > 0)
	{
		ImGui::Indent((m_middle.getRect().GetWidth() + m_centerDummySpace * (1 - diwne.mp_settingsDiwne->middleAlign)) *
		                  diwne.getWorkAreaZoom() +
		              ImGui::GetStyle().ItemSpacing.x);
	}
	m_right.begin();
	rightContent(context);
	m_right.end();

	m_middle.end();

	m_bottom.begin();
	bottomContent(context);
	m_bottom.end();
}


void BasicNode::end(DrawInfo& context)
{
	Node::end(context);
}

void BasicNode::topContent(DrawInfo& context)
{
	drawHeader();
	ImGui::Dummy(ImVec2(ImGui::GetStyle().ItemSpacing.x, 1));
	ImGui::SameLine();
	ImGui::TextUnformatted(m_topLabel.c_str());
}

void BasicNode::leftContent(DrawInfo& context) {}
void BasicNode::rightContent(DrawInfo& context) {}
void BasicNode::centerContent(DrawInfo& context) {}
void BasicNode::bottomContent(DrawInfo& context) {}

void BasicNode::updateLayout(DrawInfo& context)
{
	ImGuiStyle& style = ImGui::GetStyle();

	/* \todo can use ImGui::ItemMax/Min */
	setPosition(m_nodePositionDiwne);
	/* ItemSpacing is already scaled, node rects are using unscaled coordinates, thus the divison */
	ImVec2 spacing = ImGui::GetStyle().ItemSpacing / diwne.getWorkAreaZoom();

	float rightWidth = m_right.getRect().GetWidth();
	/* space is between left-middle and middle-right */
	float centerWidth = m_left.getRect().GetWidth() + m_center.getWidth() + rightWidth + (spacing.x * 2);
	float maxWidth = std::max(m_top.getRect().GetWidth(), std::max(centerWidth, m_bottom.getWidth()));

	float bottomYOfCentre = m_bottom.getRect().Min.y - spacing.y;

	m_top.setMaxX(m_top.getMinX() + maxWidth); /* top.Max.x is most right point now */

	m_left.setMaxY(bottomYOfCentre);

	m_centerDummySpace = maxWidth - centerWidth; /* how much shift middle or right content for
	                                                right-alignation */
	/* \todo span graphic of middle background to fill middle of node or left it
	  just around middle content? m_middleRectDiwne.Min.x = m_leftRectDiwne.Max.x
	  + spacing.x; m_middleRectDiwne.Max.x = m_top.getMax().x - rightWidth -
	  spacing.x;*/ /* space between middle-right */
	m_middle.setMaxY(bottomYOfCentre);

	m_right.setMaxY(bottomYOfCentre);

	m_bottom.setMaxX(m_top.getMaxX());

	// Update DIWNE rect
	m_rect.Min = m_top.getMin();
	m_rect.Max = m_bottom.getMax();
}

void BasicNode::translate(const ImVec2& vec)
{
	DiwneObject::translate(vec);
	m_top.translate(vec);
	m_left.translate(vec);
	m_middle.translate(vec);
	m_center.translate(vec);
	m_right.translate(vec);
	m_bottom.translate(vec);
}

void BasicNode::drawHeader()
{
	diwne.m_renderer->AddRectFilledDiwne(m_top.getRect().Min, m_top.getRect().Max, m_headerBackground, m_rounding,
	                                     ImDrawFlags_RoundCornersTop);
}

void BasicNode::drawBody()
{
	diwne.m_renderer->AddRectFilledDiwne(m_top.getRect().Min, m_bottom.getRect().Max, m_background, m_rounding,
	                                     ImDrawFlags_RoundCornersAll);
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