#include "Manipulators.h"

#include "glm/gtc/matrix_transform.hpp"
#include "magic_enum.hpp"

#include "Viewport.h"

#include "Core/Nodes/Camera.h"
#include "Core/Nodes/Tracking.h"
#include "Core/Nodes/Transform.h"
#include "GUI/Toolkit.h"
#include "Logger/Logger.h"
#include "Utils/Format.h"

using namespace Vp;

// TODO: (DR) MOVE MANIPULATORS INTO THE GUI PACKAGE ALONG WITH VIEWPORT HIGHLIGHTER
//  This class builds upon viewport but also manages nodes, thus it shouldn't be in the viewport package
//  WorkspaceDiwne should also probably be the one holding a reference to it as it invokes it and callbacks will need to
//  be registered with nodes

Manipulators::Manipulators(Viewport* viewport) : m_viewport(viewport)
{
	m_operationMap.insert(std::make_pair("Translation", ManipulatorType::TRANSLATE));
	m_operationMap.insert(std::make_pair("Scale", ManipulatorType::SCALE));
	m_operationMap.insert(std::make_pair("EulerX", ManipulatorType::ROTATE_X));
	m_operationMap.insert(std::make_pair("EulerY", ManipulatorType::ROTATE_Y));
	m_operationMap.insert(std::make_pair("EulerZ", ManipulatorType::ROTATE_Z));
	m_operationMap.insert(std::make_pair("AxisAngle", ManipulatorType::ROTATE));
	m_operationMap.insert(std::make_pair("Quat", ManipulatorType::ROTATE));

	m_operationMap.insert(std::make_pair("Ortho", ManipulatorType::ORTHO));
	m_operationMap.insert(std::make_pair("Frustum", ManipulatorType::FRUSTUM));
	m_operationMap.insert(std::make_pair("Perspective", ManipulatorType::PERSPECTIVE));
	m_operationMap.insert(std::make_pair("LookAt", ManipulatorType::LOOKAT));

	m_operationMap.insert(std::make_pair("Free", ManipulatorType::UNIMPLEMENTED));
}

bool Manipulators::draw(glm::vec2 windowPos, glm::vec2 windowSize)
{
	// ImGizmo test
	glm::mat4 view = m_viewport->getMainViewportCamera().lock()->getView();
	glm::mat4 projection = m_viewport->getMainViewportCamera().lock()->getProjection();

	glm::mat4 objectMatrix = glm::mat4(1);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

	// ImGuizmo::DrawGrid(glm::value_ptr(view), glm::value_ptr(projection), glm::value_ptr(glm::mat4(1)), 100.f);
	// ImGuizmo::DrawCubes(glm::value_ptr(view), glm::value_ptr(projection), &objectMatrix[0][0], 1);

	int axesSize = 98;
	int padding = 8;
	ImVec2 axesPosition = ImVec2(windowPos.x + windowSize.x - axesSize - padding, windowPos.y + padding);

	// ImGuizmo::ViewManipulate(glm::value_ptr(view), 9, GUI::glmToIm(windowPos), ImVec2(128, 128), 0x10101010);
	ImGuizmo::ViewAxes(glm::value_ptr(view), glm::value_ptr(projection), 9, axesPosition, ImVec2(axesSize, axesSize));

	bool interactedWithManipulator = false;

	for (int id = 0; id < m_activeManipulators.size(); id++)
	{
		const auto& manipulator = m_activeManipulators[id];
		manipulator->m_id = id;
		const ManipulatorType type = manipulator->m_type;

		if (type == ManipulatorType::UNKNOWN || type == ManipulatorType::UNIMPLEMENTED)
			continue;

		// TODO: (DR) This shouldn't run every frame!! Needs to be called on updateValues, so could use the update callback?
		// Update the manipulator with the latest data
		updateManipulatorMatrices(*manipulator, manipulator->m_node);

		// IMGUIZMO NOTE:
		// ImGuizmo's matrix_t * operator multiplies the other way around!
		// In ImGuizmo A * B is actually equivalent to B * A in GLM!
		// It's important to keep that in mind when browsing ImGuizmo source code!

		ImGuizmo::SetGizmoSizeClipSpace(m_viewport->getSettings().manipulator_size);
		// ImGuizmo::GetStyle().RotationOuterLineThickness = 4.0f;
		ImGuizmo::GetStyle().RotationLineThickness = 2.5f;

		if (type == ManipulatorType::LOOKAT)
		{
			if (drawLookAt(manipulator, view, projection))
			{
				interactedWithManipulator = true;
			}
			continue;
		}

		// "Local" space change from the manipulator
		glm::mat4 deltaMatrix = glm::mat4(1);

		// Construct combined matrix, combined "reference" space and "local" space
		const glm::mat4 combinedMatrix = manipulator->m_referenceSpace * manipulator->m_editedMatrix;
		glm::mat4 combinedMatrixMutable; // Mutable copy of the combined matrix, modified by manipulator
		combinedMatrixMutable = combinedMatrix;

		// Edited array of projection parameters in case for projection manipulators
		float projectionParams[6] = {0};

		// Draw manipulator
		ImGuizmo::SetID(manipulator->m_id); // Required to interact with multiple manipulators at the same time

		bool changeOccurred = false;
		if (type == ManipulatorType::ORTHO || type == ManipulatorType::PERSPECTIVE || type == ManipulatorType::FRUSTUM)
		{
			ImGuizmo::ManipulateProjection(glm::value_ptr(view), glm::value_ptr(projection),
			                               glm::value_ptr(combinedMatrixMutable),
			                               glm::value_ptr(manipulator->m_auxillaryMatrix), projectionParams);

			for (int i = 0; i < 6; i++)
			{
				if (projectionParams[i] != 0)
				{
					changeOccurred = true;
					break;
				}
			}
		}
		else
		{
			int imGuizmoOperation = 0;
			switch (type)
			{
			case ManipulatorType::TRANSLATE:
				imGuizmoOperation = ImGuizmo::TRANSLATE;
				break;
			case ManipulatorType::ROTATE:
				// imGuizmoOperation = ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z;
				imGuizmoOperation = ImGuizmo::ROTATE;
				break;
			case ManipulatorType::ROTATE_X:
				imGuizmoOperation = ImGuizmo::ROTATE_X;
				break;
			case ManipulatorType::ROTATE_Y:
				imGuizmoOperation = ImGuizmo::ROTATE_Y;
				break;
			case ManipulatorType::ROTATE_Z:
				imGuizmoOperation = ImGuizmo::ROTATE_Z;
				break;
			case ManipulatorType::SCALE:
				imGuizmoOperation = ImGuizmo::SCALE;
				break;
			default:
				throw std::runtime_error("Invalid manipulator type!");
			}

			// Draw and handle the manipulator
			bool modified = ImGuizmo::Manipulate(
			    glm::value_ptr(view), glm::value_ptr(projection), static_cast<ImGuizmo::OPERATION>(imGuizmoOperation),
			    ImGuizmo::LOCAL, glm::value_ptr(combinedMatrixMutable), glm::value_ptr(deltaMatrix), NULL, NULL, NULL);

			changeOccurred = deltaMatrix != glm::mat4(1);
		}

		// Update the data if the manipulator is being actively used
		if (!ImGuizmo::IsUsing())
			continue;

		interactedWithManipulator = true;

		if (!changeOccurred)
			continue;

		// Process changes and update manipulator's node with new values

		if (type == ManipulatorType::ORTHO || type == ManipulatorType::FRUSTUM)
		{
			// Change the default value that was modified
			const static std::string paramNames[6] = {"left", "right", "bottom", "top", "near", "far"};
			for (int i = 0; i < 6; i++)
			{
				if (Math::eq(projectionParams[i], 0))
					continue;
				ValueSetResult result = manipulator->m_node->setDefaultValue(
				    paramNames[i], manipulator->m_node->getDefaultValue(paramNames[i]).getFloat() + projectionParams[i]);
				if (result.status != ValueSetResult::Status::Ok)
				{
					LOG_WARN("Manipulators: Failed to set value for manipulator of type: {}! Status: {}, Message: {}", n(type),
					         n(result.status), result.message);
				}
				break;
			}
		}
		else if (type == ManipulatorType::PERSPECTIVE)
		{
			// Get original projection parameters
			glm::mat4 projinv = glm::inverse(combinedMatrixMutable);
			glm::vec4 p = projinv * glm::vec4(-1.0f, 0.0f, -1.0f, 1.0f);
			p /= p[3];
			float left = p[0];
			p = projinv * glm::vec4(1.0f, 0.0f, -1.0f, 1.0f);
			p /= p[3];
			float right = p[0];
			p = projinv * glm::vec4(0.0f, 1.0f, -1.0f, 1.0f);
			p /= p[3];
			float top = p[1];
			p = projinv * glm::vec4(0.0f, -1.0f, -1.0f, 1.0f);
			p /= p[3];
			float bottom = p[1];
			p = projinv * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
			p /= p[3];
			float near = -p[2];
			p = projinv * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			p /= p[3];
			float far = -p[2];

			// Apply delta
			const static std::string paramNames[6] = {"left", "right", "bottom", "top", "near", "far"};
			for (int i = 0; i < 6; i++)
			{
				if (Math::eq(projectionParams[i], 0))
					continue;
				ValueSetResult result;
				// left and right
				if (i == 0 || i == 1)
				{
					left += projectionParams[0];
					right += projectionParams[1];
					float aspect = (right - left) / (top - bottom);
					result = manipulator->m_node->setDefaultValue("aspect", aspect);
				}
				// bottom and top
				if (i == 2 || i == 3)
				{
					bottom += projectionParams[2];
					top += projectionParams[3];
					float angle = 2.0f * glm::degrees(atan((0.5f * (top - bottom)) / (near)));
					result = manipulator->m_node->setDefaultValue("fovy", glm::radians(angle));
				}
				// near and far
				if (i == 4 || i == 5)
				{
					result = manipulator->m_node->setDefaultValue(
					    paramNames[i], manipulator->m_node->getDefaultValue(paramNames[i]).getFloat() + projectionParams[i]);
				}
				if (result.status != ValueSetResult::Status::Ok)
				{
					LOG_WARN("Manipulators: Failed to set value for manipulator of type: {}! Status: {}, Message: {}", n(type),
					         n(result.status), result.message);
				}
				break;
			}
		}
		else
		{
			glm::mat4 resultMatrix = glm::mat4(1.0f);
			switch (type)
			{
			case ManipulatorType::SCALE:
			{
				if (manipulator->m_node->hasSynergies())
				{
					// Uniform scaling
					glm::mat4 localDelta = glm::inverse(combinedMatrix) * deltaMatrix * combinedMatrix;
					float scaleDelta = 1.0f;
					if (Math::eq(localDelta[0][0], localDelta[1][1]))
					{
						scaleDelta = localDelta[2][2];
					}
					else if (Math::eq(localDelta[0][0], localDelta[2][2]))
					{
						scaleDelta = localDelta[1][1];
					}
					else if (Math::eq(localDelta[1][1], localDelta[2][2]))
					{
						scaleDelta = localDelta[0][0];
					}
					resultMatrix = manipulator->m_editedMatrix * glm::scale(glm::mat4(1), glm::vec3(scaleDelta));
					break;
				}
			}
			case ManipulatorType::TRANSLATE:
			case ManipulatorType::ROTATE_X:
			case ManipulatorType::ROTATE_Y:
			case ManipulatorType::ROTATE_Z:
			case ManipulatorType::ROTATE:
			{
				glm::mat4 localDelta = glm::inverse(combinedMatrix) * deltaMatrix * combinedMatrix;
				resultMatrix = manipulator->m_editedMatrix * localDelta;
			}
			break;
			default:
				throw std::runtime_error("Invalid manipulator type!!");
			}

			// Set the new matrix value, defaults should be updated within setValue by the respective implementation
			ValueSetResult result = manipulator->m_node->setValue(resultMatrix);
			if (result.status != ValueSetResult::Status::Ok)
			{
				LOG_WARN("Manipulators: Failed to set value for manipulator of type: {}! Status: {}, Message: {}", n(type),
				         n(result.status), result.message);
			}
		}
	}
	return interactedWithManipulator;
}

bool Manipulators::drawLookAt(Ptr<Manipulators::Manipulator> manipulator, glm::mat4 view, glm::mat4 projection)
{
	bool interacted = false;

	const static std::string pointNames[2] = {"eye", "center"};
	glm::vec3 points[2] = {manipulator->m_node->getDefaultValue(pointNames[0]).getVec3(),
	                       manipulator->m_node->getDefaultValue(pointNames[1]).getVec3()};

	// Draw line between eye and center
	// TODO: (DR) Add some indication of direction (a cube? a point? an arrow?, is a little tricky with imguizmo)
	ImGuizmo::DrawInit(glm::value_ptr(view), glm::value_ptr(projection));
	ImGuizmo::DrawLine(glm::value_ptr(points[0]), glm::value_ptr(points[1]), IM_COL32(0xFF, 0xFF, 0xFF, 200), 2.0f);

	// Draw point manipulators
	for (int i = 0; i < 2; i++)
	{
		glm::mat4 deltaMatrix = glm::mat4(1);

		const glm::vec3 point = points[i];
		const glm::mat4 pointMat = glm::translate(point);

		const glm::mat4 combinedMatrix = manipulator->m_referenceSpace * pointMat;
		glm::mat4 combinedMatrixMutable; // Mutable copy of the combined matrix, modified by manipulator
		combinedMatrixMutable = combinedMatrix;

		ImGuizmo::SetID(((manipulator->m_id + 1) * 1000) + i);
		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), ImGuizmo::TRANSLATE, ImGuizmo::LOCAL,
		                     glm::value_ptr(combinedMatrixMutable), glm::value_ptr(deltaMatrix), NULL, NULL, NULL);

		if (ImGuizmo::IsUsing())
		{
			interacted = true;
		}

		// Update the data if the manipulator is being actively used
		if (interacted && deltaMatrix != glm::mat4(1))
		{
			glm::mat4 localDelta = glm::inverse(combinedMatrix) * deltaMatrix * combinedMatrix;
			glm::mat4 resultMatrix = pointMat * localDelta;

			ValueSetResult result = manipulator->m_node->setDefaultValue(pointNames[i], glm::vec3(resultMatrix[3]));
			;
			if (result.status != ValueSetResult::Status::Ok)
			{
				LOG_WARN("Manipulators: Failed to set value for manipulator of type: {}! Status: {}, Message: {}",
				         n(manipulator->m_type), n(result.status), result.message);
			}
		}
	}
	return interacted;
}

void Manipulators::clearManipulators() { m_activeManipulators.clear(); }

void Manipulators::addManipulator(Ptr<Core::Node> node)
{
	I3T_ASSERT(node != nullptr, "Manipulator's node is null!");
	auto transformation = std::dynamic_pointer_cast<Core::Transformation>(node);
	I3T_ASSERT(transformation != nullptr, "Only transformation manipulators are implemented!");
	auto manipulator = std::make_shared<Manipulator>(transformation);
	manipulator->m_type = determineManipulatorType(node);
	m_activeManipulators.push_back(manipulator);
}

Manipulators::ManipulatorType Manipulators::determineManipulatorType(std::shared_ptr<Core::Node> node)
{
	const char* keyword = node->getOperation()->keyWord.c_str();
	ManipulatorType type;
	auto it = m_operationMap.find(keyword);
	if (it != m_operationMap.end())
	{
		type = it->second;
	}
	else
	{
		type = ManipulatorType::UNKNOWN;
	}
	return type;
}

void Manipulators::updateManipulatorMatrices(Manipulator& manipulator, std::shared_ptr<Core::Node> node)
{
	manipulator.m_editedMatrix = glm::mat4(node->getData().getMat4());
	manipulator.m_referenceSpace = glm::mat4(1);

	// Get reference space if applicable
	auto sequence = node->as<Core::Transformation>()->getCurrentSequence();
	if (sequence == nullptr)
		return;

	// Special projection node handling
	if (sequence->getOwner() != nullptr)
	{
		if (auto camera = std::dynamic_pointer_cast<Core::Camera>(sequence->getOwner()))
		{
			if (camera->getProj() == sequence)
			{
				glm::mat4 viewMatrix(1.0f);
				auto matrices = camera->getView()->getMatrices();
				for (const auto& mat : matrices)
				{
					viewMatrix *= mat->getData().getMat4();
				}
				manipulator.m_auxillaryMatrix = viewMatrix;
			}
		}
	}

	// Iterate over all transformations prior to this one to create reference space
	glm::mat4 result = glm::mat4(1);
	auto st = Core::SequenceTree(sequence);
	auto it = st.begin();

	// Move the iterator to the transformation before this one
	while (it != st.end())
	{
		auto n = *it;
		// printf("skip %s\n", nodeindex.get()->getOperation()->keyWord.c_str());
		// printf("%p %p\n",nodeindex.get(),node->get());
		it++;
		if (n == node)
		{
			break;
		}
	}

	// Iterate over the rest and multiply together their matrices
	while (it != st.end())
	{
		auto n = *it;
		result = n->getData().getMat4() * result;
		++it;
	}
	manipulator.m_referenceSpace = result;
}
