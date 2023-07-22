#include <filesystem>

#include "rttr/registration.h"
#include "rttr/type.h"

RTTR_REGISTRATION
{
	rttr::type::register_converter_func([](const std::string& str, bool& result) -> std::filesystem::path {
		result = true;
		return std::filesystem::path(str);
	});

	rttr::type::register_converter_func([](const std::filesystem::path& path, bool& result) -> std::string {
		result = true;
		return path.string();
	});
};
