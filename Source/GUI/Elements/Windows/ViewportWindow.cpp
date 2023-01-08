#include "ViewportWindow.h"

#include "imgui.h"

#include "Core/API.h"
#include "Core/Application.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"

#include "../../../World/Select.h"
#include "../../../World/World.h"

#include "../Nodes/WorkspaceElementsWithCoreData.h"

#include "GUI/WindowManager.h"
#include "Viewport/Viewport.h"
#include "World/Components.h"
#include "World/HardcodedMeshes.h"
#include "World/RenderTexture.h"
#include "World/World.h"

using namespace UI;

ViewportWindow::ViewportWindow(bool show, World* world2, Vp::Viewport* viewport) : IWindow(show)
{
	m_world = world2;
	m_viewport = viewport;
	Input.bindAxis("scroll", [this](float val) { m_world->sceneZoom(val); });

	m_framebuffer = std::make_unique<Vp::Framebuffer>(100, 100, true);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_MULTISAMPLE);
	// glCullFace(GL_BACK); //TODO: (DR) Do we need culling? Maybe add a toggle?
	// glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// init vectors definig size to display
	m_wcMin = ImVec2(0, 0);
	m_wcMax = ImVec2(0, 0);

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

float localData;

void ViewportWindow::render()
{
	// ImVec2 main_viewport_pos = ImGui::GetMainViewport()->Pos;
	// ImGui::SetNextWindowPos(ImVec2(main_viewport_pos.x + 650,
	// main_viewport_pos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		auto name = setName("Scene View");

		ImGui::PushStyleColor(ImGuiCol_TabActive, App::get().getUI()->getTheme().get(EColor::DockTabActive));

		ImGui::Begin(name.c_str(), getShowPtr(), g_WindowFlags); // | ImGuiWindowFlags_MenuBar);

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		// if (ImGui::BeginMenuBar())
		//{
		//	showViewportsMenu();

		//	ImGui::EndMenuBar();
		//}

		// TODO: (DR) what is this doing?
		InputManager::processViewportEvents();

		// get positions of min max points of the window
		ImVec2 newWcMin = ImGui::GetWindowContentRegionMin();
		ImVec2 newWcMax = ImGui::GetWindowContentRegionMax();
		// change them to actual screen positions
		newWcMin.x += ImGui::GetWindowPos().x;
		newWcMin.y += ImGui::GetWindowPos().y;
		newWcMax.x += ImGui::GetWindowPos().x;
		newWcMax.y += ImGui::GetWindowPos().y;

		m_wcMin = newWcMin;
		m_wcMax = newWcMax;

		int width = static_cast<int>(abs(newWcMax.x - newWcMin.x));
		int height = static_cast<int>(abs(newWcMax.y - newWcMin.y));

		// ImGui::GetWindowDrawList()->AddCallback(render_callback, NULL); // Option
		// 1 (did not manage to get it working correctly - too hard to grasp all the
		// stuff for it)

		m_framebuffer->start(width, height);
		glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (InputManager::isFocused<UI::ViewportWindow>())
		{
			m_viewport->processInput();
		}
		m_viewport->draw(width, height);

		// ImGui::GetForegroundDrawList()->AddRect(m_wcMin, m_wcMax, IM_COL32(255,
		// 255, 0, 255)); // test

		m_framebuffer->end();

		CHECK_GL_ERROR();

		ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)m_framebuffer->getColorTexture(), m_wcMin, m_wcMax,
		                                     ImVec2(0, 1),
		                                     ImVec2(1, 0)); // the uv coordinates flips the picture, since it was
		// upside down at first

		ImGui::End();
	}
}

void ViewportWindow::showViewportsMenu()
{
	World* w = App::get().world();
	if (ImGui::BeginMenu("Viewports"))
	{
		// Ptr<UI::Viewport> ww = I3T::getWindowPtr<UI::Viewport>();
		if (ImGui::MenuItem("View-x"))
		{
			// Num 1
			w->getCameraControl()->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), false);
		}

		if (ImGui::MenuItem("View-y"))
		{
			// Num 2
			w->getCameraControl()->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), true);
		}

		if (ImGui::MenuItem("View-z"))
		{
			// Num 3
			w->getCameraControl()->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), false);
		}

		if (ImGui::MenuItem("World-x"))
		{
			// Num 4
			w->getCameraControl()->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), true);
		}

		if (ImGui::MenuItem("World-y"))
		{
			// Num 5
			w->getCameraControl()->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), true);
		}

		if (ImGui::MenuItem("World-z"))
		{
			// Num 6
			w->getCameraControl()->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), true);
		}

		if (ImGui::MenuItem("Center"))
		{
			// Num 0
			// App::get().world()->scene->setCamToCenter();
		}
		ImGui::Separator();
		const char* action = w->getCameraControl()->m_rotateAroundCenter ? "Orbit eye" : "Orbit center";
		if (ImGui::MenuItem(action))
		{
			w->getCameraControl()->m_rotateAroundCenter = !w->getCameraControl()->m_rotateAroundCenter;
			// Num 0
			// App::get().world()->scene->setCamToCenter();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Manipulators"))
	{
		const char* action = w->manipulatorsGetVisible() ? "Hide" : "Show";
		if (ImGui::MenuItem(action))
		{
			w->manipulatorsSetVisible(!w->manipulatorsGetVisible());
		}
		ImGui::EndMenu();
	}
}
