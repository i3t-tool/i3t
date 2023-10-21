/**
 * \file
 * \brief
 * \authors Martin Herich, Dan Rakušan
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
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
	rttr::type::register_converter_func([](const glm::vec2& v, bool& result) -> std::vector<float> {
		result = true;
		return {v.x, v.y};
	});

	rttr::type::register_converter_func([](const std::vector<float>& v, bool& result) -> glm::vec2 {
		result = true;
		return {v[0], v[1]};
	});

	// glm::vec3
	rttr::type::register_converter_func([](const glm::vec3& v, bool& result) -> std::vector<float> {
		result = true;
		return {v.x, v.y, v.z};
	});

	rttr::type::register_converter_func([](const std::vector<float>& v, bool& result) -> glm::vec3 {
		result = true;
		return {v[0], v[1], v[2]};
	});

	// glm::vec4
	rttr::type::register_converter_func([](const glm::vec4& v, bool& result) -> std::vector<float> {
		result = true;
		return {v.x, v.y, v.z, v.w};
	});

	rttr::type::register_converter_func([](const std::vector<float>& v, bool& result) -> glm::vec4 {
		result = true;
		return {v[0], v[1], v[2], v[3]};
	});

	// glm::mat4
	rttr::type::register_converter_func([](const glm::mat4& mat, bool& result) -> std::vector<float> {
		result = true;
		const float* v = glm::value_ptr(mat);
		return {v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]};
	});

	rttr::type::register_converter_func([](const std::vector<float>& v, bool& result) -> glm::mat4 {
		result = true;
		return {{v[0], v[1], v[2], v[3]},
		        {v[4], v[5], v[6], v[7]},
		        {v[8], v[9], v[10], v[11]},
		        {v[12], v[13], v[14], v[15]}};
	});
};
