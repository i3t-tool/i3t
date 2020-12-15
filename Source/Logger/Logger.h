/**
 * \file	logger.h
 *
 * Facade logger class for the spdlog library
 *
 * \date  	2019/10/22
 * \author 	Filip Uhlik, CTU Prague
 */
#pragma once

#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"

#include "Config.h"
#include "Core/InputController.h"

#ifdef _DEBUG
#define LOG_DEBUG(...) Logger::getInstance().getConsoleLogger()->trace(__VA_ARGS__);
#else
#define LOG_DEBUG(...)
#endif
#define LOG_INFO(...) Logger::getInstance().getConsoleLogger()->info(__VA_ARGS__);
#define LOG_WARN(...) Logger::getInstance().getConsoleLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...) Logger::getInstance().getConsoleLogger()->error(__VA_ARGS__);
#define LOG_FATAL(...) Logger::getInstance().getConsoleLogger()->critical(__VA_ARGS__);
#define LOG_EVENT_MOUSE_POS(mouseX, mouseY) Logger::getInstance().log(\
	LoggingOption::MOUSE_MOVEMENT, Logger::getInstance().getLogString("mousePos"), 2, mouseX, mouseY)
#define LOG_EVENT_MOUSE_CLICK(button, mouseX, mouseY) Logger::getInstance().log(\
	LoggingOption::MOUSE_CLICK, Logger::getInstance().getLogString("mouseClick"), 3, button, mouseX, mouseY)
#define LOG_EVENT_MOUSE_RELEASE(button, mouseX, mouseY) Logger::getInstance().log(\
	LoggingOption::MOUSE_CLICK, Logger::getInstance().getLogString("mouseRelease"), 3, button, mouseX, mouseY)
#define LOG_EVENT_OPEN_POP_UP(clickedTab) Logger::getInstance().log(\
	LoggingOption::POP_UP, Logger::getInstance().getLogString("openPopUp"), 1, clickedTab->getNameableParentNameId())
#define LOG_EVENT_CLOSE_POP_UP(tab) Logger::getInstance().log(\
	LoggingOption::POP_UP, Logger::getInstance().getLogString("closePopUp"), 1, tab->getNameableParentNameId())
#define LOG_EVENT_CLOSE_POP_UP_IN(tab) Logger::getInstance().log(\
	LoggingOption::POP_UP, Logger::getInstance().getLogString("closePopUpIn"), 1, tab->getNameableParentNameId())
#define LOG_EVENT_BUTTON_CLICK(tab) if (dynamic_cast<Button*>(tab)) Logger::getInstance().log(\
	LoggingOption::POP_UP, Logger::getInstance().getLogString("button"), 1, tab->getNameableParentNameId())
#define LOG_EVENT_CONNECT(startTab, endTab) Logger::getInstance().log(\
	LoggingOption::LOGIC, Logger::getInstance().getLogString("connect"), 2, startTab->getNameableParentNameId(), endTab->getNameableParentNameId())
#define LOG_EVENT_DISCONNECT(startTab, endTab) Logger::getInstance().log(\
	LoggingOption::LOGIC, Logger::getInstance().getLogString("disconnect"), 2, startTab->getNameableParentNameId(), endTab->getNameableParentNameId())
#define LOG_EVENT_TAB_ADDED_AT_INDEX(index, addedTab, addedToTab) Logger::getInstance().log(\
	LoggingOption::LOGIC, Logger::getInstance().getLogString("tabAddIndex"), 3, addedTab->getNameableParentNameId(), addedToTab->getNameableParentNameId(), index)
#define LOG_EVENT_TAB_ADDED(addedTab, addedToTab) Logger::getInstance().log(\
	LoggingOption::LOGIC, Logger::getInstance().getLogString("tabAdd"), 2, addedTab->getNameableParentNameId(), addedToTab->getNameableParentNameId())
#define LOG_EVENT_TAB_REMOVED(removedTab, removedFromTab) Logger::getInstance().log(\
	LoggingOption::LOGIC, Logger::getInstance().getLogString("tabRem"), 2, removedTab->getNameableParentNameId(), removedFromTab->getNameableParentNameId())
#define LOG_EVENT_OBJECT_ADDED(objectName, tab) Logger::getInstance().log(\
	LoggingOption::LOGIC, Logger::getInstance().getLogString("objAdd"), 2, objectName, tab->getNameableParentNameId())
#define LOG_EVENT_OBJECT_REMOVED(tab) Logger::getInstance().log(\
	LoggingOption::LOGIC, Logger::getInstance().getLogString("objRem"), 1, tab->getNameableParentNameId())
#define LOG_EVENT_MATRIX_VALUE_UPDATE(tabGroup, matrix, index, newVal) if (tabGroup->getDraggedNumberBox() == NULL) Logger::getInstance().log(\
	LoggingOption::MATRIX_FIELD, Logger::getInstance().getLogString("matrix"), 3, index, matrix->getNameableParentNameId(), newVal)
#define LOG_EVENT_MATRIX_VALUE_UPDATE_DRAG(matrix, index, newVal) Logger::getInstance().log(\
	LoggingOption::MATRIX_FIELD, Logger::getInstance().getLogString("matrix"), 3, index, matrix->getNameableParentNameId(), newVal)
#define LOG_EVENT_TUTORIAL_STEP(tutorial, step, name) Logger::getInstance().log(\
	LoggingOption::TUTORIAL, Logger::getInstance().getLogString("tutorial"), 3, tutorial, step, name)
#define ADD_TO_LOG_BUFFER(logType, message, numOfArgs, ...) Logger::getInstance().addToLogBuffer(logType, message, numOfArgs, __VA_ARGS__)
#define INIT_LOGGER(argc, argv) Logger::getInstance().initLogger(argc, argv)
#define END_LOGGER Logger::getInstance().endLogger()
#define UPDATE_LOGGER() Logger::getInstance().update()


// MANUAL TUTORIAL LOGGING - marked with #TUTORIAL - can be safely deleted when tutorial is implemented
//	- Key in LoggingToggle struct (.h and .cpp)
//	- Count variables in Logger class (.h)
//  - Event call in Logger::update() (.cpp)

enum class LoggingOption
{
  POP_UP,
  LOGIC,
  MATRIX_FIELD,
  MOUSE_MOVEMENT,
  MOUSE_CLICK,
  TUTORIAL
};


struct LoggingToggle final
{
  static Keys::Code KEY_LoggingToggle_popUps;
  static Keys::Code KEY_LoggingToggle_logic;
  static Keys::Code KEY_LoggingToggle_matrixFields;
  static Keys::Code KEY_LoggingToggle_mouseMovement;
  static Keys::Code KEY_Dummy_tutorialStep;	// #TUTORIAL
};


class Logger
{
public:
  /**
   * Logger class is a singleton.
   * @return Logger instance.
   */
  static Logger& getInstance();

  /// \todo
  inline std::shared_ptr<spdlog::logger> getAppLogger() { return appLogger; };

  inline std::shared_ptr<spdlog::logger> getConsoleLogger() { return m_consoleLogger; }

  std::string getLogString(const std::string& key) const;

  void initLogger(int argc, char* argv[]);
  void endLogger() const;
  void update();
  void log(const LoggingOption& logType, const std::string message, const int numOfArgs, ...);
  void addToLogBuffer(const LoggingOption& logType, const std::string message, const int numOfArgs, ...);

private:
  Logger() = default;
  ~Logger() = default;
  Logger(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;
  
  std::shared_ptr<spdlog::logger> appLogger = spdlog::basic_logger_mt("app_logger", "logs/main.log");
  std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("basic_logger", "logs/user_interaction.log");
  std::shared_ptr<spdlog::logger> mouseLogger = spdlog::basic_logger_mt("mouse_logger", "logs/mouse_log.log");

  std::shared_ptr<spdlog::logger> m_consoleLogger;

  std::stack<std::string> logBuffer;

  void vAddToLogBuffer(const LoggingOption& logType, const std::string& message, const int numOfArgs, va_list& arguments);
  void flushBuffer(std::shared_ptr<spdlog::logger> logger);
  std::string formatMessage(const std::string& message, std::queue<std::string> arguments) const;

  std::queue<std::string> vaListToQueue(const int numOfArgs, va_list& args);

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
  const int MOUSE_MOVEMENT_LOG_INTERVALS = 2000;
  const std::string LOG_STRINGS_PATH = Config::getAbsolutePath("/Resources/events_definition.json");

  int tutorialCount = 1; // #TUTORIAL
  int stepCount = 1; // #TUTORIAL
};
