#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "Core/Window.h"

static inline const std::string BASE_WINDOW_TITLE = "I3T - An Interactive Tool for Teaching Transformations";

class ICommand;
class Window;
class MainMenuBar;
class UIModule;

namespace Vp
{
class Viewport;
}

/**
 * Application class.
 * A wrapper for UI windows.
 */
class Application
{
protected:
	Application();

private:
	Application(const Application&) = default;
	Application& operator=(const Application&) = default;

public:
	~Application();

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

	Vp::Viewport* viewport();

	/**
	 * Issue command.
	 *
	 * Application will process all enqueued command in the main loop.
	 *
	 * \param command Pointer to command base class.
	 */
	void enqueueCommand(ICommand* command);

	/// Performs initialization of the application.
	/// 1. Initializes the window and OpenGL context.
	void init();

protected:
	virtual void onInit() {}

public:
	/// Creates instance of module, registers it to the application, and calls its init() method.
	template <typename T, typename... Args> static auto& createModule(Args&&... args);

public:
	template <typename T> static T& getModule();

private:
	static Application* s_instance;

	std::unordered_map<std::size_t, std::unique_ptr<Module>> m_modules;

	bool m_bShouldClose = false;

	Vp::Viewport* m_viewport;

	Window* m_window;

	/// Array of commands which the application is going to process in its main
	/// loop.
	std::vector<ICommand*> m_commands;

public:
	bool m_debugWindowManager = false; ///< Debug switch for WindowManager, toggled in Help > Debug window manager
	bool m_debugTrackball = false;     ///< Debug switch for WindowManager, toggled in Help > Debug trackball

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
	 * Called when CloseCommand is received.
	 *
	 * Shutdown whole application.
	 */
	void onClose();

	// TODO: (DR) Somewhat outdated docs
	/**
	 * \brief	Updates the world and the mouse button state, mouseDelta, mousePrev
	 * and throws JUST_Pressed to PRESSED PreUpdate, world update, update
	 */
	void logicUpdate(double delta);
};

template <typename T, typename... Args> inline auto& Application::createModule(Args&&... args)
{
	static_assert(std::is_base_of_v<Module, T>, "Class T must be derived from the Module class");

	const auto hash = typeid(T).hash_code();
	auto& self = Application::get();
	self.m_modules[hash] = std::make_unique<T>(std::forward(args)...);
	self.m_modules[hash]->init();

	return *self.m_modules[hash];
}

template <typename T> T& Application::getModule()
{
	const auto hash = typeid(T).hash_code();
	const auto& self = Application::get();

	I3T_ASSERT(self.m_modules.count(hash) != 0, "Module is not created!");
	I3T_ASSERT(dynamic_cast<T*>(self.m_modules.at(hash).get()) != nullptr, "Invalid type.");

	return *(T*) self.m_modules.at(hash).get();
}

typedef Application App;
