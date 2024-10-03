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

#include "CorePins.h"

using namespace Workspace;

CoreLink::CoreLink(DIWNE::NodeEditor& diwne, DIWNE::ID id, CoreInPin* endPin)
    : DIWNE::Link(diwne, id), m_endPin(endPin), m_startPin(nullptr)
{}

void CoreLink::unplug()
{
	m_endPin->unplug();
	m_startPin = nullptr;
}

void CoreLink::popupContent()
{
	if (ImGui::MenuItem("Delete"))
	{
		unplug();
	}
}

void CoreLink::updateEndpoints()
{
	ImVec2 start, end;
	CoreOutPin* startPin = getStartPin();
	CoreInPin* endPin = getEndPin();
	if (startPin)
		start = startPin->getLinkConnectionPointDiwne();
	if (endPin)
		end = endPin->getLinkConnectionPointDiwne();
	setLinkEndpointsDiwne(start, end);
}

void CoreLink::updateControlPointsOffsets()
{
	float offset =
	    (getEndpoint().x - getStartpoint().x) * I3T::getTheme().get(ESize::Links_ControlpointsPositionFraction);
	if (offset < I3T::getTheme().get(ESize::Links_ControlpointsPositionMin))
		offset = I3T::getTheme().get(ESize::Links_ControlpointsPositionMin);
	diwne.mp_settingsDiwne->linkStartControlOffsetDiwne = ImVec2(offset, 0);
	diwne.mp_settingsDiwne->linkEndControlOffsetDiwne = ImVec2(-offset, 0);
}

bool CoreLink::initialize()
{
	updateControlPointsOffsets();

	diwne.mp_settingsDiwne->linkColor = I3T::getTheme().get(PinColorBackground[m_endPin->getType()]);
	diwne.mp_settingsDiwne->linkThicknessDiwne = I3T::getTheme().get(ESize::Links_Thickness);

	if (m_selected)
	{
		diwne.mp_settingsDiwne->linkThicknessSelectedBorderDiwne =
		    I3T::getTheme().get(ESize::Links_ThicknessSelectedBorder);
		diwne.mp_settingsDiwne->linkColorSelected = diwne.mp_settingsDiwne->linkColor;

		diwne.mp_settingsDiwne->linkColorSelected.x += I3T::getColor(EColor::Links_selected_colorShift).x;
		diwne.mp_settingsDiwne->linkColorSelected.y += I3T::getColor(EColor::Links_selected_colorShift).y;
		diwne.mp_settingsDiwne->linkColorSelected.z += I3T::getColor(EColor::Links_selected_colorShift).z;

		diwne.mp_settingsDiwne->linkColorSelected.w = I3T::getSize(ESize::Links_selected_alpha);
	}
	return false;
}