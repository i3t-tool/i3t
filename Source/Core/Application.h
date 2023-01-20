#pragma once

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "gl_core_4_4.h"
#include "imgui.h"

#include "Core/Defs.h"
#include "Core/Module.h"
#include "Core/Window.h"
#include "State/Stateful.h"

static const std::string g_baseTitle = "I3T - An Interactive Tool for Teaching Transformations";

class ICommand;
class Window;
class MainMenuBar;
class World;
class Scripting;
class UIModule;

namespace Vp
{
class Viewport;
}

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

	Vp::Viewport* viewport();

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
	template <typename T, typename... Args> static auto& createModule(Args&&... args);

public:
	template <typename T> static T& getModule();

private:
	static Application* s_instance;

	std::unordered_map<std::size_t, std::unique_ptr<Module>> m_modules;

	/**
	 * \brief	Window display flag - if true, it disables the onDisplay callback
	 * resulting in no window update.
	 */
	bool m_isPaused;

	bool m_bShouldClose = false;

	World* m_world;
	Vp::Viewport* m_viewport;

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

template <typename T, typename... Args> inline auto& Application::createModule(Args&&... args)
{
	static_assert(std::is_base_of_v<Module, T>, "Class T must be derived from the Module class");

	const auto hash = typeid(T).hash_code();
	auto& self = Application::get();
	self.m_modules[hash] = std::make_unique<T>(std::forward(args)...);

	return *self.m_modules[hash];
}

template <typename T> T& Application::getModule()
{
	const auto hash = typeid(T).hash_code();
	const auto& self = Application::get();

	I3T_ASSERT(self.m_modules.count(hash) != 0 && "Module is not created!");
	I3T_ASSERT(dynamic_cast<T*>(self.m_modules.at(hash).get()) != nullptr && "Invalid type.");

	return *(T*)self.m_modules.at(hash).get();
}

typedef Application App;
