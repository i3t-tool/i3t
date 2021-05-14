#pragma once

#include "Node.h"
#include "Sequence.h"

namespace Core
{
class Camera : public NodeBase
{
	friend class GraphManager;

	SequencePtr m_proj;
	SequencePtr m_view;

public:
	Camera();

	const SequencePtr& getProj() { return m_proj; }
	const SequencePtr& getView() { return m_view; }

	void updateValues(int inputIndex) override;
};

using CameraPtr = Ptr<Camera>;
} // namespace Core
