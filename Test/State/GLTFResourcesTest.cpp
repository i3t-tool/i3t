#include "gtest/gtest.h"

#include <array>
#include <filesystem>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include "Config.h"
#include "Core/Application.h"
#include "Core/Resources/ResourceManager.h"

#include "Common.h"

using namespace std::string_literals;

const std::array g_DefaultModelNames = {
    "Bunny"s, "Cube"s, "Duck"s, "Suzanne"s, "Teapot"s,
};

TEST(GLTFResourcesTest, AssimpCanImportGLTFFiles)
{
	const auto defaultModelsDir =
	    std::filesystem::path(I3T_PROJECT_ROOT "/Data/Models");

	for (const auto& defaultFile : g_DefaultModelNames)
	{
		Assimp::Importer importer;
		const auto* scene = importer.ReadFile(
		    (defaultModelsDir / (defaultFile + ".gltf")).string().c_str(), 0);

		if (scene == nullptr)
		{
			std::cerr << importer.GetErrorString() << std::endl;
		}
		EXPECT_TRUE(scene != nullptr);
	}
}

TEST(GLTFResourcesTest, DefaultModelAreImported)
{
	using namespace Core;

	createTestApplication();

	const auto& defaultModels = ResourceManager::instance().defaultModels();

	ASSERT_TRUE(defaultModels.size() == g_DefaultModelNames.size());

	int i = 0;
	for (const auto& [key, model] : defaultModels)
	{
		EXPECT_TRUE(key == g_DefaultModelNames[i]);
		++i;
	}

	destroyTestApplication();
}
