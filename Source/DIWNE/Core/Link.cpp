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
#include "Link.h"

#include "NodeEditor.h"

namespace DIWNE
{
Link::Link(DIWNE::NodeEditor& diwne, DIWNE::ID id, std::string const labelDiwne /*="DiwneLink"*/)
    : DiwneObject(diwne, id, labelDiwne), m_startDiwne(ImVec2(0, 0)), m_endDiwne(ImVec2(0, 0)), m_just_pluged(false)
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

void Link::initialize(FrameContext& context)
{
	diwne.mp_settingsDiwne->linkColor.w =
	    m_focusedForInteraction ? diwne.mp_settingsDiwne->linkAlphaHovered : diwne.mp_settingsDiwne->linkAlpha;
	diwne.mp_settingsDiwne->linkColorSelected.w = m_focusedForInteraction
	                                                  ? diwne.mp_settingsDiwne->linkAlphaSelectedHovered
	                                                  : diwne.mp_settingsDiwne->linkAlphaSelected;
}
void Link::initializeDiwne(FrameContext& context)
{
	initialize(context);
	updateEndpoints();
	updateControlPoints();
	updateSquareDistanceMouseFromLink();
}

bool Link::isHovered()
{
	return m_squaredDistanceMouseFromLink <
	       (diwne.mp_settingsDiwne->linkThicknessDiwne * diwne.mp_settingsDiwne->linkThicknessDiwne);
}
bool Link::bypassFocusForInteractionAction()
{
	return isHovered();
}
//
//bool Link::processFocused()
//{
//	if (bypassTouchAction())
//	{
//		diwne.setDiwneAction(getTouchActionType());
//	}
//	diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
//	                          diwne.mp_settingsDiwne->objectFocusBorderColor,
//	                          diwne.mp_settingsDiwne->objectFocusBorderThicknessDiwne);
//	return true;
//}
//
//bool Link::processFocusedForInteraction()
//{
//	diwne.AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
//	                          diwne.mp_settingsDiwne->objectFocusForInteractionBorderColor,
//	                          diwne.mp_settingsDiwne->objectFocusForInteractionBorderThicknessDiwne);
//	return true;
//}

void Link::content(FrameContext& context)
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
	DIWNE_DEBUG(diwne, {
		diwne.AddLine(m_startDiwne, m_controlPointStartDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
		diwne.AddLine(m_controlPointStartDiwne, m_controlPointEndDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
		diwne.AddLine(m_controlPointEndDiwne, m_endDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
	});
}

} /* namespace DIWNE */
