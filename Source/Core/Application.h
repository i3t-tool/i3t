/**
 * \file	GUI\App.h
 *
 * Application class.
 */
#pragma once

#include "Core/GlfwWindow.h"
#include <gl_core_4_4.h>
#include <imgui.h>

#include <array>
#include <map>
#include <vector>

#include "Core/Defs.h"
#include "GUI/Elements/IWindow.h"

constexpr char* ImGui_GLSLVersion = "#version 140";

#define FONT_MENU_LARGE    0
#define FONT_TUTORIAL_TEXT 0
#define FONT_BUTTON        1
#define FONT_TAB           1
#define FONT_NODE          2
#define FONT_MENU_SMALL    2
#define FONT_TITLE         3
#define FONT_TASK_TITLE    4
#define FONTS_COUNT        4

class ICommand;
class GlfwWindow;
class MainMenuBar;
class World;

/**
 * Application class.
 * A wrapper for UI windows.
 */
class Application
{
public:
  Application();

  /**
   * \fn	void finalize()
   *
   * \brief	Finalize application
   *
   * 			Delete the object tree, world, shaders, textures, geometries, tabSpace, shaper.
   */
  void finalize();

  static Application& get();

  /**
   * Init OpenGL stuffs before display loop.
   * Taken from Muller GLFW. Initializes ImGui and I3T stuffs.
   */
  void initWindow();

  /**
   * Enter main loop.
   *
   * Sequence of actions:
   *   -# Update the mouse button state.
   *   -# Update World = handle 2D and 3D interaction.
   *   -# Update mouseDelta, mousePrev, and the stored statuses of the keys in the \a keyMap array
   *        (JUST_UP -> UP, JUST_DOWN -> DOWN).
   *   -# Query ImGui states (active window position, ...).
   */
  void run();

  World* world();
  ImFont* getFont(int fontId);
  GLFWwindow* mainWindow();

  /**
   * Create and show desired window.
   *
   * \tparam T window to create. T must be derived from IWindow class.
   */
  template <typename T> void showUniqueWindow();

  /**
   * Issue command.
   *
   * Application will process all enqueued command in the main loop.
   *
   * \param command Pointer to command base class.
   */
  void enqueueCommand(ICommand* command);

  bool hasWindow(const std::string& id);

  /**
   * Initialize ImGui stuffs.
   */
  void initImGui();

  /**
   * \fn	int init()
   *
   * \brief	Load hard coded objects (geometry and textures), load some shaders, create the world and load the initial
   * scene. \brief	Loads the shaders, updates the camera mode.
   *
   * \retval 0 OK
   * \retval 1 files error (not loaded textures, shaders)
   * \retval 2 shader compile error
   */
  int initI3T();

private:
  friend class MainMenuBar;
  friend class TutorialWindow;
  friend class ViewportWindow;
  friend class WorkspaceWindow;

  static Application s_instance;

  /**
   * \brief	Window display flag - if true, it disables the onDisplay callback resulting in no
   * 			window update.
   */
  bool m_isPaused;

  bool m_showWorkspaceWindow;
  bool m_showTutorialWindow;
  bool m_showConsoleWindow;
  bool m_showZoomWindow;
  bool m_showViewportWindow;

  bool m_bShouldClose = false;

  MainMenuBar* m_menu;

  World* m_world; ///< all GUI and logic objects

  // GLFWwindow* m_window;
  GlfwWindow* m_window;

  /// Application subwindows/dockable windows such as Viewport, Node editor. A window can be showed or hidden.
  std::vector<IWindow*> m_dockableWindows;

  std::map<std::string, std::unique_ptr<IWindow>> m_windows;

  std::array<ImFont*, (unsigned)FONTS_COUNT + 1> m_fonts = {nullptr, nullptr, nullptr, nullptr, nullptr};

  /// Array of commands which the application is going to process in its main loop.
  std::vector<ICommand*> m_commands;

  /**
   * \brief	GLUT Callback: render the whole I3T window (update the Logic, draw 3D scene and 2D workspace)
   *
   * - logicUpdate() - handles events, 3D interaction, editor, 3D scene
   * - World::render(); - draw 3D scene
   * - TabSpace::draw(); - draw the workspace with all boxes
   */
  void onDisplay();

  /**
   * Called when BeforeCloseCommand is received.
   *
   * If current project is in unsaved state, a modal window "Do your really want to leave?"
   * is displayed. Otherwise, the CloseCommand is issued.
   */
  void onBeforeClose();

  /**
   * Called when CloseCommand is received.
   *
   * Shutdown whole application.
   */
  void onClose();

  /**
   * Hide and remove window.
   *
   * Called by HideWindowCommand.
   * \param windowId identification of window to hide.
   */
  void popWindow(const std::string& windowId);

  /**
   * \brief	Updates the world and the mouse button state, mouseDelta, mousePrev and throws JUST_Pressed to PRESSED
   * 			PreUpdate, world update, update
   */
  void logicUpdate();
};

typedef Application App;

template <typename T> void Application::showUniqueWindow()
{
  Debug::Assert(std::is_base_of<IWindow, T>::value, "Type parameter must be derived from IWindow class.");

  if (!hasWindow(T::id))
    m_windows.insert(std::pair<std::string, std::unique_ptr<IWindow>>(T::id, std::make_unique<T>()));
}
