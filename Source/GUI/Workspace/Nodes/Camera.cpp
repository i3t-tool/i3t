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

#include "GUI/I3TGui.h"
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
	m_drawContextMenuButton = true;
	m_contentSpacing = 0;

	m_projAndView.push_back(m_projection);
	m_projAndView.push_back(m_view);

	// connect matrix P to matrix V internally
	// TODO: Figure out how to draw this link manually and not from the editor
	//  That way we can draw it at the right moment inside the actual camera node content
	//  Likely we'll add a flag to DiwneObject that indicates its drawing is handled custom and not by the editor
	//  This mechanism could be also used for the dragged link instead of checking the active action
	// TODO: After this is done, also apply the same mechanism on internal camera pins (eg. pins of the two sequences)
	//  Any pin thats inside another node must actually draw the pin itself, as otherwise it would be hidden behind
	//  the node.

	// DIWNE::Pin* view0 = m_view->getInputs().at(0).get();
	// DIWNE::Pin* proj0 = m_projection->getOutputs().at(0).get();
	// view0->plug(proj0, false);
	// They are already plugged in Core!

	m_projection->setSelectable(false);
	for (auto& pin : m_projection->getNodebase()->getInputPins())
		pin.setRendered(false);
	for (auto& pin : m_projection->getNodebase()->getOutputPins())
		pin.setRendered(false);

	// todo refactor - use the constant Core::I3T_CAMERA_OUT_MATRIX
	m_projection->getNodebase()->getInputPins()[Core::I3T_SEQ_IN_MAT].setRendered(true);
	m_projection->getNodebase()->getOutputPins()[Core::I3T_SEQ_OUT_MAT].setRendered(true);
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

	// TODO: Remove <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Hide multiplication output to discourage interaction
	// getNodebase()->getOutputPins()[Core::I3T_CAMERA_OUT_MUL].setRendered(false);

	// TODO: Use some flag to make it not interactive
	//	getOutputs()[Core::I3T_CAMERA_OUT_MUL]->m_interactive = false;

	m_viewportCamera = I3T::getViewport()->createCamera(getNodebase()->getId());

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

	// Move screen output to the input (left) side
	auto it = std::next(m_rightPins.begin(), 0);
	(*it)->m_isLeft = true;
	(*it)->makeOutput();
	(*it)->m_showData = false;
	m_leftPins.insert(m_leftPins.begin(), *it);
	m_rightPins.erase(it);
}

Camera::~Camera()
{
	// TODO: Shouldn't this be in onDestroy?
	I3T::getViewport()->removeEntity(m_viewportCamera);
}

void Camera::popupContent(DIWNE::DrawInfo& context)
{
	drawMenuSetEditable();

	ImGui::Separator();

	auto cameraPtr = m_viewportCamera.lock();

	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
	if (I3TGui::BeginMenuWithLog(_t("Set visibility")))
	{
		if (I3TGui::MenuItemWithLog(_t("Show axes"), NULL, cameraPtr->m_showAxes))
		{
			cameraPtr->m_showAxes = !cameraPtr->m_showAxes;
		}
		if (I3TGui::MenuItemWithLog(_t("Show camera"), NULL, cameraPtr->m_visible))
		{
			cameraPtr->m_visible = !cameraPtr->m_visible;
		}
		if (I3TGui::MenuItemWithLog(_t("Show frustum"), NULL, cameraPtr->m_showFrustum))
		{
			cameraPtr->m_showFrustum = !cameraPtr->m_showFrustum;
		}
		if (I3TGui::MenuItemWithLog(_t("Fill frustum"), NULL, cameraPtr->m_fillFrustum))
		{
			cameraPtr->m_fillFrustum = !cameraPtr->m_fillFrustum;
		}
		ImGui::EndMenu();
	}
	if (I3TGui::BeginMenuWithLog(_t("Change frustum color")))
	{
		if (I3TGui::BeginMenuWithLog(_t("Frustum fill color")))
		{
			if (I3TGui::MenuItemWithLog(_t("Default")))
			{
				cameraPtr->m_frustumColor = glm::vec3(0.35f, 0.27f, 0.06f);
			}
			if (I3TGui::MenuItemWithLog(_t("Red")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::RED);
			}
			if (I3TGui::MenuItemWithLog(_t("Blue")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::BLUE);
			}
			if (I3TGui::MenuItemWithLog(_t("Green")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::GREEN);
			}
			if (I3TGui::MenuItemWithLog(_t("Yellow")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::YELLOW);
			}
			if (I3TGui::MenuItemWithLog(_t("Orange")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::ORANGE);
			}
			if (I3TGui::MenuItemWithLog(_t("Magenta")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::MAGENTA);
			}
			if (I3TGui::MenuItemWithLog(_t("Teal")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::TEAL);
			}
			ImGui::EndMenu();
		}
		if (I3TGui::BeginMenuWithLog(_t("Frustum outline color")))
		{
			if (I3TGui::MenuItemWithLog(_t("Red")))
			{
				cameraPtr->m_frustumOutlineColor = Color::RED;
			}
			if (I3TGui::MenuItemWithLog(_t("Blue")))
			{
				cameraPtr->m_frustumOutlineColor = Color::BLUE;
			}
			if (I3TGui::MenuItemWithLog(_t("Green")))
			{
				cameraPtr->m_frustumOutlineColor = Color::GREEN;
			}
			if (I3TGui::MenuItemWithLog(_t("Yellow")))
			{
				cameraPtr->m_frustumOutlineColor = Color::YELLOW;
			}
			if (I3TGui::MenuItemWithLog(_t("Orange")))
			{
				cameraPtr->m_frustumOutlineColor = Color::ORANGE;
			}
			if (I3TGui::MenuItemWithLog(_t("Magenta")))
			{
				cameraPtr->m_frustumOutlineColor = Color::MAGENTA;
			}
			if (I3TGui::MenuItemWithLog(_t("Teal")))
			{
				cameraPtr->m_frustumOutlineColor = Color::TEAL;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	ImGui::PopItemFlag();
	ImGui::Separator();

	drawMenuDuplicate(context);

	ImGui::Separator();

	Super::popupContent(context);
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
void Camera::afterDraw(DIWNE::DrawInfo& context)
{
	const Core::TrackedNodeData* t = this->getNodebase()->getTrackingData();
	if (t)
	{
		if (t->chain)
			drawTrackingBorder(t->active, t->interpolating, t->progress);
	}

	Super::afterDraw(context);
}
void Camera::drawOutputPins(DIWNE::DrawInfo& context)
{
	auto& pins = m_rightPins;
	assert(pins.size() == 2); // Camera has special pin handling, expecting matrix mul at 1

	m_outputPinsVstack.begin();
	for (auto pin : {pins[0]})
	{
		updatePinStyle(*pin);
		if (pin->allowDrawing())
		{
			DIWNE::DiwnePanel* row = m_outputPinsVstack.beginRow();
			row->spring(1.0f);
			pin->drawDiwne(context);
			m_outputPinsVstack.endRow();
		}
	}

	auto& pin = pins[1];
	updatePinStyle(*pin);
	if (pin->allowDrawing())
	{
		m_outputPinsVstack.spring(0.4f);
		DIWNE::DiwnePanel* row = m_outputPinsVstack.beginRow();
		row->spring(1.0f);
		pin->drawDiwne(context);
		m_outputPinsVstack.endRow();
	}
	m_outputPinsVstack.end();
}

void Camera::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}
LevelOfDetail Camera::switchLevelOfDetail(LevelOfDetail oldLevel)
{
	return LevelOfDetail::Full;
}
int Camera::getLODCount()
{
	return 0;
}

int Camera::maxLengthOfData()
{
	return 0;
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
	Super::onDestroy(logEvent);
}

void Camera::onSelection(bool selected)
{
	Super::onSelection(selected);
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
