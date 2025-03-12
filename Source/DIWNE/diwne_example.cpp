// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context
// creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "Core/diwne_imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include "Basic/BasicNode.h"
#include "Basic/SequenceNodeContainer.h"
#include "Core/NodeEditor.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

DIWNE::SettingsDiwne settings;
std::shared_ptr<DIWNE::NodeEditor> editor;

void diwneInit()
{
	editor = std::make_shared<DIWNE::NodeEditor>(&settings);
	editor->addNode(std::make_shared<DIWNE::BasicNode>(*editor, "Node 1"), ImVec2(20, 20));
	editor->addNode(std::make_shared<DIWNE::BasicNode>(*editor, "Node 2"), ImVec2(150, 20));
	editor->addNode(std::make_shared<DIWNE::BasicNode>(*editor, "Node 3"), ImVec2(300, 20));
	auto sequence1 = std::make_shared<DIWNE::SequenceNodeContainer>(*editor, "Sequence 1");
	editor->addNode(sequence1, ImVec2(20, 100));
	auto sequence2 = std::make_shared<DIWNE::SequenceNodeContainer>(*editor, "Sequence 2");
	editor->addNode(sequence2, ImVec2(200, 100));
	editor->addNode(std::make_shared<DIWNE::SequenceNodeContainer>(*editor, "Sequence 3"), ImVec2(200, 300));
}

void buttonHoverTest()
{
	ImGui::Dummy(ImVec2(50, 50));
	std::cout << ImGui::IsItemHovered() << ",";
	ImGui::Button("test", ImVec2(50, 50));
	std::cout << ImGui::IsItemHovered() << ",";
	ImGui::BeginDisabled(true);
	ImGui::InvisibleButton("test2", ImVec2(50, 50));
	ImGui::EndDisabled();
	std::cout << ImGui::IsItemHovered() << ",";
	ImGui::SetCursorPos(ImVec2(0, 0));
	ImGui::Button("blocker", ImVec2(100, 200));
	std::cout << ImGui::IsItemHovered();
	std::cout << std::endl << std::endl;
}

void diwneWindow()
{
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

	//	buttonHoverTest();
	editor->draw();

	ImGui::End();
}

void mainLoop(GLFWwindow* window)
{
	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	diwneInit();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		diwneWindow();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
		             clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

int setupWindow(GLFWwindow*& window)
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

void cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(int, char**)
{
	GLFWwindow* window = nullptr;
	int ret = setupWindow(window);
	if (ret)
		return ret;

	// Load Fonts
	// TODO: Load an enlarged node editor font

	mainLoop(window);

	cleanup(window);
	return 0;
}