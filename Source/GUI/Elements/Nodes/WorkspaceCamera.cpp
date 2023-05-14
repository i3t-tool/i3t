#include "WorkspaceCamera.h"

#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneCamera.h"

#include "Utils/Color.h"
#include "Utils/HSLColor.h"

WorkspaceCamera::WorkspaceCamera(DIWNE::Diwne& diwne)
    : WorkspaceNodeWithCoreDataWithPins(diwne, Core::GraphManager::createCamera(), false),
      m_projection(std::make_shared<WorkspaceSequence>(diwne, m_nodebase->as<Core::Camera>()->getProj(), true, true)),
      m_view(std::make_shared<WorkspaceSequence>(diwne, m_nodebase->as<Core::Camera>()->getView(), true, true))
{
	(m_view->getInputs().at(0).get())->plug(m_projection->getOutputs().at(0).get());

	m_projection->m_selectable = false;
	for (int i = 0; i < m_projection->getNodebase()->getInputPins().size(); i++)
	{
		if (i != 1)
		{
			m_projection->getNodebase()->getInputPins()[i].setRenderPins(false);
		}
	}
	for (int j = 0; j < m_projection->getNodebase()->getOutputPins().size(); j++)
	{
		if (j != 1)
		{
			m_projection->getNodebase()->getOutputPins()[j].setRenderPins(false);
		}
	}
	m_projection->getNodebase()->getInputPins()[1].setRenderPins(true);
	m_projection->getNodebase()->getOutputPins()[1].setRenderPins(true);
	m_projection->setTopLabel("projection");

	m_view->m_selectable = false;
	for (int k = 0; k < m_view->getNodebase()->getInputPins().size(); k++)
	{
		m_view->getNodebase()->getInputPins()[k].setRenderPins(false);
	}
	for (int l = 0; l < m_view->getNodebase()->getOutputPins().size(); l++)
	{
		m_view->getNodebase()->getOutputPins()[l].setRenderPins(false);
	}
	m_view->getNodebase()->getInputPins()[1].setRenderPins(true);
	m_view->getNodebase()->getOutputPins()[1].setRenderPins(true);
	m_view->setTopLabel("view");

	// Hide multiplication output to discourage interaction
	// getNodebase()->getOutputPins()[Core::I3T_CAMERA_OUT_MUL].setRenderPins(false);

	getOutputs()[Core::I3T_CAMERA_OUT_MUL]->m_drawMode = DIWNE::DrawMode::JustDraw;

	m_viewportCamera = App::get().viewport()->createCamera(getId());
	auto cameraPtr = m_viewportCamera.lock();
	cameraPtr->m_showAxes = m_axisOn;
	cameraPtr->m_visible = m_showCamera;
	cameraPtr->m_showFrustum = m_showFrustum;
	cameraPtr->m_fillFrustum = m_fillFrustum;
	cameraPtr->m_frustumColor = m_frustumColor;

	// Callback that gets called when the underlying Camera node updates values
	// The Camera node also updates public projection and view matrix variables
	// which we can read
	m_nodebase->addUpdateCallback(
	    [this](Core::Node* node)
	    {
		    Core::Camera* cameraNode = dynamic_cast<Core::Camera*>(node);
		    if (cameraNode)
		    {
			    auto viewportCameraPtr = m_viewportCamera.lock();
			    viewportCameraPtr->m_projectionMatrix = cameraNode->m_projectionMatrix;
			    viewportCameraPtr->m_viewMatrix = cameraNode->m_viewMatrix;
		    }
	    });
}

WorkspaceCamera::~WorkspaceCamera() { App::get().viewport()->removeEntity(m_viewportCamera); }

void WorkspaceCamera::popupContent()
{
	WorkspaceNodeWithCoreData::drawMenuSetEditable();

	ImGui::Separator();

	if (ImGui::MenuItem("Show axes", NULL, m_axisOn))
	{
		m_axisOn = !m_axisOn;
		m_viewportCamera.lock()->m_showAxes = m_axisOn;
	}
	if (ImGui::MenuItem("Show camera", NULL, m_showCamera))
	{
		m_showCamera = !m_showCamera;
		m_viewportCamera.lock()->m_visible = m_showCamera;
	}
	if (ImGui::MenuItem("Show frustum", NULL, m_showFrustum))
	{
		m_showFrustum = !m_showFrustum;
		m_viewportCamera.lock()->m_showFrustum = m_showFrustum;
	}
	if (ImGui::MenuItem("Fill frustum", NULL, m_fillFrustum))
	{
		m_fillFrustum = !m_fillFrustum;
		m_viewportCamera.lock()->m_fillFrustum = m_fillFrustum;
	}
	if (ImGui::BeginMenu("Frustum fill color"))
	{
		if (ImGui::MenuItem("Default"))
		{
			m_frustumColor = glm::vec3(0.35f, 0.27f, 0.06f);
			m_viewportCamera.lock()->m_frustumColor = m_frustumColor;
		}
		if (ImGui::MenuItem("Red"))
		{
			m_frustumColor = calculateFrustumColor(Color::RED);
			m_viewportCamera.lock()->m_frustumColor = m_frustumColor;
		}
		if (ImGui::MenuItem("Blue"))
		{
			m_frustumColor = calculateFrustumColor(Color::BLUE);
			m_viewportCamera.lock()->m_frustumColor = m_frustumColor;
		}
		if (ImGui::MenuItem("Green"))
		{
			m_frustumColor = calculateFrustumColor(Color::GREEN);
			m_viewportCamera.lock()->m_frustumColor = m_frustumColor;
		}
		if (ImGui::MenuItem("Yellow"))
		{
			m_frustumColor = calculateFrustumColor(Color::YELLOW);
			m_viewportCamera.lock()->m_frustumColor = m_frustumColor;
		}
		if (ImGui::MenuItem("Orange"))
		{
			m_frustumColor = calculateFrustumColor(Color::ORANGE);
			m_viewportCamera.lock()->m_frustumColor = m_frustumColor;
		}
		if (ImGui::MenuItem("Magenta"))
		{
			m_frustumColor = calculateFrustumColor(Color::MAGENTA);
			m_viewportCamera.lock()->m_frustumColor = m_frustumColor;
		}
		if (ImGui::MenuItem("Teal"))
		{
			m_frustumColor = calculateFrustumColor(Color::TEAL);
			m_viewportCamera.lock()->m_frustumColor = m_frustumColor;
		}
		ImGui::EndMenu();
	}
	ImGui::Separator();

	WorkspaceNodeWithCoreData::drawMenuDuplicate();

	ImGui::Separator();

	WorkspaceNode::popupContent();
}

glm::vec3 WorkspaceCamera::calculateFrustumColor(glm::vec3 color)
{
	glm::vec3 hsl;
	rgbToHsl(color.r, color.g, color.b, &hsl.x, &hsl.y, &hsl.z);
	hsl.y = 0.9;
	hsl.z = 0.25;
	hslToRgb(hsl.x, hsl.y, hsl.z, &color.r, &color.g, &color.b);
	return color;
}
bool WorkspaceCamera::topContent()
{
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeader),
	                         I3T::getSize(ESize::Nodes_Rounding), ImDrawCornerFlags_Top);

	return WorkspaceNodeWithCoreData::topContent();
}

bool WorkspaceCamera::middleContent()
{
	bool inner_interaction_happen = false;
	inner_interaction_happen |=
	    m_projection->drawNodeDiwne<WorkspaceSequence>(DIWNE::DrawModeNodePosition::OnCoursorPosition, m_drawMode);
	ImGui::SameLine();
	inner_interaction_happen |=
	    m_view->drawNodeDiwne<WorkspaceSequence>(DIWNE::DrawModeNodePosition::OnCoursorPosition, m_drawMode);
	return inner_interaction_happen;
}

void WorkspaceCamera::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()),
	                              {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
}

// bool WorkspaceCamera::leftContent(){return false;};
// bool WorkspaceCamera::rightContent(){return
// WorkspaceNodeWithCoreDataWithPins::rightContent();}; /* draw camera pin on
// opposite side */

int WorkspaceCamera::maxLenghtOfData() { return 0; }

bool WorkspaceCamera::isCamera() { return true; }