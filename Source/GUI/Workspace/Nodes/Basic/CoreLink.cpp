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
#include "CoreLink.h"

#include <cmath>

#include "CorePins.h"

using namespace Workspace;

CoreLink::CoreLink(DIWNE::NodeEditor& diwne) : DIWNE::Link(diwne) {}

void CoreLink::popupContent(DIWNE::DrawInfo& context)
{
	if (ImGui::MenuItem("Delete"))
	{
		destroy();
	}
}

void CoreLink::updateControlPointsOffsets()
{
	ImVec2 diff = getEndPoint() - getStartPoint();

	// constexpr float maxAngle = 1 * I3_PI / 4;
	// float angle = std::atan2f(diff.y, diff.x);
	// if (angle <= I3_PI)
	// 	angle = std::min(angle, maxAngle);
	// else
	// 	angle = 2 * I3_PI - std::min(2 * I3_PI - angle, maxAngle);
	// ImVec2 lockedDir = ImVec2(cos(angle), sin(angle));

	const float diminishingDistFactor = sqrtf(300.f);

	float distanceFactor = I3T::getTheme().get(ESize::Links_ControlpointsPositionFraction);
	float negHorizontalFactor = 0.3f;

	float minOffset = I3T::getTheme().get(ESize::Links_ControlpointsPositionMin);
	float maxOffset = I3T::getTheme().get(ESize::Links_ControlpointsPositionMax);

	float dist = DIWNE::DMath::len(diff);
	float offset = diminishingDistFactor * sqrtf(dist) * distanceFactor;

	if (diff.x < 15.f)
		offset += abs(diff.x) * negHorizontalFactor;

	offset = DIWNE::DMath::clamp(offset, minOffset, maxOffset);

	// Small dist correction
	float deadzone = offset * 2;
	if (dist < deadzone)
	{
		offset *= DIWNE::DMath::smoothstep(0.f, 1.f, dist / deadzone);
		if (offset < 0.01f)
			offset = 0;
	}

	// Trying to degenerate link to a line when its a horizontal line to prevent curve drawing artefacts
	// Modifying the control points doesn't really fix the issue, hence
	// TODO: Split the line into multiple curves or change its rendering when it nears a horizontal line
	// float yDeadzone = 12.0f;
	// if (abs(diff.y) < yDeadzone)
	// 	offset *= (abs(diff.y) / yDeadzone);

	diwne.mp_settingsDiwne->linkStartControlOffsetDiwne = ImVec2(offset, 0);
	diwne.mp_settingsDiwne->linkEndControlOffsetDiwne = ImVec2(-offset, 0);
}

void CoreLink::initialize(DIWNE::DrawInfo& context)
{
	updateControlPointsOffsets();

	ImVec4 color;
	DIWNE::Pin* pin = getAnyPin();
	if (pin)
	{
		color = I3T::getTheme().get(PinColorBackground[static_cast<CorePin*>(pin)->getType()]);
	}
	else
	{
		color = ImColor(0.5f, 0.5f, 0.5f);
	}
	m_color = color;

	// TODO: Hookup link thickness
	//	diwne.mp_settingsDiwne->linkThicknessDiwne = I3T::getTheme().get(ESize::Links_Thickness);

	if (m_selected)
	{
		m_color = m_color + I3T::getColor(EColor::Links_selected_colorShift);
	}
}

bool CoreLink::allowSelectOnClick(const DIWNE::DrawInfo& context) const
{
	return false; // Disable selection of links in I3T
}
