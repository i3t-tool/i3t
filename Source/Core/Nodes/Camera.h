#pragma once

#include "Node.h"
#include "Sequence.h"

namespace Core
{
class Camera : public NodeBase
{
SequencePtr m_proj;
SequencePtr m_view;

public:
	Camera();

  const Matrices& getProjMatrices() { return m_proj->getMatrices(); }
  const Matrices& getViewMatrices() { return m_view->getMatrices(); }

  const glm::mat4& getProjResult() { return m_proj->getData().getMat4(); }
  const glm::mat4& getViewResult() { return m_view->getData().getMat4(); }

	void updateValues(int inputIndex) override;
};

using CameraPtr = Ptr<Camera>;
}
