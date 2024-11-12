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
Link::Link(DIWNE::NodeEditor& diwne, std::string const labelDiwne /*="DiwneLink"*/)
    : DiwneObject(diwne, labelDiwne), m_startDiwne(ImVec2(0, 0)), m_endDiwne(ImVec2(0, 0)), m_just_pluged(false)
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

void Link::initialize(DrawInfo& context) {}
void Link::initializeDiwne(DrawInfo& context)
{
	diwne.mp_settingsDiwne->linkColor.w =
	    m_focusedForInteraction ? diwne.mp_settingsDiwne->linkAlphaHovered : diwne.mp_settingsDiwne->linkAlpha;
	diwne.mp_settingsDiwne->linkColorSelected.w = m_focusedForInteraction
	                                                  ? diwne.mp_settingsDiwne->linkAlphaSelectedHovered
	                                                  : diwne.mp_settingsDiwne->linkAlphaSelected;

	initialize(context);
	updateEndpoints();
	updateControlPoints();
	updateSquareDistanceMouseFromLink();
}

void Link::updateLayout(DrawInfo& context)
{
	m_rect = ImRect(std::min({m_controlPointStartDiwne.x, m_startDiwne.x, m_controlPointEndDiwne.x, m_endDiwne.x}),
	                std::min({m_controlPointStartDiwne.y, m_startDiwne.y, m_controlPointEndDiwne.y, m_endDiwne.y}),
	                std::max({m_controlPointStartDiwne.x, m_startDiwne.x, m_controlPointEndDiwne.x, m_endDiwne.x}),
	                std::max({m_controlPointStartDiwne.y, m_startDiwne.y, m_controlPointEndDiwne.y, m_endDiwne.y}));
}

/// Link isn't represented by an ImGui item so we need to detect hovering manually.
bool Link::isHoveredDiwne()
{
	return m_squaredDistanceMouseFromLink <
	       (diwne.mp_settingsDiwne->linkThicknessDiwne * diwne.mp_settingsDiwne->linkThicknessDiwne);
}

void Link::onHover(DrawInfo& context)
{
	// TODO: Investigate what the touch action is for
	//	if (bypassTouchAction())
	//	{
	//		diwne.setDiwneAction(getTouchActionType());
	//	}
	diwne.m_renderer->AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                                      diwne.mp_settingsDiwne->objectHoverBorderColor,
	                                      diwne.mp_settingsDiwne->objectHoverBorderThicknessDiwne);
}
// bool Link::processFocusedForInteraction()
//{
//	diwne.m_renderer->AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
//	                          diwne.mp_settingsDiwne->objectFocusForInteractionBorderColor,
//	                          diwne.mp_settingsDiwne->objectFocusForInteractionBorderThicknessDiwne);
//	return true;
//}

void Link::content(DrawInfo& context)
{
	if (m_selected)
	{
		diwne.m_renderer->AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne,
		                                      m_endDiwne, diwne.mp_settingsDiwne->linkColorSelected,
		                                      diwne.mp_settingsDiwne->linkThicknessDiwne +
		                                          diwne.mp_settingsDiwne->linkThicknessSelectedBorderDiwne);
	}
	diwne.m_renderer->AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                                      diwne.mp_settingsDiwne->linkColor,
	                                      diwne.mp_settingsDiwne->linkThicknessDiwne);
	DIWNE_DEBUG(diwne, {
		diwne.m_renderer->AddLine(m_startDiwne, m_controlPointStartDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
		diwne.m_renderer->AddLine(m_controlPointStartDiwne, m_controlPointEndDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
		diwne.m_renderer->AddLine(m_controlPointEndDiwne, m_endDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
	});
}

} /* namespace DIWNE */
