/**
 * \file
 * \brief Model box in the workspace
 * \authors Martin Herich, Petr Felkel, Dan Rakušan
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
private:
	// initial render texture size - should be large enough or changed during zoom
	ImVec2 m_textureSize = {84, 100};
	std::shared_ptr<Vp::SceneRenderTarget> m_renderTarget;
	Vp::RenderOptions m_renderOptions;

public:
	std::weak_ptr<Vp::SceneModel> m_viewportModel;

	bool m_influenceHighlight{false}; ///< Whether the model is being influenced by node selection.
	                                  ///< Set by ViewportHighlightResolver.

	WPtr<Vp::SceneModel> viewportModel() const
	{
		return m_viewportModel;
	}

	Model(DIWNE::NodeEditor& diwne);
	~Model();

	// Double dispatch
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<Model>(shared_from_this()));
	}

	// bool drawDataFull(, int index);
	int maxLengthOfData() override;                        // todo
	void centerContent(DIWNE::DrawInfo& context) override; // the most important function
	void drawMenuLevelOfDetail() override;                 // todo

	void popupContent(DIWNE::DrawInfo& context) override;
	void popupContent_axis_showmodel();

	/// Overridden for viewport model selection highlight.
	void onSelection(bool selected) override;

private:
	void init();
	glm::vec3 calculateTint(glm::vec3 color, Ptr<Vp::SceneModel> model);
};

class ModelProxy : public Core::IModelProxy
{
public:
	ModelProxy(Ptr<Model> model);
	~ModelProxy() override;

	void update(const glm::mat4& transform) override;
	Ptr<Core::Model> getModel() override;

private:
	Ptr<Model> m_model;
};
} // namespace Workspace