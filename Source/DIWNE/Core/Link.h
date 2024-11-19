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

#include <algorithm>
#include <limits>

#include "DiwneObject.h"

namespace DIWNE
{
class Link : public DiwneObject
{
public:
	Link(DIWNE::NodeEditor& diwne, std::string const labelDiwne = "DiwneLink");

	/** Default destructor */
	virtual ~Link(){};

	void initialize(DrawInfo& context) override;
	void initializeDiwne(DrawInfo& context) override;
	void begin(DrawInfo& context) override{}; /*!< link is not ImGui element - it is just picture of line */
	void end(DrawInfo& context) override{}; ///< No need to set m_internalHover as we handle hovering differently
	void content(DrawInfo& context) override;

	DIWNE::DiwneAction getHoldActionType() const final
	{
		return DiwneAction::HoldLink;
	};
	DIWNE::DiwneAction getDragActionType() const final
	{
		return DiwneAction::DragLink;
	};
	DIWNE::DiwneAction getTouchActionType() const final
	{
		return DiwneAction::TouchLink;
	};

	virtual void updateEndpoints(){};
	void updateControlPoints();
	void updateSquareDistanceMouseFromLink();

	void updateLayout(DrawInfo& context) override;

	ImVec2 getStartpoint()
	{
		return m_startDiwne;
	};
	ImVec2 getEndpoint()
	{
		return m_endDiwne;
	};
	ImVec2 getStartControlPoint()
	{
		return m_controlPointStartDiwne;
	};
	ImVec2 getEndControlPoint()
	{
		return m_controlPointEndDiwne;
	};

	void setLinkEndpointsDiwne(const ImVec2 start, const ImVec2 end)
	{
		m_startDiwne = start;
		m_endDiwne = end;
	};

	bool isLinkOnWorkArea(); /*!< in fact just rectangle (from startPoint to
	                            endPoint) check - so could return true while Link
	                            is not visible */


	void onHover(DrawInfo& context) override;

	bool m_just_pluged /*!< not select link when just pluged */;

protected:
	bool isHoveredDiwne() override;

private:
	ImVec2 m_startDiwne, m_endDiwne;
	ImVec2 m_controlPointStartDiwne, m_controlPointEndDiwne;
	float m_squaredDistanceMouseFromLink{std::numeric_limits<float>::max()};
};

} /* namespace DIWNE */
