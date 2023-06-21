#pragma once

#include "Node.h"
#include "Sequence.h"

namespace Core
{
class Camera;

namespace Builder
{
Ptr<Camera> createCamera();
}

constexpr unsigned I3T_CAMERA_OUT_SCREEN = 0;
constexpr unsigned I3T_CAMERA_OUT_MATRIX = 1;
constexpr unsigned I3T_CAMERA_OUT_MUL = 2;

/**
 * Camera has following outputs:
 * 0 -> screen output
 * 1 -> proj * view matrix output
 * 2 -> mul output
 */
class Camera : public Node
{
	friend class GraphManager;

	Ptr<Sequence> m_proj;
	Ptr<Sequence> m_view;

public:
	/// Latest projection matrix value
	glm::mat4 m_projectionMatrix;
	/// Latest view matrix value
	glm::mat4 m_viewMatrix;

	Camera();

	Ptr<Node> clone() override;

	void createComponents();

	const SequencePtr& getProj()
	{
		return m_proj;
	}
	const SequencePtr& getView()
	{
		return m_view;
	}

	void updateValues(int inputIndex) override;
};

using CameraPtr = Ptr<Camera>;
} // namespace Core
