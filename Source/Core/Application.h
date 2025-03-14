/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Core/Defs.h"
#include "Core/Module.h"
#include "Core/Window.h"

class ICommand;
class Window;
class MainMenuBar;
class UIModule;

static inline const std::string BASE_WINDOW_TITLE = "I3T - An Interactive Tool for Teaching Transformations";

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
	virtual ~Application();

	static Application& get();

	/**
	 * Performs initialization of the application.
	 * @see onInit()
	 */
	bool init();

	/**
	 * Enter main loop.
	 */
	int run();

	/**
	 * Single iteration of the game loop
	 * @return Whether the game loop should continue
	 */
	bool frame();

	void beginFrame();
	void endFrame();

	/**
	 * Perform logic update.
	 * @see onUpdate()
	 */
	void update();

	void display();

	/**
	 * Shutdown the whole application.
	 * Called when CloseCommand is received.
	 * @see onClose()
	 */
	void close();

	/// Creates instance of module, registers it to the application, and calls its init() method.
	///
	/// \note Modules are destroyed in the reverse order of their creation!
	///
	/// \tparam T
	/// \tparam Args
	/// \param args
	/// \return
	template <typename T, typename... Args>
	static T* createModule(Args&&... args);

	template <typename T>
	static T& getModule();

	/**
	 * Issue command.
	 *
	 * Application will process all enqueued command in the main loop.
	 *
	 * \param command Pointer to command base class.
	 */
	void enqueueCommand(ICommand* command);

	/**
	 * Init OpenGL stuffs before display loop.
	 * Taken from Muller GLFW. Initializes ImGui and I3T stuffs.
	 */
	void initWindow();

	/**
	 * Get the main GLFW window.
	 */
	GLFWwindow* getWindow();

	/**
	 * Get the active GLFW window.
	 * This may return a different window than the getWindow() method due to ImGui potentially create new native windows
	 * to handle ImGui windows being dragged out of the main window.
	 */
	static GLFWwindow* getCurrentWindow();

	const std::string& getTitle();
	void setTitle(const std::string& title);

protected:
	virtual void onInit() {}
	virtual void onBeginFrame() {}
	virtual void onEndFrame() {}
	virtual void onUpdate(double delta) {}
	virtual void onClose() {}

private:
	// std::unordered_map<std::size_t, std::unique_ptr<Module>> m_modules;
	std::vector<std::unique_ptr<Module>> m_modules;

	bool m_shouldClose = false;
	double m_lastFrameSeconds{0.0}; // PF changed to double

	Window* m_window;

	/// Array of commands which the application is going to process in its main loop.
	std::vector<ICommand*> m_commands;

	static Application* s_instance;

	std::unordered_map<std::size_t, std::size_t> m_modulesLookup;
};

template <typename T, typename... Args>
inline T* Application::createModule(Args&&... args)
{
	static_assert(std::is_base_of_v<Module, T>, "Class T must be derived from the Module class");

	const auto hash = typeid(T).hash_code();
	auto& self = Application::get();
	self.m_modules.push_back(std::make_unique<T>(std::forward(args)...));

	const auto idx = self.m_modules.size() - 1;
	self.m_modulesLookup[hash] = idx;

	auto* result = self.m_modules.back().get();
	result->onInit();

	return (T*) result;
}

template <typename T>
T& Application::getModule()
{
	const auto hash = typeid(T).hash_code();
	const auto& self = Application::get();

	I3T_ASSERT(self.m_modulesLookup.count(hash) != 0, "Module is not created!: " + std::string(typeid(T).name()));

	const auto idx = self.m_modulesLookup.at(hash);
	auto result = self.m_modules[idx].get();
	I3T_ASSERT(dynamic_cast<T*>(result) != nullptr, "Invalid type.");

	return *(T*) result;
}

typedef Application App;
