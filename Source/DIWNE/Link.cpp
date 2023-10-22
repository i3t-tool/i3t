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
#include "diwne_include.h"

namespace DIWNE
{
Link::Link(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne /*="DiwneLink"*/)
    : DiwneObject(diwne, id, labelDiwne), m_startDiwne(ImVec2(0, 0)) /* only initialize value - see initializeDiwne() */
      ,
      m_endDiwne(ImVec2(0, 0)), m_just_pluged(false)
{}

void Link::updateSquareDistanceMouseFromLink()
{
	ImVec2 mousePosDiwne = diwne.screen2diwne(diwne.bypassGetMousePos());
	ImVec2 closestPointOnLink =
	    ImBezierCubicClosestPointCasteljau(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                                       mousePosDiwne, ImGui::GetStyle().CurveTessellationTol);
	ImVec2 diff = (closestPointOnLink - mousePosDiwne) / diwne.getWorkAreaZoom();
	m_squaredDistanceMouseFromLink = diff.x * diff.x + diff.y * diff.y;
}

void Link::updateControlPoints()
{
	m_controlPointStartDiwne = m_startDiwne + diwne.mp_settingsDiwne->linkStartControlOffsetDiwne;
	m_controlPointEndDiwne = m_endDiwne + diwne.mp_settingsDiwne->linkEndControlOffsetDiwne;
}

bool Link::isLinkOnWorkArea()
{
	return diwne.getWorkAreaDiwne().Overlaps(getRectDiwne());
}

bool Link::initialize()
{
	diwne.mp_settingsDiwne->linkColor.w =
	    m_focusedForInteraction ? diwne.mp_settingsDiwne->linkAlphaHovered : diwne.mp_settingsDiwne->linkAlpha;
	diwne.mp_settingsDiwne->linkColorSelected.w = m_focusedForInteraction
	                                                  ? diwne.mp_settingsDiwne->linkAlphaSelectedHovered
	                                                  : diwne.mp_settingsDiwne->linkAlphaSelected;

	return false;
}
bool Link::initializeDiwne()
{
	bool interaction_happen = initialize();
	updateEndpoints();
	updateControlPoints();
	updateSquareDistanceMouseFromLink();
	return interaction_happen;
}

bool Link::bypassFocusAction()
{
	return m_squaredDistanceMouseFromLink <
	       (diwne.mp_settingsDiwne->linkThicknessDiwne * diwne.mp_settingsDiwne->linkThicknessDiwne);
}
bool Link::bypassFocusForInteractionAction()
{
	return bypassFocusAction();
}

bool Link::processFocused()
{
	if (bypassTouchAction())
	{
		diwne.setDiwneAction(getTouchActionType());
	}
	diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                          diwne.mp_settingsDiwne->objectFocusBorderColor,
	                          diwne.mp_settingsDiwne->objectFocusBorderThicknessDiwne);
	return true;
}

bool Link::processFocusedForInteraction()
{
	diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                          diwne.mp_settingsDiwne->objectFocusForInteractionBorderColor,
	                          diwne.mp_settingsDiwne->objectFocusForInteractionBorderThicknessDiwne);
	return true;
}

bool Link::content()
{
	if (m_selected)
	{
		diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
		                          diwne.mp_settingsDiwne->linkColorSelected,
		                          diwne.mp_settingsDiwne->linkThicknessDiwne +
		                              diwne.mp_settingsDiwne->linkThicknessSelectedBorderDiwne);
	}
	diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                          diwne.mp_settingsDiwne->linkColor, diwne.mp_settingsDiwne->linkThicknessDiwne);
	return false;
}

} /* namespace DIWNE */
