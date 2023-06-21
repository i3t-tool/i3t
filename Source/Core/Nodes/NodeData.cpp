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
