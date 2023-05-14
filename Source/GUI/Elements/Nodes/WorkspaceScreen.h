/**
 * \file WorkspaceScreen.h
 * \author Petr Felkel, Martin Herich, Dan Rakušan
 * \brief screen box in the workspace
 * Based on WorkspaceScreen
 */
#pragma once

#include "WorkspaceElementsWithCoreData.h"

#include "Viewport/data/DisplayOptions.h"
#include "Viewport/data/RenderOptions.h"

namespace Vp
{
class SceneRenderTarget;
}

class WorkspaceScreen : public WorkspaceNodeWithCoreDataWithPins
{
private:
	// variables of the workspace box
	GLuint m_textureID = 0;            // rendered texture name (COLOR_ATTACHMENT0 in m_fbo)
	ImVec2 m_textureSize = {100, 100}; // initial render texture size - should be
	                                   // large enough or changed during zoo

	std::shared_ptr<Vp::SceneRenderTarget> m_renderTarget;
	Vp::DisplayOptions m_displayOptions;
	Vp::RenderOptions m_renderOptions;

public:
	WorkspaceScreen(DIWNE::Diwne& diwne);
	~WorkspaceScreen();

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceScreen>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	// bool drawDataFull(, int index);
	int maxLenghtOfData();        // todo
	bool middleContent();         // the most important function
	bool topContent();						// rendering header part
	void drawMenuLevelOfDetail(); // todo
	void popupContent() override;

	virtual std::vector<Ptr<WorkspaceCoreOutputPin>> const getOutputsToShow() const
	{
		return {getOutputs()[1]};
	}; /* \todo Some name for pin -> similar to I3T_CAM_MUL */

    ImVec2 getAspect() const;
    void setAspect(ImVec2 aspect);

private:
	void init();
};
