#include <filesystem>

#include "rttr/registration.h"
#include "rttr/type.h"

#include "glm/glm.hpp"

#include "Utils/JSON.h"

RTTR_REGISTRATION
{
	// std::filesystem::path
	rttr::type::register_converter_func([](const std::string& str, bool& ok) -> std::filesystem::path {
		ok = true;
		return std::filesystem::path(str);
	});
	rttr::type::register_converter_func([](const std::filesystem::path& path, bool& ok) -> std::string {
		ok = true;
		return path.string();
	});

	// glm::vec2
	rttr::type::register_converter_func([](const glm::vec2& vec, bool& ok) -> std::string {
		ok = true;
		return JSON::serializeVec2(vec);
	});
	rttr::type::register_converter_func([](const std::string& str, bool& ok) -> glm::vec2 {
		bool error;
		glm::vec2 v = JSON::deserializeVec2(str, &error);
		ok = !error;
		return v;
	});

	// glm::vec3
	rttr::type::register_converter_func([](const glm::vec3& vec, bool& ok) -> std::string {
		ok = true;
		return JSON::serializeVec3(vec);
	});
	rttr::type::register_converter_func([](const std::string& str, bool& ok) -> glm::vec3 {
		bool error;
		glm::vec3 v = JSON::deserializeVec3(str, &error);
		ok = !error;
		return v;
	});

	// glm::vec4
	rttr::type::register_converter_func([](const glm::vec4& vec, bool& ok) -> std::string {
		ok = true;
		return JSON::serializeVec4(vec);
	});
	rttr::type::register_converter_func([](const std::string& str, bool& ok) -> glm::vec4 {
		bool error;
		glm::vec4 v = JSON::deserializeVec4(str, &error);
		ok = !error;
		return v;
	});

	// glm::mat4
	rttr::type::register_converter_func([](const glm::mat4& mat, bool& ok) -> std::string {
		ok = true;
		return JSON::serializeMat4(mat);
	});
	rttr::type::register_converter_func([](const std::string& str, bool& ok) -> glm::mat4 {
		bool error;
		glm::mat4 mat = JSON::deserializeMat4(str, &error);
		ok = !error;
		return mat;
	});
};
