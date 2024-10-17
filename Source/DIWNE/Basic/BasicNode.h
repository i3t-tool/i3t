#pragma once

#include "DIWNE/Core/DiwnePanel.h"
#include "DIWNE/Core/Node.h"

namespace DIWNE
{
class BasicNode : public Node
{
public:
	DiwnePanel m_top, m_left, m_center, m_right, m_bottom;
	DiwnePanel m_middle;

protected:
	std::string m_topLabel;

public:
	// TODO: Remove centerDummySpace and handle layouting in DiwnePanels

	/** \brief Rectangle of parts of node in diwne,
	 * are computed every frame based on node content and m_nodePositionDiwne.
	 * Note that these rects are using "world" node editor coordinates, not scaled and translated screen ones.
	 */
	float m_centerDummySpace{0}; ///< indent value to center the middle part

	BasicNode(NodeEditor& editor, std::string label);

	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;

	virtual void topContent(DrawInfo& context);
	virtual void leftContent(DrawInfo& context);
	virtual void centerContent(DrawInfo& context);
	virtual void rightContent(DrawInfo& context);
	virtual void bottomContent(DrawInfo& context);

	/**< store object size and position after this frame */
	virtual void updateLayout();

	virtual void drawHeader();
	virtual void drawBody();

	void translate(const ImVec2& vec) override;

public:
	// Styling
	ImVec4 m_background;
	ImVec4 m_headerBackground;
	int m_rounding;

public:
	// Getters / Setters
	const std::string& getTopLabel() const;
	void setTopLabel(const std::string& str);
};
} // namespace DIWNE
