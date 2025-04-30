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
#include "GUI/I3TGui.h"
#include "GUI/Workspace/Nodes/Model.h"
#include "GUI/Workspace/WorkspaceDiwne.h"

using namespace Workspace;

CoreLink::CoreLink(DIWNE::NodeEditor& diwne) : DIWNE::Link(diwne) {}

void CoreLink::popupContent(DIWNE::DrawInfo& context)
{
	if (I3TGui::MenuItemWithLog("Delete"))
	{
		destroy();
	}
}

// TODO: Move this to DIWNE eventually
void CoreLink::updateControlPoints()
{
	bool rightToLeft = true;
	if (auto* startPin = getStartPin())
		rightToLeft = !startPin->isLeft();
	else if (auto* endPin = getEndPin())
		rightToLeft = endPin->isLeft();

	ImVec2 diff;
	if (rightToLeft)
		diff = getEndPoint() - getStartPoint();
	else
		diff = getStartPoint() - getEndPoint();

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

	m_controlPointStartDiwne = m_startDiwne + ImVec2(rightToLeft ? offset : -offset, 0);
	m_controlPointEndDiwne = m_endDiwne + ImVec2(rightToLeft ? -offset : offset, 0);
}

void CoreLink::initialize(DIWNE::DrawInfo& context)
{
	ImVec4 color;
	bool unplugged = !this->isPlugged() && !m_previewPlugged;
	DIWNE::Pin* pin = getAnyPin();
	m_coreType = Core::EValueType::Pulse;
	if (pin)
	{
		m_coreType = static_cast<CorePin*>(pin)->getType();
		color = I3T::getTheme().get(PinColorBackground[m_coreType]);
		if (unplugged)
			color.w = style().decimal(DIWNE::Style::LINK_UNPLUGGED_ALPHA);
	}
	else
	{
		color = ImColor(0.5f, 0.5f, 0.5f);
	}
	m_color = color;

	if (m_selected)
	{
		m_color = m_color + I3T::getColor(EColor::Links_selected_colorShift);
	}
}

void CoreLink::content(DIWNE::DrawInfo& context)
{
	ImDrawList* idl = ImGui::GetWindowDrawList();

	// Tracking border
	m_widthOffset = 0.f;
	if (static_cast<WorkspaceDiwne&>(diwne).isTracking() && m_coreType == Core::EValueType::MatrixMul)
	{
		ImVec4 trackingBorderColor;
		Core::TrackedNodeData* startTrackingData = nullptr;
		Core::TrackedNodeData* endTrackingData = nullptr;
		if (auto* startPin = getStartPin())
		{
			if (auto* node = startPin->getNode())
			{
				if (auto* t = static_cast<CoreNode*>(node)->getNodebase()->getTrackingData())
				{
					startTrackingData = t;
				}
			}
		}
		if (auto* endPin = getEndPin())
		{
			if (auto* node = endPin->getNode())
			{
				if (auto* t = static_cast<CoreNode*>(node)->getNodebase()->getTrackingData())
				{
					endTrackingData = t;
				}
			}
		}
		if (this->isPlugged())
		{
			if (startTrackingData && endTrackingData)
			{
				if (startTrackingData->chain && endTrackingData->chain)
				{
					if (startTrackingData->active && endTrackingData->active)
						trackingBorderColor = I3T::getColor(EColor::Nodes_Tracking_ColorActive);
					else
						trackingBorderColor = I3T::getColor(EColor::Nodes_Tracking_ColorInactive);
				}
				if ((startTrackingData->modelSubtree || startTrackingData->chain) && endTrackingData->modelSubtree)
					trackingBorderColor = I3T::getColor(EColor::Nodes_Tracking_ColorInactive);
			}
		}
		if (trackingBorderColor.w > 0)
		{
			float trackingLinkWidth = I3T::getSize(ESize::Nodes_Tracking_LinkWidth);
			diwne.canvas().AddBezierCurveDiwne(idl, m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne,
			                                   m_endDiwne, trackingBorderColor, trackingLinkWidth);
			m_widthOffset = -style().decimal(DIWNE::Style::LINK_WIDTH) * 0.3f;
		}
	}
	Link::content(context);
}

bool CoreLink::allowSelectOnClick(const DIWNE::DrawInfo& context) const
{
	return false; // Disable selection of links in I3T
}
