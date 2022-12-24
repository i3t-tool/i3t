#include "ViewportWindow.h"

#include "imgui.h"

#include "Core/API.h"
#include "Core/Application.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"

#include "../../../World/Select.h"

#include "../Nodes/WorkspaceElementsWithCoreData.h"

#include "GUI/WindowManager.h"
#include "Viewport/Viewport.h"
#include "Viewport/framebuffer/Framebuffer.h"
#include "World/Components.h"
#include "World/World.h"

using namespace UI;

ViewportWindow::ViewportWindow(bool show, World* world2) : IWindow(show)
{
	m_world = world2;
	Input.bindAxis("scroll", [this](float val) { m_world->sceneZoom(val); });

	renderOptions.wboit = false;
	renderOptions.framebufferAlpha = false;
	renderOptions.multisample = true;
	renderOptions.clearColor = Config::BACKGROUND_COLOR;

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_MULTISAMPLE);
	// glCullFace(GL_BACK); //TODO: (DR) Do we need culling? Maybe add a toggle? (Handled by
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
		ImGui::PushStyleColor(ImGuiCol_TabActive, App::get().getUI()->getTheme().get(EColor::DockTabActive));
		auto name = setName("Scene View");
		ImGui::Begin(name.c_str(), getShowPtr(), g_WindowFlags | ImGuiWindowFlags_MenuBar); // | ImGuiWindowFlags_MenuBar);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar())
		{
			showViewportsMenu();

			ImGui::EndMenuBar();
		}

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

		if (InputManager::isFocused<UI::ViewportWindow>())
		{
			App::get().viewport()->processInput();
		}
		Ptr<Vp::Framebuffer> framebuffer =
		    App::get().viewport()->drawViewport(width, height, renderOptions, displayOptions).lock();

		// ImGui::GetForegroundDrawList()->AddRect(m_wcMin, m_wcMax, IM_COL32(255,
		// 255, 0, 255)); // test

		if (framebuffer)
		{

			// GLuint texture = framebuffer->getColorAttachment(0).m_texture;
			GLuint texture = framebuffer->getColorTexture();
			// the uv coordinates flips the picture, since it was upside down at first
			ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)texture, m_wcMin, m_wcMax, ImVec2(0, 1), ImVec2(1, 0));

			// WIP code for button popups
			//			if (ImGui::Button("With a menu.."))
			//				ImGui::OpenPopup("my_file_popup");
			//			if (ImGui::BeginPopup("my_file_popup", ImGuiWindowFlags_NoMove))
			//			{
			//				ImGui::Text("File yeah");
			//				bool e = true;
			//				ImGui::Checkbox("Poggers", &e);
			////				if (ImGui::BeginMenuBar())
			////				{
			////					if (ImGui::BeginMenu("File"))
			////					{
			////						ImGui::Text("File yeah");
			////						bool e = true;
			////						ImGui::Checkbox("Box", &e);
			////						ImGui::EndMenu();
			////					}
			////					if (ImGui::BeginMenu("Edit"))
			////					{
			////						ImGui::MenuItem("Dummy");
			////						ImGui::EndMenu();
			////					}
			////					ImGui::EndMenuBar();
			////				}
			//				ImGui::Text("Hello from popup!");
			//				ImGui::Button("This is a dummy button..");
			//				ImGui::EndPopup();
			//			}
		}
		else
		{
			ImGui::Text("Failed to draw viewport!");
		}

		ImGui::End();
	}
}

void ViewportWindow::showViewportsMenu()
{
	if (ImGui::BeginMenu("Settings"))
	{
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

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		ImGui::MenuItem("Show objects", nullptr, &displayOptions.showDefault);
		ImGui::MenuItem("Show axes", nullptr, &displayOptions.showAxes);
		ImGui::MenuItem("Show grid", nullptr, &displayOptions.showGrid);
		ImGui::MenuItem("Show cameras", nullptr, &displayOptions.showCamera);
		ImGui::MenuItem("Show frustums", nullptr, &displayOptions.showFrustum);
		ImGui::EndMenu();
	}

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
