#include "WorkspaceCamera.h"

#include "Viewport/Viewport.h"
#include "Viewport/entity/SceneCamera.h"

#include "Utils/Color.h"
#include "Utils/HSLColor.h"

WorkspaceCamera::WorkspaceCamera(DIWNE::Diwne& diwne)
    : WorkspaceNodeWithCoreDataWithPins(diwne, Core::GraphManager::createCamera(), false),
      m_projection(std::make_shared<WorkspaceSequence>(diwne, m_nodebase->as<Core::Camera>()->getProj(), true, true)),
      m_view(std::make_shared<WorkspaceSequence>(diwne, m_nodebase->as<Core::Camera>()->getView(), true, true))
{
	(m_view->getInputs().at(0).get())->plug(m_projection->getOutputs().at(0).get());
	m_projection->m_selectable = false;
	m_view->m_selectable = false;

	getOutputs()[Core::I3T_CAMERA_OUT_MUL]->m_drawMode = DIWNE::DrawMode::JustDraw;

	m_viewportCamera = App::get().viewport()->createCamera();
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
	    [this]()
	    {
		    std::shared_ptr<Core::Camera> cameraNode = dynamic_pointer_cast<Core::Camera>(m_nodebase);
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
	WorkspaceNodeWithCoreData::popupContent();

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