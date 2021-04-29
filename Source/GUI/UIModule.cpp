#include "UIModule.h"

#include "Commands/ApplicationCommands.h"
#include "Config.h"
#include "Core/Input/InputBindings.h"
#include "Core/Input/InputManager.h"
#include "GUI/Elements/MainMenuBar.h"
#include "GUI/Elements/Windows/Console.h"
#include "GUI/Elements/Windows/LogWindow.h"
#include "GUI/Elements/Windows/TutorialWindow.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

#include "GUI/ImGui/imgui_impl_glfw.h"
#include "GUI/ImGui/imgui_impl_opengl3.h"

#include "Logger/Logger.h"

using namespace UI;

UIModule::~UIModule()
{
  delete m_menu;
}

void UIModule::init()
{
  // Create GUI Elements.
  m_menu = new MainMenuBar();
  m_dockableWindows.push_back(std::make_shared<TutorialWindow>(false));
  m_dockableWindows.push_back(std::make_shared<Viewport>(true, App::get().world2()));
  m_dockableWindows.push_back(std::make_shared<WorkspaceWindow>(true));
  m_dockableWindows.push_back(std::make_shared<Console>(true));
  m_dockableWindows.push_back(std::make_shared<LogWindow>());

  HideWindowCommand::addListener([this](const std::string& id) { popWindow(id); });

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

  m_currentTheme.init();
  m_currentTheme.apply();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(App::get().mainWindow(), true);
  ImGui_ImplOpenGL3_Init(ImGui_GLSLVersion);
}

void UIModule::beginFrame()
{
  // Start the Dear ImGui frame ----------------------
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  buildDockspace();

  // Render UI elements
  m_menu->render();

  // Scene is rendered in the Viewport window.
  // TODO -> Do not render scene in the ViewportWindow class.
  for (auto element : m_dockableWindows)
  {
    if (element->isVisible())
    {
      element->render();
    }
    // if (InputController::isKeyJustPressed(Keys::f)) { printf("UP %s\n",element->getID()); }
  }
  // if (InputController::isKeyJustPressed(Keys::f)) { printf("--- \n"); }
  // Render other windows.
  for (const auto& [id, w] : m_windows)
  {
    if (w->isVisible())
    {
      w->render();
    }
  }

	queryCameraState();

  setFocusedWindow();

  // ImGui rendering ----------------------------------------------------------
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Update and Render additional Platform Windows
  // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this
  // code elsewhere.
  //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
  // ImGuiIO& io = ImGui::GetIO(); (void)io;
  auto& io = ImGui::GetIO();
  (void)io;
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void UIModule::buildDockspace()
{
  // create dockspace -----------------------------
  static bool opt_fullscreen_persistant = true;
  bool opt_fullscreen = opt_fullscreen_persistant;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoWindowMenuButton;

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
}

void UIModule::queryCameraState()
{
  // ORBIT camera rotation
  if (InputManager::isKeyJustPressed(InputBindings::KeyWorld_mouseRotate))
  {
    InputManager::beginCameraControl();
  }
  if (InputManager::isKeyJustUp(InputBindings::KeyWorld_mouseRotate))
  {
    InputManager::endCameraControl();
  }

  // CAMERA PANNING - set a new orbit center
  if (InputManager::isKeyJustPressed(InputBindings::KeyWorld_mousePan))
  {
    InputManager::beginCameraControl();
  }
  if (InputManager::isKeyJustUp(InputBindings::KeyWorld_mousePan))
  {
    InputManager::endCameraControl();
  }
}

/**
 * Not thread safe (using strtok).
 */
std::string makeIDNice(const char* ID)
{
  // Remove ## or ### from ImGui window name.
  //auto sanitizedWindowID = ID;
  //while (*sanitizedWindowID == '#')
  //  ++sanitizedWindowID;

	std::string IDCopy = std::string(ID);

  // Get fist part of "window/child-window" id.
  char* currID = std::strtok(const_cast<char*>(IDCopy.c_str()), "###");
  char* lastID;
	while (currID != nullptr)
  {
	  lastID = currID;
		currID = std::strtok(nullptr, "###");
	}

	currID = std::strtok(const_cast<char*>(lastID), "/");

  return std::string(currID);
}

void UIModule::setFocusedWindow()
{
  ImGuiContext& g = *GImGui;
  const char* hoveredWindowID = g.HoveredWindow ? g.HoveredWindow->Name : "";
  const char* focusedWindowID = g.ActiveIdWindow ? g.ActiveIdWindow->Name : "";

	// Check for hovered window.
  if (strlen(hoveredWindowID) != 0)
  {
    auto mainID = makeIDNice(hoveredWindowID);

    Ptr<IWindow> window;
    window = findWindow(mainID.c_str(), m_dockableWindows);

    if (m_windows.count(mainID) != 0)
      window = m_windows[mainID];

    if (window != nullptr)
    {
      // Set focus if hovered window was clicked.
      if (InputManager::isMouseClicked())
      {
        ImGui::SetWindowFocus(g.HoveredWindow->Name);
        InputManager::setFocusedWindow(window);
      }

      InputManager::setHoveredWindow(window);
    }
    else
    {
      //
      InputManager::setHoveredWindow(nullptr);
    }
  }

  if (strlen(focusedWindowID) != 0)
  {
    auto mainID = makeIDNice(focusedWindowID);

    Ptr<IWindow> window;
    if ((window = findWindow(mainID.c_str(), m_dockableWindows)) != nullptr)
    {
      InputManager::setFocusedWindow(window);
    }
  }
}

void UIModule::popWindow(const std::string& windowId)
{
  if (hasWindow(windowId))
    m_windows.erase(windowId);
}

bool UIModule::hasWindow(const std::string& id)
{
  return m_windows.count(id);
}
