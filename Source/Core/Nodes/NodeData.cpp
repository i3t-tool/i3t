/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "NodeData.h"

namespace Core
{
Data::Data(EValueType valueType) : opValueType(valueType)
{
	switch (valueType)
	{
	case EValueType::Pulse:
		setValue(false);
		break;
	case EValueType::Ptr:
	case EValueType::Screen:
		setValue(std::make_pair(glm::mat4(1.0f), glm::mat4(1.0f)));
		break;
	case EValueType::Float:
		setValue(0.0f);
		break;
	case EValueType::Vec3:
		setValue(glm::vec3());
		break;
	case EValueType::Vec4:
		setValue(glm::vec4());
		break;
	case EValueType::Quat:
		setValue(glm::quat());
		break;
	case EValueType::Matrix:
	case EValueType::MatrixMul:
		setValue(glm::mat4(1.0f));
	default:
		break;
	}
}
} // namespace Core
