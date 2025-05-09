/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Screen.h"

namespace Core
{
Ptr<Screen> Builder::createScreen()
{
	auto ret = std::make_shared<Core::Screen>();
	ret->init();
	ret->updateValues(0);
	return ret;
}

Screen::Screen() : Node(g_screen) {}

void Screen::updateValues(int inputIndex)
{
	if (areAllInputsPlugged())
	{
		assert(inputIndex == 0);
		m_screenData = m_inputs[inputIndex].data().getScreen();
		setInternalValue(m_outputs[I3T_SCREEN_OUT_WIDTH].data().getFloat() /
		                     m_outputs[I3T_SCREEN_OUT_HEIGHT].data().getFloat(),
		                 I3T_SCREEN_OUT_ASPECT);
		Node::updateValues(inputIndex);
	}
}

const glm::mat4& Screen::getProjection() const
{
	return m_screenData.m_projection;
}
const glm::mat4& Screen::getView() const
{
	return m_screenData.m_view;
}
const glm::mat4& Screen::getViewport() const
{
	return m_screenData.m_viewport;
}
bool Screen::isViewportExplicit() const
{
	return !m_screenData.m_implicitViewportEnabled;
}

} // namespace Core