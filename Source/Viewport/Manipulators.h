#pragma once

#include <unordered_map>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "ImGuizmo.h"

#include "Core/Nodes/Node.h"
#include "Core/Nodes/Transform.h"

namespace Vp
{
class Viewport;

class Manipulators
{
public:
	enum class ManipulatorType
	{
		UNKNOWN,
		UNIMPLEMENTED,
		TRANSLATE,
		ROTATE_X,
		ROTATE_Y,
		ROTATE_Z,
		ROTATE,
		SCALE,
		ORTHO,
		PERSPECTIVE,
		FRUSTUM,
		LOOKAT
	};

	class Manipulator
	{
	public:
		int m_id{-1}; ///< Unique id among the current active manipulators
		ManipulatorType m_type{ManipulatorType::UNKNOWN};

		// TODO: (DR) Should probably be a weak ptr
		// Currently only Transformations are manipulated, can easily be generalized later, this is so that we don't dynamic
		// cast for no reason
		std::shared_ptr<Core::Transformation> m_node; ///< Reference to the selected/manipulated node

		glm::mat4 m_editedMatrix{glm::mat4(1)};    ///< Matrix of the currently selected transformation, the "local" space
		glm::mat4 m_referenceSpace{glm::mat4(1)};  ///< Combined matrix of all transformations prior to selection
		glm::mat4 m_auxillaryMatrix{glm::mat4(1)}; ///< Currently used to store view matrix for projection matrices

		explicit Manipulator(std::shared_ptr<Core::Transformation> node) { m_node = node; }
	};

	Viewport* m_viewport;
	std::vector<std::shared_ptr<Manipulator>> m_activeManipulators;
	std::unordered_map<std::string, ManipulatorType> m_operationMap;

	explicit Manipulators(Viewport* viewport);

	bool drawViewAxes(glm::vec2 windowPos, glm::vec2 windowSize);

	/**
	 * @param windowPos
	 * @param windowSize
	 * @return Whether manipulators have been interacted with.
	 */
	bool drawManipulators(glm::vec2 windowPos, glm::vec2 windowSize);

	// WIP method to set the active manipulator
	void clearManipulators();
	void addManipulator(std::shared_ptr<Core::Node> node);
	ManipulatorType determineManipulatorType(std::shared_ptr<Core::Node> node);

private:
	bool drawLookAt(Ptr<Manipulators::Manipulator> manipulator, glm::mat4 view, glm::mat4 projection);
	void updateManipulatorMatrices(Manipulator& manipulator, std::shared_ptr<Core::Node> node);
};
} // namespace Vp
