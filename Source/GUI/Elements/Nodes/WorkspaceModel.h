/**
 * \file WorkspaceModel.h
 * \author Martin Herich, Petr Felkel, Dan Rakušan
 * \brief model box in the workspace
 * The initial version with a lot of comments explaining the basics.
 */

#pragma once

#include "Viewport/Framebuffer.h"
#include "WorkspaceElementsWithCoreData.h"

namespace Vp
{
class SceneModel;
}

class WorkspaceModel : public WorkspaceNodeWithCoreDataWithPins
{
private:
	std::weak_ptr<Vp::SceneModel> m_viewportModel;
	std::unique_ptr<Vp::Framebuffer> m_framebuffer;

	// initial render texture size - should be large enough or changed during zoom
	ImVec2 m_textureSize = {84, 100};

public:
	bool m_axisOn{true};
	bool m_showModel{true};
	glm::vec3 m_tint{1.0f};

	WPtr<Vp::SceneModel> viewportModel() { m_viewportModel; }

	WorkspaceModel(DIWNE::Diwne& diwne);
	~WorkspaceModel();

	// Double dispatch
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceModel>(shared_from_this()));
	}

	// bool drawDataFull(, int index);
	int maxLenghtOfData();        // todo
	bool middleContent();         // the most important function
	void drawMenuLevelOfDetail(); // todo

	void popupContent();
	void popupContent_axis_showmodel();

private:
	void init();
	glm::vec3 calculateTint(glm::vec3 color);
};

//----------------------------------------------------------------------------//

class TrackingModelProxy : public Core::IModelProxy
{
public:
	TrackingModelProxy(WorkspaceModel& model);
	~TrackingModelProxy() override;

private:
	void update(const glm::mat4& modelMatrix) override;

	WPtr<Vp::SceneModel> m_modelCopy;
};
