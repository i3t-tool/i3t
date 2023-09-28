#include "ViewportWindow.h"

#include "imgui.h"

#include "Core/API.h"
#include "Core/Application.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"

#include "../Nodes/WorkspaceElementsWithCoreData.h"

#include "GUI/Toolkit.h"
#include "GUI/WindowManager.h"
#include "Viewport/Viewport.h"
#include "Viewport/camera/AggregateCamera.h"
#include "Viewport/framebuffer/Framebuffer.h"
#include "Viewport/shader/GridShader.h"

using namespace UI;

using CameraMode = Vp::AggregateCamera::CameraMode;

ViewportWindow::ViewportWindow(bool show, Vp::Viewport* viewport) : IWindow(show)
{
	m_viewport = viewport;
	// TODO: (DR) Not sure if binding callbacks to an axis really makes sense
	//  Binding like this hides where the actual action is meant to occur, like here, binding zoom in the viewport
	//  window and its difficult to unbind stuff
	// TODO: (DR) In fact the whole axis/axes system is a little odd to me
	// Input.bindAxis("scroll", [this](float val) { m_world->sceneZoom(val); });

	m_renderOptions.wboit = true;
	m_renderOptions.wboitFunc = 0;
	m_renderOptions.framebufferAlpha = false;
	m_renderOptions.multisample = true;
	m_renderOptions.clearColor = Config::BACKGROUND_COLOR;
	m_renderOptions.selection = true;

	// TODO: (DR) Move actions to methods so we dont repeat code here
	InputManager::setInputAction("viewpoint-right", Keys::n3);
	Input.bindAction("viewpoint-right", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::RIGHT);
		}
	});
	InputManager::setInputAction("viewpoint-left", Keys::n3, {Keys::ctrll});
	Input.bindAction("viewpoint-left", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::LEFT);
		}
	});
	InputManager::setInputAction("viewpoint-top", Keys::n7);
	Input.bindAction("viewpoint-top", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::TOP);
		}
	});
	InputManager::setInputAction("viewpoint-bottom", Keys::n7, {Keys::ctrll});
	Input.bindAction("viewpoint-bottom", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::BOTTOM);
		}
	});
	InputManager::setInputAction("viewpoint-front", Keys::n1);
	Input.bindAction("viewpoint-front", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::FRONT);
		}
	});
	InputManager::setInputAction("viewpoint-back", Keys::n1, {Keys::ctrll});
	Input.bindAction("viewpoint-back", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->viewpoint(Vp::AbstractCamera::Viewpoint::BACK);
		}
	});
	InputManager::setInputAction("viewpoint-center-scene", Keys::home);
	Input.bindAction("viewpoint-center-scene", EKeyState::Pressed, [&]() {
		if (auto camera = m_viewport->getMainViewportCamera().lock())
		{
			camera->centerOnScene(*m_viewport->getMainScene().lock().get());
		}
	});
	InputManager::setInputAction("viewpoint-center-selection", Keys::n0);
	Input.bindAction("viewpoint-center-selection", EKeyState::Pressed, [&]() {
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
	//	Input.bindAction("fire", EKeyState::Pressed, []() { LOG_INFO("Action fired."); });
	//	Input.bindAction("fire", EKeyState::Released, []() { LOG_INFO("Action released."); });
	//	Input.bindAxis("move", [](float val) { LOG_INFO("move: {}", val); });
	/// todoend
}

void ViewportWindow::render()
{
	// ImVec2 main_viewport_pos = ImGui::GetMainViewport()->Pos;
	// ImGui::SetNextWindowPos(ImVec2(main_viewport_pos.x + 650,
	// main_viewport_pos.y + 20), ImGuiCond_FirstUseEver);

	// Update background according to color
	ImVec4 bgColor = App::get().getUI()->getTheme().get(EColor::SceneViewBackground);
	m_renderOptions.clearColor = glm::vec3(bgColor.x, bgColor.y, bgColor.z);

	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_TabActive, App::get().getUI()->getTheme().get(EColor::DockTabActive));
	auto name = setName("Scene View");
	ImGui::Begin(name.c_str(), getShowPtr(), g_WindowFlags | ImGuiWindowFlags_MenuBar); // | ImGuiWindowFlags_MenuBar);
	ImGui::PopStyleColor();
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

	m_channelSplitter.Split(ImGui::GetWindowDrawList(), 2);
	m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), 1);
	// Manipulators need to get drawn last, but here, before viewport drawing, we want to know if the user is
	// interacting with them, hence we draw them beforehand using a channel splitter
	bool manipulatorInteraction = m_viewport->m_manipulators->drawViewAxes(m_windowPos, m_windowSize);
	if (m_viewport->getSettings().scene().manipulator_enabled)
	{
		manipulatorInteraction |= m_viewport->m_manipulators->drawManipulators(m_windowPos, m_windowSize);
	}
	m_channelSplitter.SetCurrentChannel(ImGui::GetWindowDrawList(), 0);

	// TODO: (DR) This is somewhat unclear, might need a comment, we're checking if this window is focused, but through
	//  the InputManager's active input rather than asking the WindowManager
	if (InputManager::isInputActive(getInputPtr()) && !menuInteraction && !manipulatorInteraction && m_renderTarget)
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

	ImGui::End();
}

bool ViewportWindow::showViewportMenu()
{
	// TODO: (DR) Sometimes closing of nested menus causes a debug assertion fail, its a fixed bug as per:
	//  https://github.com/ocornut/imgui/issues/4640
	//  Can only be fixed by updating imgui

	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

	Vp::ViewportSettings& stg = m_viewport->getSettings();

	bool userInteractedWithMenus = false;
	if (ImGui::BeginMenu("Settings"))
	{
		userInteractedWithMenus = true;

		if (ImGui::BeginMenu("Scene"))
		{
			ImGui::Checkbox("World space lighting", &stg.scene().mainScene.lightFollowsCamera);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Manipulators"))
		{
			ImGui::Checkbox("Show manipulators", &stg.scene().manipulator_enabled);
			ImGui::SliderFloat("Size", &stg.global().manipulator_size, 0.01f, 1.0f, "%.2f");
			ImGui::EndMenu();
		}

		bool msaaOff = !m_renderOptions.multisample;
		bool msaa2x = m_renderOptions.multisample && m_renderOptions.samples == 2;
		bool msaa4x = m_renderOptions.multisample && m_renderOptions.samples == 4;
		bool msaa8x = m_renderOptions.multisample && m_renderOptions.samples == 8;
		if (ImGui::BeginMenu("MSAA"))
		{
			if (ImGui::MenuItem("OFF", nullptr, &msaaOff))
			{
				m_renderOptions.multisample = false;
			}
			if (ImGui::MenuItem("2x", nullptr, &msaa2x))
			{
				m_renderOptions.multisample = true;
				m_renderOptions.samples = 2;
			}
			if (ImGui::MenuItem("4x", nullptr, &msaa4x))
			{
				m_renderOptions.multisample = true;
				m_renderOptions.samples = 4;
			}
			if (ImGui::MenuItem("8x", nullptr, &msaa8x))
			{
				m_renderOptions.multisample = true;
				m_renderOptions.samples = 8;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Highlight"))
		{
			userInteractedWithMenus = true;
			if (ImGui::MenuItem("Ultra", nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 1.0f;
				stg.global().highlight.kernelSize = 4;
				stg.global().highlight.outlineCutoff = 0.15f;
				stg.global().highlight.useDepth = true;
			}
			if (ImGui::MenuItem("High", nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 0.8f;
				stg.global().highlight.kernelSize = 4;
				stg.global().highlight.outlineCutoff = 0.18f;
				stg.global().highlight.useDepth = true;
			}
			if (ImGui::MenuItem("Medium", nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 0.5f;
				stg.global().highlight.kernelSize = 2;
				stg.global().highlight.outlineCutoff = 0.23f;
				stg.global().highlight.useDepth = true;
			}
			if (ImGui::MenuItem("Low", nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 1.0f / 3;
				stg.global().highlight.kernelSize = 2;
				stg.global().highlight.outlineCutoff = 0.3f;
				stg.global().highlight.useDepth = true;
			}
			if (ImGui::MenuItem("Lowest", nullptr, nullptr))
			{
				stg.global().highlight.downscaleFactor = 0.25;
				stg.global().highlight.kernelSize = 2;
				stg.global().highlight.outlineCutoff = 0.5f;
				stg.global().highlight.useDepth = false;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Transparency"))
		{
			ImGui::MenuItem("Use WBOIT", nullptr, &m_renderOptions.wboit);
			if (ImGui::BeginMenu("WBOIT weight function"))
			{
				if (ImGui::MenuItem("OFF", nullptr, m_renderOptions.wboitFunc == 0))
				{
					m_renderOptions.wboitFunc = 0;
				}
				if (ImGui::MenuItem("Equation 7", nullptr, m_renderOptions.wboitFunc == 1))
				{
					m_renderOptions.wboitFunc = 1;
				}
				if (ImGui::MenuItem("Equation 8", nullptr, m_renderOptions.wboitFunc == 2))
				{
					m_renderOptions.wboitFunc = 2;
				}
				if (ImGui::MenuItem("Equation 9", nullptr, m_renderOptions.wboitFunc == 3))
				{
					m_renderOptions.wboitFunc = 3;
				}
				if (ImGui::MenuItem("Equation 10", nullptr, m_renderOptions.wboitFunc == 4))
				{
					m_renderOptions.wboitFunc = 4;
				}
				if (ImGui::MenuItem("LOGL Eq. 9 color bias", nullptr, m_renderOptions.wboitFunc == 5))
				{
					m_renderOptions.wboitFunc = 5;
				}
				if (ImGui::MenuItem("LOGL Eq. 10", nullptr, m_renderOptions.wboitFunc == 6))
				{
					m_renderOptions.wboitFunc = 6;
				}
				if (ImGui::MenuItem("z^-3", nullptr, m_renderOptions.wboitFunc == 7))
				{
					m_renderOptions.wboitFunc = 7;
				}
				if (ImGui::MenuItem("abs(z - zFar + Eps)", nullptr, m_renderOptions.wboitFunc == 8))
				{
					m_renderOptions.wboitFunc = 8;
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		// TODO: (DR) Probably move to preferences or help/debug menu
		if (ImGui::MenuItem("Reload shaders", nullptr, nullptr))
		{
			bool ok = Vp::Shaders::instance().reload();
			// Flash green if all shaders reloaded successfully, red otherwise
			ImGui::GetForegroundDrawList()->AddRectFilled(GUI::glmToIm(m_windowPos), GUI::glmToIm(m_windowMax),
			                                              ImColor((ok ? 0 : 255), (ok ? 255 : 0), 0));
		}

		ImGui::EndMenu();
	}

	// TODO: (DR) To follow the "unified api methodology", the UI here should only update the viewport settings and not
	// actually perform the changes.
	//  Although this is more "efficient" in a way. But cumbersome I suppose.
	if (ImGui::BeginMenu("View"))
	{
		userInteractedWithMenus = true;
		if (ImGui::MenuItem("Orbit camera", nullptr, stg.scene().mainScene.camera.mode == CameraMode::ORBIT))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->switchMode(CameraMode::ORBIT);
				stg.scene().mainScene.camera.mode = CameraMode::ORBIT;
			}
		}
		if (ImGui::MenuItem("Trackball camera", nullptr, stg.scene().mainScene.camera.mode == CameraMode::TRACKBALL))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->switchMode(CameraMode::TRACKBALL);
				stg.scene().mainScene.camera.mode = CameraMode::TRACKBALL;
			}
		}
		if (ImGui::MenuItem("Smooth scroll", nullptr, stg.global().camera.smoothScroll))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				stg.global().camera.smoothScroll = !stg.global().camera.smoothScroll;
				camera->getOrbitCamera()->setSmoothScroll(stg.global().camera.smoothScroll);
				camera->getTrackballCamera()->setSmoothScroll(stg.global().camera.smoothScroll);
			}
		}
		if (ImGui::SliderFloat("Camera fov", &stg.scene().mainScene.camera.fov, 1, 180, "%.1f"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->getOrbitCamera()->setFov(stg.scene().mainScene.camera.fov);
				camera->getTrackballCamera()->setFov(stg.scene().mainScene.camera.fov);
			}
		}

		ImVec2 gridButtonSize = ImVec2(ImGui::GetFontSize() * 2, 0.0f);
		ImGui::Checkbox("Show grid", &m_displayOptions.showGrid);
		ImGui::SameLine();
		GUI::ToggleButton("XZ", m_displayOptions.showGridLines, gridButtonSize);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(4.0f, 0.0f));
		ImGui::SameLine();
		GUI::ToggleButton("X", m_displayOptions.showGridXAxis, gridButtonSize);
		ImGui::SameLine();
		GUI::ToggleButton("Y", m_displayOptions.showGridYAxis, gridButtonSize);
		ImGui::SameLine();
		GUI::ToggleButton("Z", m_displayOptions.showGridZAxis, gridButtonSize);

		ImGui::Separator();

		if (ImGui::MenuItem("Viewpoint right", "Num3"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::RIGHT);
			}
		}
		if (ImGui::MenuItem("Viewpoint left", "Ctrl+Num3"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::LEFT);
			}
		}
		if (ImGui::MenuItem("Viewpoint top", "Num7"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::TOP);
			}
		}
		if (ImGui::MenuItem("Viewpoint bottom", "Ctrl+Num7"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::BOTTOM);
			}
		}
		if (ImGui::MenuItem("Viewpoint front", "Num1"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::FRONT);
			}
		}
		if (ImGui::MenuItem("Viewpoint back", "Ctrl+Num1"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->viewpoint(Vp::AbstractCamera::Viewpoint::BACK);
			}
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Center camera on selection", "Num0"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->centerOnSelection(*m_viewport->getMainScene().lock().get());
			}
		}

		if (ImGui::MenuItem("Center camera on scene", "Home"))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->centerOnScene(*m_viewport->getMainScene().lock().get());
			}
		}

		ImGui::Separator();

		ImGui::MenuItem("Show objects", nullptr, &m_displayOptions.showDefault);
		ImGui::MenuItem("Show axes", nullptr, &m_displayOptions.showAxes);
		ImGui::MenuItem("Show grid", nullptr, &m_displayOptions.showGrid);
		ImGui::MenuItem("Show cameras", nullptr, &m_displayOptions.showCamera);
		ImGui::MenuItem("Show frustums", nullptr, &m_displayOptions.showFrustum);

		ImGui::EndMenu();
	}
	ImGui::PopItemFlag();

	return userInteractedWithMenus;

	//	World* w = App::get().world();
	//	if (ImGui::BeginMenu("Viewports"))
	//	{
	//		// Ptr<UI::Viewport> ww = I3T::getWindowPtr<UI::Viewport>();
	//		if (ImGui::MenuItem("View-x"))
	//		{
	//			// Num 1
	//			w->getCameraControl()->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), false);
	//		}
	//
	//		if (ImGui::MenuItem("View-y"))
	//		{
	//			// Num 2
	//			w->getCameraControl()->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), true);
	//		}
	//
	//		if (ImGui::MenuItem("View-z"))
	//		{
	//			// Num 3
	//			w->getCameraControl()->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), false);
	//		}
	//
	//		if (ImGui::MenuItem("World-x"))
	//		{
	//			// Num 4
	//			w->getCameraControl()->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), true);
	//		}
	//
	//		if (ImGui::MenuItem("World-y"))
	//		{
	//			// Num 5
	//			w->getCameraControl()->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), true);
	//		}
	//
	//		if (ImGui::MenuItem("World-z"))
	//		{
	//			// Num 6
	//			w->getCameraControl()->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), true);
	//		}
	//
	//		if (ImGui::MenuItem("Center"))
	//		{
	//			// Num 0
	//			// App::get().world()->scene->setCamToCenter();
	//		}
	//		ImGui::Separator();
	//		const char* action = w->getCameraControl()->m_rotateAroundCenter ? "Orbit eye" : "Orbit center";
	//		if (ImGui::MenuItem(action))
	//		{
	//			w->getCameraControl()->m_rotateAroundCenter = !w->getCameraControl()->m_rotateAroundCenter;
	//			// Num 0
	//			// App::get().world()->scene->setCamToCenter();
	//		}
	//		ImGui::EndMenu();
	//	}
	//	if (ImGui::BeginMenu("Manipulators"))
	//	{
	//		const char* action = w->manipulatorsGetVisible() ? "Hide" : "Show";
	//		if (ImGui::MenuItem(action))
	//		{
	//			w->manipulatorsSetVisible(!w->manipulatorsGetVisible());
	//		}
	//		ImGui::EndMenu();
	//	}
}
