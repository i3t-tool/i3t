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

#include "GUI/Fonts/Bindings/IconsFontAwesome6.h"
#include "GUI/I3TGui.h"
#include "GUI/Workspace/Nodes/Basic/DataRenderer.h"
#include "GUI/Workspace/WorkspaceDiwne.h"

#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneCamera.h"

#include "Utils/Color.h"
#include "Utils/HSLColor.h"
#include "Viewport/entity/ColoredObject.h"

using namespace Workspace;

Camera::Camera(DIWNE::NodeEditor& diwne)
    : CoreNodeWithPins(diwne, Core::GraphManager::createCamera(), false),
      m_projection(std::make_shared<Sequence>(diwne, m_nodebase->as<Core::Camera>()->getProj(), true)),
      m_view(std::make_shared<Sequence>(diwne, m_nodebase->as<Core::Camera>()->getView(), true)),
      m_viewport(std::make_shared<Sequence>(diwne, m_nodebase->as<Core::Camera>()->getViewport(), true))

{
	m_drawContextMenuButton = true;
	m_contentSpacing = 0;

	m_sequences.push_back(m_viewport);
	m_sequences.push_back(m_projection);
	m_sequences.push_back(m_view);

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

	setupInnerSequence(m_projection, "projection");
	setupInnerSequence(m_view, "view");
	setupInnerSequence(m_viewport, "viewport");

	m_viewport->setRendered(false); // Hidden by default

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

void Camera::setupInnerSequence(Ptr<Sequence>& sequence, const std::string& label)
{
	sequence->setSelectable(false);
	for (auto& pin : sequence->getNodebase()->getInputPins())
		pin.setRendered(false);
	for (auto& pin : sequence->getNodebase()->getOutputPins())
		pin.setRendered(false);
	sequence->getNodebase()->getInputPins()[Core::I3T_SEQ_IN_MAT].setRendered(true);
	sequence->getNodebase()->getOutputPins()[Core::I3T_SEQ_OUT_MAT].setRendered(true);
	sequence->setTopLabel(label);
	sequence->setFixed(true);
	sequence->setParentObject(this);
	sequence->m_draggable = false;
	sequence->m_deletable = false;
}

Camera::~Camera()
{
	// TODO: Shouldn't this be in onDestroy?
	I3T::getViewport()->removeEntity(m_viewportCamera);
}

glm::vec4 Camera::calculateFrustumColor(glm::vec3 color, float alpha)
{
	glm::vec3 hsl;
	rgbToHsl(color.r, color.g, color.b, &hsl.x, &hsl.y, &hsl.z);
	hsl.y = 0.9;
	hsl.z = 0.25;
	hslToRgb(hsl.x, hsl.y, hsl.z, &color.r, &color.g, &color.b);
	return glm::vec4(color, alpha);
}

void Camera::initialize(DIWNE::DrawInfo& context)
{
	Super::initialize(context);

	updateTrackedCamera();
}

void Camera::centerContent(DIWNE::DrawInfo& context)
{
	// TODO: (DR) Investigate LOD behavior of the camera node <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// if (m_levelOfDetail == LevelOfDetail::Full) // todo it is not so simple - input wires are missing in
	// Label LOD
	{
		if (m_viewport->isRendered())
		{
			m_viewport->drawDiwne(context, m_drawMode);
			ImGui::SameLine();
		}
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
	return DIWNE::NodeRange<>(&m_sequences);
}
DIWNE::NodeList& Camera::getNodeList()
{
	return m_sequences;
}
void Camera::onDestroy(bool logEvent)
{
	m_projection->destroy(logEvent);
	m_view->destroy(logEvent);
	m_viewport->destroy(logEvent);
	Super::onDestroy(logEvent);
}

void Camera::setViewportEnabled(bool val)
{
	m_viewportEnabled = val;
	auto* coreCam = getNodebase()->asRaw<Core::Camera>();
	coreCam->m_viewportEnabled = m_viewportEnabled;
	m_viewport->setRendered(m_viewportEnabled);

	if (!m_viewportEnabled)
	{
		// Disconnect all pins from the viewport seq, to avoid having to deal with any invisible wires
		m_viewport->unplugAll();
	}
}
bool Camera::getViewportEnabled() const
{
	return m_viewportEnabled;
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

void Camera::updateTrackedCamera()
{
	auto cameraPtr = m_viewportCamera.lock();
	cameraPtr->m_visualizeDepth = false;

	// Camera model must be placed in a special manner when projection transform is tracked
	if (auto t = this->getNodebase()->getTrackingData())
	{
		cameraPtr->m_visualizeDepth = static_cast<WorkspaceDiwne&>(diwne).m_visualizeDepth;
		Core::MatrixTracker* tracker = t->tracker;
		const Core::MatrixTracker::TrackedTransform* transform = tracker->getInterpolatedTransform();
		if (transform->data.space >= Core::TransformSpace::Projection)
		{
			auto tCameraPtr = m_viewportCamera.lock()->m_trackedCameraModel.lock();
			const Core::MatrixTracker::TrackedMatrix* trackedMatrix = tracker->getInterpolatedMatrixObject();

			float tt = transform->data.progress;
			float mt = trackedMatrix->progress;

			// Modify camera position so that it doesn't intefere with the frustum and is facing the near plane
			tCameraPtr->m_modelMatrix = glm::mat4(1.f);
			if (abs(trackedMatrix->cameraNDCOffset > 0))
			{
				float zPos = trackedMatrix->moveCameraOutOfNDC ? mt * trackedMatrix->cameraNDCOffset
				                                               : trackedMatrix->cameraNDCOffset;
				tCameraPtr->m_modelMatrix = glm::translate(tCameraPtr->m_modelMatrix, glm::vec3(0.0f, 0.0f, zPos));
			}
			if (trackedMatrix->ndcType == Core::NDCType::MinusOneToOne)
			{
				// tCameraPtr->m_modelMatrix = Math::flipAxis(tCameraPtr->m_modelMatrix, 2);
				glm::mat4 neg(1.f);
				neg[2][2] = -1;
				// tCameraPtr->m_modelMatrix = neg * tCameraPtr->m_modelMatrix;
				tCameraPtr->m_modelMatrix =
				    Math::lerp(glm::mat4(1.f), neg, trackedMatrix->space == Core::TransformSpace::Screen ? 1.f : mt,
				               false) *
				    tCameraPtr->m_modelMatrix;
			}
			// Hide the camera and near frustum indicators gradually when tracking screen space
			if (transform->data.space >= Core::TransformSpace::Screen)
			{
				tCameraPtr->m_opaque = false;
				tCameraPtr->m_opacity = 1.f - mt;
				m_viewportCamera.lock()->m_trackedFrustumNear.lock()->m_opacity = 1.f - mt;
			}
			else
			{
				tCameraPtr->m_opaque = true;
				tCameraPtr->m_opacity = 1.f;
				m_viewportCamera.lock()->m_trackedFrustumNear.lock()->m_opacity = 1.f;
			}
		}
	}
}

void Camera::popupContent(DIWNE::DrawInfo& context)
{
	drawMenuSetEditable();

	ImGui::Separator();

	popupContentTracking();

	ImGui::Separator();

	if (I3TGui::MenuItemWithLog(ICON_TBD(ICON_FA_DISPLAY " ", "Show viewport"), NULL, &m_viewportEnabled))
	{
		setViewportEnabled(m_viewportEnabled);
		if (Core::GraphManager::isTracking())
			Core::GraphManager::getTracker()->requestChainUpdate();
	}

	ImGui::Separator();

	auto cameraPtr = m_viewportCamera.lock();

	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
	if (I3TGui::BeginMenuWithLog(_t("Set visibility")))
	{
		if (I3TGui::MenuItemWithLog(_t("Show axes"), NULL, cameraPtr->m_showAxes))
		{
			cameraPtr->m_showAxes = !cameraPtr->m_showAxes;
		}
		if (I3TGui::MenuItemWithLog(_t("Show camera"), NULL, cameraPtr->m_showCamera))
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
			// ImGui::SetNextItemWidth(ImGui::GetFontSize() * 6);
			ImGui::SliderFloat(_t("Opacity"), &cameraPtr->m_frustumColor.w, 0.0f, 1.0f, "%.2f");
			if (I3TGui::MenuItemWithLog(_t("Default")))
			{
				cameraPtr->m_frustumColor = glm::vec4(0.35f, 0.27f, 0.06f, cameraPtr->m_frustumColor.w);
			}
			if (I3TGui::MenuItemWithLog(_t("Red")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::RED, cameraPtr->m_frustumColor.w);
			}
			if (I3TGui::MenuItemWithLog(_t("Blue")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::BLUE, cameraPtr->m_frustumColor.w);
			}
			if (I3TGui::MenuItemWithLog(_t("Green")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::GREEN, cameraPtr->m_frustumColor.w);
			}
			if (I3TGui::MenuItemWithLog(_t("Yellow")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::YELLOW, cameraPtr->m_frustumColor.w);
			}
			if (I3TGui::MenuItemWithLog(_t("Orange")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::ORANGE, cameraPtr->m_frustumColor.w);
			}
			if (I3TGui::MenuItemWithLog(_t("Magenta")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::MAGENTA, cameraPtr->m_frustumColor.w);
			}
			if (I3TGui::MenuItemWithLog(_t("Teal")))
			{
				cameraPtr->m_frustumColor = calculateFrustumColor(Color::TEAL, cameraPtr->m_frustumColor.w);
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

	CoreNode::drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}

void Camera::popupContentTracking()
{
	auto& workspaceDiwne = static_cast<WorkspaceDiwne&>(diwne);
	if (workspaceDiwne.isTracking() && workspaceDiwne.getTracker()->getCameraID() == this->getNodebase()->getId())
	{
		if (I3TGui::MenuItemWithLog(_t("Stop tracking"), ""))
		{
			workspaceDiwne.stopTracking();
		}
		if (I3TGui::MenuItemWithLog(_t("Smooth tracking"), "", workspaceDiwne.m_smoothTracking, true))
		{
			workspaceDiwne.trackingModeSwitch();
		}
	}
	else
	{
		if (I3TGui::BeginMenuWithLog(ICON_T(ICON_FA_CROSSHAIRS " ", "Tracking")))
		{
			if (I3TGui::MenuItemWithLog(ICON_T(ICON_FA_ARROW_LEFT " ", "Start tracking from right"), ""))
			{
				workspaceDiwne.startTracking(getView().get(), false);
			}
			// if (I3TGui::MenuItemWithLog(ICON_T(ICON_FA_ARROW_RIGHT " ", "Start tracking from left"), ""))
			// {
			// 	workspaceDiwne.startTracking(getView().get(), true);
			// }
			ImGui::EndMenu();
		}
	}
}