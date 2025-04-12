#pragma once

#include "DIWNE/Core/Elements/Node.h"
#include "DIWNE/Core/Layout/DiwnePanel.h"

namespace DIWNE
{
class StyleOverride;

/**
 * Parts of the node:
 *  -------------------
 *  |      Top        |
 *  -------------------
 *  |Left|Center|Right|  --> |L|C|R| == Middle
 *  -------------------
 *  |     Bottom      |
 *  -------------------
 */
class BasicNode : public Node
{
	using Super = Node;

public:
	DiwnePanel m_top, m_left, m_center, m_right;
	DiwnePanel m_middle; // Parent panel of m_left, m_center and m_right.

protected:
	std::string m_topLabel;

	float m_topLabelWidth{0};       ///< Top label width in DIWNE coordinates.
	float m_topLabelWidthChange{0}; ///< Change of top label width since last frame, used to prevent layout flicker.

	// Style related stuff
	bool m_headerSpacing = true; ///< Whether to add a gap between the node header and its content
	bool m_bottomSpacing = true; ///< Whether to add a gap between the node content end the end.
	float m_contentSpacing = -1; ///< Gap between left, center and right panels (-1 for default item spacing).

public:
	BasicNode(NodeEditor& editor, std::string label);

	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;

	virtual void topContent(DrawInfo& context);
	virtual void leftContent(DrawInfo& context);
	virtual void centerContent(DrawInfo& context);
	virtual void rightContent(DrawInfo& context);
	virtual void bottomContent(DrawInfo& context);

	void updateLayout(DrawInfo& context) override;

	virtual void drawHeader(ImDrawFlags corners = ImDrawFlags_RoundCornersTop);
	virtual void drawBody();

	void translate(const ImVec2& vec) override;

public:
	// Getters / Setters
	const std::string& getTopLabel() const;
	void setTopLabel(const std::string& str);
};
} // namespace DIWNE
