/**
 * \file
 * \brief
 * \authors Martin Herich, Petr Felkel, Dan Rakušan
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Model.h"

#include "Commands/Command.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Dialogs/ImportedModelsDialog.h"
#include "GUI/I3TGui.h"
#include "GUI/Toolkit.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "Utils/Color.h"
#include "Utils/HSLColor.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"
#include "Viewport/shader/PhongShader.h"

using namespace Workspace;

Model::Model(DIWNE::NodeEditor& diwne) : CoreNodeWithPins(diwne, Core::Builder::createModelNode())
{
	m_viewportModel = I3T::getViewport()->createModel(getNodebase()->getId());
	auto modelPtr = m_viewportModel.lock();
	modelPtr->m_showAxes = true;
	modelPtr->m_visible = true;
	modelPtr->m_opacity = m_opacity;
	modelPtr->m_opaque = m_opaque;

	// Setup preview render options
	m_renderOptions.framebufferAlpha = true;

	// Callback that gets called when the underlying Model node updates values
	// The Model node also updates a public model matrix variable which we can
	// read
	m_nodebase->addUpdateCallback([this](Core::Node* node) {
		Core::Model* modelNode = dynamic_cast<Core::Model*>(node);
		if (modelNode && !m_viewportModel.expired())
		{
			m_viewportModel.lock()->m_modelMatrix = modelNode->m_modelMatrix;
		}
	});

	m_drawContextMenuButton = true;
}

Model::~Model()
{
	// TODO: Shouldn't this be in onDestroy?
	const auto node = getNodebase()->as<Core::Model>();
	node->resetModelPosition();

	I3T::getViewport()->removeEntity(m_viewportModel);
	if (!m_trackedModel.expired())
		I3T::getViewport()->removeEntity(m_trackedModel);
}

void Model::initialize(DIWNE::DrawInfo& context)
{
	CoreNodeWithPins::initialize(context);

	// Update tracked model
	Ptr<Vp::SceneModel> modelPtr = m_viewportModel.lock();

	Core::Model* model = getNodebase()->asRaw<Core::Model>();
	if (auto t = model->getTrackingData())
	{
		assert(t->modelData != nullptr && "Models should be carrying TrackedModelData");
		Ptr<Vp::SceneModel> trackedPtr;
		if (m_trackedModel.expired())
		{
			m_trackedModel = I3T::getViewport()->createModel(getNodebase()->getId());
			trackedPtr = m_trackedModel.lock();
			trackedPtr->m_visible = true;
			trackedPtr->m_selectable = true;
		}
		trackedPtr = m_trackedModel.lock();
		trackedPtr->m_showAxes = modelPtr->m_showAxes;
		trackedPtr->m_modelMatrix = t->modelData->m_interpolatedMatrix;
		trackedPtr->setHighlightColor(GUI::imToGlm(I3T::getColor(EColor::Nodes_Tracking_ColorActive)));
		trackedPtr->setHighlighted(true);
		trackedPtr->setModel(modelPtr->getModel());

		modelPtr->m_opaque = false;
		modelPtr->m_opacity = std::min(m_opacity, 0.8f);
		modelPtr->m_tint = m_tint * glm::vec3(0.28f);
		// modelPtr->m_tintStrength = std::max(m_tintStrength, 1.0f);
	}
	else
	{
		if (!m_trackedModel.expired())
		{
			I3T::getViewport()->removeEntity(m_trackedModel);
			m_trackedModel.reset();
		}
		modelPtr->m_opaque = m_opaque;
		modelPtr->m_opacity = m_opacity;
		modelPtr->m_tint = m_tint;
		modelPtr->m_tintStrength = m_tintStrength;
	}

	// TODO: Some kind of tint or transparency / highlight effect on the real model
	// if (model->m_trackedModel)
	// 	m_viewportModel.lock()->m_tint = glm::vec3(0.f, 1.f, 0.f);
	// else
	// 	m_viewportModel.lock()->m_tint = glm::vec3(1.f, 1.f, 1.f);
}

void Model::centerContent(DIWNE::DrawInfo& context)
{
	if (m_levelOfDetail == LevelOfDetail::Label)
	{
		return;
	}

#define FLOOR_VEC2(_VAL) (ImVec2((float) (int) ((_VAL).x), (float) (int) ((_VAL).y))) // version of IM_FLOOR for Vec2
	ImVec2 zoomedTextureSize = FLOOR_VEC2(m_textureSize * ImGui::GetFontSize()); // floored position - same as in ImGui

	m_renderOptions.lightingModel = Vp::PhongShader::LightingModel::PHONG;

	auto modelPtr = m_viewportModel.lock();
	float origOpacity = modelPtr->m_opacity;
	modelPtr->m_opacity = 1.f;
	I3T::getViewport()->drawPreview(m_renderTarget, zoomedTextureSize.x, zoomedTextureSize.y, m_viewportModel,
	                                m_renderOptions);
	modelPtr->m_opacity = origOpacity;

	Ptr<Vp::Framebuffer> framebuffer = m_renderTarget->getOutputFramebuffer().lock();

	if (framebuffer)
	{
		ImGui::Image((void*) (intptr_t) framebuffer->getColorTexture(), zoomedTextureSize, ImVec2(0.0f, 1.0f),
		             ImVec2(1.0f, 0.0f) // vertical flip
		);
	}
	else
	{
		ImGui::Text("Failed to draw preview!");
	}
}
void Model::afterDraw(DIWNE::DrawInfo& context)
{
	if (!m_trackedModel.expired())
		drawTrackingBorder(false, false, 0.f);

	CoreNodeWithPins::afterDraw(context);
}

void Model::drawInputPins(DIWNE::DrawInfo& context)
{
	// Matrix mul pin is centered
	const std::vector<Ptr<CorePin>>& pins = m_workspaceInputs;
	assert(pins.size() == 1); // Models have special pin handling, expecting matrix mul at 0

	updatePinStyle(*pins[0]);

	if (pins[0]->allowDrawing())
	{
		m_left.vspring(0.4f);
		pins[0]->drawDiwne(context);
		DIWNE::DGui::NewLine();
	}
}

int Model::maxLengthOfData() // todo
{
	return 0;
}

void Model::drawMenuLevelOfDetail() // todo
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}
void Model::onSelection(bool selected)
{
	CoreNode::onSelection(selected);
	auto model = m_viewportModel.lock();
	if (selected)
	{
		model->m_highlight = true;
		model->m_highlightColor = I3T::getViewport()->getSettings().global().highlight.selectionColor;
	}
	else
	{
		if (m_influenceHighlight)
		{
			model->m_highlight = true;
			model->m_highlightColor = I3T::getViewport()->getSettings().global().highlight.highlightColor;
		}
		else
		{
			model->m_highlight = false;
		}
	}
}

glm::vec3 Model::calculateTint(glm::vec3 color, Ptr<Vp::SceneModel> model)
{
	//	if (model->m_mesh->m_textureCount > 0)
	//	{
	//		glm::vec3 hsl;
	//		rgbToHsl(color.r, color.g, color.b, &hsl.x, &hsl.y, &hsl.z);
	//		hsl.y = 0.8;
	//		hsl.z = 0.8;
	//		hslToRgb(hsl.x, hsl.y, hsl.z, &color.r, &color.g, &color.b);
	//	}
	return color;
}

void Model::popupContent(DIWNE::DrawInfo& context)
{
	CoreNode::drawMenuSetEditable();

	ImGui::Separator();

	drawMenuLevelOfDetail();

	ImGui::Separator();

	popupContent_axis_showmodel();

	ImGui::Separator();

	CoreNode::drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}

void Model::popupContent_axis_showmodel()
{
	auto model = m_viewportModel.lock();

	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
	if (I3TGui::BeginMenuWithLog(_t("Set visibility")))
	{
		if (I3TGui::MenuItemWithLog(_t("Show axes"), NULL, model->m_showAxes))
		{
			model->m_showAxes = !model->m_showAxes;
		}
		if (I3TGui::MenuItemWithLog(_t("Show model"), NULL, model->m_visible))
		{
			model->m_visible = !model->m_visible;
		}
		if (I3TGui::BeginMenuWithLog(_t("Set opacity")))
		{
			ImGui::Checkbox(_t("Back-face culling"), &model->m_backFaceCull);
			ImGui::Checkbox(_t("Opaque"), &m_opaque);
			if (!m_opaque)
			{
				ImGui::SetNextItemWidth(ImGui::GetFontSize() * 6);
				ImGui::SliderFloat(_t("Opacity"), &m_opacity, 0.0f, 1.0f, "%.2f");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	if (I3TGui::BeginMenuWithLog(_t("Change color")))
	{
		if (I3TGui::MenuItemWithLog(_t("None")))
		{
			m_tint = glm::vec3(1.0f);
		}
		if (I3TGui::MenuItemWithLog(_t("Red")))
		{
			m_tint = calculateTint(Color::RED, model);
		}
		if (I3TGui::MenuItemWithLog(_t("Green")))
		{
			m_tint = calculateTint(Color::GREEN, model);
		}
		if (I3TGui::MenuItemWithLog(_t("Blue")))
		{
			m_tint = calculateTint(Color::BLUE, model);
		}
		if (I3TGui::MenuItemWithLog(_t("Yellow")))
		{
			m_tint = calculateTint(Color::YELLOW, model);
		}
		if (I3TGui::MenuItemWithLog(_t("Teal")))
		{
			m_tint = calculateTint(Color::TEAL, model);
		}
		if (I3TGui::MenuItemWithLog(_t("Magenta")))
		{
			m_tint = calculateTint(Color::MAGENTA, model);
		}
		if (I3TGui::MenuItemWithLog(_t("Light Blue")))
		{
			m_tint = calculateTint(Color::LIGHT_BLUE, model);
		}
		if (I3TGui::MenuItemWithLog(_t("Orange")))
		{
			m_tint = calculateTint(Color::ORANGE, model);
		}
		if (I3TGui::MenuItemWithLog(_t("Brown")))
		{
			m_tint = calculateTint(Color::BROWN, model);
		}
		if (I3TGui::BeginMenuWithLog(_t("Adjust tint")))
		{
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 6);
			ImGui::SliderFloat(_t("Strength"), &m_tintStrength, 0.0f, 1.0f, "%.2f");

			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	if (I3TGui::BeginMenuWithLog(_t("Change model")))
	{
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 6);

		// TODO: (DR) Polish scale factor impl (serialization + adjust preview radius)
		// ImGui::SliderFloat(_t("Scale"), &model->m_scale, 0.01f, 10.0f, "%.01f");

		for (const auto& resource : RMI.getDefaultResources(Core::ResourceType::Model))
		{
			if (I3TGui::MenuItemWithLog(resource.alias.c_str(), nullptr, false, true, [&]() {
				    LOG_EVENT_OBJECT_ADDED(resource.alias, m_labelDiwne);
			    }))
			{
				model->setModel(resource.alias);
			}
		}
		std::vector<std::string> importedResources = RMI.getImportedResourceAliases();
		if (!importedResources.empty())
		{
			ImGui::Separator();
			for (const auto& alias : importedResources)
			{
				Core::Mesh* importedModel = RMI.meshByAlias(alias);
				if (importedModel)
				{
					if (I3TGui::MenuItemWithLog(alias.c_str(), nullptr, false, true, [&]() {
						    LOG_EVENT_OBJECT_ADDED(alias, m_labelDiwne);
					    }))
					{
						model->setModel(alias);
					}
				}
				else
				{
					I3TGui::MenuItemWithLog(alias.c_str(), nullptr, false, false, [&]() {
						LOG_EVENT_OBJECT_ADDED(alias, m_labelDiwne);
					});
				}
			}
		}
		ImGui::Separator();
		if (I3TGui::MenuItemWithLog(_t("Import models...")))
		{
			App::getModule<UIModule>().getWindowManager().showUniqueWindow<ImportedModelsDialog>();
		}

		ImGui::EndMenu();
	}
	ImGui::PopItemFlag();
}
