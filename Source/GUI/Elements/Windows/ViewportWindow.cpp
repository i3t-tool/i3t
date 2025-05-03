/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "ViewportWindow.h"

#include "Config.h"
#include "Core/Input/InputManager.h"
#include "GUI/Fonts/Icons.h"
#include "GUI/I3TGui.h"
#include "GUI/Toolkit.h"
#include "GUI/Viewport/ViewportModule.h"
#include "GUI/WindowManager.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "I3T.h"
#include "Localization/Localization.h"

#include "Viewport/Viewport.h"
#include "Viewport/camera/AggregateCamera.h"
#include "Viewport/entity/GridObject.h"
#include "Viewport/framebuffer/Framebuffer.h"
#include "Viewport/shader/GridShader.h"

using namespace UI;

using CameraMode = Vp::AggregateCamera::CameraMode;

ViewportWindow::ViewportWindow(ViewportModule* module, int index, bool show)
    : IWindow(ICON_TS(ICON_I3T_SCENE " ", "Scene View") + " " + std::to_string(index), index, show), m_module(module),
      m_viewport(&module->getViewport())
{
	m_autoFocus = true;
	// TODO: (DR) Not sure if binding callbacks to an axis really makes sense
	//  Binding like this hides where the actual action is meant to occur, like here, binding zoom in the viewport
	//  window and its difficult to unbind stuff
	// TODO: (DR) In fact the whole axis/axes system is a little odd to me
	// m_input->bindAxis("scroll", [this](float val) { m_world->sceneZoom(val); });

	assert(index >= 1);
	if (index == 1)
	{
		assert(std::dynamic_pointer_cast<Vp::AggregateCamera>(m_viewport->getMainScene()->m_camera) != nullptr);
		m_camera = std::static_pointer_cast<Vp::AggregateCamera>(m_viewport->getMainScene()->m_camera);
	}
	else
	{
		m_camera = std::make_shared<Vp::AggregateCamera>();
	}

	m_renderOptions.wboit = true;
	m_renderOptions.wboitFunc = 0;
	m_renderOptions.framebufferAlpha = false;
	m_renderOptions.multisample = true;
	m_renderOptions.clearColor = Config::BACKGROUND_COLOR;
	m_renderOptions.selection = true;

	// TODO: (DR) Move actions to methods so we dont repeat code here
	InputManager::setInputAction("viewpoint-right", Keys::n3);
	m_input->bindAction("viewpoint-right", EKeyState::Pressed, [&]() {
		m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::RIGHT);
	});
	InputManager::setInputAction("viewpoint-left", Keys::n3, {Keys::ctrll});
	m_input->bindAction("viewpoint-left", EKeyState::Pressed, [&]() {
		m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::LEFT);
	});
	InputManager::setInputAction("viewpoint-top", Keys::n7);
	m_input->bindAction("viewpoint-top", EKeyState::Pressed, [&]() {
		m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::TOP);
	});
	InputManager::setInputAction("viewpoint-bottom", Keys::n7, {Keys::ctrll});
	m_input->bindAction("viewpoint-bottom", EKeyState::Pressed, [&]() {
		m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::BOTTOM);
	});
	InputManager::setInputAction("viewpoint-front", Keys::n1);
	m_input->bindAction("viewpoint-front", EKeyState::Pressed, [&]() {
		m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::FRONT);
	});
	InputManager::setInputAction("viewpoint-back", Keys::n1, {Keys::ctrll});
	m_input->bindAction("viewpoint-back", EKeyState::Pressed, [&]() {
		m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::BACK);
	});
	InputManager::setInputAction("viewpoint-center-scene", Keys::home);
	m_input->bindAction("viewpoint-center-scene", EKeyState::Pressed, [&]() {
		m_camera->centerOnScene(*m_viewport->getMainScene());
	});
	InputManager::setInputAction("viewpoint-center-selection", Keys::n0);
	m_input->bindAction("viewpoint-center-selection", EKeyState::Pressed, [&]() {
		m_camera->centerOnSelection(*m_viewport->getMainScene());
	});

	m_input->bindAction("trackingEscOff", EKeyState::Pressed, [&]() {
		WorkspaceModule::g_editor->stopTracking();
	});
	m_input->bindAxis("trackingSmoothLeft", [&](float val) {
		WorkspaceModule::g_editor->trackingSmoothLeft();
	});
	m_input->bindAxis("trackingSmoothRight", [&](float val) {
		WorkspaceModule::g_editor->trackingSmoothRight();
	});
	m_input->bindAction("trackingJaggedLeft", EKeyState::Pressed, [&]() {
		WorkspaceModule::g_editor->trackingJaggedLeft();
	});
	m_input->bindAction("trackingJaggedRight", EKeyState::Pressed, [&]() {
		WorkspaceModule::g_editor->trackingJaggedRight();
	});

	/// \todo MH This is example code, it can be removed anytime.
	//	InputManager::setInputAction("fire", Keys::b);
	//	InputManager::setInputAction("fire", Keys::m);
	//	InputManager::setInputAxis("move", 1.0f, Keys::o);
	//	InputManager::setInputAxis("move", -1.0f, Keys::p);
	//
	//	m_input->bindAction("fire", EKeyState::Pressed, []() { LOG_INFO("Action fired."); });
	//	m_input->bindAction("fire", EKeyState::Released, []() { LOG_INFO("Action released."); });
	//	m_input->bindAxis("move", [](float val) { LOG_INFO("move: {}", val); });
	/// todoend
}

void ViewportWindow::render()
{
	// Update title if necessary
	if (m_instanceIndex == 1)
	{
		bool showNum = false;
		for (int i = 1; i < m_module->getWindowCount(); i++)
		{
			auto& window = m_module->getWindow(i);
			if (window->isVisible())
			{
				showNum = true;
				break;
			}
		}
		if (showNum)
			setTitle(ICON_TS(ICON_I3T_SCENE " ", "Scene View") + " " + std::to_string(m_instanceIndex));
		else
			setTitle(ICON_TS(ICON_I3T_SCENE " ", "Scene View"));
	}

	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, I3T::getTheme().getBorderSize());
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Border, I3T::getTheme().get(EColor::BorderDim));
	GUI::dockTabStylePush();
	if (ImGui::Begin(getName(), getShowPtr(), g_WindowFlags | ImGuiWindowFlags_MenuBar))
	{
		GUI::dockTabStylePop(); // MUST BE POPPED IN THE WINDOW BEGIN ELSE STATEMENT TOO!!
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
		// Get info about current window's dimensions
		this->updateWindowInfo();

		// Update render options
		m_renderOptions.lightingModel = m_viewport->getSettings().global().lighting_lightingModel;
		// Update background according to color
		ImVec4 bgColor = I3T::getUI()->getTheme().get(EColor::SceneViewBackground);
		m_renderOptions.clearColor = glm::vec3(bgColor.x, bgColor.y, bgColor.z);

		bool menuInteraction = false;
		if (ImGui::BeginMenuBar())
		{
			menuInteraction |= showViewportMenu();
			ImGui::EndMenuBar();
		}

		// Some UI elements need to get drawn last to overlay them over the viewport, however we want to know if the
		// user has interacted with them before drawing the viewport, hence we draw them beforehand using a channel
		// splitter.
		m_channelSplitter.Split(ImGui::GetWindowDrawList(), 2);

		// UI overlay channel
		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), 1);

		menuInteraction |= showViewportButtons();

		glm::mat4 view = m_camera->getView();

		// Update reference space
		updateSpace();
		menuInteraction |= showSpaceIndicators(view);

		bool manipulatorInteraction = false;
		// Manipulators need to get drawn last, but here, before viewport drawing, we want to know if the user is
		// interacting with them, hence we draw them beforehand using a channel splitter
		if (m_settings.manipulator_enabled && m_instanceIndex == 1)
		// if (m_settings.manipulator_enabled)
		{
			manipulatorInteraction |= m_viewport->m_manipulators->drawManipulators(
			    m_windowPos, m_windowSize, &m_space.m_referenceSpace, view, m_camera->getProjection());
		}

		// Viewport channel
		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), 0);

		bool windowFocused = InputManager::isInputActive(getInput());

		// Process viewport input
		// TODO: (DR) This is somewhat unclear, might need a comment, we're checking if this window is focused, but
		//  through the InputManager's active input rather than asking the WindowManager
		if (windowFocused && !menuInteraction && !manipulatorInteraction && m_renderTarget)
		{
			glm::vec2 relativeMousePos = WindowManager::getMousePositionForWindow(this);
			m_camera->processInput(ImGui::GetIO().DeltaTime, relativeMousePos, m_windowSize);
			m_viewport->processSelection(m_renderTarget, relativeMousePos, m_windowSize);
		}

		// Draw viewport
		int windowWidth = m_windowSize.x;
		int windowHeight = m_windowSize.y;
		ImVec2 windowMin = GUI::glmToIm(m_windowMin);
		ImVec2 windowMax = GUI::glmToIm(m_windowMax);

		m_viewport->drawViewport(m_renderTarget, windowWidth, windowHeight, m_camera, m_space.m_referenceSpace,
		                         m_renderOptions, m_displayOptions);

		Ptr<Vp::Framebuffer> framebuffer = m_renderTarget->getOutputFramebuffer().lock();
		if (framebuffer)
		{
			GLuint texture = framebuffer->getColorTexture();
			// the uv coordinates flips the picture, since it was upside down at first
			ImGui::GetWindowDrawList()->AddImage((void*) (intptr_t) texture, windowMin, windowMax, ImVec2(0, 1),
			                                     ImVec2(1, 0));
		}
		else
		{
			ImGui::Text("Failed to draw viewport!");
		}

		m_channelSplitter.Merge(ImGui::GetWindowDrawList());
	}
	else
	{
		GUI::dockTabStylePop();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
	}
	ImGui::End();
}

void ViewportWindow::updateSpace()
{
	Vp::ViewportSettings& stg = m_viewport->getSettings();

	bool spaceSet = false;
	bool labelSet = false;

	// Reset some variables
	stg.global().grid.programShow = true;         // Disabling world grid for projection spaces
	stg.global().localGrid.programStrength = 1.f; // Fading local grid in view space

	m_space.tracking = Core::GraphManager::isTracking() && m_instanceIndex == 1;
	m_space.trackingSpaceParam = 0.f;
	m_space.trackingSpace = Core::TransformSpace::Model;
	m_space.simulateLHS = false;
	m_space.trackingMatrixProgress = 0.f;

	// TODO: Dim scene view background when tracking or using ref space

	// React to active tracking
	if (m_space.tracking)
	{
		Core::MatrixTracker* tracker = Core::GraphManager::getTracker();
		if (!tracker->m_trackInWorldSpace)
		{
			// TODO: Add interpolation slider or different progress indicator
			//  The slider should indicate progress from one space to another
			spaceSet = true;
			m_space.m_referenceSpace = tracker->m_iProjMatrix * tracker->m_iViewMatrix;

			const Core::MatrixTracker::TrackedTransform* transform = tracker->getInterpolatedTransform();
			m_space.trackingSpaceParam = transform->data.progress;
			m_space.trackingSpace = transform->data.space;

			const Core::MatrixTracker::TrackedMatrix* trackedMatrix = tracker->getInterpolatedMatrixObject();
			m_space.trackingMatrixProgress = trackedMatrix->progress;

			switch (m_space.trackingSpace)
			{
			case Core::TransformSpace::Model:
				m_space.label = m_space.worldSpaceLabel;
				break;
			case Core::TransformSpace::View:
				// TODO: Maybe add the name of the camera?
				m_space.label = _tbd("View space");
				stg.global().localGrid.programStrength = m_space.trackingSpaceParam;
				break;
			case Core::TransformSpace::Projection:
			{
				// TODO: Apply fade out shaders
				// TODO: World grid can be shown in ortho, but with perspective the grid shader cannot be used
				m_space.label = _tbd("NDC space");
				stg.global().grid.programShow = false; // Disabling world grid for projection spaces

				// Switch view axes to LHS mode
				if (tracker->getInterpolatedMatrixObject()->useLHS)
				{
					m_space.simulateLHS = true;
				}
			}
			break;
			}
			labelSet = true;
		}
	}
	else
	{
		// React to reference space set from a sequence/node
		if (m_space.customSource)
		{
			if (m_space.sourceNode.expired())
			{
				m_space.customSource = false;
				m_space.sourceNode.reset();
			}
			else
			{
				spaceSet = true;
				m_space.m_referenceSpace =
				    glm::inverse(m_space.sourceNode.lock()->getNodebase()->data(Core::I3T_SEQ_IN_MUL).getMat4());
			}
		}
	}

	if (!spaceSet)
		m_space.m_referenceSpace = glm::identity<glm::mat4>();

	m_space.standard = m_space.m_referenceSpace == glm::identity<glm::mat4>();
	if (m_space.standard)
	{
		if (!labelSet)
			m_space.label = _tbd("World space");
		m_space.labelCol = ImGui::GetStyleColorVec4(ImGuiCol_Text);
		m_space.m_referenceSpaceInv = m_space.m_referenceSpace;
	}
	else
	{
		if (!labelSet)
			m_space.label = _tbd("Local space");
		m_space.labelCol = I3T::getTheme().get(EColor::SceneViewGridLocalColor, 1.f);
		m_space.m_referenceSpaceInv = glm::inverse(m_space.m_referenceSpace);
	}

	updateGrids();
}

void ViewportWindow::updateGrids()
{
	// Setup world and local grid
	if (m_module->getSettings().keepWorldGridStatic)
	{
		auto mainScene = m_viewport->getMainScene();
		mainScene->m_worldGrid->setReferenceSpace(m_space.m_referenceSpace, m_space.m_referenceSpaceInv);
	}
	else
	{
		auto mainScene = m_viewport->getMainScene();
		mainScene->m_localGrid->setReferenceSpace(m_space.m_referenceSpace, m_space.m_referenceSpaceInv);
	}

	m_displayOptions.localGrid.show = !m_space.standard;
	if (!m_settings.showLocalGrid)
		m_displayOptions.localGrid.show = false;
}

bool ViewportWindow::showSpaceIndicators(glm::mat4& view)
{
	bool interacted = false;
	int axesSize = 6.125f * ImGui::GetFontSize();
	int padding = 0.5f * ImGui::GetFontSize();
	ImVec2 axesPosition = ImVec2(m_windowPos.x + m_windowSize.x - axesSize - padding, m_windowPos.y + padding);

	Theme& theme = I3T::getTheme();

	float oldAlpha = ImGui::GetStyle().Alpha;

	if (m_space.tracking && m_space.trackingSpace == Core::TransformSpace::Projection)
		ImGui::GetStyle().Alpha = pow(1.f - m_space.trackingSpaceParam, 4.f);

	const float& alpha = ImGui::GetStyle().Alpha;

	// Draw world space axes, except when tracking projection
	ImGuizmo::GetStyle().Colors[ImGuizmo::COLOR::DIRECTION_X] = theme.get(EColor::SceneViewGridX, alpha);
	ImGuizmo::GetStyle().Colors[ImGuizmo::COLOR::DIRECTION_Y] = theme.get(EColor::SceneViewGridY, alpha);
	ImGuizmo::GetStyle().Colors[ImGuizmo::COLOR::DIRECTION_Z] = theme.get(EColor::SceneViewGridZ, alpha);

	// TODO: Add a theme variable for axes text color
	ImGui::PushStyleColor(ImGuiCol_Text, theme.get(EColor::TextDark));
	interacted |= m_viewport->m_manipulators->drawViewAxes(
	    m_windowPos, m_windowSize, axesPosition, ImVec2(axesSize, axesSize),
	    m_space.standard ? nullptr : &m_space.m_referenceSpace, view, m_camera->getProjection());
	ImGui::PopStyleColor();

	// Draw local space axes if applicable
	if (!m_space.standard)
	{
		// Draw the world space label
		float tWidth = ImGui::CalcTextSize(m_space.worldSpaceLabel.c_str()).x;
		ImGui::SetCursorScreenPos({axesPosition.x + axesSize / 2 - tWidth / 2, axesPosition.y + axesSize});
		GUI::TextShadow(m_space.worldSpaceLabel.c_str());

		if (m_space.tracking)
		{
			if (m_space.trackingSpace == Core::TransformSpace::Projection)
				ImGui::GetStyle().Alpha = 1.0f;
			if (m_space.trackingSpace == Core::TransformSpace::View)
				ImGui::GetStyle().Alpha = m_space.trackingSpaceParam;
		}

		ImGuizmo::GetStyle().Colors[ImGuizmo::COLOR::DIRECTION_X] = theme.get(EColor::SceneViewGridLocalX, alpha);
		ImGuizmo::GetStyle().Colors[ImGuizmo::COLOR::DIRECTION_Y] = theme.get(EColor::SceneViewGridLocalY, alpha);
		ImGuizmo::GetStyle().Colors[ImGuizmo::COLOR::DIRECTION_Z] = theme.get(EColor::SceneViewGridLocalZ, alpha);

		float worldAxesSize = axesSize + padding + ImGui::GetTextLineHeightWithSpacing();
		if (m_space.tracking && m_space.trackingSpace == Core::TransformSpace::Projection)
			worldAxesSize *= 1.f - m_space.trackingSpaceParam;

		axesPosition =
		    ImVec2(m_windowPos.x + m_windowSize.x - axesSize - padding, m_windowPos.y + padding + worldAxesSize);

		ImGui::PushStyleColor(ImGuiCol_Text, theme.get(EColor::TextDark));

		// glm::mat4* viewPtr = &view;
		// glm::mat4 viewMod;
		// if (m_space.simulateLHS)
		// {
		// 	viewMod = Math::flipAxis(view, 0);
		// 	viewPtr = &viewMod;
		// }

		glm::vec3 aFacs;
		float* axisFactors = nullptr;
		if (m_space.simulateLHS)
		{
			aFacs = glm::vec3(Math::range(m_space.trackingMatrixProgress, 0, 1, 1, -1), 1.f, 1.f);
			axisFactors = glm::value_ptr(aFacs);
		}

		interacted |= m_viewport->m_manipulators->drawViewAxes(m_windowPos, m_windowSize, axesPosition,
		                                                       ImVec2(axesSize, axesSize), nullptr, view,
		                                                       m_camera->getProjection(), axisFactors);
		ImGui::PopStyleColor();

		tWidth = ImGui::CalcTextSize(m_space.label.c_str()).x;

		ImVec2 textPos = {axesPosition.x + axesSize / 2 - tWidth / 2, axesPosition.y + axesSize};
		ImGui::SetCursorScreenPos(textPos);
		GUI::TextColoredShadow((m_space.label).c_str(), m_space.labelCol);

		if (m_space.simulateLHS && m_space.trackingMatrixProgress >= 0.5f)
		{
			std::string str = _tbd("(Left handed)");
			tWidth = ImGui::CalcTextSize(str.c_str()).x;

			textPos = {axesPosition.x + axesSize / 2 - tWidth / 2,
			           axesPosition.y + axesSize + ImGui::GetTextLineHeightWithSpacing()};
			ImGui::SetCursorScreenPos(textPos);
			GUI::TextColoredShadow(str.c_str(), m_space.labelCol);
		}
	}

	ImGui::GetStyle().Alpha = oldAlpha;

	// TODO: Better indicators (more prominent text, world space axes?, interpolation indicator, buttons to disable
	//   either of the grids, perhaps even grid opacity control)

	return interacted;
}

bool ViewportWindow::showViewportButtons()
{
	bool interacted = false;

	Vp::ViewportSettings& stg = m_viewport->getSettings();

	float popupTitleDividerSize = 0.3f * ImGui::GetTextLineHeight();

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

	glm::vec2 topLeftButtonsOffset = 0.4f * glm::vec2(ImGui::GetFontSize(), ImGui::GetFontSize());
	ImVec2 topLeftButtonsOrigin = GUI::glmToIm(this->m_windowMin + topLeftButtonsOffset);

	ImGui::SetCursorScreenPos(topLeftButtonsOrigin);

	// Object visibility options
	const char* displayPopupId = "popup_display";
	if (GUI::FloatingButton(ICON_FA_EYE "###DisplayButton"))
	{
		interacted = true;
		ImGui::OpenPopup(displayPopupId);
	}
	else
	{
		if (ImGui::IsItemHovered())
		{
			interacted = true;
			GUI::Tooltip(_t("Object visibility toggle"), "");
		}
	}

	if (ImGui::BeginPopup(displayPopupId))
	{
		interacted = true;
		ImGui::TextDisabled(_t("Object visibility"));
		ImGui::Dummy({0.0f, popupTitleDividerSize});

		I3TGui::MenuItemWithLog(ICON_T(ICON_I3T_MODEL " ", "Objects"), nullptr, &m_displayOptions.showDefault);
		I3TGui::MenuItemWithLog(ICON_T(ICON_I3T_MANIPULATOR " ", "Axes"), nullptr, &m_displayOptions.showAxes);
		I3TGui::MenuItemWithLog(ICON_T(ICON_I3T_GRID " ", "Grid"), nullptr, &m_displayOptions.showGrids);
		I3TGui::MenuItemWithLog(ICON_T(ICON_I3T_CAMERA " ", "Cameras"), nullptr, &m_displayOptions.showCamera);
		I3TGui::MenuItemWithLog(ICON_T(ICON_I3T_FRUSTUM " ", "Frustums"), nullptr, &m_displayOptions.showFrustum);

		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (m_instanceIndex == 1)
	{
		if (GUI::FloatingToggleButton(ICON_I3T_EARTH "###WorldLightingButton",
		                              m_module->getSettings().lightFollowsCamera, true))
		{
			m_viewport->getMainScene()->m_lightFollowsCamera = m_module->getSettings().lightFollowsCamera;
			interacted = true;
		}
		else
		{
			if (ImGui::IsItemHovered())
			{
				interacted = true;
				GUI::Tooltip(_t("Toggle world lighting"), "");
			}
		}
		ImGui::SameLine();
	}

	if (GUI::FloatingToggleButton(ICON_I3T_MANIPULATOR "###ManipulatorButton", m_settings.manipulator_enabled))
	{
		interacted = true;
	}
	else
	{
		if (ImGui::IsItemHovered())
		{
			interacted = true;
			GUI::Tooltip(_t("Toggle manipulators"), "");
		}
	}
	ImGui::SameLine();

	if (m_displayOptions.showGrids)
	{
		interacted |= GUI::FloatingToggleButton(ICON_FA_TABLE_CELLS "###GridBtn", m_displayOptions.grid.show);
		interacted |= GUI::ItemTooltip(m_space.standard ? _tbd("Toggle grid") : _tbd("Toggle world grid"), "");
		ImGui::SameLine();

		if (!m_space.standard)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, I3T::getTheme().get(EColor::SceneViewGridLocalColor, 1.f));
			interacted |= GUI::FloatingToggleButton(ICON_FA_TABLE_CELLS "###GridNLocalBtn", m_settings.showLocalGrid);
			ImGui::PopStyleColor();
			interacted |= GUI::ItemTooltip(_tbd("Toggle local grid"), "");
			ImGui::SameLine();
		}
	}

	ImGui::PopItemFlag();
	ImGui::PopStyleVar();

	// TESTING BEGIN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// TODO: Move this into some kind of a popup / collapsible item below the local view axes
	{
		bool valChanged = false;
		int row, col;
		float val;
		GUI::DrawMatrix("refSpaceMat", m_space.m_referenceSpace, 2, Core::EValueState::Editable, valChanged, row, col,
		                val);
		if (valChanged)
		{
			m_space.m_referenceSpace[col][row] = val;
		}
	}

	return interacted;
}

bool ViewportWindow::showViewportMenu()
{
	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

	Vp::ViewportSettings& stg = m_viewport->getSettings();

	bool userInteractedWithMenus = false;
	if (I3TGui::BeginMenuWithLog(_t("Settings")))
	{
		userInteractedWithMenus = true;

		if (I3TGui::BeginMenuWithLog(_t("Scene")))
		{
			bool b = !m_module->getSettings().lightFollowsCamera;
			if (ImGui::Checkbox(_t("World space lighting"), &b))
			{
				m_module->getSettings().lightFollowsCamera = !b;
				m_viewport->getMainScene()->m_lightFollowsCamera = m_module->getSettings().lightFollowsCamera;
			}
			ImGui::EndMenu();
		}

		if (I3TGui::BeginMenuWithLog(_t("Manipulators")))
		{
			ImGui::Checkbox(_t("Show manipulators"), &m_settings.manipulator_enabled);
			ImGui::SliderFloat(_t("Size"), &stg.global().manipulator_size, 0.01f, 1.0f, "%.2f");
			ImGui::EndMenu();
		}

		bool msaaOff = !m_renderOptions.multisample;
		bool msaa2x = m_renderOptions.multisample && m_renderOptions.samples == 2;
		bool msaa4x = m_renderOptions.multisample && m_renderOptions.samples == 4;
		bool msaa8x = m_renderOptions.multisample && m_renderOptions.samples == 8;
		if (I3TGui::BeginMenuWithLog(_t("MSAA")))
		{
			if (I3TGui::MenuItemWithLog(_t("OFF"), nullptr, &msaaOff))
			{
				m_renderOptions.multisample = false;
			}
			if (I3TGui::MenuItemWithLog("2x", nullptr, &msaa2x))
			{
				m_renderOptions.multisample = true;
				m_renderOptions.samples = 2;
			}
			if (I3TGui::MenuItemWithLog("4x", nullptr, &msaa4x))
			{
				m_renderOptions.multisample = true;
				m_renderOptions.samples = 4;
			}
			if (I3TGui::MenuItemWithLog("8x", nullptr, &msaa8x))
			{
				m_renderOptions.multisample = true;
				m_renderOptions.samples = 8;
			}
			ImGui::EndMenu();
		}

		if (I3TGui::BeginMenuWithLog(_t("Highlight")))
		{
			userInteractedWithMenus = true;
			if (I3TGui::MenuItemWithLog(_t("Ultra"), nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 1.0f;
				stg.global().highlight.kernelSize = 3;
				stg.global().highlight.outlineCutoff = 0.15f;
				stg.global().highlight.useDepth = true;
			}
			if (I3TGui::MenuItemWithLog(_t("High"), nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 0.7f;
				stg.global().highlight.kernelSize = 3;
				stg.global().highlight.outlineCutoff = 0.3f;
				stg.global().highlight.useDepth = true;
			}
			if (I3TGui::MenuItemWithLog(_t("Medium"), nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 0.5f;
				stg.global().highlight.kernelSize = 2;
				stg.global().highlight.outlineCutoff = 0.23f;
				stg.global().highlight.useDepth = true;
			}
			if (I3TGui::MenuItemWithLog(_t("Low"), nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 1.0f / 3;
				stg.global().highlight.kernelSize = 2;
				stg.global().highlight.outlineCutoff = 0.3f;
				stg.global().highlight.useDepth = true;
			}
			if (I3TGui::MenuItemWithLog(_t("Lowest"), nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 0.25;
				stg.global().highlight.kernelSize = 2;
				stg.global().highlight.outlineCutoff = 0.5f;
				stg.global().highlight.useDepth = false;
			}
			ImGui::EndMenu();
		}

		if (I3TGui::BeginMenuWithLog(_t("Transparency")))
		{
			I3TGui::MenuItemWithLog(_t("Use WBOIT"), nullptr, &m_renderOptions.wboit);
			if (I3TGui::BeginMenuWithLog(_t("WBOIT weight function")))
			{
				if (I3TGui::MenuItemWithLog(_t("OFF"), nullptr, m_renderOptions.wboitFunc == 0))
				{
					m_renderOptions.wboitFunc = 0;
				}
				if (I3TGui::MenuItemWithLog("Equation 7", nullptr, m_renderOptions.wboitFunc == 1))
				{
					m_renderOptions.wboitFunc = 1;
				}
				if (I3TGui::MenuItemWithLog("Equation 8", nullptr, m_renderOptions.wboitFunc == 2))
				{
					m_renderOptions.wboitFunc = 2;
				}
				if (I3TGui::MenuItemWithLog("Equation 9", nullptr, m_renderOptions.wboitFunc == 3))
				{
					m_renderOptions.wboitFunc = 3;
				}
				if (I3TGui::MenuItemWithLog("Equation 10", nullptr, m_renderOptions.wboitFunc == 4))
				{
					m_renderOptions.wboitFunc = 4;
				}
				if (I3TGui::MenuItemWithLog("LOGL Eq. 9 color bias", nullptr, m_renderOptions.wboitFunc == 5))
				{
					m_renderOptions.wboitFunc = 5;
				}
				if (I3TGui::MenuItemWithLog("LOGL Eq. 10", nullptr, m_renderOptions.wboitFunc == 6))
				{
					m_renderOptions.wboitFunc = 6;
				}
				if (I3TGui::MenuItemWithLog("z^-3", nullptr, m_renderOptions.wboitFunc == 7))
				{
					m_renderOptions.wboitFunc = 7;
				}
				if (I3TGui::MenuItemWithLog("abs(z - zFar + Eps)", nullptr, m_renderOptions.wboitFunc == 8))
				{
					m_renderOptions.wboitFunc = 8;
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		// TODO: (DR) Probably move to preferences or help/debug menu
		if (I3TGui::MenuItemWithLog(_t("Reload shaders"), nullptr, nullptr))
		{
			bool ok = Vp::Shaders::instance().reload();
			// Flash green if all shaders reloaded successfully, red otherwise
			ImGui::GetForegroundDrawList()->AddRectFilled(GUI::glmToIm(m_windowPos), GUI::glmToIm(m_windowMax),
			                                              ImColor((ok ? 0 : 255), (ok ? 255 : 0), 0));
		}

		ImGui::EndMenu();
	}

	// TODO: (DR) To follow the "unified api methodology", the UI here should only update the viewport settings and not
	//  actually perform the changes. Although this is more "efficient" in a way. But cumbersome I suppose.
	if (I3TGui::BeginMenuWithLog(_t("View")))
	{
		userInteractedWithMenus = true;

		if (I3TGui::MenuItemWithLog(_t("Center camera on selection"), "Num0"))
		{

			m_camera->centerOnSelection(*m_viewport->getMainScene());
		}

		if (I3TGui::MenuItemWithLog(_t("Center camera on scene"), "Home"))
		{
			m_camera->centerOnScene(*m_viewport->getMainScene());
		}

		ImGui::Separator();

		if (I3TGui::MenuItemWithLog(_t("Orbit camera"), nullptr, m_settings.camera.mode == CameraMode::ORBIT))
		{
			m_settings.camera.mode = CameraMode::ORBIT;
			m_camera->switchMode(CameraMode::ORBIT);
		}
		if (I3TGui::MenuItemWithLog(_t("Trackball camera"), nullptr, m_settings.camera.mode == CameraMode::TRACKBALL))
		{
			m_settings.camera.mode = CameraMode::TRACKBALL;
			m_camera->switchMode(CameraMode::TRACKBALL);
		}
		if (I3TGui::MenuItemWithLog(_t("Smooth scroll"), nullptr, stg.global().camera.smoothScroll))
		{
			stg.global().camera.smoothScroll = !stg.global().camera.smoothScroll;
			m_module->updateGlobal();
		}
		if (ImGui::SliderFloat(_t("Camera fov"), &m_settings.camera.fov, 1, 180, "%.1f"))
		{
			m_camera->setFov(m_settings.camera.fov);
		}

		ImVec2 gridButtonSize = ImVec2(ImGui::GetFontSize() * 2, 0.0f);
		ImGui::Checkbox(_t("Show grid"), &m_displayOptions.grid.show);
		ImGui::SameLine();
		GUI::ToggleButton("XZ", m_displayOptions.grid.showGridLines, false, gridButtonSize);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(4.0f, 0.0f));
		ImGui::SameLine();
		GUI::ToggleButton("X", m_displayOptions.grid.showGridXAxis, false, gridButtonSize);
		ImGui::SameLine();
		GUI::ToggleButton("Y", m_displayOptions.grid.showGridYAxis, false, gridButtonSize);
		ImGui::SameLine();
		GUI::ToggleButton("Z", m_displayOptions.grid.showGridZAxis, false, gridButtonSize);

		ImGui::Separator();

		if (I3TGui::MenuItemWithLog(_t("Viewpoint right"), "Num3"))
		{
			m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::RIGHT);
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint left"), "Ctrl+Num3"))
		{
			m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::LEFT);
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint top"), "Num7"))
		{
			m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::TOP);
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint bottom"), "Ctrl+Num7"))
		{
			m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::BOTTOM);
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint front"), "Num1"))
		{

			m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::FRONT);
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint back"), "Ctrl+Num1"))
		{
			m_camera->viewpoint(Vp::AbstractCamera::Viewpoint::BACK);
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Debug"))
	{

		ImGui::EndMenu();
	}
	ImGui::PopItemFlag();

	return userInteractedWithMenus;
}
