#pragma once

#include "LoggerInternal.h"

#include "Core/Defs.h"

namespace Log
{
#ifdef I3T_DEBUG
#define I3T_DEBUG_LOG(...) std::cout << fmt::format(__VA_ARGS__) << std::endl;
#else
#define I3T_DEBUG_LOG(...)
#endif

template <typename... Args>
FORCE_INLINE void debug(const std::string& fmt, Args&&... args)
{
	// Logger::getInstance().getConsoleLogger()->trace(fmt,
	// std::forward<Args>(args)...);
	Logger::getInstance().getConsoleLogger()->info(fmt,
	                                               std::forward<Args>(args)...);
}

template <typename... Args>
FORCE_INLINE void info(const std::string& fmt, Args&&... args)
{
	Logger::getInstance().getConsoleLogger()->info(fmt,
	                                               std::forward<Args>(args)...);
}

template <typename... Args>
FORCE_INLINE void error(const std::string& fmt, Args&&... args)
{
	Logger::getInstance().getConsoleLogger()->error(fmt,
	                                                std::forward<Args>(args)...);
}

template <typename... Args>
FORCE_INLINE void fatal(const std::string& fmt, Args&&... args)
{
	Logger::getInstance().getConsoleLogger()->critical(
	    fmt, std::forward<Args>(args)...);
}
} // namespace Log
