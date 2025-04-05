/**
 * \file
 * \brief Facade logger class for the spdlog library
 * \authors Filip Uhlik
 * \date 2019/10/22
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 *
 * CHANGELOG
 * - add app logger
 * - add flush call after each log
 * - va_args replaced by variadic templates
 */
#pragma once

#include <map>
#include <sstream>
#include <stack>
#include <string>

#include "spdlog/spdlog.h"

// Omit trace from release builds in case of any perf issues
#define LOG_TRACE(...) Logger::getInstance().getAppLogger()->trace(__VA_ARGS__)
#ifndef NDEBUG
#define LOG_DEBUG(...) Logger::getInstance().getAppLogger()->debug(__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif
#define LOG_INFO(...) Logger::getInstance().getAppLogger()->info(__VA_ARGS__);
#define LOG_WARN(...) Logger::getInstance().getAppLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...) Logger::getInstance().getAppLogger()->error(__VA_ARGS__);
#define LOG_FATAL(...) Logger::getInstance().getAppLogger()->critical(__VA_ARGS__);
#define LOG_EVENT_MOUSE_POS(mouseX, mouseY)                                                                            \
	Logger::getInstance().log(LoggingOption::MOUSE_MOVEMENT, Logger::getInstance().getLogString("mousePos"), mouseX,   \
	                          mouseY)
#define LOG_EVENT_MOUSE_CLICK(button, mouseX, mouseY)                                                                  \
	Logger::getInstance().log(LoggingOption::MOUSE_CLICK, Logger::getInstance().getLogString("mouseClick"), button,    \
	                          mouseX, mouseY)
#define LOG_EVENT_MOUSE_RELEASE(button, mouseX, mouseY)                                                                \
	Logger::getInstance().log(LoggingOption::MOUSE_CLICK, Logger::getInstance().getLogString("mouseRelease"), button,  \
	                          mouseX, mouseY)
#define LOG_EVENT_OPEN_POP_UP(label)                                                                                   \
	Logger::getInstance().log(LoggingOption::POP_UP, Logger::getInstance().getLogString("openPopUp"), label)
#define LOG_EVENT_MENU_CLICK(label)                                                                                    \
	Logger::getInstance().log(LoggingOption::POP_UP, Logger::getInstance().getLogString("menu"), label)
#define LOG_EVENT_BUTTON_CLICK(label)                                                                                  \
	Logger::getInstance().log(LoggingOption::POP_UP, Logger::getInstance().getLogString("button"), label)
#define LOG_EVENT_SELECT(label)                                                                                        \
	Logger::getInstance().log(LoggingOption::POP_UP, Logger::getInstance().getLogString("select"), label)
#define LOG_EVENT_CONNECT(outputPin, inputPin)                                                                         \
	Logger::getInstance().log(LoggingOption::LOGIC, Logger::getInstance().getLogString("connect"),                     \
	                          outputPin->getSignature(), inputPin->getSignature())
#define LOG_EVENT_DISCONNECT(outputPin, inputPin)                                                                      \
	Logger::getInstance().log(LoggingOption::LOGIC, Logger::getInstance().getLogString("disconnect"),                  \
	                          outputPin->getSignature(), inputPin->getSignature())
#define LOG_EVENT_NODE_ADDED_AT_INDEX(index, addedNode, addedToNode)                                                   \
	Logger::getInstance().log(LoggingOption::LOGIC, Logger::getInstance().getLogString("nodeAddIndex"), addedNode,     \
	                          addedToNode, index)
#define LOG_EVENT_NODE_ADDED(addedNode, addedToNode)                                                                   \
	Logger::getInstance().log(LoggingOption::LOGIC, Logger::getInstance().getLogString("nodeAdd"), addedNode,          \
	                          addedToNode)
#define LOG_EVENT_NODE_REMOVED(removedNode, removedFromNode)                                                           \
	Logger::getInstance().log(LoggingOption::LOGIC, Logger::getInstance().getLogString("nodeRem"), removedNode,        \
	                          removedFromNode)
#define LOG_EVENT_OBJECT_ADDED(objectName, node)                                                                       \
	Logger::getInstance().log(LoggingOption::LOGIC, Logger::getInstance().getLogString("objAdd"), objectName, node)
#define LOG_EVENT_MATRIX_VALUE_UPDATE(matrix, index, newVal)                                                           \
	Logger::getInstance().log(LoggingOption::MATRIX_FIELD, Logger::getInstance().getLogString("matrix"), index,        \
	                          matrix, newVal)
#define LOG_EVENT_TUTORIAL_STEP(tutorial, step, name)                                                                  \
	Logger::getInstance().log(LoggingOption::TUTORIAL, Logger::getInstance().getLogString("tutorial"), tutorial, step, \
	                          name)
#define ADD_TO_LOG_BUFFER(logType, message, numOfArgs, ...)                                                            \
	Logger::getInstance().addToLogBuffer(logType, message, numOfArgs, __VA_ARGS__)
#define INIT_LOGGER(argc, argv) Logger::getInstance().initLogger(argc, argv)
#define END_LOGGER Logger::getInstance().endLogger()
#define UPDATE_LOGGER() Logger::getInstance().update()


enum class LoggingOption
{
	POP_UP,
	LOGIC,
	MATRIX_FIELD,
	MOUSE_MOVEMENT,
	MOUSE_CLICK,
	TUTORIAL
};

class Logger
{
	/// Creates default logger, custom one can be created by Logger::initLogger.
	Logger() = default;
	~Logger() = default;

public:
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(const Logger&) = delete;

	/**
	 * Logger class is a singleton.
	 * \return Logger instance.
	 */
	static Logger& getInstance();

	std::shared_ptr<spdlog::logger> getAppLogger()
	{
		return appLogger;
	}

	std::string getLogString(const std::string& key) const;

	void initLogger(int argc, char* argv[]);
	void endLogger() const;
	void update();

	template <typename... Args>
	void log(const LoggingOption& logType, const std::string& message, Args&&... args);

	void addToLogBuffer(const LoggingOption& logType, const std::string& message, const std::vector<std::string>& args);

	std::ostringstream& getBuffer()
	{
		return m_buffer;
	}

private:
	/// Main logger for the application.
	std::shared_ptr<spdlog::logger> appLogger;

	/// Interaction logger for logging user interactions, has special format.
	std::shared_ptr<spdlog::logger> interactionLogger;

	/// Interaction logger for logging user interactions, has special format.
	std::shared_ptr<spdlog::logger> mouseLogger;

	std::ostringstream m_buffer;

	std::stack<std::string> logBuffer;

	void flushBuffer(std::shared_ptr<spdlog::logger> logger);
	std::string formatMessage(const std::string& message, const std::vector<std::string>& args) const;

	std::map<std::string, std::string> logStrings;

	void toggleLoggingPopUps();
	void toggleLoggingLogic();
	void toggleLoggingMatrixFields();
	void toggleLoggingMouseRaw();

	bool isLoggingPopUps = false;
	bool isLoggingLogic = false;
	bool isLoggingMatrixFields = false;
	bool isLoggingMouseRaw = false;

	bool shouldLogMouse();

	void loadStrings();

	int previousTime = 0;
	const int MOUSE_MOVEMENT_LOG_INTERVALS = 2; // time in seconds
};

//--------------------------------------------------------------------------------------------------------------------//

template <typename... Args>
void Logger::log(const LoggingOption& logType, const std::string& message, Args&&... args)
{
	std::vector<std::string> logData = {args...};
	addToLogBuffer(logType, message, logData);

	if (logType == LoggingOption::MOUSE_CLICK || logType == LoggingOption::MOUSE_MOVEMENT)
	{
		flushBuffer(mouseLogger);
	}
	else
	{
		flushBuffer(interactionLogger);
	}
}
