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

#include "Core/Nodes/GraphManager.h"

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"
#include "Sequence.h"

namespace Vp
{
class SceneCamera;
}

namespace Workspace
{
class Camera : public CoreNodeWithPins
{
protected:
	// TODO: (DR) I think these pointers are unused
	Ptr<Sequence> m_projection = nullptr;
	Ptr<Sequence> m_view = nullptr;

public:
	bool m_axisOn{true};
	bool m_showCamera{true};
	bool m_showFrustum{true};
	bool m_fillFrustum{true};
	glm::vec3 m_frustumColor{0.35f, 0.27f, 0.06f};
	glm::vec3 m_frustumOutlineColor{1.f, 0.f, 0.f};

	std::weak_ptr<Vp::SceneCamera> m_viewportCamera;

	Camera(DIWNE::NodeEditor& diwne);
	~Camera();

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<Camera>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	Ptr<Sequence> const& getProjection() const
	{
		return m_projection;
	};
	Ptr<Sequence> const& getView() const
	{
		return m_view;
	};

	void centerContent(DIWNE::DrawInfo& context) override;

	void drawMenuLevelOfDetail() override;

	void popupContent(DIWNE::DrawInfo& context) override;

	int maxLengthOfData() override;

	bool isCamera();

	//	void leftContent(DIWNE::Diwne &diwne);
	//	void rightContent(DIWNE::Diwne &diwne);

	// TODO: Uncomment
//	bool processSelect() override;
//	bool processUnselect() override;
	float updateDataItemsWidth() override;

private:
	glm::vec3 calculateFrustumColor(glm::vec3 color);
};
} // namespace Workspace