#pragma once

#include "WorkspaceElementsWithCoreData.h"
#include "WorkspaceSequence.h"

#include "Core/Nodes/GraphManager.h"

namespace Vp
{
class SceneCamera;
}

class WorkspaceCamera : public WorkspaceNodeWithCoreDataWithPins
{
protected:
	// TODO: (DR) I think these pointers are unused
	Ptr<WorkspaceSequence> m_projection = nullptr;
	Ptr<WorkspaceSequence> m_view = nullptr;

public:
	bool m_axisOn{true};
	bool m_showCamera{true};
	bool m_showFrustum{true};
	bool m_fillFrustum{true};
	glm::vec3 m_frustumColor{0.35f, 0.27f, 0.06f};
	glm::vec3 m_frustumOutlineColor{1.f, 0.f, 0.f};

	std::weak_ptr<Vp::SceneCamera> m_viewportCamera;

	WorkspaceCamera(DIWNE::Diwne& diwne);
	~WorkspaceCamera();

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceCamera>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	Ptr<WorkspaceSequence> const& getProjection() const
	{
		return m_projection;
	};
	Ptr<WorkspaceSequence> const& getView() const
	{
		return m_view;
	};

	// bool drawDataFull(DIWNE::Diwne &diwne){return false;}; /* camera has no
	// data */
	bool topContent() override;
	bool middleContent() override;

	void drawMenuLevelOfDetail() override;

	void popupContent() override;

	int maxLengthOfData() override;

	bool isCamera();

	//	bool leftContent(DIWNE::Diwne &diwne);
	//	bool rightContent(DIWNE::Diwne &diwne);

	bool processSelect();
	bool processUnselect();

private:
	glm::vec3 calculateFrustumColor(glm::vec3 color);
};
