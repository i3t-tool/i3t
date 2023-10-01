#include "WorkspaceModel.h"

#include "Commands/Command.h"
#include "Core/Resources/ResourceManager.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Utils/Color.h"
#include "Utils/HSLColor.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"
#include "Viewport/shader/PhongShader.h"

WorkspaceModel::WorkspaceModel(DIWNE::Diwne& diwne)
    : WorkspaceNodeWithCoreDataWithPins(diwne, Core::Builder::createModelNode())
{
	init();
	// setDataItemsWidth();
}

WorkspaceModel::~WorkspaceModel()
{
	const auto node = getNodebase()->as<Core::Model>();
	node->resetModelPosition();

	I3T::getViewport()->removeEntity(m_viewportModel);
}

void WorkspaceModel::popupContent_axis_showmodel()
{
	auto model = m_viewportModel.lock();

	if (ImGui::MenuItem("Show axes", NULL, model->m_showAxes))
	{
		model->m_showAxes = !model->m_showAxes;
	}
	if (ImGui::MenuItem("Show model", NULL, model->m_visible))
	{
		model->m_visible = !model->m_visible;
	}
	if (ImGui::BeginMenu("Transparency"))
	{
		if (ImGui::Checkbox("Opaque", &model->m_opaque))
		{
			// model->m_opaque = !model->m_opaque;
		}
		ImGui::Checkbox("Back-face cull", &model->m_backFaceCull);
		ImGui::SliderFloat("Opacity", &model->m_opacity, 0.0f, 1.0f, "%.2f");
		ImGui::EndMenu();
	}

	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
	if (ImGui::BeginMenu("Set tint"))
	{
		if (ImGui::BeginMenu("Color"))
		{
			if (ImGui::MenuItem("None"))
			{
				model->m_tint = glm::vec3(1.0f);
			}
			static glm::vec3 RED;
			static glm::vec3 GREEN;
			static glm::vec3 BLUE;
			static glm::vec3 YELLOW;
			static glm::vec3 TEAL;
			static glm::vec3 MAGENTA;
			static glm::vec3 LIGHT_BLUE;
			static glm::vec3 ORANGE;
			static glm::vec3 BROWN;

			if (ImGui::MenuItem("Red"))
			{
				model->m_tint = calculateTint(Color::RED, model);
			}
			if (ImGui::MenuItem("Green"))
			{
				model->m_tint = calculateTint(Color::GREEN, model);
			}
			if (ImGui::MenuItem("Blue"))
			{
				model->m_tint = calculateTint(Color::BLUE, model);
			}
			if (ImGui::MenuItem("Yellow"))
			{
				model->m_tint = calculateTint(Color::YELLOW, model);
			}
			if (ImGui::MenuItem("Teal"))
			{
				model->m_tint = calculateTint(Color::TEAL, model);
			}
			if (ImGui::MenuItem("Magenta"))
			{
				model->m_tint = calculateTint(Color::MAGENTA, model);
			}
			if (ImGui::MenuItem("Light Blue"))
			{
				model->m_tint = calculateTint(Color::LIGHT_BLUE, model);
			}
			if (ImGui::MenuItem("Orange"))
			{
				model->m_tint = calculateTint(Color::ORANGE, model);
			}
			if (ImGui::MenuItem("Brown"))
			{
				model->m_tint = calculateTint(Color::BROWN, model);
			}
			ImGui::EndMenu();
		}
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 6);
		ImGui::SliderFloat("Strength", &model->m_tintStrength, 0.0f, 1.0f, "%.2f");
		ImGui::EndMenu();
	}
	ImGui::PopItemFlag();

	if (ImGui::BeginMenu("Change model"))
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

glm::vec3 WorkspaceModel::calculateTint(glm::vec3 color, Ptr<Vp::SceneModel> model)
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

void WorkspaceModel::popupContent()
{
	WorkspaceNodeWithCoreData::drawMenuSetEditable();

	ImGui::Separator();

	popupContent_axis_showmodel();

	ImGui::Separator();

	drawMenuLevelOfDetail();

	ImGui::Separator();

	WorkspaceNodeWithCoreData::drawMenuDuplicate();

	ImGui::Separator();

	WorkspaceNode::popupContent();
}

void WorkspaceModel::init()
{
	m_viewportModel = I3T::getViewport()->createModel(getId());
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
		if (modelNode)
		{
			m_viewportModel.lock()->m_modelMatrix = modelNode->m_modelMatrix;
		}
	});
}

bool WorkspaceModel::topContent()
{
	// TODO: (DR) This call might be unnecessary as the same call is made in WorkspaceNode, for color
	// override perhaps? Similarly in WorkspaceScreen. (Note sure, noting so I don't forget)
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeader),
	                         I3T::getSize(ESize::Nodes_Rounding), ImDrawCornerFlags_Top);

	return WorkspaceNodeWithCoreData::topContent();
}

bool WorkspaceModel::middleContent()
{
	bool interaction_happen = false;

	if (m_levelOfDetail == WorkspaceLevelOfDetail::Label)
	{
		return interaction_happen;
	}

	int width = m_textureSize.x * diwne.getWorkAreaZoom();
	int height = m_textureSize.y * diwne.getWorkAreaZoom();

#define FLOOR_VEC2(_VAL) (ImVec2((float) (int) ((_VAL).x), (float) (int) ((_VAL).y))) // version of IM_FLOOR for Vec2
	ImVec2 zoomedTextureSize =
	    FLOOR_VEC2(m_textureSize * diwne.getWorkAreaZoom()); // floored position - same as in ImGui

	m_renderOptions.lightingModel = Vp::PhongShader::LightingModel::PHONG;

	I3T::getViewport()->drawPreview(m_renderTarget, width, height, m_viewportModel, m_renderOptions);
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

	return interaction_happen;
}

int WorkspaceModel::maxLengthOfData() // todo
{
	return 0;
}

void WorkspaceModel::drawMenuLevelOfDetail() // todo
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
	                              {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}

bool WorkspaceModel::processSelect()
{
	auto model = m_viewportModel.lock();
	model->m_highlight = true;
	model->m_highlightColor = I3T::getViewport()->getSettings().global().highlight.selectionColor;

	return WorkspaceNodeWithCoreDataWithPins::processSelect();
}

bool WorkspaceModel::processUnselect()
{
	auto model = m_viewportModel.lock();
	if (m_influenceHighlight)
	{
		model->m_highlight = true;
		model->m_highlightColor = I3T::getViewport()->getSettings().global().highlight.highlightColor;
	}
	else
	{
		model->m_highlight = false;
	}

	return WorkspaceNodeWithCoreDataWithPins::processUnselect();
}
