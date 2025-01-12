#pragma once
/**
 * Application specific DIWNE config
 */

#include "Logger/Logger.h"

#undef DIWNE_LOG_DEBUG
#define DIWNE_LOG_DEBUG(message) LOG_DEBUG(message)
#undef DIWNE_INFO
#define DIWNE_INFO(message) LOG_INFO(message)
#undef DIWNE_WARN
#define DIWNE_WARN(message) LOG_WARN(message)
#undef DIWNE_ERROR
#define DIWNE_ERROR(message) LOG_ERROR(message)