#pragma once

#include <filesystem>
#include <fstream>
#include <istream>
#include <optional>

#include "imgui.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/schema.h"

#include "Core/Defs.h"
#include "Core/Nodes/NodeData.h"
#include "Logger/Logger.h"
#include "Utils/Filesystem.h"
#include "rapidjson/prettywriter.h"

namespace JSON
{
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

/// Don't know how to implement this without copying...
inline bool merge(rapidjson::Value& dstObject, rapidjson::Value& srcObject,
                  rapidjson::Document::AllocatorType& allocator)
{
	for (auto srcIt = srcObject.MemberBegin(); srcIt != srcObject.MemberEnd(); ++srcIt)
	{
		auto dstIt = dstObject.FindMember(srcIt->name);
		if (dstIt == dstObject.MemberEnd())
		{
			rapidjson::Value dstName;
			dstName.CopyFrom(srcIt->name, allocator);
			rapidjson::Value dstVal;
			dstVal.CopyFrom(srcIt->value, allocator);

			dstObject.AddMember(dstName, dstVal, allocator);

			dstName.CopyFrom(srcIt->name, allocator);
			dstIt = dstObject.FindMember(dstName);
			if (dstIt == dstObject.MemberEnd())
				return false;
		}
		else
		{
			auto srcT = srcIt->value.GetType();
			auto dstT = dstIt->value.GetType();
			if (srcT != dstT)
				return false;

			if (srcIt->value.IsArray())
			{
				for (auto arrayIt = srcIt->value.Begin(); arrayIt != srcIt->value.End(); ++arrayIt)
				{
					rapidjson::Value dstVal;
					dstVal.CopyFrom(*arrayIt, allocator);
					dstIt->value.PushBack(dstVal, allocator);
				}
			}
			else if (srcIt->value.IsObject())
			{
				if (!merge(dstIt->value, srcIt->value, allocator))
					return false;
			}
			else
			{
				dstIt->value.CopyFrom(srcIt->value, allocator);
			}
		}
	}

	return true;
}

inline std::optional<rapidjson::Document> parse(const fs::path& inputSrc)
{
	std::ifstream file(inputSrc);
	if (!file.good())
	{
		LOG_ERROR("Cannot open input file \"{}\"!", inputSrc.string());
		return std::nullopt;
	}

	rapidjson::IStreamWrapper inputStreamWrapper(file);

	rapidjson::Document document;
	const auto hasError = document.ParseStream(inputStreamWrapper).HasParseError();
	if (hasError)
	{
		LOG_ERROR("Cannot parse config file!");
		return std::nullopt;
	}

	return document;
}

inline std::optional<rapidjson::Document> parse(const fs::path& inputSrc, const fs::path& schemaSrc)
{
	std::ifstream schemaFile(schemaSrc);
	I3T_ASSERT(schemaFile.good(), "Cannot open schema file!");

	rapidjson::Document schemaDocument;

	rapidjson::IStreamWrapper schemaInputStreamWrapper(schemaFile);
	auto hasError = schemaDocument.ParseStream(schemaInputStreamWrapper).HasParseError();
	I3T_ASSERT(!hasError, "Cannot parse schema file!");

	rapidjson::SchemaDocument schema(schemaDocument);
	rapidjson::SchemaValidator validator(schema);

	auto maybeDocument = parse(inputSrc);
	if (!maybeDocument.has_value())
	{
		return std::nullopt;
	}
	auto document = std::move(maybeDocument.value());

	if (!document.Accept(validator))
	{
		rapidjson::StringBuffer sb;
		validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
		LOG_ERROR("Invalid schema: {}", sb.GetString());
		LOG_ERROR("Invalid keyword: {}", validator.GetInvalidSchemaKeyword());
		sb.Clear();
		validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
		LOG_ERROR("Invalid document: {}", sb.GetString());
	}

	return document;
}

inline bool save(const fs::path& path, const rapidjson::Document& document)
{
	try
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		document.Accept(writer);

		const char* data = buffer.GetString();

		std::ofstream file(path);
		file << data;
	}
	catch (...)
	{
		return false;
	}

	return true;
}

inline std::string toString(const rapidjson::Document& document)
{
	try
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		document.Accept(writer);

		const char* data = buffer.GetString();

		return std::string(data);
	}
	catch (...)
	{
	}

	return "";
}

//

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

inline std::optional<DataStore> getData(const rapidjson::Value& value, EValueType dataType)
{
	switch (dataType)
	{
	case EValueType::Float:
	{
		if (!value.IsFloat())
			return std::nullopt;

		return DataStore{value.GetFloat()};
	}
	case EValueType::Vec3:
	{
		if (!value.IsArray() || value.GetArray().Size() != 3)
			return std::nullopt;

		return DataStore{getVec3(value)};
	}
	case EValueType::Vec4:
	{
		if (!value.IsArray() || value.GetArray().Size() != 4)
			return std::nullopt;

		return DataStore{getVec4(value)};
	}
	case EValueType::Matrix:
	{
		if (!value.IsArray() || value.GetArray().Size() != 4)
			return std::nullopt;

		return DataStore{getMat(value)};
	}
	case EValueType::Quat:
	case EValueType::Pulse:
	case EValueType::MatrixMul:
	case EValueType::Screen:
	case EValueType::Ptr:
		break;
	}

	return std::nullopt;
}
} // namespace JSON
