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
	if (ImGui::BeginMenu("Set tint"))
	{
		if (ImGui::MenuItem("None"))
		{
			model->m_tint = glm::vec3(1.0f);
		}
		if (ImGui::MenuItem("Red"))
		{
			model->m_tint = calculateTint(Color::RED);
		}
		if (ImGui::MenuItem("Blue"))
		{
			model->m_tint = calculateTint(Color::BLUE);
		}
		if (ImGui::MenuItem("Green"))
		{
			model->m_tint = calculateTint(Color::GREEN);
		}
		if (ImGui::MenuItem("Yellow"))
		{
			model->m_tint = calculateTint(Color::YELLOW);
		}
		if (ImGui::MenuItem("Orange"))
		{
			model->m_tint = calculateTint(Color::ORANGE);
		}
		if (ImGui::MenuItem("Magenta"))
		{
			model->m_tint = calculateTint(Color::MAGENTA);
		}
		if (ImGui::MenuItem("Teal"))
		{
			model->m_tint = calculateTint(Color::TEAL);
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
	modelPtr->m_showAxes = true;
	modelPtr->m_visible = true;

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

	Ptr<Vp::Framebuffer> framebuffer =
	    App::get().viewport()->drawPreview(m_textureSize.x, m_textureSize.y, m_viewportModel).lock();

	// This is the original solution to the duplicate model preview issue, uncomment this and comment the jank one to
	// return to how things were
	//	// ORIGINAL START ///////////////////////////////////
	//	if (framebuffer)
	//	{
	//		ImGui::Image((void*)(intptr_t)framebuffer->getColorTexture(), m_textureSize, ImVec2(0.0f, 1.0f),
	//		             ImVec2(1.0f, 0.0f) // vertical flip
	//		);
	//	}
	//	else
	//	{
	//		ImGui::Text("Failed to draw preview!");
	//	}
	//	// ORIGINAL END /////////////////////////////////////

	// JANK START ///////////////////////////////////////
	// This is a janky workaround for an issue
	// TODO: (DR) Make this a proper function of the Vp::Framebuffer class

	if (m_framebuffer == nullptr)
	{
		m_framebuffer = std::make_unique<Vp::Framebuffer>(framebuffer->getWidth(), framebuffer->getHeight(),
		                                                  framebuffer->isMultisampled(), framebuffer->getSampleCount());

		Vp::ColorAttachment colorAttachment = Vp::ColorAttachment(framebuffer->getColorAttachment(0));
		m_framebuffer->addColorAttachment(colorAttachment);
	}
	// Will break if other parameters change
	m_framebuffer->update(framebuffer->getWidth(), framebuffer->getHeight());

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->getId());
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer->getId());
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, framebuffer->getWidth(), framebuffer->getHeight(), 0, 0, framebuffer->getWidth(),
	                  framebuffer->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (m_framebuffer)
	{
		ImGui::Image((void*)(intptr_t)m_framebuffer->getColorTexture(), m_textureSize, ImVec2(0.0f, 1.0f),
		             ImVec2(1.0f, 0.0f) // vertical flip
		);
	}
	else
	{
		ImGui::Text("Failed to draw preview!");
	}
	// JANK END ///////////////////////////////////////

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
