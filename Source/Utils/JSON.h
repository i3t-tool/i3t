#pragma once

#define RAPIDJSON_HAS_STDSTRING 1

#include <filesystem>
#include <fstream>
#include <istream>
#include <optional>

#include "imgui.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/schema.h"
#include "rttr/type"

#include "Core/Defs.h"
#include "Core/Nodes/NodeData.h"
#include "Core/Result.h"
#include "Logger/Logger.h"
#include "Utils/FilesystemUtils.h"

namespace JSON
{
std::optional<rapidjson::Document> parse(const fs::path& inputSrc);

std::optional<rapidjson::Document> parse(const fs::path& inputSrc, const fs::path& schemaSrc);

bool save(const fs::path& path, const rapidjson::Document& document);

inline ImVec2 getVec2(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsArray(), "Invalid type");

	ImVec2 result;

	result.x = value[0].GetFloat();
	result.y = value[1].GetFloat();

	return result;
}

inline glm::vec3 getVec3(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsArray(), "Invalid type");

	glm::vec3 result;

	result.x = value[0].GetFloat();
	result.y = value[1].GetFloat();
	result.z = value[2].GetFloat();

	return result;
}

inline glm::vec4 getVec4(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsArray(), "Invalid type");

	glm::vec4 result;

	result.x = value[0].GetFloat();
	result.y = value[1].GetFloat();
	result.z = value[2].GetFloat();
	result.w = value[3].GetFloat();

	return result;
}

inline glm::mat4 getMat(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsArray(), "Invalid type");

	glm::mat4 result;

	for (int i = 0; i < 4; ++i)
	{
		const auto& column = value[i].GetArray();

		result[i] = {column[0].GetFloat(), column[1].GetFloat(), column[2].GetFloat(), column[3].GetFloat()};
	}

	return result;
}

//

inline std::optional<Core::Data> getData(const rapidjson::Value& value, Core::EValueType dataType)
{
	switch (dataType)
	{
	case Core::EValueType::Float:
	{
		if (!value.IsFloat())
			return std::nullopt;

		return Core::Data{value.GetFloat()};
	}
	case Core::EValueType::Vec3:
	{
		if (!value.IsArray() || value.GetArray().Size() != 3)
			return std::nullopt;

		return Core::Data{getVec3(value)};
	}
	case Core::EValueType::Vec4:
	{
		if (!value.IsArray() || value.GetArray().Size() != 4)
			return std::nullopt;

		return Core::Data{getVec4(value)};
	}
	case Core::EValueType::Matrix:
	{
		if (!value.IsArray() || value.GetArray().Size() != 4)
			return std::nullopt;

		return Core::Data{getMat(value)};
	}
	case Core::EValueType::Quat:
	case Core::EValueType::Pulse:
	case Core::EValueType::MatrixMul:
	case Core::EValueType::Screen:
	case Core::EValueType::Ptr:
		break;
	}

	return std::nullopt;
}

Result<std::string, Error> serialize(rttr::instance obj);

Result<Void, Error> serialize(rttr::instance obj, const fs::path& path);

Result<Void, Error> deserialize(const std::string& json, rttr::instance obj);

Result<Void, Error> deserialize(rapidjson::Value& document, rttr::instance obj);

Result<Void, Error> deserialize(const fs::path& path, rttr::instance obj);

/// Don't know how to implement this without copying...
/*
inline void merge(rapidjson::Value& target, rapidjson::Value& source,
                  rapidjson::Value::AllocatorType& allocator)
{
  // Not working
  assert(target.IsObject());
  assert(source.IsObject());

for (auto itr = source.MemberBegin(); itr != source.MemberEnd(); ++itr)
{
    const char* str = itr->name.GetString();

    target.AddMember(itr->name, std::move(itr->value), allocator);
}
}
*/
bool merge(rapidjson::Value& dstObject, rapidjson::Value& srcObject, rapidjson::Document::AllocatorType& allocator);
} // namespace JSON
