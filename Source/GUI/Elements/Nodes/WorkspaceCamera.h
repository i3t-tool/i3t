/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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

	bool processSelect() override;
	bool processUnselect() override;
	float updateDataItemsWidth() override;

private:
	glm::vec3 calculateFrustumColor(glm::vec3 color);
};
