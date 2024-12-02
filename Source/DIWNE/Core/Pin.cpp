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
#include "Pin.h"

#include "Logger/Logger.h"
#include "NodeEditor.h"
#include "diwne_actions.h"

namespace DIWNE
{

Pin::Pin(DIWNE::NodeEditor& diwne, std::string const labelDiwne /*="DiwnePin"*/)
    : DiwneObject(diwne, labelDiwne), m_connectionPointDiwne(ImVec2(0, 0))
{
	setSelectable(false);
}

void Pin::begin(DrawInfo& context)
{
	ImGui::PushID(m_labelDiwne.c_str());
	ImGui::BeginGroup();
}

void Pin::end(DrawInfo& context)
{
	ImGui::EndGroup();
	ImGui::PopID();
	m_internalHover = ImGui::IsItemHovered();
}

void Pin::updateLayout(DrawInfo& context)
{
	m_rect.Min = diwne.screen2diwne(ImGui::GetItemRectMin());
	m_rect.Max = diwne.screen2diwne(ImGui::GetItemRectMax());

	updateConnectionPointDiwne();
}

void Pin::processInteractions(DrawInfo& context)
{
	DiwneObject::processInteractions(context);

	// Check if another pin was dragged over this one
	bool connectActionActive = context.action == Actions::connectPin && context.actionSource != m_labelDiwne;
	if (m_hovered && context.dragging && connectActionActive && allowConnection())
	{
		onPlug(!context.dragEnd);
	}
}

void Pin::onDrag(DrawInfo& context, bool dragStart, bool dragEnd)
{
	DiwneObject::onDrag(context, dragStart, dragEnd);

	if (dragStart)
	{
		diwne.setLastActivePin(std::static_pointer_cast<DIWNE::Pin>(shared_from_this()));
		context.setAction(Actions::connectPin, m_labelDiwne, nullptr);
	}
	if (dragEnd)
	{
		context.clearAction();
	}
}

void Pin::onPlug(bool hovering) {}
bool Pin::allowPopup() const
{
	return false;
}
bool Pin::allowConnection() const
{
	return true;
}
bool Pin::allowDrag() const
{
	return DiwneObject::allowDrag() && allowConnection();
}

} /* namespace DIWNE */
