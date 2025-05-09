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
#pragma once

#include "Node.h"

namespace Core
{
class Screen;

namespace Builder
{
Ptr<Screen> createScreen();
}

constexpr size_t I3T_SCREEN_IN_DISPLAY = 0;
constexpr size_t I3T_SCREEN_OUT_WIDTH = 0;
constexpr size_t I3T_SCREEN_OUT_HEIGHT = 1;
constexpr size_t I3T_SCREEN_OUT_ASPECT = 2;

class Screen : public Node
{
	friend class GraphManager;

public:
	ScreenData m_screenData; ///< Storage for the data received as input

	Screen();

	const glm::mat4& getProjection() const;
	const glm::mat4& getView() const;
	const glm::mat4& getViewport() const;

	/// When true, the passed viewport transformation is used, instead of an auto-generated one.
	bool isViewportExplicit() const;

	void updateValues(int inputIndex) override;
};

} // namespace Core