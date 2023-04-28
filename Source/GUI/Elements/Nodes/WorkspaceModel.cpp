#include "WorkspaceModel.h"

#include "Commands/Command.h"
#include "Core/Resources/ResourceManager.h"

#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "Utils/Color.h"
#include "Utils/HSLColor.h"

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

	App::get().viewport()->removeEntity(m_viewportModel);
}

void WorkspaceModel::popupContent_axis_showmodel()
{
	auto model = m_viewportModel.lock();

	ImGui::Separator();

	if (ImGui::MenuItem("Show axes", NULL, m_axisOn))
	{
		m_axisOn = !m_axisOn;
		model->m_showAxes = m_axisOn;
	}
	if (ImGui::MenuItem("Show model", NULL, m_showModel))
	{
		m_showModel = !m_showModel;
		model->m_visible = m_showModel;
	}
	if (ImGui::BeginMenu("Transparency"))
	{
		bool transparent = !model->m_opaque;
		if (ImGui::Checkbox("Transparent", &transparent))
		{
			model->m_opaque = !model->m_opaque;
		}
		ImGui::Checkbox("Back-face cull", &model->m_backFaceCull);
		ImGui::SliderFloat("Opacity", &model->m_opacity, 0.0f, 1.0f, "%.2f");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Set tint"))
	{
		if (ImGui::MenuItem("None"))
		{
			m_tint = glm::vec3(1.0f);
			model->m_tint = m_tint;
		}
		if (ImGui::MenuItem("Red"))
		{
			m_tint = calculateTint(Color::RED);
			model->m_tint = m_tint;
		}
		if (ImGui::MenuItem("Blue"))
		{
			m_tint = calculateTint(Color::BLUE);
			model->m_tint = m_tint;
		}
		if (ImGui::MenuItem("Green"))
		{
			m_tint = calculateTint(Color::GREEN);
			model->m_tint = m_tint;
		}
		if (ImGui::MenuItem("Yellow"))
		{
			m_tint = calculateTint(Color::YELLOW);
			model->m_tint = m_tint;
		}
		if (ImGui::MenuItem("Orange"))
		{
			m_tint = calculateTint(Color::ORANGE);
			model->m_tint = m_tint;
		}
		if (ImGui::MenuItem("Magenta"))
		{
			m_tint = calculateTint(Color::MAGENTA);
			model->m_tint = m_tint;
		}
		if (ImGui::MenuItem("Teal"))
		{
			m_tint = calculateTint(Color::TEAL);
			model->m_tint = m_tint;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Change model"))
	{
		for (const auto& resource : RMI.getDefaultResources(Core::ResourceType::Model))
		{
			if (ImGui::MenuItem(resource.alias.c_str()))
			{
				model->setModel(resource.alias);
			}
		}
		ImGui::EndMenu();
	}
}

glm::vec3 WorkspaceModel::calculateTint(glm::vec3 color)
{
	glm::vec3 hsl;
	rgbToHsl(color.r, color.g, color.b, &hsl.x, &hsl.y, &hsl.z);
	hsl.y = 0.8;
	hsl.z = 0.8;
	hslToRgb(hsl.x, hsl.y, hsl.z, &color.r, &color.g, &color.b);
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
	m_viewportModel = App::get().viewport()->createModel(getId());
	auto modelPtr = m_viewportModel.lock();
	modelPtr->m_showAxes = m_axisOn;
	modelPtr->m_visible = m_showModel;

	// Callback that gets called when the underlying Model node updates values
	// The Model node also updates a public model matrix variable which we can
	// read
	m_nodebase->addUpdateCallback(
	    [this](Core::Node* node)
	    {
		    Core::Model* modelNode = dynamic_cast<Core::Model*>(node);
		    if (modelNode)
		    {
			    m_viewportModel.lock()->m_modelMatrix = modelNode->m_modelMatrix;
		    }
	    });
}

bool WorkspaceModel::topContent()
{
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeader),
	                         I3T::getSize(ESize::Nodes_Rounding), ImDrawCornerFlags_Top);

	return WorkspaceNodeWithCoreData::topContent();
}

bool WorkspaceModel::middleContent()
{
	bool interaction_happen = false;

	int width = m_textureSize.x * diwne.getWorkAreaZoom();
	int height = m_textureSize.y * diwne.getWorkAreaZoom();

#define FLOOR_VEC2(_VAL) (ImVec2((float)(int)((_VAL).x), (float)(int)((_VAL).y))) // version of IM_FLOOR for Vec2
	ImVec2 zoomedTextureSize = FLOOR_VEC2(m_textureSize * diwne.getWorkAreaZoom()); // floored position - same as in ImGui

	App::get().viewport()->drawPreview(m_renderTarget, width, height, m_viewportModel);
	Ptr<Vp::Framebuffer> framebuffer = m_renderTarget->getOutputFramebuffer().lock();

	if (framebuffer)
	{
		ImGui::Image((void*)(intptr_t)framebuffer->getColorTexture(), zoomedTextureSize, ImVec2(0.0f, 1.0f),
		             ImVec2(1.0f, 0.0f) // vertical flip
		);
	}
	else
	{
		ImGui::Text("Failed to draw preview!");
	}

	return interaction_happen;
}

int WorkspaceModel::maxLenghtOfData() // todo
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
	model->m_highlightColor = App::get().viewport()->getSettings().highlight_selectionColor;

	return WorkspaceNodeWithCoreDataWithPins::processSelect();
}

bool WorkspaceModel::processUnselect()
{
	auto model = m_viewportModel.lock();
	if (m_influenceHighlight)
	{
		model->m_highlight = true;
		model->m_highlightColor = App::get().viewport()->getSettings().highlight_highlightColor;
	}
	else
	{
		model->m_highlight = false;
	}

	return WorkspaceNodeWithCoreDataWithPins::processUnselect();
}
