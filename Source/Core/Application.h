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
	std::unordered_map<std::size_t, std::unique_ptr<Module>> m_modules;

	bool m_shouldClose = false;
	double m_lastFrameSeconds{0.0}; // PF changed to double

	Window* m_window;

	/// Array of commands which the application is going to process in its main loop.
	std::vector<ICommand*> m_commands;

	static Application* s_instance;

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
	void init();

	/**
	 * Enter main loop.
	 */
	void run();

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
	template <typename T, typename... Args> static T* createModule(Args&&... args);

	template <typename T> static T& getModule();

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

	GLFWwindow* getWindow();

	const std::string& getTitle();
	void setTitle(const std::string& title);

protected:
	virtual void onInit() {}
	virtual void onBeginFrame() {}
	virtual void onEndFrame() {}
	virtual void onUpdate(double delta) {}
	virtual void onClose() {}
};

template <typename T, typename... Args> inline T* Application::createModule(Args&&... args)
{
	static_assert(std::is_base_of_v<Module, T>, "Class T must be derived from the Module class");

	const auto hash = typeid(T).hash_code();
	auto& self = Application::get();
	self.m_modules[hash] = std::make_unique<T>(std::forward(args)...);
	self.m_modules[hash]->onInit();

	return (T*) self.m_modules[hash].get();
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
