/**
 * \file
 * \brief Class for representing color helper class
 * \author Michal Folta <michal.fol@gmail.com>
 * \date 2014/11/16
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <glm/glm.hpp>

#include <cstdlib>

struct Color final
{
	static glm::vec3 WHITE;
	static glm::vec3 BLACK;
	static glm::vec3 LIGHT_GRAY;
	static glm::vec3 GRAY;
	static glm::vec3 DARK_GRAY;
	static glm::vec3 RED;
	static glm::vec3 GREEN;
	static glm::vec3 BLUE;
	static glm::vec3 YELLOW;
	static glm::vec3 TEAL;
	static glm::vec3 MAGENTA;
	static glm::vec3 LIGHT_BLUE;
	static glm::vec3 ORANGE;
	static glm::vec3 BROWN;

	static glm::vec3 getColor(const float r, const float g, const float b)
	{
		return glm::vec3(r, g, b);
	}

	static glm::vec3 randomColor()
	{
		return glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
	}
};
