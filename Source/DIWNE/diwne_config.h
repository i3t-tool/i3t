#pragma once
/**
 * Application specific DIWNE configuration file
 * Much like ImGui's imconfig.h, this file can be edited directly
 * or the '#define DIWNE_USER_CONFIG "my_diwne_config.h"' macro can be defined before including the library.
 */

#include <iostream>

#undef DIWNE_LOG_DEBUG
#define DIWNE_LOG_DEBUG(...) std::cout << "[DIWNE DEBUG] " << __VA_ARGS__ << std::endl
#undef DIWNE_TRACE
#define DIWNE_TRACE(...) std::cout << "[DIWNE TRACE] " << __VA_ARGS__ << std::endl
#undef DIWNE_INFO
#define DIWNE_INFO(...) std::cout << "[DIWNE INFO] " << __VA_ARGS__ << std::endl
#undef DIWNE_WARN
#define DIWNE_WARN(...) std::cout << "[DIWNE WARN] " << __VA_ARGS__ << std::endl
#undef DIWNE_ERROR
#define DIWNE_ERROR(...) std::cerr << "[DIWNE ERROR] " << __VA_ARGS__ << std::endl