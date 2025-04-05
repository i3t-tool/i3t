#include <stdio.h>

#include <functional>
#include <iostream>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"

#include "pgr.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include "DIWNE/Core/NodeEditor.h"

namespace DIWNE
{
class DiwneTestApp
{
public:
	GLFWwindow* window{nullptr};
	DIWNE::NodeEditor* editor{nullptr};
	std::function<void()> testCallback;

	void setTestCallback(std::function<void()> callback)
	{
		testCallback = std::move(callback);
	}

	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "GLFW Error %d: %s\n", error, description);
	}

	void frame()
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(1200, 700), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(20, 10), ImGuiCond_Once);
		ImGui::Begin("DIWNE example", NULL, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
				ImGui::MenuItem("Enable", nullptr, &editor->m_diwneDebug);
				ImGui::MenuItem("Layout", nullptr, &editor->m_diwneDebugLayout);
				ImGui::MenuItem("Interaction", nullptr, &editor->m_diwneDebugInteractions);
				ImGui::MenuItem("Objects", nullptr, &editor->m_diwneDebugObjects);
				ImGui::PopItemFlag();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		editor->draw();

		ImGui::End();

		ImGui::Begin("TestWindow");
		if (testCallback)
			testCallback();
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
		             clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			frame();
		}
	}

	int setupWindow()
	{
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			return 1;

		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

		// Create window with graphics context
		window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
		if (window == nullptr)
			return 1;
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void) io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		return 0;
	}

	void cleanup()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	int create(NodeEditor* editor)
	{
		this->editor = editor;
		int ret = setupWindow();
		if (ret)
			return ret;

		if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR, pgr::DEBUG_OFF))
		{
			return 2;
		}

		ImGui::GetIO().IniFilename = nullptr;

		// Load Fonts
		// TODO: Load an enlarged node editor font
		return 0;
	}

	void destroy()
	{
		cleanup();
	}
};
} // namespace DIWNE