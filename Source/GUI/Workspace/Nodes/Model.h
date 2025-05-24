/**
 * \file
 * \brief Model box in the workspace
 * \authors Martin Herich, Petr Felkel, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Viewport/data/RenderOptions.h"
#include "Viewport/framebuffer/Framebuffer.h"

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"

namespace Vp
{
class SceneModel;
class SceneRenderTarget;
} // namespace Vp

namespace Workspace
{
class Model : public CoreNodeWithPins
{
	// Render texture size in font units
	ImVec2 m_textureSize = {5.25f, 6.25f};
	std::shared_ptr<Vp::SceneRenderTarget> m_renderTarget;
	Vp::RenderOptions m_renderOptions;

public:
	std::weak_ptr<Vp::SceneModel> m_viewportModel;
	std::weak_ptr<Vp::SceneModel> m_trackedModel;

	// Model properties
	float m_opacity{1.0f};
	bool m_opaque{true};
	glm::vec3 m_tint{1.0f};
	float m_tintStrength{.5f};

	bool m_influenceHighlight{false}; ///< Whether the model is being influenced by node selection.
	                                  ///< Set by ViewportHighlightResolver.

	Model(DIWNE::NodeEditor& diwne);
	~Model() override;

	WPtr<Vp::SceneModel> viewportModel() const
	{
		return m_viewportModel;
	}

	void initialize(DIWNE::DrawInfo& context) override;
	void centerContent(DIWNE::DrawInfo& context) override; // the most important function
	void afterDraw(DIWNE::DrawInfo& context) override;

	// bool drawDataFull(, int index);
	int maxLengthOfData() override; // todo

	void drawInputPins(DIWNE::DrawInfo& context) override;

	void drawMenuLevelOfDetail() override; // todo

	void popupContent(DIWNE::DrawInfo& context) override;
	void popupContent_axis_showmodel();

	/// Overridden for viewport model selection highlight.
	void onSelection(bool selected) override;

	// Double dispatch
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<Model>(shared_from_this()));
	}

private:
	glm::vec3 calculateTint(glm::vec3 color, Ptr<Vp::SceneModel> model);
};
} // namespace Workspace