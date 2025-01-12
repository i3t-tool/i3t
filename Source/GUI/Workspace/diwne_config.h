#pragma once

#include "DIWNE/diwne_include.h" // Ensure that it was already included

#include "Logger/Logger.h"

#undef DIWNE_LOG_DEBUG
#undef DIWNE_INFO
#undef DIWNE_WARN
#undef DIWNE_ERROR

#define DIWNE_LOG_DEBUG(message) LOG_DEBUG("[DIWNE Library]" message)
#define DIWNE_INFO(message) LOG_INFO("[DIWNE Library]" message)
#define DIWNE_WARN(message) LOG_WARN("[DIWNE Library]" message)
#define DIWNE_ERROR(message) LOG_ERROR("[DIWNE Library]" message)