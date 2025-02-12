#pragma once
/**
 * Application specific DIWNE config
 */

#include "Logger/Logger.h"

#undef DIWNE_LOG_DEBUG
#define DIWNE_LOG_DEBUG(...) LOG_DEBUG(__VA_ARGS__)
#undef DIWNE_INFO
#define DIWNE_INFO(...) LOG_INFO(__VA_ARGS__)
#undef DIWNE_WARN
#define DIWNE_WARN(...) LOG_WARN(__VA_ARGS__)
#undef DIWNE_ERROR
#define DIWNE_ERROR(...) LOG_ERROR(__VA_ARGS__)