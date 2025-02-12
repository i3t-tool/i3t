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
#include "Pin.h"

namespace DIWNE
{
Link::Link(DIWNE::NodeEditor& diwne, std::string const labelDiwne /*="DiwneLink"*/)
    : DiwneObject(diwne, labelDiwne), m_startDiwne(ImVec2(0, 0)), m_endDiwne(ImVec2(0, 0))
{}

void Link::updateSquareDistanceMouseFromLink()
{
	ImVec2 mousePosDiwne = diwne.canvas().screen2diwne(diwne.input().bypassGetMousePos());
	ImVec2 closestPointOnLink =
	    ImBezierCubicClosestPointCasteljau(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                                       mousePosDiwne, ImGui::GetStyle().CurveTessellationTol);
	ImVec2 diff = (closestPointOnLink - mousePosDiwne) / diwne.getZoom();
	m_squaredDistanceMouseFromLink = diff.x * diff.x + diff.y * diff.y;
}

void Link::updateControlPoints()
{
	m_controlPointStartDiwne = m_startDiwne + diwne.mp_settingsDiwne->linkStartControlOffsetDiwne;
	m_controlPointEndDiwne = m_endDiwne + diwne.mp_settingsDiwne->linkEndControlOffsetDiwne;
}

bool Link::isLinkOnWorkArea()
{
	return diwne.canvas().getViewportRectDiwne().Overlaps(getRectDiwne());
}

bool Link::isPlugged()
{
	return m_startPin != nullptr && m_endPin != nullptr;
}

void Link::initialize(DrawInfo& context) {}
void Link::initializeDiwne(DrawInfo& context)
{
	//	diwne.mp_settingsDiwne->linkColor.w =
	//	    m_focusedForInteraction ? diwne.mp_settingsDiwne->linkAlphaHovered : diwne.mp_settingsDiwne->linkAlpha;
	//	diwne.mp_settingsDiwne->linkColorSelected.w = m_focusedForInteraction
	//	                                                  ? diwne.mp_settingsDiwne->linkAlphaSelectedHovered
	//	                                                  : diwne.mp_settingsDiwne->linkAlphaSelected;

	// TODO: Review <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	diwne.mp_settingsDiwne->linkColor.w = diwne.mp_settingsDiwne->linkAlpha;
	diwne.mp_settingsDiwne->linkColorSelected.w = diwne.mp_settingsDiwne->linkAlphaSelected;

	initialize(context);
	updateEndpoints();
	updateControlPoints();
	updateSquareDistanceMouseFromLink();
}

void Link::end(DrawInfo& context)
{
	DIWNE_DEBUG_OBJECTS((diwne), {
		ImVec2 originPos = ImVec2(getRectDiwne().Min.x, getRectDiwne().Min.y);
		ImGui::GetForegroundDrawList()->AddText(
		    diwne.canvas().diwne2screen(originPos) + ImVec2(0, 0), m_destroy ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE,
		    (std::string() + m_labelDiwne + "\nstart: " + (m_startPin ? m_startPin->m_labelDiwne : "null") +
		     "\nend: " + (m_endPin ? m_endPin->m_labelDiwne : "null"))
		        .c_str());
	});
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
	diwne.canvas().AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                                      diwne.mp_settingsDiwne->objectHoverBorderColor,
	                                      diwne.mp_settingsDiwne->objectHoverBorderThicknessDiwne);
}
// bool Link::processFocusedForInteraction()
//{
//	diwne.canvas().AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
//	                          diwne.mp_settingsDiwne->objectFocusForInteractionBorderColor,
//	                          diwne.mp_settingsDiwne->objectFocusForInteractionBorderThicknessDiwne);
//	return true;
//}

void Link::content(DrawInfo& context)
{
	if (m_selected)
	{
		diwne.canvas().AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne,
		                                      m_endDiwne, diwne.mp_settingsDiwne->linkColorSelected,
		                                      diwne.mp_settingsDiwne->linkThicknessDiwne +
		                                          diwne.mp_settingsDiwne->linkThicknessSelectedBorderDiwne);
	}
	diwne.canvas().AddBezierCurveDiwne(m_startDiwne, m_controlPointStartDiwne, m_controlPointEndDiwne, m_endDiwne,
	                                      diwne.mp_settingsDiwne->linkColor,
	                                      diwne.mp_settingsDiwne->linkThicknessDiwne);
	DIWNE_DEBUG(diwne, {
		diwne.canvas().AddLine(m_startDiwne, m_controlPointStartDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
		diwne.canvas().AddLine(m_controlPointStartDiwne, m_controlPointEndDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
		diwne.canvas().AddLine(m_controlPointEndDiwne, m_endDiwne, ImVec4(1.f, 1.f, 1.f, 1.f), true);
	});
}

void Link::updateEndpoints()
{
	Pin* startPin = getStartPin();
	Pin* endPin = getEndPin();
	if (startPin)
		m_startDiwne = startPin->getConnectionPoint();
	if (endPin)
		m_endDiwne = endPin->getConnectionPoint();
}

void Link::setStartPoint(const ImVec2& mStartDiwne)
{
	m_startDiwne = mStartDiwne;
}
void Link::setEndPoint(const ImVec2& mEndDiwne)
{
	m_endDiwne = mEndDiwne;
}
bool Link::connect(Pin* startPin, Pin* endPin, bool logEvent)
{
	// Aside from special cases both ends of the link should be connected or not
	// If both ends are already plugged and the ends differ from the new ones we must unplug them
	if (isPlugged())
	{
		if (m_startPin != startPin)
			disconnectPin(true, logEvent);
		if (m_endPin != endPin)
			disconnectPin(false, logEvent);
	}

	bool alreadyExisted = false;
	assert(startPin != nullptr);
	assert(endPin != nullptr);

	if (!startPin->registerLink(this))
		alreadyExisted = true;
	if (!endPin->registerLink(this))
		alreadyExisted = true;

	setStartPin(startPin);
	setEndPin(endPin);

	startPin->onPlug(endPin, logEvent);
	endPin->onPlug(startPin, logEvent);
	return alreadyExisted;
}

bool Link::disconnect(bool logEvent)
{
	bool success = true;
	success &= disconnectPin(true, logEvent);
	success &= disconnectPin(false, logEvent);
	return success;
}

bool Link::disconnectPin(Pin* pin, bool logEvent)
{
	bool isStartPin = pin == getStartPin();
	bool isEndPin = pin == getEndPin();
	if (!isStartPin && !isEndPin)
	{
		DIWNE_WARN("Attempted to disconnect link from a pin it is not attached to!");
		return false;
	}
	return disconnectPin(isStartPin, logEvent);
}

bool Link::disconnectPin(bool startOrEndPin, bool logEvent)
{
	Pin* pin = startOrEndPin ? getStartPin() : getEndPin();
	if (pin != nullptr)
	{
		if (!pin->unregisterLink(this))
			return false;
	}
	startOrEndPin ? setStartPin(nullptr) : setEndPin(nullptr);

	// Call the onUnplug callback
	if (pin != nullptr)
		pin->onUnplug(logEvent);
	return true;
}

void Link::onDestroy(bool logEvent)
{
	disconnect(logEvent);
}

} /* namespace DIWNE */
