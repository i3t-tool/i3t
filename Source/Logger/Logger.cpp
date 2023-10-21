/**
 * \file
 * \brief
 * \author Filip Uhlík
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Logger/Logger.h"

#include <sstream>

#ifdef GetObject
#undef GetObject
#endif

#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"

#include "Config.h"
#include "Core/Window.h"
/// \todo Test JustPressed keys in Application and set set logging options there.
#include "Core/Input/InputManager.h"
#include "Utils/JSON.h"

#define DEFAULT_LOG_PATTERN "[%H:%M:%S] [%l]: %v"

struct LoggingToggle final
{
	static Keys::Code KEY_LoggingToggle_popUps;
	static Keys::Code KEY_LoggingToggle_logic;
	static Keys::Code KEY_LoggingToggle_matrixFields;
	static Keys::Code KEY_LoggingToggle_mouseMovement;
	static Keys::Code KEY_Dummy_tutorialStep; // #TUTORIAL
};

Logger::Logger()
{
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	sinks[0]->set_level(spdlog::level::trace);
	sinks[0]->set_pattern(DEFAULT_LOG_PATTERN);

	appLogger = std::make_shared<spdlog::logger>("app_logger", sinks.begin(), sinks.end());
	logger = std::make_shared<spdlog::logger>("basic_logger", sinks.begin(), sinks.end());
	mouseLogger = std::make_shared<spdlog::logger>("mouse_logger", sinks.begin(), sinks.end());
}

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
	else
	{
		return "String not found!";
	}
}

void Logger::initLogger(int argc, char* argv[])
{
	loadStrings();

	spdlog::set_pattern(DEFAULT_LOG_PATTERN);

	// Console sink.
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	// sinks.push_back(std::make_shared<spdlog::sinks::ostream_sink_st>(m_buffer));
	sinks[0]->set_level(spdlog::level::trace);
	sinks[0]->set_pattern(DEFAULT_LOG_PATTERN);

	constexpr auto maxFileCount = 1;
	constexpr auto maxLogSize = 1024 * 1024 * 50; // 50 MB

	appLogger = std::make_shared<spdlog::logger>("app_logger", sinks.begin(), sinks.end());
	appLogger->sinks().push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(Configuration::appLog.string(),
	                                                                                    maxLogSize, maxFileCount));

	logger = std::make_shared<spdlog::logger>("basic_logger", sinks.begin(), sinks.end());
	logger->sinks().push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
	    Configuration::userInteractionLog.string(), maxLogSize, maxFileCount));
	logger->set_level(spdlog::level::trace);

	mouseLogger = std::make_shared<spdlog::logger>("mouse_logger", sinks.begin(), sinks.end());
	mouseLogger->sinks().push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
	    Configuration::mouseLog.string(), maxLogSize, maxFileCount));
	mouseLogger->set_level(spdlog::level::trace);

	std::ostringstream init_message;
	init_message << " >> Logger strings version: " << getLogString("version") << " <<";

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
	if (InputManager::isKeyJustPressed(LoggingToggle::KEY_LoggingToggle_popUps))
	{
		toggleLoggingPopUps();
	}
	if (InputManager::isKeyJustPressed(LoggingToggle::KEY_LoggingToggle_logic))
	{
		toggleLoggingLogic();
	}
	if (InputManager::isKeyJustPressed(LoggingToggle::KEY_LoggingToggle_matrixFields))
	{
		toggleLoggingMatrixFields();
	}
	if (InputManager::isKeyJustPressed(LoggingToggle::KEY_LoggingToggle_mouseMovement))
	{
		toggleLoggingMouseRaw();
	}
	// #TUTORIAL
	if (InputManager::isKeyJustPressed(LoggingToggle::KEY_Dummy_tutorialStep))
	{
		if (stepCount > 4)
		{
			tutorialCount++;
			stepCount = 1;
		}
		LOG_EVENT_TUTORIAL_STEP(std::to_string(tutorialCount), std::to_string(stepCount),
		                        std::string("Dummy tutorial step"));
		stepCount++;
	}
}

void Logger::addToLogBuffer(const LoggingOption& logType, const std::string& message,
                            const std::vector<std::string>& args)
{
	if (logType == LoggingOption::MOUSE_MOVEMENT && isLoggingMouseRaw)
	{
		if (shouldLogMouse())
		{
			logBuffer.push(formatMessage(message, args));
		}
	}
	else if (logType == LoggingOption::MOUSE_CLICK && isLoggingMouseRaw ||
	         logType == LoggingOption::LOGIC && isLoggingLogic || logType == LoggingOption::POP_UP && isLoggingPopUps ||
	         logType == LoggingOption::MATRIX_FIELD && isLoggingMatrixFields || logType == LoggingOption::TUTORIAL)
	{
		logBuffer.push(formatMessage(message, args));
	}
}

bool Logger::shouldLogMouse()
{
	// Current time in milliseconds.
	// const int currentTime = glutGet(GLUT_ELAPSED_TIME);
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
	std::cout << "[info]: Load strings from: " << Configuration::logEventsDefinition << std::endl;
	rapidjson::Document doc;
	if (!JSON::parse(Configuration::logEventsDefinition, doc))
	{
		std::cerr << "[error]: Failed to load logger config from " << Configuration::logEventsDefinition << std::endl;
		return;
	}

	for (const auto& [name, value] : doc.GetObject())
	{
		logStrings[std::string(name.GetString())] = std::string(value.GetString());
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
	logger->flush();
}

std::string Logger::formatMessage(const std::string& message, const std::vector<std::string>& args) const
{
	std::ostringstream outMessage;
	int nextArgIdx = 0;

	for (int i = 0; i < message.size(); i++)
	{
		if (message[i] == '{' && message[++i] == '}')
		{
			outMessage << args[nextArgIdx];

			if (nextArgIdx < args.size() - 1)
				++nextArgIdx;
		}
		else
		{
			outMessage << message[i];
		}
	}
	return outMessage.str();
}

void Logger::toggleLoggingPopUps()
{
	isLoggingPopUps = !isLoggingPopUps;
	LOG_INFO("pop ups logging {}", isLoggingPopUps);
}

void Logger::toggleLoggingLogic()
{
	isLoggingLogic = !isLoggingLogic;
	LOG_INFO("logic logging {}", isLoggingLogic);
}

void Logger::toggleLoggingMatrixFields()
{
	isLoggingMatrixFields = !isLoggingMatrixFields;
	LOG_INFO("matrix fields logging {}", isLoggingMatrixFields);
}

void Logger::toggleLoggingMouseRaw()
{
	isLoggingMouseRaw = !isLoggingMouseRaw;
	LOG_INFO("mouse raw logging {}", isLoggingMouseRaw);
}

// Control key definitions
Keys::Code LoggingToggle::KEY_LoggingToggle_popUps = Keys::f1;
Keys::Code LoggingToggle::KEY_LoggingToggle_logic = Keys::f2;
Keys::Code LoggingToggle::KEY_LoggingToggle_matrixFields = Keys::f3;
Keys::Code LoggingToggle::KEY_LoggingToggle_mouseMovement = Keys::f4;
Keys::Code LoggingToggle::KEY_Dummy_tutorialStep = Keys::f5; // #TUTORIAL
