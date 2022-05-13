#include "NodeData.h"

const std::string& valueTypeToString(EValueType type)
{
	static const std::string names[] = {
			"Pulse",
			"Float", ///< standard data type
			"Vec3",
			"Vec4",
			"Matrix",
			"Quat",
			"MatrixMul", ///< connection of sequences in the scene graph - represents a matrix multiplication
			"Screen",    ///< projection and camera view transformation
			"Ptr",
	};
	return names[static_cast<int>(type)];
}

DataStore::DataStore(EValueType valueType) : opValueType(valueType)
{
	switch (valueType)
	{
	case EValueType::Pulse:
		setValue(false);
		break;
	case EValueType::Ptr:
	case EValueType::Screen:
		// setValue((void*)nullptr);
		setValue(glm::mat4(1.0f));
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
