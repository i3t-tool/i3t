#include "Scene.h"

#include "Utils/JSON.h"

namespace State
{
bool isReadOnly(const fs::path& scene)
{
	rapidjson::Document doc;
	if (!JSON::parse(scene, doc, I3T_SCENE_SCHEMA))
	{
		return false;
	}

	if (!doc.HasMember("readOnly"))
	{
		return false;
	}

	return doc["readOnly"].GetBool();
}
} // namespace State
