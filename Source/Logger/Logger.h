#pragma once

#include "LoggerInternal.h"

namespace Log
{
template <typename... Args>
FORCE_INLINE void debug(const std::string& fmt, Args&&... args)
{
  Logger::getInstance().getConsoleLogger()->trace(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
FORCE_INLINE void info(const std::string& fmt, Args&&... args)
{
  Logger::getInstance().getConsoleLogger()->info(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
FORCE_INLINE void fatal(const std::string& fmt, Args&&... args)
{
  Logger::getInstance().getConsoleLogger()->critical(fmt, std::forward<Args>(args)...);
}
}
