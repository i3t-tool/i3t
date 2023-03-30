#include "gtest/gtest.h"

#include "Common.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Window.h"
#include "Logger/Logger.h"
#include "Utils/Color.h"
#include "Viewport/Shaper.h"

class ResourceManagerTest : public ::testing::Test
{
protected:
	void SetUp() override { createTestApplication(false); }

	void TearDown() override { destroyTestApplication(); }
};

TEST_F(ResourceManagerTest, TextureTest)
{
	std::string texture1 = "Data/textures/cube.png";
	std::string texture2 = "Data/textures/dummy.png";

	GLuint texture1Id = Core::ResourceManager::instance().texture(texture1);
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture(texture1));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture(texture1));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture(texture1));

	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture(texture1));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture("Texture1", texture1));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture("Texture1", texture1));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture(texture1));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().textureByAlias("Texture1"));

	GLuint texture2Id = Core::ResourceManager::instance().texture("Texture2", texture2);
	ASSERT_NE(texture1Id, texture2Id);
	ASSERT_EQ(texture2Id, Core::ResourceManager::instance().texture("Texture2", texture2));
	ASSERT_EQ(texture2Id, Core::ResourceManager::instance().texture(texture2));

	// Alternative alias
	ASSERT_EQ(texture2Id, Core::ResourceManager::instance().texture("Texture2_newAlias", texture2));
	// Find resource under the new alternative alias without specifying path
	ASSERT_EQ(texture2Id, Core::ResourceManager::instance().textureByAlias("Texture2_newAlias"));
	// The original alias remains unchanged
	ASSERT_EQ(texture2Id, Core::ResourceManager::instance().texture("Texture2", texture2));

	// Loading existing resource with a different existing alias
	ASSERT_EQ(0, Core::ResourceManager::instance().texture("Texture1", texture2));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture("Texture1", texture1));
}

TEST_F(ResourceManagerTest, ShaderTest)
{
	std::string shader1Vert = "Data/Shaders/phongVert.glsl";
	std::string shader1Frag = "Data/Shaders/phongFrag.glsl";

	std::string shader2Vert = "Data/Shaders/gridVert.glsl";
	std::string shader2Frag = "Data/Shaders/gridFrag.glsl";

	GLuint shader1Id = Core::ResourceManager::instance().shader(shader1Vert, shader1Frag);
	ASSERT_EQ(shader1Id, Core::ResourceManager::instance().shader(shader1Vert, shader1Frag));
	ASSERT_EQ(shader1Id, Core::ResourceManager::instance().shader("Shader1", shader1Vert, shader1Frag));
	ASSERT_EQ(shader1Id, Core::ResourceManager::instance().shaderByAlias("Shader1"));

	GLuint shader2Id = Core::ResourceManager::instance().shader("Shader2", shader2Vert, shader2Frag);
	ASSERT_NE(shader1Id, shader2Id);
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shader("Shader2", shader2Vert, shader2Frag));
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shader(shader2Vert, shader2Frag));
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shaderByAlias("Shader2"));

	// Attempt alias change (will report error but still return resource)
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shader("Shader2_newAlias", shader2Vert, shader2Frag));
	// Find resource under the new alternative alias without specifying path
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shaderByAlias("Shader2_newAlias"));
	// The original alias remains unchanged
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shader("Shader2", shader2Vert, shader2Frag));

	// Loading existing resource with a different existing alias
	ASSERT_EQ(0, Core::ResourceManager::instance().shader("Shader1", shader2Vert, shader2Frag));
	ASSERT_EQ(shader1Id, Core::ResourceManager::instance().shader("Shader1", shader1Vert, shader1Frag));
}

TEST_F(ResourceManagerTest, TypeMismatchTest)
{
	std::string texture1 = "Data/textures/cube.png";
	std::string texture2 = "Data/textures/dummy.png";

	std::string shader1Vert = "Data/Shaders/phongVert.glsl";
	std::string shader1Frag = "Data/Shaders/phongFrag.glsl";

	std::string shader2Vert = "Data/Shaders/gridVert.glsl";
	std::string shader2Frag = "Data/Shaders/gridFrag.glsl";

	GLuint texture1Id = Core::ResourceManager::instance().texture("Texture1", texture1);
	GLuint texture2Id = Core::ResourceManager::instance().texture("Texture2", texture2);

	GLuint shader1Id = Core::ResourceManager::instance().shader("Shader1", shader1Vert, shader1Frag);
	GLuint shader2Id = Core::ResourceManager::instance().shader("Shader2", shader2Vert, shader2Frag);

	ASSERT_NE(texture1Id, texture2Id);
	ASSERT_NE(texture1Id, shader1Id);
	ASSERT_NE(shader1Id, shader2Id);

	ASSERT_EQ(0, Core::ResourceManager::instance().texture("Shader1", texture2));
	ASSERT_EQ(0, Core::ResourceManager::instance().texture("Shader2", texture2));
	ASSERT_EQ(0, Core::ResourceManager::instance().shader("Texture1", shader1Vert, shader1Frag));
	ASSERT_EQ(0, Core::ResourceManager::instance().shader("Texture2", shader1Vert, shader1Frag));

	ASSERT_EQ(0, Core::ResourceManager::instance().shaderByAlias("Texture1"));
	ASSERT_EQ(0, Core::ResourceManager::instance().shaderByAlias("Texture2"));
	ASSERT_EQ(0, Core::ResourceManager::instance().textureByAlias("Shader1"));
	ASSERT_EQ(0, Core::ResourceManager::instance().textureByAlias("Shader1"));

	ASSERT_EQ(shader1Id, Core::ResourceManager::instance().shaderByAlias("Shader1"));
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shaderByAlias("Shader2"));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().textureByAlias("Texture1"));
	ASSERT_EQ(texture2Id, Core::ResourceManager::instance().textureByAlias("Texture2"));
}

TEST_F(ResourceManagerTest, ModelFromFileTest)
{
	std::string metalBox = "Data/Models/box_metal.gltf";
	std::string teapot = "Data/Models/Teapot.gltf";

	Core::Mesh* metalBoxMesh = Core::ResourceManager::instance().mesh(metalBox);
	ASSERT_EQ(metalBoxMesh, Core::ResourceManager::instance().mesh(metalBox));
	ASSERT_EQ(metalBoxMesh, Core::ResourceManager::instance().mesh("MetalBox", metalBox));
	ASSERT_EQ(metalBoxMesh, Core::ResourceManager::instance().mesh("MetalBox", metalBox));
	ASSERT_EQ(metalBoxMesh, Core::ResourceManager::instance().mesh(metalBox));
	ASSERT_EQ(metalBoxMesh, Core::ResourceManager::instance().meshByAlias("MetalBox"));

	Core::Mesh* teapotmesh = Core::ResourceManager::instance().mesh("Teapot", teapot);
	ASSERT_NE(teapotmesh, metalBoxMesh);
	ASSERT_EQ(teapotmesh, Core::ResourceManager::instance().mesh("Teapot", teapot));
	ASSERT_EQ(teapotmesh, Core::ResourceManager::instance().mesh(teapot));

	// Attempt alias change (will report error but still return resource)
	ASSERT_EQ(teapotmesh, Core::ResourceManager::instance().mesh("Teapot_new", teapot));
	// Find resource under the new alternative alias without specifying path
	ASSERT_EQ(teapotmesh, Core::ResourceManager::instance().meshByAlias("Teapot_new"));
	// The original alias remains unchanged
	ASSERT_EQ(teapotmesh, Core::ResourceManager::instance().meshByAlias("Teapot"));

	// Loading existing resource with a different existing alias
	ASSERT_EQ(0, Core::ResourceManager::instance().mesh("MetalBox", teapot));
	ASSERT_EQ(metalBoxMesh, Core::ResourceManager::instance().mesh("MetalBox", metalBox));
}

TEST_F(ResourceManagerTest, ModelFromDataTest)
{
	Shaper shaper;
	glm::vec3 pos = glm::vec3(0, 0, 0);
	float size = 1.0f;
	shaper.line(pos, pos + glm::vec3(1.0f, 0.0f, 0.0f) * size, Color::RED);
	shaper.line(pos, pos + glm::vec3(0.0f, 1.0f, 0.0f) * size, Color::GREEN);
	shaper.line(pos, pos + glm::vec3(0.0f, 0.0f, 1.0f) * size, Color::BLUE);

	Core::Mesh* shape1Lines = shaper.createLineMesh("Shape1Lines");
	ASSERT_EQ(shape1Lines->m_drawType, Core::Mesh::DrawType::ARRAYS);
	ASSERT_EQ(shape1Lines->m_primitiveType, Core::Mesh::PrimitiveType::LINES);
	ASSERT_EQ(shape1Lines->m_nVertices, 6);
	ASSERT_EQ(shape1Lines->m_useNormals, false);
	ASSERT_EQ(shape1Lines->m_useTexcoords, false);
	ASSERT_EQ(shape1Lines->m_useTangents, false);
	ASSERT_EQ(shape1Lines->m_useColors, true);

	Core::Mesh* shape1Triangles = shaper.createMesh("Shape1Triangles");
	ASSERT_EQ(shape1Triangles->m_drawType, Core::Mesh::DrawType::ARRAYS);
	ASSERT_EQ(shape1Triangles->m_primitiveType, Core::Mesh::PrimitiveType::TRIANGLES);
	ASSERT_EQ(shape1Triangles->m_nVertices, 6);
	ASSERT_EQ(shape1Triangles->m_useNormals, false);
	ASSERT_EQ(shape1Triangles->m_useTexcoords, false);
	ASSERT_EQ(shape1Triangles->m_useTangents, false);
	ASSERT_EQ(shape1Triangles->m_useColors, true);

	ASSERT_NE(shape1Lines, shape1Triangles);
	ASSERT_NE(shape1Lines->m_vao, shape1Triangles->m_vao);

	ASSERT_EQ(shape1Lines, Core::ResourceManager::instance().meshByAlias("Shape1Lines"));
	ASSERT_EQ(shape1Triangles, Core::ResourceManager::instance().meshByAlias("Shape1Triangles"));
	ASSERT_NE(Core::ResourceManager::instance().meshByAlias("Shape1Lines"),
	          Core::ResourceManager::instance().meshByAlias("Shape1Triangles"));

	ASSERT_EQ(shape1Lines, shaper.createLineMesh("Shape1Lines"));
	ASSERT_EQ(shape1Lines, shaper.createLineMesh("GIBBERISH"));
	ASSERT_EQ(shape1Lines, Core::ResourceManager::instance().meshByAlias("GIBBERISH"));
	ASSERT_EQ(shape1Lines, Core::ResourceManager::instance().meshByAlias("GIBBERISH"));
	ASSERT_EQ(shape1Lines, Core::ResourceManager::instance().meshByAlias("Shape1Lines"));
}

TEST_F(ResourceManagerTest, DisposeTest)
{
	std::string box = "Data/Models/box_metal.gltf";
	std::string teapot = "Data/Models/Teapot.gltf";
	std::string bunny = "Data/Models/Bunny.gltf";

	Core::Mesh* boxMesh = Core::ResourceManager::instance().mesh(box);
	ASSERT_EQ(boxMesh, Core::ResourceManager::instance().mesh("Box box", box));
	ASSERT_EQ(boxMesh, Core::ResourceManager::instance().mesh("Second Box", box));
	ASSERT_EQ(boxMesh, Core::ResourceManager::instance().mesh("Third box even", box));

	Core::Mesh* teapotMesh = Core::ResourceManager::instance().mesh("Teapot", teapot);
	ASSERT_EQ(teapotMesh, Core::ResourceManager::instance().mesh("Teapot2", teapot));
	Core::ResourceManager::instance().registerDefault("Teapot");
	bool found = false;
	for (const auto& res : Core::ResourceManager::instance().getDefaultResources(Core::ResourceType::Model))
	{
		if (res.alias == "Teapot")
		{
			found = true;
			break;
		}
	}
	ASSERT_TRUE(found);

	Core::Mesh* bunnyMesh = Core::ResourceManager::instance().mesh("bUnny", bunny);

	Core::ResourceManager::instance().dispose(teapotMesh);
	ASSERT_EQ(nullptr, Core::ResourceManager::instance().meshByAlias("Teapot2"));
	for (const auto& res : Core::ResourceManager::instance().getDefaultResources(Core::ResourceType::Model))
	{
		ASSERT_NE(res.alias, "Teapot");
	}

	Core::ResourceManager::instance().dispose(bunnyMesh);
	ASSERT_EQ(nullptr, Core::ResourceManager::instance().meshByAlias("bUnny"));

	Core::ResourceManager::instance().dispose(boxMesh);
	ASSERT_EQ(nullptr, Core::ResourceManager::instance().meshByAlias("Second Box"));
	ASSERT_EQ(nullptr, Core::ResourceManager::instance().meshByAlias("Third box even"));
	Core::Mesh* newBox = Core::ResourceManager::instance().mesh("New box", box);
	ASSERT_NE(nullptr, newBox);
	ASSERT_EQ(newBox, Core::ResourceManager::instance().meshByAlias("New box"));
	Core::ResourceManager::instance().dispose(newBox);
	ASSERT_EQ(nullptr, Core::ResourceManager::instance().meshByAlias("New box"));
}