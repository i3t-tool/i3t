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

#include "Viewport/data/DisplayOptions.h"
#include "Viewport/data/RenderOptions.h"

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"
#include "Viewport/entity/nodes/SceneScreen.h"

namespace Vp
{
class SceneRenderTarget;
}

namespace Workspace
{
class Screen : public CoreNodeWithPins
{
	// variables of the workspace box
	GLuint m_textureID = 0;            // rendered texture name (COLOR_ATTACHMENT0 in m_fbo)
	ImVec2 m_textureSize = {100, 100}; // initial render texture size in pixels

	std::shared_ptr<Vp::SceneRenderTarget> m_renderTarget;
	Vp::DisplayOptions m_displayOptions;
	Vp::RenderOptions m_renderOptions;

	std::weak_ptr<Vp::SceneScreen> m_viewportScreen; ///< Reference to the Scene View (3D Viewport) representation
public:
	explicit Screen(DIWNE::NodeEditor& diwne);
	~Screen() override;

	void centerContent(DIWNE::DrawInfo& context) override;
	void finalize(DIWNE::DrawInfo& context) override;

	void onSelection(bool selected) override;

	int maxLengthOfData() override;
	void drawMenuLevelOfDetail() override;
	void popupContent(DIWNE::DrawInfo& context) override;

	/////////////////////////////////////////////////////////////

	ImVec2 getScreenSize() const;
	void setScreenSize(ImVec2 aspect);

	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<Screen>(shared_from_this()));
	}

private:
	bool drawResizeHandle(ImVec2 topLeftCursorPos, ImVec2 zoomedTextureSize);
	void updateCoreData();
	void updateTrackedScreen(); ///< Updates the 3D Scene view representation of the screen
};
} // namespace Workspace