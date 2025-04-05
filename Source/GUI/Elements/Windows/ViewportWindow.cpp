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
#include "GUI/WindowManager.h"
#include "I3T.h"
#include "Localization/Localization.h"

#include "Viewport/Viewport.h"
#include "Viewport/camera/AggregateCamera.h"
#include "Viewport/framebuffer/Framebuffer.h"
#include "Viewport/shader/GridShader.h"

using namespace UI;

using CameraMode = Vp::AggregateCamera::CameraMode;

ViewportWindow::ViewportWindow(bool show, Vp::Viewport* viewport)
    : IWindow(ICON_T(ICON_I3T_SCENE " ", "Scene View"), show)
{
	m_autoFocus = true;
	m_viewport = viewport;
	// TODO: (DR) Not sure if binding callbacks to an axis really makes sense
	//  Binding like this hides where the actual action is meant to occur, like here, binding zoom in the viewport
	//  window and its difficult to unbind stuff
	// TODO: (DR) In fact the whole axis/axes system is a little odd to me
	// m_input->bindAxis("scroll", [this](float val) { m_world->sceneZoom(val); });

	m_renderOptions.wboit = true;
	m_renderOptions.wboitFunc = 0;
	m_renderOptions.framebufferAlpha = false;
	m_renderOptions.multisample = true;
	m_renderOptions.clearColor = Config::BACKGROUND_COLOR;
	m_renderOptions.selection = true;

	// TODO: (DR) Move actions to methods so we dont repeat code here
	InputManager::setInputAction("viewpoint-right", Keys::n3);
	m_input->bindAction("viewpoint-right", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::RIGHT);
		}
	});
	InputManager::setInputAction("viewpoint-left", Keys::n3, {Keys::ctrll});
	m_input->bindAction("viewpoint-left", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::LEFT);
		}
	});
	InputManager::setInputAction("viewpoint-top", Keys::n7);
	m_input->bindAction("viewpoint-top", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::TOP);
		}
	});
	InputManager::setInputAction("viewpoint-bottom", Keys::n7, {Keys::ctrll});
	m_input->bindAction("viewpoint-bottom", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::BOTTOM);
		}
	});
	InputManager::setInputAction("viewpoint-front", Keys::n1);
	m_input->bindAction("viewpoint-front", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::FRONT);
		}
	});
	InputManager::setInputAction("viewpoint-back", Keys::n1, {Keys::ctrll});
	m_input->bindAction("viewpoint-back", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::BACK);
		}
	});
	InputManager::setInputAction("viewpoint-center-scene", Keys::home);
	m_input->bindAction("viewpoint-center-scene", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->centerOnScene(*m_viewport->getMainScene().lock().get());
		}
	});
	InputManager::setInputAction("viewpoint-center-selection", Keys::n0);
	m_input->bindAction("viewpoint-center-selection", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->centerOnSelection(*m_viewport->getMainScene().lock().get());
		}
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
	// ImVec2 main_viewport_pos = ImGui::GetMainViewport()->Pos;
	// ImGui::SetNextWindowPos(ImVec2(main_viewport_pos.x + 650,
	// main_viewport_pos.y + 20), ImGuiCond_FirstUseEver);

	// Update background according to color
	ImVec4 bgColor = I3T::getUI()->getTheme().get(EColor::SceneViewBackground);
	m_renderOptions.clearColor = glm::vec3(bgColor.x, bgColor.y, bgColor.z);

	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	GUI::dockTabStylePush();
	if (ImGui::Begin(getName(), getShowPtr(), g_WindowFlags | ImGuiWindowFlags_MenuBar))
	{
		GUI::dockTabStylePop();
		ImGui::PopStyleVar();
		// Get info about current window's dimensions
		this->updateWindowInfo();

		int windowWidth = m_windowSize.x;
		int windowHeight = m_windowSize.y;
		ImVec2 windowMin = GUI::glmToIm(m_windowMin);
		ImVec2 windowMax = GUI::glmToIm(m_windowMax);

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

		// Manipulators need to get drawn last, but here, before viewport drawing, we want to know if the user is
		// interacting with them, hence we draw them beforehand using a channel splitter
		bool manipulatorInteraction = m_viewport->m_manipulators->drawViewAxes(m_windowPos, m_windowSize);
		if (m_viewport->getSettings().scene().manipulator_enabled)
		{
			manipulatorInteraction |= m_viewport->m_manipulators->drawManipulators(m_windowPos, m_windowSize);
		}

		// Viewport channel
		m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), 0);

		// TODO: (DR) This is somewhat unclear, might need a comment, we're checking if this window is focused, but
		// through
		//  the InputManager's active input rather than asking the WindowManager
		if (InputManager::isInputActive(getInput()) && !menuInteraction && !manipulatorInteraction && m_renderTarget)
		{
			glm::vec2 relativeMousePos = WindowManager::getMousePositionForWindow(this);
			m_viewport->processInput(ImGui::GetIO().DeltaTime, relativeMousePos, m_windowSize);
			m_viewport->processSelection(m_renderTarget, relativeMousePos, m_windowSize);
		}

		m_renderOptions.lightingModel = m_viewport->getSettings().global().lighting_lightingModel;
		m_viewport->drawViewport(m_renderTarget, windowWidth, windowHeight, m_renderOptions, m_displayOptions);
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
		ImGui::PopStyleVar();
	}
	ImGui::End();
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
	if (I3TGui::ButtonWithLog(ICON_FA_EYE "###DisplayButton"))
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
		I3TGui::MenuItemWithLog(ICON_T(ICON_I3T_GRID " ", "Grid"), nullptr, &m_displayOptions.showGrid);
		I3TGui::MenuItemWithLog(ICON_T(ICON_I3T_CAMERA " ", "Cameras"), nullptr, &m_displayOptions.showCamera);
		I3TGui::MenuItemWithLog(ICON_T(ICON_I3T_FRUSTUM " ", "Frustums"), nullptr, &m_displayOptions.showFrustum);

		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (GUI::ToggleButton(ICON_I3T_EARTH "###WorldLightingButton", stg.scene().mainScene.lightFollowsCamera, true))
	{
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

	if (GUI::ToggleButton(ICON_I3T_MANIPULATOR "###ManipulatorButton", stg.scene().manipulator_enabled))
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

	ImGui::PopItemFlag();
	ImGui::PopStyleVar();
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
			ImGui::Checkbox(_t("World space lighting"), &stg.scene().mainScene.lightFollowsCamera);
			ImGui::EndMenu();
		}

		if (I3TGui::BeginMenuWithLog(_t("Manipulators")))
		{
			ImGui::Checkbox(_t("Show manipulators"), &stg.scene().manipulator_enabled);
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
				stg.global().highlight.kernelSize = 4;
				stg.global().highlight.outlineCutoff = 0.15f;
				stg.global().highlight.useDepth = true;
			}
			if (I3TGui::MenuItemWithLog(_t("High"), nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 0.8f;
				stg.global().highlight.kernelSize = 4;
				stg.global().highlight.outlineCutoff = 0.18f;
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
		if (I3TGui::MenuItemWithLog(_t("Orbit camera"), nullptr,
		                            stg.scene().mainScene.camera.mode == CameraMode::ORBIT))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->switchMode(CameraMode::ORBIT);
				stg.scene().mainScene.camera.mode = CameraMode::ORBIT;
			}
		}
		if (I3TGui::MenuItemWithLog(_t("Trackball camera"), nullptr,
		                            stg.scene().mainScene.camera.mode == CameraMode::TRACKBALL))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->switchMode(CameraMode::TRACKBALL);
				stg.scene().mainScene.camera.mode = CameraMode::TRACKBALL;
			}
		}
		if (I3TGui::MenuItemWithLog(_t("Smooth scroll"), nullptr, stg.global().camera.smoothScroll))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				stg.global().camera.smoothScroll = !stg.global().camera.smoothScroll;
				camera->getOrbitCamera()->setSmoothScroll(stg.global().camera.smoothScroll);
				camera->getTrackballCamera()->setSmoothScroll(stg.global().camera.smoothScroll);
			}
		}
		if (ImGui::SliderFloat(_t("Camera fov"), &stg.scene().mainScene.camera.fov, 1, 180, "%.1f"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->getOrbitCamera()->setFov(stg.scene().mainScene.camera.fov);
				camera->getTrackballCamera()->setFov(stg.scene().mainScene.camera.fov);
			}
		}

		ImVec2 gridButtonSize = ImVec2(ImGui::GetFontSize() * 2, 0.0f);
		ImGui::Checkbox(_t("Show grid"), &m_displayOptions.showGrid);
		ImGui::SameLine();
		GUI::ToggleButton("XZ", m_displayOptions.showGridLines, false, gridButtonSize);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(4.0f, 0.0f));
		ImGui::SameLine();
		GUI::ToggleButton("X", m_displayOptions.showGridXAxis, false, gridButtonSize);
		ImGui::SameLine();
		GUI::ToggleButton("Y", m_displayOptions.showGridYAxis, false, gridButtonSize);
		ImGui::SameLine();
		GUI::ToggleButton("Z", m_displayOptions.showGridZAxis, false, gridButtonSize);

		ImGui::Separator();

		if (I3TGui::MenuItemWithLog(_t("Viewpoint right"), "Num3"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::RIGHT);
			}
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint left"), "Ctrl+Num3"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::LEFT);
			}
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint top"), "Num7"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::TOP);
			}
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint bottom"), "Ctrl+Num7"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::BOTTOM);
			}
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint front"), "Num1"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::FRONT);
			}
		}
		if (I3TGui::MenuItemWithLog(_t("Viewpoint back"), "Ctrl+Num1"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::BACK);
			}
		}

		ImGui::Separator();

		if (I3TGui::MenuItemWithLog(_t("Center camera on selection"), "Num0"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->centerOnSelection(*m_viewport->getMainScene().lock().get());
			}
		}

		if (I3TGui::MenuItemWithLog(_t("Center camera on scene"), "Home"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->centerOnScene(*m_viewport->getMainScene().lock().get());
			}
		}

		ImGui::Separator();

		I3TGui::MenuItemWithLog(_t("Show objects"), nullptr, &m_displayOptions.showDefault);
		I3TGui::MenuItemWithLog(_t("Show axes"), nullptr, &m_displayOptions.showAxes);
		I3TGui::MenuItemWithLog(_t("Show grid"), nullptr, &m_displayOptions.showGrid);
		I3TGui::MenuItemWithLog(_t("Show cameras"), nullptr, &m_displayOptions.showCamera);
		I3TGui::MenuItemWithLog(_t("Show frustums"), nullptr, &m_displayOptions.showFrustum);

		ImGui::EndMenu();
	}
	ImGui::PopItemFlag();

	return userInteractedWithMenus;

	//	World* w = App::get().world();
	//	if (I3TGui::BeginMenu("Viewports"))
	//	{
	//		// Ptr<UI::Viewport> ww = I3T::getWindowPtr<UI::Viewport>();
	//		if (I3TGui::MenuItemWithLog("View-x"))
	//		{
	//			// Num 1
	//			w->getCameraControl()->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), false);
	//		}
	//
	//		if (I3TGui::MenuItemWithLog("View-y"))
	//		{
	//			// Num 2
	//			w->getCameraControl()->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), true);
	//		}
	//
	//		if (I3TGui::MenuItemWithLog("View-z"))
	//		{
	//			// Num 3
	//			w->getCameraControl()->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), false);
	//		}
	//
	//		if (I3TGui::MenuItemWithLog("World-x"))
	//		{
	//			// Num 4
	//			w->getCameraControl()->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), true);
	//		}
	//
	//		if (I3TGui::MenuItemWithLog("World-y"))
	//		{
	//			// Num 5
	//			w->getCameraControl()->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), true);
	//		}
	//
	//		if (I3TGui::MenuItemWithLog("World-z"))
	//		{
	//			// Num 6
	//			w->getCameraControl()->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), true);
	//		}
	//
	//		if (I3TGui::MenuItemWithLog("Center"))
	//		{
	//			// Num 0
	//			// App::get().world()->scene->setCamToCenter();
	//		}
	//		ImGui::Separator();
	//		const char* action = w->getCameraControl()->m_rotateAroundCenter ? "Orbit eye" : "Orbit center";
	//		if (I3TGui::MenuItemWithLog(action))
	//		{
	//			w->getCameraControl()->m_rotateAroundCenter = !w->getCameraControl()->m_rotateAroundCenter;
	//			// Num 0
	//			// App::get().world()->scene->setCamToCenter();
	//		}
	//		ImGui::EndMenu();
	//	}
	//	if (I3TGui::BeginMenu("Manipulators"))
	//	{
	//		const char* action = w->manipulatorsGetVisible() ? "Hide" : "Show";
	//		if (I3TGui::MenuItemWithLog(action))
	//		{
	//			w->manipulatorsSetVisible(!w->manipulatorsGetVisible());
	//		}
	//		ImGui::EndMenu();
	//	}
}
