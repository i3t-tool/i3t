/**
 * \file
 * \brief
 * \authors Martin Herich <martin.herich@phire.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Utils/JSON.h"

#include "rttr/type"

#include "Utils/Format.h"

namespace JSONRTTR
{
using namespace rapidjson;
using namespace rttr;

/////////////////////////////////////////////////////////////////////////////////////////

void to_json_recursively(const instance& obj, PrettyWriter<StringBuffer>& writer);

/////////////////////////////////////////////////////////////////////////////////////////

bool write_variant(const variant& var, PrettyWriter<StringBuffer>& writer);

bool write_atomic_types_to_json(const type& t, const variant& var, PrettyWriter<StringBuffer>& writer)
{
	if (t.is_arithmetic())
	{
		if (t == type::get<bool>())
			writer.Bool(var.to_bool());
		else if (t == type::get<char>())
			writer.Bool(var.to_bool());
		else if (t == type::get<int8_t>())
			writer.Int(var.to_int8());
		else if (t == type::get<int16_t>())
			writer.Int(var.to_int16());
		else if (t == type::get<int32_t>())
			writer.Int(var.to_int32());
		else if (t == type::get<int64_t>())
			writer.Int64(var.to_int64());
		else if (t == type::get<uint8_t>())
			writer.Uint(var.to_uint8());
		else if (t == type::get<uint16_t>())
			writer.Uint(var.to_uint16());
		else if (t == type::get<uint32_t>())
			writer.Uint(var.to_uint32());
		else if (t == type::get<uint64_t>())
			writer.Uint64(var.to_uint64());
		else if (t == type::get<float>())
			writer.Double(var.to_double());
		else if (t == type::get<double>())
			writer.Double(var.to_double());

		return true;
	}
	else if (t.is_enumeration())
	{
		bool ok = false;
		auto result = var.to_string(&ok);
		if (ok)
		{
			writer.String(var.to_string());
		}
		else
		{
			ok = false;
			auto value = var.to_uint64(&ok);
			if (ok)
				writer.Uint64(value);
			else
				writer.Null();
		}

		return true;
	}
	else if (var.can_convert<std::string>())
	{
		auto value = var.to_string();
		if (t == type::get<fs::path>())
		{
			value = Utils::toString(value);
		}

		writer.String(value);
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

static void write_array(const variant_sequential_view& view, PrettyWriter<StringBuffer>& writer)
{
	writer.StartArray();
	for (const auto& item : view)
	{
		if (item.is_sequential_container())
		{
			write_array(item.create_sequential_view(), writer);
		}
		else
		{
			variant wrapped_var = item.extract_wrapped_value();
			type value_type = wrapped_var.get_type();

			if (value_type.is_arithmetic() || wrapped_var.can_convert<std::string>() || value_type.is_enumeration())
			{
				write_atomic_types_to_json(value_type, wrapped_var, writer);
			}
			else // object
			{
				to_json_recursively(wrapped_var, writer);
			}
		}
	}
	writer.EndArray();
}


/////////////////////////////////////////////////////////////////////////////////////////

static void write_associative_container(const variant_associative_view& view, PrettyWriter<StringBuffer>& writer)
{
	static const string_view key_name("key");
	static const string_view value_name("value");

	writer.StartArray();

	if (view.is_key_only_type())
	{
		for (auto& item : view)
		{
			write_variant(item.first, writer);
		}
	}
	else
	{
		for (auto& item : view)
		{
			writer.StartObject();
			writer.String(key_name.data(), static_cast<rapidjson::SizeType>(key_name.length()), false);

			write_variant(item.first, writer);

			writer.String(value_name.data(), static_cast<rapidjson::SizeType>(value_name.length()), false);

			write_variant(item.second, writer);

			writer.EndObject();
		}
	}

	writer.EndArray();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool write_variant(const variant& var, PrettyWriter<StringBuffer>& writer)
{
	auto value_type = var.get_type();
	auto wrapped_type = value_type.is_wrapper() ? value_type.get_wrapped_type() : value_type;
	bool is_wrapper = wrapped_type != value_type;

	if (write_atomic_types_to_json(is_wrapper ? wrapped_type : value_type,
	                               is_wrapper ? var.extract_wrapped_value() : var, writer))
	{}
	else if (var.is_sequential_container())
	{
		write_array(var.create_sequential_view(), writer);
	}
	else if (var.is_associative_container())
	{
		write_associative_container(var.create_associative_view(), writer);
	}
	else
	{
		auto child_props = is_wrapper ? wrapped_type.get_properties() : value_type.get_properties();
		if (!child_props.empty())
		{
			to_json_recursively(var, writer);
		}
		else
		{
			// Var is an unknown type that isn't directly convertible to a string
			// (we already tried in write_atomic_types_to_json())
			bool ok = false;

			// Try convert to std::vector<float>
			variant vecVar;
			if (var.can_convert<std::vector<float>>())
			{
				vecVar = var.convert<std::vector<float>>(&ok);
			}
			if (ok)
			{
				// Success
				write_variant(vecVar, writer);
			}
			else
			{
				// Try convert to string
				auto text = var.to_string(&ok);
				if (!ok)
				{
					writer.String(text);
					return false;
				}

				writer.String(text);
			}
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

void to_json_recursively(const instance& obj2, PrettyWriter<StringBuffer>& writer)
{
	writer.StartObject();
	instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;

	auto prop_list = obj.get_derived_type().get_properties();
	for (auto prop : prop_list)
	{
		if (prop.get_metadata("NO_SERIALIZE"))
			continue;

		variant prop_value = prop.get_value(obj);
		if (!prop_value)
			continue; // cannot serialize, because we cannot retrieve the value

		const auto name = prop.get_name();
		writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length()), false);
		if (!write_variant(prop_value, writer))
		{
			std::cerr << "cannot serialize property: " << name << std::endl;
		}
	}

	writer.EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////

void fromjson_recursively(instance obj, const Value& json_object);

/////////////////////////////////////////////////////////////////////////////////////////

variant extract_basic_types(const Value& json_value)
{
	switch (json_value.GetType())
	{
	case kStringType:
	{
		return std::string(json_value.GetString());
		break;
	}
	case kNullType:
		break;
	case kFalseType:
	case kTrueType:
	{
		return json_value.GetBool();
		break;
	}
	case kNumberType:
	{
		if (json_value.IsInt())
			return json_value.GetInt();
		else if (json_value.IsDouble())
			return json_value.GetDouble();
		else if (json_value.IsUint())
			return json_value.GetUint();
		else if (json_value.IsInt64())
			return json_value.GetInt64();
		else if (json_value.IsUint64())
			return json_value.GetUint64();
		break;
	}
	// we handle only the basic types here
	case kObjectType:
	case kArrayType:
		return variant();
	}

	return variant();
}


/////////////////////////////////////////////////////////////////////////////////////////

static void write_array_recursively(variant_sequential_view& view, const Value& json_array_value)
{
	view.set_size(json_array_value.Size());
	const type array_value_type = view.get_rank_type(1);

	for (SizeType i = 0; i < json_array_value.Size(); ++i)
	{
		auto& json_index_value = json_array_value[i];
		if (json_index_value.IsArray())
		{
			auto sub_array_view = view.get_value(i).create_sequential_view();
			write_array_recursively(sub_array_view, json_index_value);
		}
		else if (json_index_value.IsObject())
		{
			variant var_tmp = view.get_value(i);
			variant wrapped_var = var_tmp.extract_wrapped_value();
			fromjson_recursively(wrapped_var, json_index_value);
			view.set_value(i, wrapped_var);
		}
		else
		{
			variant extracted_value = extract_basic_types(json_index_value);
			if (extracted_value.convert(array_value_type))
				view.set_value(i, extracted_value);
		}
	}
}

variant extract_value(Value::ConstMemberIterator& itr, const type& t)
{
	auto& json_value = itr->value;
	variant extracted_value = extract_basic_types(json_value);
	const bool could_convert = extracted_value.convert(t);
	if (!could_convert)
	{
		if (json_value.IsObject())
		{
			constructor ctor = t.get_constructor();
			for (auto& item : t.get_constructors())
			{
				if (item.get_instantiated_type() == t)
					ctor = item;
			}
			extracted_value = ctor.invoke();
			fromjson_recursively(extracted_value, json_value);
		}
	}

	return extracted_value;
}

static void write_associative_view_recursively(variant_associative_view& view, const Value& json_array_value)
{
	for (SizeType i = 0; i < json_array_value.Size(); ++i)
	{
		auto& json_index_value = json_array_value[i];
		if (json_index_value.IsObject()) // a key-value associative view
		{
			Value::ConstMemberIterator key_itr = json_index_value.FindMember("key");
			Value::ConstMemberIterator value_itr = json_index_value.FindMember("value");

			if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd())
			{
				auto key_var = extract_value(key_itr, view.get_key_type());
				auto value_var = extract_value(value_itr, view.get_value_type());
				if (key_var && value_var)
				{
					view.insert(key_var, value_var);
				}
			}
		}
		else // a key-only associative view
		{
			variant extracted_value = extract_basic_types(json_index_value);
			if (extracted_value && extracted_value.convert(view.get_key_type()))
				view.insert(extracted_value);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

void fromjson_recursively(instance obj2, const Value& json_object)
{
	instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;
	const auto prop_list = obj.get_derived_type().get_properties();

	for (auto prop : prop_list)
	{
		Value::ConstMemberIterator ret = json_object.FindMember(prop.get_name().data());
		if (ret == json_object.MemberEnd())
			continue;
		const type value_t = prop.get_type();

		auto& json_value = ret->value;
		switch (json_value.GetType())
		{
		case kArrayType:
		{
			variant var = prop.get_value(obj);

			// Check if property type is convertible from std::vector<float> and vice versa
			variant vecVar = std::vector<float>();
			if (vecVar.can_convert(value_t) && var.can_convert(vecVar.get_type()))
			{
				// Load data into an empty float vector
				auto seqView = vecVar.create_sequential_view();
				write_array_recursively(seqView, json_value);

				// Convert float vector into the native type
				if (vecVar.convert(value_t))
				{
					prop.set_value(obj, vecVar);
					break;
				}
				LOG_ERROR("Failed to convert float array to {}!", std::string(value_t.get_name()));
			}

			if (value_t.is_sequential_container())
			{
				var = prop.get_value(obj);
				auto view = var.create_sequential_view();
				write_array_recursively(view, json_value);
			}
			else if (value_t.is_associative_container())
			{
				var = prop.get_value(obj);
				auto associative_view = var.create_associative_view();
				write_associative_view_recursively(associative_view, json_value);
			}

			prop.set_value(obj, var);
			break;
		}
		case kObjectType:
		{
			variant var = prop.get_value(obj);
			fromjson_recursively(var, json_value);
			prop.set_value(obj, var);
			break;
		}
		default:
		{
			variant extracted_value = extract_basic_types(json_value);
			if (extracted_value.convert(
			        value_t)) // REMARK: CONVERSION WORKS ONLY WITH "const type", check whether this is correct or not!
				prop.set_value(obj, extracted_value);
		}
		}
	}
}
} // namespace JSONRTTR

namespace JSON
{
using namespace rapidjson;
using namespace rttr;

bool parse(const fs::path& inputPath, rapidjson::Document& doc)
{
	std::ifstream file(inputPath);
	if (!file.good())
	{
		LOG_ERROR("Cannot open json file '{}'!", inputPath.string());
		return false;
	}

	rapidjson::IStreamWrapper inputStreamWrapper(file);

	const auto hasError = doc.ParseStream(inputStreamWrapper).HasParseError();
	if (hasError)
	{
		LOG_ERROR("Cannot parse json file '{}'!", inputPath.string());
		return false;
	}

	return true;
}

bool parse(const fs::path& inputPath, rapidjson::Document& doc, const fs::path& schemaSrc)
{
	std::ifstream schemaFile(schemaSrc);
	I3T_ASSERT(schemaFile.good(), fmt::format("Cannot open schema file {}: {},\n   working directory is {}",
	                                          schemaSrc.string(), strerror(errno), fs::current_path().string()));

	rapidjson::IStreamWrapper schemaInputStreamWrapper(schemaFile);
	auto hasError = doc.ParseStream(schemaInputStreamWrapper).HasParseError();
	I3T_ASSERT(!hasError, "Cannot parse schema file!");

	rapidjson::SchemaDocument schema(doc);
	rapidjson::SchemaValidator validator(schema);

	if (!parse(inputPath, doc))
	{
		return false;
	}

	if (!doc.Accept(validator))
	{
		rapidjson::StringBuffer sb;
		validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
		LOG_ERROR("Invalid schema: {}", sb.GetString());
		LOG_ERROR("Invalid keyword: {}", validator.GetInvalidSchemaKeyword());
		sb.Clear();
		validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
		LOG_ERROR("Invalid document: {}", sb.GetString());
	}
	return true;
}

bool parseString(const std::string& jsonStr, rapidjson::Document& doc)
{
	if (doc.Parse(jsonStr).HasParseError())
	{
		LOG_ERROR("Cannot parse json string! String:\n{}", jsonStr);
		return false;
	}
	return true;
}

bool save(const fs::path& path, const rapidjson::Document& document)
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

Result<std::string, Error> serializeToString(rttr::instance obj)
{
	if (!obj.is_valid())
	{
		return Err("invalid reflected object to serialize");
	}

	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	JSONRTTR::to_json_recursively(obj, writer);

	return sb.GetString();
}

// TODO: (DR) Should be rewritten to use RTTR to create a rapidjson::Document directly and avoid parsing json again
//   Martin pls fix someday :).
Result<Void, Error> serializeToDocument(rttr::instance obj, rapidjson::Document& doc)
{
	if (!obj.is_valid())
	{
		return Err("invalid reflected object to serialize");
	}

	auto jsonStr = JSON::serializeToString(obj);
	if (!jsonStr)
	{
		return Err(jsonStr.error());
	}

	doc.SetObject();

	// Parse them again (cause they get serialized in a string not a doc //TODO: Fix)
	bool success = JSON::parseString(jsonStr.value(), doc);
	if (!success)
	{
		return Err("Failed to parse serialized object json!");
	}
	return Void{};
}

Result<Void, Error> serializeToFile(rttr::instance obj, const fs::path& path)
{
	auto result = serializeToString(obj);
	if (!result)
	{
		return Err(result.error());
	}

	try
	{
		std::ofstream file(path);
		file << result.value();
	}
	catch (...)
	{
		return Err("unable to save serialized JSON to file " + path.string() + "!");
	}

	return Void{};
}

Result<Void, Error> deserializeDocument(const rapidjson::Value& document, rttr::instance obj)
{
#ifdef GetObject
#undef GetObject
#endif

	JSONRTTR::fromjson_recursively(obj, document);

	return Void{};
}

Result<Void, Error> deserializeString(const std::string& json, rttr::instance obj)
{
	Document document; // Default template parameter uses UTF8 and MemoryPoolAllocator.

	// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
	if (document.Parse(json.c_str()).HasParseError())
	{
		return Err("unable to parse JSON");
	}

	JSONRTTR::fromjson_recursively(obj, document);

	return deserializeDocument(document, obj);
}

Result<Void, Error> deserializeFile(const fs::path& path, rttr::instance obj)
{
	rapidjson::Document doc;
	if (parse(path, doc))
	{
		return deserializeDocument(doc, obj);
	}

	return Err("unable to open JSON file " + path.string());
}

bool merge(rapidjson::Value& dstObject, rapidjson::Value& srcObject, rapidjson::Document::AllocatorType& allocator)
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

void addFloat(rapidjson::Value& target, const char* key, float value, Value::AllocatorType& alloc)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");
	target.AddMember(rapidjson::Value(key, alloc).Move(), rapidjson::Value(value), alloc);
}

void addBool(rapidjson::Value& target, const char* key, bool value, Value::AllocatorType& alloc)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");
	target.AddMember(rapidjson::Value(key, alloc).Move(), rapidjson::Value(value), alloc);
}

void addString(rapidjson::Value& target, const char* key, const std::string& value, Value::AllocatorType& alloc)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");
	target.AddMember(rapidjson::Value(key, alloc).Move(), rapidjson::Value(value.c_str(), alloc).Move(), alloc);
}

void addRect(rapidjson::Value& target, const char* key, const ImRect& rect, Value::AllocatorType& alloc)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");

	rapidjson::Value object(kObjectType);
	addVector(object, "Min", rect.Min, alloc);
	addVector(object, "Max", rect.Max, alloc);

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(object), alloc);
}

void addVector(rapidjson::Value& target, const char* key, const ImVec2& vec, Value::AllocatorType& alloc)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");

	rapidjson::Value vector(kArrayType);
	vector.PushBack(vec.x, alloc);
	vector.PushBack(vec.y, alloc);

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(vector), alloc);
}

void addVector(rapidjson::Value& target, const char* key, const glm::vec3& vec, Value::AllocatorType& alloc)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");

	rapidjson::Value vector(kArrayType);
	vector.PushBack(vec.x, alloc);
	vector.PushBack(vec.y, alloc);
	vector.PushBack(vec.z, alloc);

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(vector), alloc);
}

void addVector(rapidjson::Value& target, const char* key, const glm::vec4& vec, Value::AllocatorType& alloc)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");

	rapidjson::Value vector(kArrayType);
	vector.PushBack(vec.x, alloc);
	vector.PushBack(vec.y, alloc);
	vector.PushBack(vec.z, alloc);
	vector.PushBack(vec.w, alloc);

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(vector), alloc);
}

void addMatrix(rapidjson::Value& target, const char* key, const glm::mat4& mat, Value::AllocatorType& alloc)
{
	I3T_ASSERT(target.IsObject(), "Invalid value type");

	rapidjson::Value matrix(kArrayType);

	for (int i = 0; i < 4; ++i)
	{
		const auto& vec = mat[i];

		rapidjson::Value column(kArrayType);
		column.PushBack(vec.x, alloc);
		column.PushBack(vec.y, alloc);
		column.PushBack(vec.z, alloc);
		column.PushBack(vec.w, alloc);

		matrix.PushBack(column.Move(), alloc);
	}

	target.AddMember(rapidjson::Value(key, alloc).Move(), std::move(matrix), alloc);
}

ImVec2 getVec2(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsArray(), "Invalid type");

	ImVec2 result;

	result.x = value[0].GetFloat();
	result.y = value[1].GetFloat();

	return result;
}

ImRect getRect(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsObject(), "Invalid type");

	ImRect result;

	result.Min = getVec2(value["Min"]);
	result.Max = getVec2(value["Max"]);

	return result;
}

glm::vec3 getVec3(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsArray(), "Invalid type");

	glm::vec3 result;

	result.x = value[0].GetFloat();
	result.y = value[1].GetFloat();
	result.z = value[2].GetFloat();

	return result;
}

glm::vec4 getVec4(const rapidjson::Value& value)
{
	I3T_ASSERT(value.IsArray(), "Invalid type");

	glm::vec4 result;

	result.x = value[0].GetFloat();
	result.y = value[1].GetFloat();
	result.z = value[2].GetFloat();
	result.w = value[3].GetFloat();

	return result;
}

glm::mat4 getMat(const rapidjson::Value& value)
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

std::optional<Core::Data> getData(const rapidjson::Value& value, Core::EValueType dataType)
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
