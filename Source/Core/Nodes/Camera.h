#pragma once

#include "Node.h"
#include "Sequence.h"

namespace Core
{
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

	void createComponents();

	const SequencePtr& getProj() { return m_proj; }
	const SequencePtr& getView() { return m_view; }

	void updateValues(int inputIndex) override;
};

using CameraPtr = Ptr<Camera>;
} // namespace Core
