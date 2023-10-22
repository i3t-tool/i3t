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

#include "AbstractCamera.h"

namespace Vp
{
/**
 * Abstract camera that turns around a point (pivot) at a certain distance (radius).
 */
class AbstractOrbitCamera : public AbstractCamera
{
protected:
	glm::vec3 m_pivot = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_radius = 7.0f;

public:
	void centerOnBox(glm::vec3 boxMin, glm::vec3 boxMax, bool interpolate) override;

	glm::vec3 getPivot() const;
	void setPivot(const glm::vec3& pivot);

	void setRadius(float radius);
	float getRadius() const;
};

} // namespace Vp
