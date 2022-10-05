/**
 * \file	GUI\App.h
 *
 * Application class.
 */
#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

#include "gl_core_4_4.h"
#include "imgui.h"

#include "Core/Defs.h"
#include "Core/Window.h"
#include "State/Stateful.h"

static const std::string g_baseTitle =
    "I3T - An Interactive Tool for Teaching Transformations";

class Module;
class ICommand;
class Window;
class MainMenuBar;
class World;
class Scripting;
class UIModule;

/**
 * Application class.
 * A wrapper for UI windows.
 */
class Application : public IStateful
{
public:
	Application() = default;
	~Application() = default;

	/**
	 * \fn	void finalize()
	 *
	 * \brief	Finalize application
	 *
	 * 			Delete the object tree, world, shaders, textures, geometries,
	 * tabSpace, shaper.
	 */
	void finalize();

	static Application& get();
	UIModule* getUI();

	//===--------------------------------------------------------------------===//

	/**
	 * Init OpenGL stuffs before display loop.
	 * Taken from Muller GLFW. Initializes ImGui and I3T stuffs.
	 */
	void initWindow();

	GLFWwindow* mainWindow();

	const std::string& getTitle();

	void setTitle(const std::string& title);

	//===--------------------------------------------------------------------===//

	/**
	 * Enter main loop.
	 *
	 * Sequence of actions:
	 *   -# Update the mouse button state.
	 *   -# Update World = handle 2D and 3D interaction.
	 *   -# Update mouseDelta, mousePrev, and the stored statuses of the keys in
	 * the \a keyMap array (JUST_UP -> UP, JUST_DOWN -> DOWN).
	 *   -# Query ImGui states (active window position, ...).
	 */
	void run();

	World* world();

	/**
	 * Issue command.
	 *
	 * Application will process all enqueued command in the main loop.
	 *
	 * \param command Pointer to command base class.
	 */
	void enqueueCommand(ICommand* command);

	void init();

	/// \todo MH This function must be called after world instantiation.
	void initModules();

	/**
	 * \brief	Initializes renderer and scene.
	 */
	bool initI3T();

	//===--------------------------------------------------------------------===//

	void onStateChange() override;

	//===--------------------------------------------------------------------===//

private:
	static Application* s_instance;

	std::vector<Module*> m_modules;

	/**
	 * \brief	Window display flag - if true, it disables the onDisplay callback
	 * resulting in no window update.
	 */
	bool m_isPaused;

	bool m_bShouldClose = false;

	World* m_world;
	Scripting* m_scriptInterpreter;
	// GLFWwindow* m_window;
	Window* m_window;

	/// Array of commands which the application is going to process in its main
	/// loop.
	std::vector<ICommand*> m_commands;

public:
	/**
	 * \brief	GLUT Callback: render the whole I3T window (update the Logic, draw
	 * 3D scene and 2D workspace)
	 *
	 * - logicUpdate() - handles events, 3D interaction, editor, 3D scene
	 * - World::render(); - draw 3D scene
	 * - TabSpace::draw(); - draw the workspace with all boxes
	 */
	void onDisplay();

private:
	/**
	 * Called when BeforeCloseCommand is received.
	 *
	 * If current project is in unsaved state, a modal window "Do your really want
	 * to leave?" is displayed. Otherwise, the CloseCommand is issued.
	 */
	void onBeforeClose();

	/**
	 * Called when CloseCommand is received.
	 *
	 * Shutdown whole application.
	 */
	void onClose();

	/**
	 * \brief	Updates the world and the mouse button state, mouseDelta, mousePrev
	 * and throws JUST_Pressed to PRESSED PreUpdate, world update, update
	 */
	void logicUpdate();
};

typedef Application App;
