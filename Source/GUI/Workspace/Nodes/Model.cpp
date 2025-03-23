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
#include "GUI/Workspace/WorkspaceModule.h"
#include "Utils/Color.h"
#include "Utils/HSLColor.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"
#include "Viewport/shader/PhongShader.h"

using namespace Workspace;

Model::Model(DIWNE::NodeEditor& diwne) : CoreNodeWithPins(diwne, Core::Builder::createModelNode())
{
	init();
	// setDataItemsWidth();
}

Model::~Model()
{
	const auto node = getNodebase()->as<Core::Model>();
	node->resetModelPosition();

	I3T::getViewport()->removeEntity(m_viewportModel);
}

void Model::popupContent_axis_showmodel()
{
	auto model = m_viewportModel.lock();

	if (ImGui::MenuItem(_t("Show axes"), NULL, model->m_showAxes))
	{
		model->m_showAxes = !model->m_showAxes;
	}
	if (ImGui::MenuItem(_t("Show model"), NULL, model->m_visible))
	{
		model->m_visible = !model->m_visible;
	}
	if (ImGui::BeginMenu(_t("Transparency")))
	{
		if (ImGui::Checkbox(_t("Opaque"), &model->m_opaque))
		{
			// model->m_opaque = !model->m_opaque;
		}
		ImGui::Checkbox(_t("Back-face culling"), &model->m_backFaceCull);
		ImGui::SliderFloat(_t("Opacity"), &model->m_opacity, 0.0f, 1.0f, "%.2f");
		ImGui::EndMenu();
	}

	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
	if (ImGui::BeginMenu(_t("Set tint")))
	{
		if (ImGui::BeginMenu(_t("Color")))
		{
			if (ImGui::MenuItem(_t("None")))
			{
				model->m_tint = glm::vec3(1.0f);
			}
			if (ImGui::MenuItem(_t("Red")))
			{
				model->m_tint = calculateTint(Color::RED, model);
			}
			if (ImGui::MenuItem(_t("Green")))
			{
				model->m_tint = calculateTint(Color::GREEN, model);
			}
			if (ImGui::MenuItem(_t("Blue")))
			{
				model->m_tint = calculateTint(Color::BLUE, model);
			}
			if (ImGui::MenuItem(_t("Yellow")))
			{
				model->m_tint = calculateTint(Color::YELLOW, model);
			}
			if (ImGui::MenuItem(_t("Teal")))
			{
				model->m_tint = calculateTint(Color::TEAL, model);
			}
			if (ImGui::MenuItem(_t("Magenta")))
			{
				model->m_tint = calculateTint(Color::MAGENTA, model);
			}
			if (ImGui::MenuItem(_t("Light Blue")))
			{
				model->m_tint = calculateTint(Color::LIGHT_BLUE, model);
			}
			if (ImGui::MenuItem(_t("Orange")))
			{
				model->m_tint = calculateTint(Color::ORANGE, model);
			}
			if (ImGui::MenuItem(_t("Brown")))
			{
				model->m_tint = calculateTint(Color::BROWN, model);
			}
			ImGui::EndMenu();
		}
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 6);
		ImGui::SliderFloat(_t("Strength"), &model->m_tintStrength, 0.0f, 1.0f, "%.2f");
		ImGui::EndMenu();
	}
	ImGui::PopItemFlag();

	if (ImGui::BeginMenu(_t("Change model")))
	{
		for (const auto& resource : RMI.getDefaultResources(Core::ResourceType::Model))
		{
			if (ImGui::MenuItem(resource.alias.c_str()))
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
					if (ImGui::MenuItem(alias.c_str()))
					{
						model->setModel(alias);
					}
				}
				else
				{
					ImGui::MenuItem(alias.c_str(), NULL, false, false);
				}
			}
		}
		ImGui::EndMenu();
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

	popupContent_axis_showmodel();

	ImGui::Separator();

	drawMenuLevelOfDetail();

	ImGui::Separator();

	CoreNode::drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}

void Model::init()
{
	m_viewportModel = I3T::getViewport()->createModel(getNodebase()->getId());
	auto modelPtr = m_viewportModel.lock();
	modelPtr->m_showAxes = true;
	modelPtr->m_visible = true;

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

	I3T::getViewport()->drawPreview(m_renderTarget, zoomedTextureSize.x, zoomedTextureSize.y, m_viewportModel,
	                                m_renderOptions);
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

ModelProxy::ModelProxy(Ptr<Model> model)
{
	m_model = std::make_shared<Model>(*WorkspaceModule::g_editor);
	const auto alias = model->viewportModel().lock()->getModel();
	m_model->viewportModel().lock()->setModel(alias);
}

ModelProxy::~ModelProxy()
{
	m_model->destroy(false);
	m_model = nullptr;
}

void ModelProxy::update(const glm::mat4& transform)
{
	getModel()->m_modelMatrix = transform;
	getModel()->updateValues(0);
}

Ptr<Core::Model> ModelProxy::getModel()
{
	return m_model->getNodebase()->as<Core::Model>();
}
