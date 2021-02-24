#include "Application.h"

#include <functional>

#include "imgui.h"

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Modals/BeforeCloseModal.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"
#include "GUI/ImGui/imgui_impl_glfw.h"
#include "GUI/ImGui/imgui_impl_opengl3.h"
#include "GUI/Style.h"
#include "Rendering/ShaderProvider.h"
#include "Rendering/Shaper.h"
#include "Utils/Color.h"
#include "Utils/TextureLoader.h"

/// \todo Set dynamic scale (reload font in runtime).
float fontScale = 1.2f;

Application::Application()
{
	m_isPaused = false;
	m_world = nullptr;

	/// \todo How to get which window was open previously?
	m_showWorkspaceWindow = true;
	m_showTutorialWindow = true;
	m_showConsoleWindow = false;
	m_showZoomWindow = false;
	m_showViewportWindow = true;
}

void Application::initWindow()
{
	m_window = new GlfwWindow();
	m_window->init();

	/// \todo Show console -> ShowWindow() function is from Win32 API.
	// showConsole(Config::SHOW_CONSOLE);

	/// \todo GLFW does not support timer directly, timer callback behaviour must be implemented
	/// by yourself.
	// glutTimerFunc(Config::REFRESHTIME, onTimer, 0);
}

void Application::initImGui()
{
	// Create GUI Elements.
	m_menu = new MainMenuBar();
	m_dockableWindows.push_back(new TutorialWindow(false));
	m_dockableWindows.push_back(new ViewportWindow(true, m_world));
	m_dockableWindows.push_back(new WorkspaceWindow(true));

	BeforeCloseCommand::addListener(std::bind(&App::onBeforeClose, this));
	CloseCommand::addListener([this] { onClose(); });

	HideWindowCommand::addListener([this](const std::string& id) { App::get().popWindow(id); });

	// Setup Dear ImGui context after OpenGL context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
	// ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f; // disable alpha
	}

	// Allocate path to the imgui ini file on heap.
	auto* path = new std::string(Config::getAbsolutePath("/Data/imgui.ini"));
	io.IniFilename = path->c_str();

	/// \todo Better fonts loading.
	m_fonts = {
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Roboto-Regular.ttf").c_str(),
	                                 14.0f * fontScale),
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Roboto-Bold.ttf").c_str(), 12.0f * fontScale),
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Roboto-Regular.ttf").c_str(),
	                                 12.0f * fontScale),
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Ubuntu-Bold.ttf").c_str(), 24.0f * fontScale),
			io.Fonts->AddFontFromFileTTF(Config::getAbsolutePath("/Data/fonts/Roboto-Bold.ttf").c_str(), 14.0f * fontScale),
	};

	io.FontDefault = m_fonts[FONT_MENU_LARGE];

	setupImGuiStyle();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window->get(), true);
	ImGui_ImplOpenGL3_Init(ImGui_GLSLVersion);
}

void Application::run()
{
	while (!m_bShouldClose)
	{
		// Process commands.
		for (auto& command : m_commands)
		{
			command->execute();
			delete command;
		}
		m_commands.clear();

		// Update and display.
		onDisplay();

		// Poll events.
		glfwWaitEvents();
	}
}

void Application::onDisplay()
{
	/*
	 \todo Pause scene update.
	if (m_isPaused)
	  return; // True to stop display update. Set by main.setPause(), not used now.
	 */

	/// \todo Window title change on scene or workspace change.
	std::stringstream ss;
	// ss << Config::WIN_TITLE;
	// PF XX for video capture
	ss << Config::WIN_TITLE;

	// glfwSetWindowTitle(m_window->get(), ss.str().c_str());
	m_window->setTitle(ss.str().c_str());

	logicUpdate();
	/// \todo after pressing quit, it still updates the logic and fails on nonexistent camera in Scene::keyUpdate()
	/// \todo move the logic Update to the timer
	// TIME_STEP_ACU -= TIME_STEP;

	// Start the Dear ImGui frame ----------------------
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// create dockspace -----------------------------
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		                ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	// Prepare GUI elements for rendering.
	m_menu->render();

	// Scene is rendered in the Viewport window.
	// TODO -> Do not render scene int the ViewportWindow class.
	for (auto* element : m_dockableWindows)
		element->render();

	// Render other windows.
	for (const auto& [id, w] : m_windows)
		w->render();

	// ImGui rendering ----------------------------------------------------------
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this
	// code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	// ImGuiIO& io = ImGui::GetIO(); (void)io;
	io = ImGui::GetIO();
	(void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	// glfwSwapBuffers(m_window);
	m_window->swapBuffers();
}

void Application::logicUpdate()
{
	InputController::preUpdate(); ///< update the mouse button state

	if (m_world)
		m_world->update(); ///< update World = handle 2D and 3D interaction
	// PF glutExit called during this update - crash after window close. Solved in TabSpace::closeMessageExitEvent

	InputController::update(); ///< Update mouseDelta, mousePrev, and the stored statuses of the keys in the \a keyMap
	                           ///< array (JUST_UP -> UP, JUST_DOWN -> DOWN).
}

void Application::finalize()
{
	delete ObjectDefinitionTree::tree;

	delete m_world;
	// world = nullptr; //PF problem during glutExit...

	/// \todo Write recent files.
	// RecentFiles::writeRecent();

	ShaderProvider::dispose(); // delete red, base and alpha shaders

	Shaper::dispose(); // delete shaper shaders (local shaders in shaper.cpp)

	TextureLoader::endTextures();
	TextureLoader::endHCTextures();

	GeometryMap::dispose();
	GeometryMap::disposeHC();

	glfwTerminate();
}

int Application::initI3T()
{
	int err;

	///   - load hard coded textures to TextureLoader
	if (!TextureLoader::loadHCTexture("defAxisTex", Config::getAbsolutePath("/Data/textures/axis.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("cGrid", Config::getAbsolutePath("/Data/textures/cGrid.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("abCube", Config::getAbsolutePath("/Data/textures/cube.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("cube_color", Config::getAbsolutePath("/Data/textures/cube_color.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("dcgiLogo", Config::getAbsolutePath("/Data/textures/dcgi-logo-60.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("icons", Config::getAbsolutePath("/Data/textures/icons.png")))
		return 1;
	if (!TextureLoader::loadHCTexture("white", Config::getAbsolutePath("/Data/textures/white.png")))
		return 1;

	/// \todo the font texture name should be defined as config parameter. Now hard coded also in TabSpace::init
	if (!TextureLoader::loadHCTexture("tahoma_16_0.png",
	                                  Config::getAbsolutePath("/Data/internal/font/tahoma_16_0.png")))
		return 1;

	///   - init Shaper, probably the line debugger
	err = Shaper::initShaders();
	if (err)
		return err;

	///    - Init shaders in ShaderProvider <br> Note: Other shaders are in: shaper.cpp(162), GUI\glyphShader.cpp(26),
	///    and GUI\tabShader.cpp(24)
	err = ShaderProvider::init(Config::getAbsolutePath("/Data/internal/shaders/red.vert").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/red.frag").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/base.vert").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/base.frag").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/alpha.vert").c_str(),
	                           Config::getAbsolutePath("/Data/internal/shaders/alpha.frag").c_str());
	if (err)
		return err;

	///    - load geometry to GeometryMap
	if (GeometryMap::addHCGeometry("testCube", Config::getAbsolutePath("/Data/models/testCube.tmsh")))
		return 1;
	if (GeometryMap::addHCGeometry("orbit", Config::getAbsolutePath("/Data/models/orbit.tmsh")))
		return 1;
	if (GeometryMap::addHCGeometry("abCube", Config::getAbsolutePath("/Data/models/cube.tmsh")))
		return 1;
	if (GeometryMap::addHCGeometry("frustrum", Config::getAbsolutePath("/Data/models/testCube.tmsh"), 2.0f))
		return 1;
	if (GeometryMap::addHCGeometry("camera", Config::getAbsolutePath("/Data/models/camModel.tmsh"), 1.5f))
		return 1;
	if (GeometryMap::addHCGeometry("defAxis", Config::getAbsolutePath("/Data/models/axis.tmsh"), 1.5f))
		return 1;

	///     - load materials to MaterialMap
	MaterialMap::addHCMaterial(new Material("testMaterial", TextureLoader::getHCId("cGrid")));
	MaterialMap::addHCMaterial(new Material("abCube", TextureLoader::getHCId("abCube")));
	MaterialMap::addHCMaterial(new Material("cube_color", TextureLoader::getHCId("cube_color")));
	// MaterialMap::addHCMaterial(new Material("camera", -1, Color::GRAY, Color::WHITE, 1.0f, 1.0f, 1.0f, 1.0f));
	MaterialMap::addHCMaterial(
			new Material("camera", TextureLoader::getHCId("white"), Color::GRAY, Color::WHITE, 1.0f, 1.0f, 1.0f, 1.0f));
	MaterialMap::addHCMaterial(
			new Material("frustrum", TextureLoader::getHCId("cGrid"), Color::YELLOW, Color::WHITE, 1.0f, 1.0f, 1.0f, 1.0f));
	MaterialMap::addHCMaterial(new Material("defAxisMat", TextureLoader::getHCId("defAxisTex"), Color::WHITE,
	                                        Color::WHITE, 1.0f, 1.0f, 1.0f, 1.0f));

	GeometryMap::connectAllHCToShader(); /// \todo Connection of shader should take into account the materials too...
	/// \bug problem - dle dat se neda urcit shader - byl spatne shader pro kameru s texturou -1 a je spatne shader pro
	/// frustrum, ktery ma byt alphaShader a je baseShader

	///   - objects definitions data structure ObjectDefinitionTree
	ObjectDefinitionTree::tree = new ObjectDefinitionTree(nullptr);

	// read content files -- see 6 lines below
	// if (Reader::readContentCFG(Config::getAbsolutePath(Config::CONTENT_FILE.c_str()).c_str())) return 1;

	///   - create the \ref world World
	m_world = new World();

	// connect users geometries
	// GeometryMap::connectAllToShader();

	// world->onReshape(Config::WIN_WIDTH, Config::WIN_HEIGHT);

	///   - setup camera control
	m_world->setConfigCameraControl();

	return 0;
}

Application& Application::get()
{
	return s_instance;
}

World* Application::world()
{
	return m_world;
}

ImFont* Application::getFont(int fontId)
{
	return m_fonts[fontId];
}

GLFWwindow* Application::mainWindow()
{
	return m_window->get();
}

void Application::onBeforeClose()
{
	showUniqueWindow<BeforeCloseModal>();
}

void Application::onClose()
{
	m_bShouldClose = true;
}

void Application::enqueueCommand(ICommand* command)
{
	m_commands.push_back(command);
}

void Application::popWindow(const std::string& windowId)
{
	if (hasWindow(windowId))
		m_windows.erase(windowId);
}

bool Application::hasWindow(const std::string& id)
{
	return m_windows.count(id);
}

Application Application::s_instance;
