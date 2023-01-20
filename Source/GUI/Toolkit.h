#pragma once

#include <string>

#include "imgui.h"

namespace GUI
{
bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
               void* user_data);
}
