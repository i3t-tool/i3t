#include "gtest/gtest.h"

#include <array>
#include <filesystem>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include "Config.h"
#include "Core/Application.h"
#include "I3T.h"

// #include "Common.h"
#include "Core/Resources/ResourceManager.h"

using namespace std::string_literals;

const std::array g_DefaultModelNames = {
    "Bunny"s, "Cube"s, "Duck"s, "Suzanne"s, "Teapot"s,
};

TEST(GLTFResourcesTest, AssimpCanImportGLTFFiles)
{
	const auto defaultModelsDir = std::filesystem::path("Data/Models");

	for (const auto& defaultFile : g_DefaultModelNames)
	{
		Assimp::Importer importer;
		const auto* scene = importer.ReadFile((defaultModelsDir / (defaultFile + ".gltf")).string().c_str(), 0);

		if (scene == nullptr)
		{
			std::cerr << importer.GetErrorString() << std::endl;
		}
		EXPECT_TRUE(scene != nullptr);
	}
}

// won't pass
TEST(GLTFResourcesTest, DISABLED_DefaultModelAreImported)
{
	using namespace Core;

	I3TApplication app;
	app.init();

	const auto& defaultModels = ResourceManager::instance().getDefaultResources(ResourceType::Model);

	ASSERT_TRUE(defaultModels.size() == g_DefaultModelNames.size());

	for (const auto& name : g_DefaultModelNames)
	{
		for (auto& resource : defaultModels)
		{
			if (resource.alias == name)
			{
				goto outerloop;
			}
		}
		FAIL() << "One of default objects was not loaded";
	outerloop:;
	}
}
