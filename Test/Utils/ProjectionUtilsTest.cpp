/**
 * \file
 * \brief
 * \authors Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Utils/ProjectionUtils.h"


#include "gtest/gtest.h"

#include "Utils/Math.h"

#include "Utils/Format.h"

TEST(ProjectionUtilsTest, decomposePerspective)
{
	float fL = -0.8f;
	float fR = +0.8f;
	float fB = -0.5f;
	float fT = +0.5f;
	float fN = 1.0f;
	float fF = 10.0f;
	glm::mat4 persp = glm::frustum(fL, fR, fB, fT, fN, fF);
	auto [l, r, b, t, n, f] = ProjectionUtils::decomposePerspective(persp);

	ASSERT_TRUE(Math::compare(glm::frustum(l, r, b, t, n, f), persp, Math::FACTOR_ROUGHLY_SIMILAR))
	    << "l: " << fL << " vs " << l << //
	    "\nr: " << fR << " vs " << r <<  //
	    "\nb: " << fB << " vs " << b <<  //
	    "\nt: " << fT << " vs " << t <<  //
	    "\nn: " << fN << " vs " << n <<  //
	    "\nf: " << fF << " vs " << f;
}
