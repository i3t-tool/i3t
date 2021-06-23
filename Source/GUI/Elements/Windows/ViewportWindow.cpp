#include "ViewportWindow.h"

#include "imgui.h"

#include "Core/API.h"
#include "Core/Application.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"

#include "../../../World/Select.h"
#include "../../../World/World.h"

#include "../Nodes/WorkspaceNodeWithCoreData.h"

#include "World/Components.h"
#include "World/HardcodedMeshes.h"
#include "World/RenderTexture.h"
#include "World/World.h"

using namespace UI;

Viewport::Viewport(bool show, World* world2) : IWindow(show)
{
	m_world = world2;
	Input.bindAxis("scroll", [this](float val) { m_world->sceneZoom(val); });

	/// \todo Use Framebuffer class.
	// Framebuffer is used in Viewport window.
	// generate a framebuffer for display function
	glGenFramebuffers(1, &m_fboMain);

	// generate texture to draw on
	glGenTextures(1, &m_texColBufMain);

	// create a renderbuffer to allow depth and m_stencil
	glGenRenderbuffers(1, &m_rboMain);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
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

	Input.bindAction("fire", EKeyState::Pressed, []() { Log::info("Action fired."); });
	Input.bindAction("fire", EKeyState::Released, []() { Log::info("Action released."); });
	Input.bindAxis("move", [](float val) { Log::info("move: {}", val); });
	/// todoend
}

float localData;

void Viewport::render()
{
	// ImVec2 main_viewport_pos = ImGui::GetMainViewport()->Pos;
	// ImGui::SetNextWindowPos(ImVec2(main_viewport_pos.x + 650, main_viewport_pos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		auto name = getName("Scene Viewport");

		ImGui::Begin(name.c_str(), getShowPtr(), g_WindowFlags | ImGuiWindowFlags_MenuBar);
		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar())
		{
			showViewportsMenu();

			ImGui::EndMenuBar();
		}

		// get positions of min max points of the window
		ImVec2 newWcMin = ImGui::GetWindowContentRegionMin();
		ImVec2 newWcMax = ImGui::GetWindowContentRegionMax();
		// change them to actual screen positions
		newWcMin.x += ImGui::GetWindowPos().x;
		newWcMin.y += ImGui::GetWindowPos().y;
		newWcMax.x += ImGui::GetWindowPos().x;
		newWcMax.y += ImGui::GetWindowPos().y;

		// ImGui::GetWindowDrawList()->AddCallback(render_callback, NULL); // Option 1 (did not manage to get it working
		// correctly - too hard to grasp all the stuff for it)
		InputManager::processViewportEvents();

		// bind our special framebuffer for rendering (and binding a new texture for it if needed)
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboMain);

		// IF NEW WINDOW SIZE IS DIFFERENT, CHANGE SIZES ACCORDINGLY
		if (newWcMin.x != m_wcMin.x || newWcMin.y != m_wcMin.y || newWcMax.x != m_wcMax.x || newWcMax.y != m_wcMax.y)
		{
			m_wcMin = newWcMin;
			m_wcMax = newWcMax;

			int width	 = static_cast<int>(abs(m_wcMax.x - m_wcMin.x));
			int height = static_cast<int>(abs(m_wcMax.y - m_wcMin.y));

			// create new image in our texture
			glBindTexture(GL_TEXTURE_2D, m_texColBufMain);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);
			// attach it to currently bound framebuffer object
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColBufMain, 0);

			// resize renderbuffer
			glBindRenderbuffer(GL_RENDERBUFFER, m_rboMain);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width + 16, height + 16);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			// attach it to currently bound framebuffer object
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboMain);

			// resize all other things
			InputManager::setScreenSize((int) width, (int) height);
			Config::WIN_HEIGHT = height;
			Config::WIN_WIDTH	 = width;

			// set viewport size to be sure
			glViewport(0, 0, width, height);

			/*GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
			switch(status){
			case GL_FRAMEBUFFER_COMPLETE:printf("complete\n");break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:printf("incomplete\n");break;
			case GL_FRAMEBUFFER_UNDEFINED:printf("undefined\n");break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:printf("incomplete_missing\n");break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:printf("incomplete_draw\n");break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:printf("incomplete_read\n");break;
			case GL_FRAMEBUFFER_UNSUPPORTED:printf("unsupported\n");break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:printf("incomplete_multisample\n");break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:printf("incomplete_layer_targets\n");break;
			default:printf("other %x\n",status);break;
			}*/
		}

		//set clear color
		glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 1.0f);

		// draw
		m_world->onUpdate();

		// Unbind our framebuffer, bind main framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//ImGui::GetForegroundDrawList()->AddRect(m_wcMin, m_wcMax, IM_COL32(255, 255, 0, 255)); // test

		// add the texture to this's window drawList
		ImGui::GetWindowDrawList()->AddImage(
				(void*) (intptr_t) m_texColBufMain, m_wcMin, m_wcMax, ImVec2(0, 1),
				ImVec2(1, 0)); // the uv coordinates flips the picture, since it was upside down at first

		m_world->onGUI();
		ImGui::End();
	}
}

void Viewport::showViewportsMenu()
{
	World* w = App::get().world();
	if (ImGui::BeginMenu("Viewports"))
	{
		//Ptr<UI::Viewport> ww = I3T::getWindowPtr<UI::Viewport>();
		if (ImGui::MenuItem("View-x"))
		{
			// Num 1
			w->sceneSetView(glm::vec3(1.0f, 0.0f, 0.0f), false);
		}

		if (ImGui::MenuItem("View-y"))
		{
			// Num 2
			w->sceneSetView(glm::vec3(0.0f, 1.0f, 0.0f), false);
		}

		if (ImGui::MenuItem("View-z"))
		{
			// Num 3
			w->sceneSetView(glm::vec3(0.0f, 0.0f, 1.0f), false);
		}

		if (ImGui::MenuItem("World-x"))
		{
			// Num 4
			w->sceneSetView(glm::vec3(1.0f, 0.0f, 0.0f), true);
		}

		if (ImGui::MenuItem("World-y"))
		{
			// Num 5
			w->sceneSetView(glm::vec3(0.0f, 1.0f, 0.0f), true);
		}

		if (ImGui::MenuItem("World-z"))
		{
			// Num 6
			w->sceneSetView(glm::vec3(0.0f, 0.0f, 1.0f), true);
		}

		if (ImGui::MenuItem("Center"))
		{
			// Num 0
			// App::get().world()->scene->setCamToCenter();
		}

		ImGui::EndMenu();
	}
	else if (ImGui::BeginMenu("Manipulators")) {
		const char*action=w->manipulatorsGetVisible()?"Hide":"Show";
		if (ImGui::MenuItem(action)) {
			w->manipulatorsSetVisible(!w->manipulatorsGetVisible());
		}
		ImGui::EndMenu();
	}
}
