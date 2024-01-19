/**
 * \file
 * \brief Screen box in the workspace
 * \authors Jaroslav Holeček, Petr Felkel, Martin Herich, Dan Rakušan
 * \date 25.09.2023
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

	/////////////////////////////////////////////////////////////

	// bool drawDataFull(, int index);
	int maxLengthOfData();        // todo
	bool middleContent();         // the most important function
	bool topContent();            // rendering header part
	void drawMenuLevelOfDetail(); // todo
	void popupContent() override;

	std::vector<Ptr<WorkspaceCoreOutputPin>> const getOutputsToShow() const override
	{
		return {getOutputs()[1]};
	}; /* \todo Some name for pin -> similar to I3T_CAM_MUL */

	/////////////////////////////////////////////////////////////

	ImVec2 getAspect() const;
	void setAspect(ImVec2 aspect);

private:
	void init();

	bool drawResizeHandles(ImVec2 topLeftCursorPos, ImVec2 zoomedTextureSize);
};
