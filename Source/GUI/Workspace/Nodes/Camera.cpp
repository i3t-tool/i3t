/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Camera.h"

#include "GUI/Workspace/Nodes/Basic/DataRenderer.h"

#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneCamera.h"

#include "Utils/Color.h"
#include "Utils/HSLColor.h"

using namespace Workspace;

Camera::Camera(DIWNE::NodeEditor& diwne)
    : CoreNodeWithPins(diwne, Core::GraphManager::createCamera(), false),
      m_projection(std::make_shared<Sequence>(diwne, m_nodebase->as<Core::Camera>()->getProj(), true)),
      m_view(std::make_shared<Sequence>(diwne, m_nodebase->as<Core::Camera>()->getView(), true))
{
	m_projAndView.push_back(m_projection);
	m_projAndView.push_back(m_view);

	// connect matrix P to matrix V internally
	// TODO: Figure out how to draw this link manually and not from the editor
	//  That way we can draw it at the right moment inside the actual camera node content
	//  Likely we'll add a flag to DiwneObject that indicates its drawing is handled custom and not by the editor
	//  This mechanism could be also used for the dragged link instead of checking the active action
	DIWNE::Pin* view0 = m_view->getInputs().at(0).get();
	DIWNE::Pin* proj0 = m_projection->getOutputs().at(0).get();
	view0->plug(proj0, false);

	m_projection->setSelectable(false);
	for (int i = 0; i < m_projection->getNodebase()->getInputPins().size(); i++)
	{
		if (i != 1)
		{
			m_projection->getNodebase()->getInputPins()[i].setRendered(false);
		}
	}
	for (int j = 0; j < m_projection->getNodebase()->getOutputPins().size(); j++)
	{
		if (j != 1)
		{
			m_projection->getNodebase()->getOutputPins()[j].setRendered(false);
		}
	}
	// todo refactor - use the constant Core::I3T_CAMERA_OUT_MATRIX
	m_projection->getNodebase()->getInputPins()[1].setRendered(true);
	m_projection->getNodebase()->getOutputPins()[1].setRendered(true);
	m_projection->setTopLabel("projection");
	m_projection->setFixed(true);
	m_projection->setParentObject(this);
	m_projection->m_draggable = false;
	m_projection->m_deletable = false;

	m_view->setSelectable(false);
	for (int k = 0; k < m_view->getNodebase()->getInputPins().size(); k++)
	{
		m_view->getNodebase()->getInputPins()[k].setRendered(false);
	}
	for (int l = 0; l < m_view->getNodebase()->getOutputPins().size(); l++)
	{
		m_view->getNodebase()->getOutputPins()[l].setRendered(false);
	}
	m_view->getNodebase()->getInputPins()[1].setRendered(true);
	m_view->getNodebase()->getOutputPins()[1].setRendered(true);
	m_view->setTopLabel("view");
	m_view->setFixed(true);
	m_view->setParentObject(this);
	m_view->m_draggable = false;
	m_view->m_deletable = false;

	// Hide multiplication output to discourage interaction
	// getNodebase()->getOutputPins()[Core::I3T_CAMERA_OUT_MUL].setRendered(false);

	// TODO: Use some flag to make it not interactive
	//	getOutputs()[Core::I3T_CAMERA_OUT_MUL]->m_interactive = false;

	m_viewportCamera = I3T::getViewport()->createCamera(getNodebase()->getId());
	auto cameraPtr = m_viewportCamera.lock();
	cameraPtr->m_showAxes = m_axisOn;
	cameraPtr->m_visible = m_showCamera;
	cameraPtr->m_showFrustum = m_showFrustum;
	cameraPtr->m_fillFrustum = m_fillFrustum;
	cameraPtr->m_frustumColor = m_frustumColor;

	// Callback that gets called when the underlying Camera node updates values
	// The Camera node also updates public projection and view matrix variables
	// which we can read
	m_nodebase->addUpdateCallback([this](Core::Node* node) {
		Core::Camera* cameraNode = dynamic_cast<Core::Camera*>(node);
		if (cameraNode)
		{
			auto viewportCameraPtr = m_viewportCamera.lock();
			viewportCameraPtr->m_projectionMatrix = cameraNode->m_projectionMatrix;
			viewportCameraPtr->m_viewMatrix = cameraNode->m_viewMatrix;
		}
	});
}

Camera::~Camera()
{
	I3T::getViewport()->removeEntity(m_viewportCamera);
}

void Camera::popupContent(DIWNE::DrawInfo& context)
{
	CoreNode::drawMenuSetEditable();

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
	if (ImGui::BeginMenu("Frustum outline color"))
	{
		if (ImGui::MenuItem("Red"))
		{
			m_frustumOutlineColor = Color::RED;
			m_viewportCamera.lock()->m_frustumOutlineColor = m_frustumOutlineColor;
		}
		if (ImGui::MenuItem("Blue"))
		{
			m_frustumOutlineColor = Color::BLUE;
			m_viewportCamera.lock()->m_frustumOutlineColor = m_frustumOutlineColor;
		}
		if (ImGui::MenuItem("Green"))
		{
			m_frustumOutlineColor = Color::GREEN;
			m_viewportCamera.lock()->m_frustumOutlineColor = m_frustumOutlineColor;
		}
		if (ImGui::MenuItem("Yellow"))
		{
			m_frustumOutlineColor = Color::YELLOW;
			m_viewportCamera.lock()->m_frustumOutlineColor = m_frustumOutlineColor;
		}
		if (ImGui::MenuItem("Orange"))
		{
			m_frustumOutlineColor = Color::ORANGE;
			m_viewportCamera.lock()->m_frustumOutlineColor = m_frustumOutlineColor;
		}
		if (ImGui::MenuItem("Magenta"))
		{
			m_frustumOutlineColor = Color::MAGENTA;
			m_viewportCamera.lock()->m_frustumOutlineColor = m_frustumOutlineColor;
		}
		if (ImGui::MenuItem("Teal"))
		{
			m_frustumOutlineColor = Color::TEAL;
			m_viewportCamera.lock()->m_frustumOutlineColor = m_frustumOutlineColor;
		}
		ImGui::EndMenu();
	}
	ImGui::Separator();

	CoreNode::drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}

glm::vec3 Camera::calculateFrustumColor(glm::vec3 color)
{
	glm::vec3 hsl;
	rgbToHsl(color.r, color.g, color.b, &hsl.x, &hsl.y, &hsl.z);
	hsl.y = 0.9;
	hsl.z = 0.25;
	hslToRgb(hsl.x, hsl.y, hsl.z, &color.r, &color.g, &color.b);
	return color;
}

void Camera::centerContent(DIWNE::DrawInfo& context)
{
	// TODO: (DR) Investigate LOD behavior of the camera node <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// if (m_levelOfDetail == LevelOfDetail::Full) // todo it is not so simple - input wires are missing in
	// Label LOD
	{
		m_projection->drawDiwne(context, m_drawMode);
		ImGui::SameLine();
		m_view->drawDiwne(context, m_drawMode);
	}
}

void Camera::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}

// bool WorkspaceCamera::leftContent(){return false;};
// bool WorkspaceCamera::rightContent(){return
// WorkspaceNodeWithCoreDataWithPins::rightContent();}; /* draw camera pin on
// opposite side */

int Camera::maxLengthOfData()
{
	return 0;
}

bool Camera::isCamera()
{
	return true;
}

DIWNE::NodeRange<> Camera::getNodes() const
{
	return DIWNE::NodeRange<>(&m_projAndView);
}
DIWNE::NodeList& Camera::getNodeList()
{
	return m_projAndView;
}
void Camera::onDestroy(bool logEvent)
{
	m_projection->destroy(logEvent);
	m_view->destroy(logEvent);
	CoreNodeWithPins::onDestroy(logEvent);
}
void Camera::onSelection(bool selected)
{
	Node::onSelection(selected);
	auto model = m_viewportCamera.lock();
	if (selected)
	{
		model->m_highlightColor = I3T::getViewport()->getSettings().global().highlight.selectionColor;
		model->m_highlight = true;
	}
	else
	{
		model->m_highlight = false;
	}
}
