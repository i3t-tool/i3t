#include "ViewportWindow.h"

#include "imgui.h"

#include "Core/API.h"
#include "Core/Application.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"

#include "../../../World/Select.h"

#include "../Nodes/WorkspaceElementsWithCoreData.h"

#include "GUI/Toolkit.h"
#include "GUI/WindowManager.h"
#include "Viewport/Viewport.h"
#include "Viewport/camera/AggregateCamera.h"
#include "Viewport/framebuffer/Framebuffer.h"
#include "World/Components.h"

using namespace UI;

using CameraMode = Vp::AggregateCamera::CameraMode;

ViewportWindow::ViewportWindow(bool show, Vp::Viewport* viewport) : IWindow(show)
{
	m_viewport = viewport;
	// TODO: (DR) Not sure if binding callbacks to an axis really makes sense
	//  Binding like this hides where the actual action is meant to occur, like here, binding zoom in the viewport window
	//  and its difficult to unbind stuff
	// TODO: (DR) In fact the whole axis/axes system is a little odd to me
	// Input.bindAxis("scroll", [this](float val) { m_world->sceneZoom(val); });

	renderOptions.wboit = false;
	renderOptions.framebufferAlpha = false;
	renderOptions.multisample = true;
	renderOptions.clearColor = Config::BACKGROUND_COLOR;
	renderOptions.selection = true;

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_MULTISAMPLE);
	// glCullFace(GL_BACK); //TODO: (DR) Do we need culling? Maybe add a toggle? (Handled by
	// glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/// \todo MH This is example code, it can be removed anytime.
	InputManager::setInputAction("fire", Keys::b);
	InputManager::setInputAction("fire", Keys::m);
	InputManager::setInputAxis("move", 1.0f, Keys::o);
	InputManager::setInputAxis("move", -1.0f, Keys::p);

	Input.bindAction("fire", EKeyState::Pressed, []() { LOG_INFO("Action fired."); });
	Input.bindAction("fire", EKeyState::Released, []() { LOG_INFO("Action released."); });
	Input.bindAxis("move", [](float val) { LOG_INFO("move: {}", val); });
	/// todoend
}

void ViewportWindow::render()
{
	// ImVec2 main_viewport_pos = ImGui::GetMainViewport()->Pos;
	// ImGui::SetNextWindowPos(ImVec2(main_viewport_pos.x + 650,
	// main_viewport_pos.y + 20), ImGuiCond_FirstUseEver);
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

	bool userInteractedWithMenus = false;
	if (ImGui::BeginMenuBar())
	{
		userInteractedWithMenus |= showViewportMenu();
		ImGui::EndMenuBar();
	}

	// TODO: (DR) This is somewhat unclear, might need a comment, we're checking if this window is focused, but through
	//  the InputManager's active input rather than asking the WindowManager
	if (InputManager::isInputActive(getInputPtr()) && !userInteractedWithMenus)
	{
		glm::vec2 relativeMousePos = WindowManager::getMousePositionForWindow(this);
		m_viewport->processInput(ImGui::GetIO().DeltaTime, relativeMousePos, m_windowSize);
		m_viewport->processSelection(relativeMousePos, m_windowSize);
	}
	Ptr<Vp::Framebuffer> framebuffer =
	    m_viewport->drawViewport(windowWidth, windowHeight, renderOptions, displayOptions).lock();

	if (framebuffer)
	{
		GLuint texture = framebuffer->getColorTexture();
		// the uv coordinates flips the picture, since it was upside down at first
		ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)texture, windowMin, windowMax, ImVec2(0, 1), ImVec2(1, 0));
	}
	else
	{
		ImGui::Text("Failed to draw viewport!");
	}

	ImGui::End();
}

bool ViewportWindow::showViewportMenu()
{
	// TODO: (DR) Sometimes closing of nested menus causes a debug assertion fail, its a fixed bug as per:
	//  https://github.com/ocornut/imgui/issues/4640
	//	Can only be fixed by updating imgui

	ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

	bool userInteractedWithMenus = false;
	if (ImGui::BeginMenu("Settings"))
	{
		userInteractedWithMenus = true;
		ImGui::MenuItem("WBOIT", nullptr, &renderOptions.wboit);

		bool msaaOff = !renderOptions.multisample;
		bool msaa2x = renderOptions.multisample && renderOptions.samples == 2;
		bool msaa4x = renderOptions.multisample && renderOptions.samples == 4;
		bool msaa8x = renderOptions.multisample && renderOptions.samples == 8;
		if (ImGui::BeginMenu("MSAA"))
		{
			if (ImGui::MenuItem("OFF", nullptr, &msaaOff))
			{
				renderOptions.multisample = false;
			}
			if (ImGui::MenuItem("2x", nullptr, &msaa2x))
			{
				renderOptions.multisample = true;
				renderOptions.samples = 2;
			}
			if (ImGui::MenuItem("4x", nullptr, &msaa4x))
			{
				renderOptions.multisample = true;
				renderOptions.samples = 4;
			}
			if (ImGui::MenuItem("8x", nullptr, &msaa8x))
			{
				renderOptions.multisample = true;
				renderOptions.samples = 8;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Highlight"))
		{
			if (ImGui::BeginMenu("Preset"))
			{
				if (ImGui::MenuItem("Ultra", nullptr, nullptr))
				{
					m_viewport->getSettings().highlight_downscaleFactor = 1.0f;
					m_viewport->getSettings().highlight_kernelSize = 4;
					m_viewport->getSettings().highlight_outlineCutoff = 0.15f;
					m_viewport->getSettings().highlight_useDepth = true;
				}
				if (ImGui::MenuItem("High", nullptr, nullptr))
				{
					m_viewport->getSettings().highlight_downscaleFactor = 0.8f;
					m_viewport->getSettings().highlight_kernelSize = 4;
					m_viewport->getSettings().highlight_outlineCutoff = 0.18f;
					m_viewport->getSettings().highlight_useDepth = true;
				}
				if (ImGui::MenuItem("Medium", nullptr, nullptr))
				{
					m_viewport->getSettings().highlight_downscaleFactor = 0.5f;
					m_viewport->getSettings().highlight_kernelSize = 2;
					m_viewport->getSettings().highlight_outlineCutoff = 0.23f;
					m_viewport->getSettings().highlight_useDepth = true;
				}
				if (ImGui::MenuItem("Low", nullptr, nullptr))
				{
					m_viewport->getSettings().highlight_downscaleFactor = 1.0f / 3;
					m_viewport->getSettings().highlight_kernelSize = 2;
					m_viewport->getSettings().highlight_outlineCutoff = 0.3f;
					m_viewport->getSettings().highlight_useDepth = true;
				}
				if (ImGui::MenuItem("Lowest", nullptr, nullptr))
				{
					m_viewport->getSettings().highlight_downscaleFactor = 0.25;
					m_viewport->getSettings().highlight_kernelSize = 2;
					m_viewport->getSettings().highlight_outlineCutoff = 0.5f;
					m_viewport->getSettings().highlight_useDepth = false;
				}
				ImGui::EndMenu();
			}

			// ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
			ImGui::SliderFloat("Downscale factor", &m_viewport->getSettings().highlight_downscaleFactor, 0.01f, 1.0f, "%.2f");
			ImGui::SliderInt("Kernel size", &m_viewport->getSettings().highlight_kernelSize, 1, 10);
			ImGui::SliderFloat("Blur cutoff", &m_viewport->getSettings().highlight_outlineCutoff, 0.01f, 1.0f, "%.2f");

			ImGui::Separator();
			ImGui::Checkbox("Use depth", &m_viewport->getSettings().highlight_useDepth);
			ImGui::SliderFloat("Darken factor", &m_viewport->getSettings().highlight_useDepth_darkenFactor, 0.0f, 1.0f,
			                   "%.2f");
			ImGui::SliderFloat("Desaturate factor", &m_viewport->getSettings().highlight_useDepth_desaturateFactor, 0.0f,
			                   1.0f, "%.2f");
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	// TODO: (DR) To follow the "unified api methodology", the UI here should only update the viewport settings and not
	// actually perform the changes.
	//  Although this is more "efficient" in a way. But cumbersome I suppose.
	if (ImGui::BeginMenu("View"))
	{
		userInteractedWithMenus = true;
		if (ImGui::MenuItem("Orbit camera", nullptr, m_viewport->getSettings().mainScene_cameraMode == CameraMode::ORBIT))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->switchMode(CameraMode::ORBIT);
				m_viewport->getSettings().mainScene_cameraMode = CameraMode::ORBIT;
			}
		}
		if (ImGui::MenuItem("Trackball camera", nullptr,
		                    m_viewport->getSettings().mainScene_cameraMode == CameraMode::TRACKBALL))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				camera->switchMode(CameraMode::TRACKBALL);
				m_viewport->getSettings().mainScene_cameraMode = CameraMode::TRACKBALL;
			}
		}
		if (ImGui::MenuItem("Smooth scroll", nullptr, m_viewport->getSettings().camera_smoothScroll))
		{
			if (auto camera = m_viewport->getMainViewportCamera().lock())
			{
				m_viewport->getSettings().camera_smoothScroll = !m_viewport->getSettings().camera_smoothScroll;
				camera->getOrbitCamera()->setSmoothScroll(m_viewport->getSettings().camera_smoothScroll);
				camera->getTrackballCamera()->setSmoothScroll(m_viewport->getSettings().camera_smoothScroll);
			}
		}

		ImGui::Separator();
		ImGui::MenuItem("Show objects", nullptr, &displayOptions.showDefault);
		ImGui::MenuItem("Show axes", nullptr, &displayOptions.showAxes);
		ImGui::MenuItem("Show grid", nullptr, &displayOptions.showGrid);
		ImGui::MenuItem("Show cameras", nullptr, &displayOptions.showCamera);
		ImGui::MenuItem("Show frustums", nullptr, &displayOptions.showFrustum);
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
