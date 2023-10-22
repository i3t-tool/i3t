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
#include "GfxUtils.h"

namespace Vp
{
namespace GfxUtils
{
void extractZNearZFar(glm::mat4 projection, float& zNear, float& zFar)
{
	// Extract near and far values from projection matrix
	// Note that using this method there can be a relatively "HUGE" precision error
	// It might not suitable to use this method for something like comparing depth values (as near/far values will
	// shift)
	if (projection[2][3] == 0)
	{
		// Orthographic matrix
		float m32 = projection[3][2];
		float m22 = projection[2][2];
		zNear = (m32 + 1) / m22;
		zFar = (m32 - 1) / m22;
	}
	else
	{
		// Perspective matrix
		float m32 = projection[3][2];
		float m22 = projection[2][2];
		zNear = m32 / (m22 - 1);
		zFar = m32 / (m22 + 1);
	}
}
} // namespace GfxUtils
} // namespace Vp