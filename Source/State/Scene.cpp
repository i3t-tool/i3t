#include "Scene.h"

#include "Utils/JSON.h"

namespace State
{
bool isReadOnly(const fs::path& scene)
{
	const auto maybeScene = JSON::parse(scene, I3T_SCENE_SCHEMA);
	if (!maybeScene.has_value())
	{
		return false;
	}

	const auto& sceneJson = maybeScene.value();

	if (!sceneJson.HasMember("readOnly"))
	{
		return false;
	}

	return sceneJson["readOnly"].GetBool();
}
} // namespace State
