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
#include "GUI/I3TGui.h"

using namespace Workspace;

CoreLink::CoreLink(DIWNE::NodeEditor& diwne) : DIWNE::Link(diwne) {}

void CoreLink::popupContent(DIWNE::DrawInfo& context)
{
	if (I3TGui::MenuItemWithLog("Delete"))
	{
		destroy();
	}
}

void CoreLink::updateControlPointsOffsets()
{
	float offset =
	    (getEndPoint().x - getStartPoint().x) * I3T::getTheme().get(ESize::Links_ControlpointsPositionFraction);
	if (offset < I3T::getTheme().get(ESize::Links_ControlpointsPositionMin))
		offset = I3T::getTheme().get(ESize::Links_ControlpointsPositionMin);
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
