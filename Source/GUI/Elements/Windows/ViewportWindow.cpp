#include "ViewportWindow.h"

#include "imgui.h"

#include "Core/API.h"
#include "Core/Application.h"
#include "Core/InputController.h"
#include "GUI/Settings.h"
#include "Rendering/FrameBuffer.h"

/// \todo Use Framebuffer class.
ViewportWindow::ViewportWindow(bool show, World* world) : IWindow(show)
{
	m_world = world;

	// Framebuffer is used in Viewport window.
	// generate a framebuffer for display function
	glGenFramebuffers(1, &m_fboMain);

	// set it as the default framebuffer in framebuffer.h
	FrameBuffer::setDefaultBuffer(m_fboMain);

	// generate texture to draw on
	glGenTextures(1, &m_texColBufMain);

	// create a renderbuffer to allow depth and stencil
	glGenRenderbuffers(1, &m_rboMain);

	// init vectors definig size to display
	m_wcMin = ImVec2(0, 0);
	m_wcMax = ImVec2(0, 0);
}

void ViewportWindow::render()
{
	// ImVec2 main_viewport_pos = ImGui::GetMainViewport()->Pos;
	// ImGui::SetNextWindowPos(ImVec2(main_viewport_pos.x + 650, main_viewport_pos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin(
				"Viewport",
				getShowPtr()); // Create a window called "Hello, world!" and append into it.
		ImGui::PopStyleVar();

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
		InputController::processViewportEvents();

		// bind our special framebuffer for rendering (and binding a new texture for it if needed)
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboMain);

		// IF NEW WINDOW SIZE IS DIFFERENT, CHANGE SIZES ACCORDINGLY
		if (newWcMin.x != m_wcMin.x || newWcMin.y != m_wcMin.y || newWcMax.x != m_wcMax.x || newWcMax.y != m_wcMax.y)
		{
			m_wcMin = newWcMin;
			m_wcMax = newWcMax;

			int width = static_cast<int>(abs(m_wcMax.x - m_wcMin.x));
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
			m_world->onReshape(width, height);
			Settings::resize((float)width, (float)height);
			Config::WIN_HEIGHT = height;
			Config::WIN_WIDTH = width;

			// set viewport size to be sure
			glViewport(0, 0, width, height);
		}

		// clear
		glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 1.0f);
		// glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_MULTISAMPLE);

		// draw
		m_world->render();

		glDisable(GL_MULTISAMPLE);

		// Unbind our framebuffer, bind main framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ImGui::GetForegroundDrawList()->AddRect(wcMin, wcMax, IM_COL32(255, 255, 0, 255)); // test

		// add the texture to this's window drawList
		ImGui::GetWindowDrawList()->AddImage(
				(void*)(intptr_t)m_texColBufMain, m_wcMin, m_wcMax, ImVec2(0, 1),
				ImVec2(1, 0)); // the uv coordinates flips the picture, since it was upside down at first

		ImGui::End();
	}
}
