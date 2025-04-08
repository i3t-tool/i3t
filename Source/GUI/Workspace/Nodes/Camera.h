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

#include "DIWNE/Core/Elements/Containers/INodeContainer.h"

#include "Core/Nodes/GraphManager.h"

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"
#include "Sequence.h"

namespace Vp
{
class SceneCamera;
}

namespace Workspace
{
class Camera : public CoreNodeWithPins, public DIWNE::INodeContainer
{
protected:
	// Projection and view are stored as two pointer kinds to support DIWNE::INodeContainer queries
	// The projection and view getters could probably just use raw pointers
	Ptr<Sequence> m_projection{nullptr};
	Ptr<Sequence> m_view{nullptr};
	DIWNE::NodeList m_projAndView;

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

	DIWNE::NodeRange<> getNodes() const override;
	DIWNE::NodeList& getNodeList() override;

	const Ptr<Sequence>& getProjection() const
	{
		return m_projection;
	};
	const Ptr<Sequence>& getView() const
	{
		return m_view;
	};

	void centerContent(DIWNE::DrawInfo& context) override;
	void drawOutputPins(DIWNE::DrawInfo& context) override;

	void drawMenuLevelOfDetail() override;
	LevelOfDetail switchLevelOfDetail(LevelOfDetail oldLevel) override;
	int getLODCount() override;

	void popupContent(DIWNE::DrawInfo& context) override;

	int maxLengthOfData() override;

	// TODO: Move screen output to the left side

	void onSelection(bool selected) override;
	void onDestroy(bool logEvent) override;

private:
	glm::vec3 calculateFrustumColor(glm::vec3 color);
};
} // namespace Workspace