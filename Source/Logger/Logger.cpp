#include "Logger.h"

#include <sstream>

#include <nlohmann/json.hpp>
#include <spdlog/sinks/stdout_sinks.h>

#include "Core/GlfwWindow.h"

Logger& Logger::getInstance()
{
  static Logger instance;
  return instance;
}

std::string Logger::getLogString(const std::string& key) const
{
  auto value = logStrings.find(key);
  if (value != logStrings.end())
  {
    return value->second;
  }
  else {
    return "String not found!";
  }
}

void Logger::initLogger(int argc, char* argv[])
{
  std::cout << "initializing logger" << std::endl;

  loadStrings();

  spdlog::set_pattern("[%d.%m.%Y %T:%e]: %v");

  // Console sink.
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
  sinks[0]->set_level(spdlog::level::trace);
  // sinks[0]->set_pattern("[%d.%m.%Y %T:%e]: %v");

  m_consoleLogger = std::make_shared<spdlog::logger>("Log", sinks.begin(), sinks.end());

  logger->set_level(spdlog::level::trace);
  mouseLogger->set_level(spdlog::level::trace);

  std::ostringstream init_message;
  init_message << " >> Strings version: " << getLogString("version") << " <<";

  logger->trace(getLogString("logInit") + init_message.str());
  mouseLogger->trace(getLogString("mouseLogInit") + init_message.str());
}

void Logger::endLogger() const
{
  logger->trace(getLogString("logEnd"));
  mouseLogger->trace(getLogString("mouseLogEnd"));
}

void Logger::update()
{
  if (InputController::isKeyJustPressed(LoggingToggle::KEY_LoggingToggle_popUps))
  {
    toggleLoggingPopUps();
  }
  if (InputController::isKeyJustPressed(LoggingToggle::KEY_LoggingToggle_logic))
  {
    toggleLoggingLogic();
  }
  if (InputController::isKeyJustPressed(LoggingToggle::KEY_LoggingToggle_matrixFields))
  {
    toggleLoggingMatrixFields();
  }
  if (InputController::isKeyJustPressed(LoggingToggle::KEY_LoggingToggle_mouseMovement))
  {
    toggleLoggingMouseRaw();
  }
  // #TUTORIAL
  if (InputController::isKeyJustPressed(LoggingToggle::KEY_Dummy_tutorialStep))
  {
    if (stepCount > 4)
    {
      tutorialCount++;
      stepCount = 1;
    }
    LOG_EVENT_TUTORIAL_STEP(std::to_string(tutorialCount), std::to_string(stepCount), std::string("Dummy tutorial step"));
    stepCount++;
  }
}

void Logger::log(const LoggingOption& logType, const std::string message, const int numOfArgs, ...)
{
  va_list args;
  va_start(args, numOfArgs);
  vAddToLogBuffer(logType, message, numOfArgs, args);
  if (logType == LoggingOption::MOUSE_CLICK || logType == LoggingOption::MOUSE_MOVEMENT)
  {
    flushBuffer(mouseLogger);
  }
  else
  {
    flushBuffer(logger);
  }
  va_end(args);
}

void Logger::addToLogBuffer(const LoggingOption& logType, const std::string message, const int numOfArgs, ...)
{
  va_list args;
  va_start(args, numOfArgs);
  // TODO -> EOL.
  vAddToLogBuffer(logType, /* spdlog::details::os::default_eol + */ message, numOfArgs, args);
  va_end(args);
}

void Logger::vAddToLogBuffer(const LoggingOption& logType, const std::string& message, const int numOfArgs, va_list& arguments)
{
  if (logType == LoggingOption::MOUSE_MOVEMENT && isLoggingMouseRaw)
  {
    if (shouldLogMouse())
    {
      logBuffer.push(formatMessage(message, vaListToQueue(numOfArgs, arguments)));
    }
  }
  else if (logType == LoggingOption::MOUSE_CLICK && isLoggingMouseRaw)
  {
    logBuffer.push(formatMessage(message, vaListToQueue(numOfArgs, arguments)));
  }
  else if (logType == LoggingOption::LOGIC && isLoggingLogic)
  {
    logBuffer.push(formatMessage(message, vaListToQueue(numOfArgs, arguments)));
  }
  else if (logType == LoggingOption::POP_UP && isLoggingPopUps)
  {
    logBuffer.push(formatMessage(message, vaListToQueue(numOfArgs, arguments)));
  }
  else if (logType == LoggingOption::MATRIX_FIELD && isLoggingMatrixFields)
  {
    logBuffer.push(formatMessage(message, vaListToQueue(numOfArgs, arguments)));
  }
  else if (logType == LoggingOption::TUTORIAL)
  {
    logBuffer.push(formatMessage(message, vaListToQueue(numOfArgs, arguments)));
  }
}

bool Logger::shouldLogMouse()
{
  // Current time in milliseconds.
  //const int currentTime = glutGet(GLUT_ELAPSED_TIME);
  const int currentTime = (int) (glfwGetTime() * 1000.0f); ///< \todo Check if value is correct.
  bool shouldLog = currentTime - previousTime > MOUSE_MOVEMENT_LOG_INTERVALS;
  if (shouldLog)
  {
    previousTime = currentTime;
  }
  return shouldLog;
}

void Logger::loadStrings()
{
  nlohmann::json json_strings;
  std::ifstream i(LOG_STRINGS_PATH);
  i >> json_strings; // TODO -> throws an exception when file LOG_STRINGS_PATH does not exist.

  for (auto& el : json_strings.items()) {
    logStrings[el.key()] = el.value().get<std::string>();
  }
}

void Logger::flushBuffer(std::shared_ptr<spdlog::logger> logger)
{
  if (logBuffer.empty())
  {
    return;
  }
  std::ostringstream outMessage;
  while (!logBuffer.empty())
  {
    outMessage << logBuffer.top();
    logBuffer.pop();
  }
  logger->trace(outMessage.str());
}

std::string Logger::formatMessage(const std::string& message, std::queue<std::string> arguments) const
{
  std::ostringstream outMessage;
  for (int i = 0; i < message.size(); i++) {
    if (message[i] == '{' && message[++i] == '}')
    {
      outMessage << arguments.front();
      arguments.pop();
    }
    else
    {
      outMessage << message[i];
    }
  }
  return outMessage.str();
}

std::queue<std::string> Logger::vaListToQueue(const int numOfArgs, va_list& args)
{
  std::queue<std::string> argsQueue;
  for (size_t i = 0; i < numOfArgs; i++)
  {
    argsQueue.push(va_arg(args, std::string));
  }
  return argsQueue;
}


void Logger::toggleLoggingPopUps()
{
  isLoggingPopUps = !isLoggingPopUps;
  std::cout << "pop ups logging " << isLoggingPopUps << std::endl;
}

void Logger::toggleLoggingLogic()
{
  isLoggingLogic = !isLoggingLogic;
  std::cout << "logic logging " << isLoggingLogic << std::endl;
}

void Logger::toggleLoggingMatrixFields()
{
  isLoggingMatrixFields = !isLoggingMatrixFields;
  std::cout << "matrix fields logging " << isLoggingMatrixFields << std::endl;
}

void Logger::toggleLoggingMouseRaw()
{
  isLoggingMouseRaw = !isLoggingMouseRaw;
  std::cout << "mouse raw logging " << isLoggingMouseRaw << std::endl;
}


// Control key definitions
Keys::Code LoggingToggle::KEY_LoggingToggle_popUps = Keys::f1;
Keys::Code LoggingToggle::KEY_LoggingToggle_logic = Keys::f2;
Keys::Code LoggingToggle::KEY_LoggingToggle_matrixFields = Keys::f3;
Keys::Code LoggingToggle::KEY_LoggingToggle_mouseMovement = Keys::f4;
Keys::Code LoggingToggle::KEY_Dummy_tutorialStep = Keys::f5; // #TUTORIAL
