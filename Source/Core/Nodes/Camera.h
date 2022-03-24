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

/**
 * Camera has following outputs:
 * 0 -> screen output
 * 1 -> proj * view matrix output
 * 2 -> mul output
 */
class Camera : public Node
{
	friend class GraphManager;

	SequencePtr m_proj;
	SequencePtr m_view;

public:
	Camera();

	Ptr<Node> clone() override;

	void createComponents();

	const SequencePtr& getProj() { return m_proj; }
	const SequencePtr& getView() { return m_view; }

	void updateValues(int inputIndex) override;
};

using CameraPtr = Ptr<Camera>;
} // namespace Core
