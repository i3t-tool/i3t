#pragma once

#define RAPIDJSON_HAS_STDSTRING 1

#include <filesystem>
#include <fstream>
#include <istream>
#include <optional>

#include "rttr/type"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/schema.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "imgui.h"

#include "Core/Defs.h"
#include "Core/Nodes/NodeData.h"
#include "Core/Result.h"
#include "Logger/Logger.h"
#include "Utils/FilesystemUtils.h"
#include "glm/gtc/type_ptr.hpp"

namespace JSON
{

//////////////////////////////////////////////
// DOCUMENT SAVE/LOAD
//////////////////////////////////////////////

/**
 * Parse json file at inputPath to Document doc.
 * @return True on success, false otherwise.
 */
bool parse(const fs::path& inputPath, rapidjson::Document& doc);

/**
 * Parse json file at inputPath to Document doc.
 * Schema file located at schemaSrc is used to validate the json file.
 * @return True on success, false otherwise.
 */
bool parse(const fs::path& inputPath, rapidjson::Document& doc, const fs::path& schemaSrc);

/**
 * Parse json string to Document doc.
 * @return True on success, false otherwise.
 */
bool parseString(const std::string& jsonStr, rapidjson::Document& doc);

/**
 * Write Document into a json file at path.
 * @return True on success, false otherwise.
 */
bool save(const fs::path& path, const rapidjson::Document& document);

//////////////////////////////////////////////
// RTTR SERIALIZE/DESERIALIZE
//////////////////////////////////////////////

Result<std::string, Error> serializeToString(rttr::instance obj);
Result<Void, Error> serializeToDocument(rttr::instance obj, rapidjson::Document& doc);
Result<Void, Error> serializeToFile(rttr::instance obj, const fs::path& path);

Result<Void, Error> deserializeDocument(const rapidjson::Value& document, rttr::instance obj);
Result<Void, Error> deserializeString(const std::string& json, rttr::instance obj);
Result<Void, Error> deserializeFile(const fs::path& path, rttr::instance obj);

//////////////////////////////////////////////

// TODO: (DR) Merging should be possible without copying. Take a look.

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

//////////////////////////////////////////////

void addFloat(rapidjson::Value& target, const char* key, float value, rapidjson::Value::AllocatorType& alloc);
void addBool(rapidjson::Value& target, const char* key, bool value, rapidjson::Value::AllocatorType& alloc);
void addString(rapidjson::Value& target, const char* key, const std::string& value,
               rapidjson::Value::AllocatorType& alloc);
void addVector(rapidjson::Value& target, const char* key, const ImVec2& vec, rapidjson::Value::AllocatorType& alloc);
void addVector(rapidjson::Value& target, const char* key, const glm::vec3& vec, rapidjson::Value::AllocatorType& alloc);
void addVector(rapidjson::Value& target, const char* key, const glm::vec4& vec, rapidjson::Value::AllocatorType& alloc);
void addMatrix(rapidjson::Value& target, const char* key, const glm::mat4& mat, rapidjson::Value::AllocatorType& alloc);

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
	case Core::EValueType::Quat: // PF: is it OK this way?
	{
		if (!value.IsArray() || value.GetArray().Size() != 4)
			return std::nullopt;

		glm::vec4 v = getVec4(value); // to avoid guat(vec4) misuse as euler angles
		return Core::Data{glm::quat(v.w, glm::vec3(v))};
	}
	case Core::EValueType::Pulse:
	case Core::EValueType::MatrixMul:
	case Core::EValueType::Screen:
	case Core::EValueType::Ptr:
		break;
	}

	return std::nullopt;
}

} // namespace JSON
