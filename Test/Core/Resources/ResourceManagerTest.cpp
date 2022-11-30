#include "gtest/gtest.h"

#include "Config.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Window.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "Logger/LoggerInternal.h"
#include "Utils/Color.h"
#include "Viewport/Shaper.h"

class ResourceManagerTest : public ::testing::Test
{
protected:
	Window* m_window;

	void SetUp() override
	{
#ifdef I3T_RELEASE_STANDALONE // variant for standalone release
		Config::WORKING_DIRECTORY = "";
#else // special settings for usage in Visual Studio devenv
		Config::WORKING_DIRECTORY = I3T_PROJECT_ROOT;
#endif

		int argc = 1;
		char* argv[] = {(char*)"dummy"};
		INIT_LOGGER(argc, argv);

		m_window = new Window();
		m_window->init();

		// PGR and OpenGL initializing.
		if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR,
		                     pgr::DEBUG_OFF))
		{
			Log::error("Failed to initialize opengl / pgr!");
			FAIL();
		}
	}

	void TearDown() override { delete m_window; }
};

TEST_F(ResourceManagerTest, TextureTest)
{
	std::string texture1 = "Data/textures/cube.png";
	std::string texture2 = "Data/textures/dummy.png";

	GLuint texture1Id = Core::ResourceManager::instance().texture(texture1);
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture(texture1));
	ASSERT_EQ(texture1Id,
	          Core::ResourceManager::instance().texture("Texture1", texture1));
	ASSERT_EQ(texture1Id,
	          Core::ResourceManager::instance().texture("Texture1", texture1));
	ASSERT_EQ(texture1Id, Core::ResourceManager::instance().texture(texture1));
	ASSERT_EQ(texture1Id,
	          Core::ResourceManager::instance().textureByAlias("Texture1"));

	GLuint texture2Id =
	    Core::ResourceManager::instance().texture("Texture2", texture2);
	ASSERT_NE(texture1Id, texture2Id);
	ASSERT_EQ(texture2Id,
	          Core::ResourceManager::instance().texture("Texture2", texture2));
	ASSERT_EQ(texture2Id, Core::ResourceManager::instance().texture(texture2));

	// Attempt alias change (will report error but still return resource)
	ASSERT_EQ(texture2Id, Core::ResourceManager::instance().texture(
	                          "Texture2_newAlias", texture2));
	// Fail to find resource under the "new" alias without specifying path
	ASSERT_EQ(
	    0, Core::ResourceManager::instance().textureByAlias("Texture2_newAlias"));
	// The original alias remains unchanged
	ASSERT_EQ(texture2Id,
	          Core::ResourceManager::instance().texture("Texture2", texture2));

	// Loading existing resource with a different existing alias
	ASSERT_EQ(0, Core::ResourceManager::instance().texture("Texture1", texture2));
	ASSERT_EQ(texture1Id,
	          Core::ResourceManager::instance().texture("Texture1", texture1));
}

TEST_F(ResourceManagerTest, ShaderTest)
{
	std::string shader1Vert = "Data/Shaders/phongVert.glsl";
	std::string shader1Frag = "Data/Shaders/phongFrag.glsl";

	std::string shader2Vert = "Data/Shaders/gridVert.glsl";
	std::string shader2Frag = "Data/Shaders/gridFrag.glsl";

	GLuint shader1Id =
	    Core::ResourceManager::instance().shader(shader1Vert, shader1Frag);
	ASSERT_EQ(shader1Id,
	          Core::ResourceManager::instance().shader(shader1Vert, shader1Frag));
	ASSERT_EQ(shader1Id, Core::ResourceManager::instance().shader(
	                         "Shader1", shader1Vert, shader1Frag));
	ASSERT_EQ(shader1Id,
	          Core::ResourceManager::instance().shaderByAlias("Shader1"));

	GLuint shader2Id = Core::ResourceManager::instance().shader(
	    "Shader2", shader2Vert, shader2Frag);
	ASSERT_NE(shader1Id, shader2Id);
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shader(
	                         "Shader2", shader2Vert, shader2Frag));
	ASSERT_EQ(shader2Id,
	          Core::ResourceManager::instance().shader(shader2Vert, shader2Frag));
	ASSERT_EQ(shader2Id,
	          Core::ResourceManager::instance().shaderByAlias("Shader2"));

	// Attempt alias change (will report error but still return resource)
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shader(
	                         "Shader2_newAlias", shader2Vert, shader2Frag));
	// Fail to find resource under the "new" alias without specifying path
	ASSERT_EQ(
	    0, Core::ResourceManager::instance().shaderByAlias("Shader2_newAlias"));
	// The original alias remains unchanged
	ASSERT_EQ(shader2Id, Core::ResourceManager::instance().shader(
	                         "Shader2", shader2Vert, shader2Frag));

	// Loading existing resource with a different existing alias
	ASSERT_EQ(0, Core::ResourceManager::instance().shader("Shader1", shader2Vert,
	                                                      shader2Frag));
	ASSERT_EQ(shader1Id, Core::ResourceManager::instance().shader(
	                         "Shader1", shader1Vert, shader1Frag));
}

TEST_F(ResourceManagerTest, TypeMismatchTest)
{
	std::string texture1 = "Data/textures/cube.png";
	std::string texture2 = "Data/textures/dummy.png";

	std::string shader1Vert = "Data/Shaders/phongVert.glsl";
	std::string shader1Frag = "Data/Shaders/phongFrag.glsl";

	std::string shader2Vert = "Data/Shaders/gridVert.glsl";
	std::string shader2Frag = "Data/Shaders/gridFrag.glsl";

	GLuint texture1Id =
	    Core::ResourceManager::instance().texture("Texture1", texture1);
	GLuint texture2Id =
	    Core::ResourceManager::instance().texture("Texture2", texture2);

	GLuint shader1Id = Core::ResourceManager::instance().shader(
	    "Shader1", shader1Vert, shader1Frag);
	GLuint shader2Id = Core::ResourceManager::instance().shader(
	    "Shader2", shader2Vert, shader2Frag);

	ASSERT_NE(texture1Id, texture2Id);
	ASSERT_NE(texture1Id, shader1Id);
	ASSERT_NE(shader1Id, shader2Id);

	ASSERT_EQ(0, Core::ResourceManager::instance().texture("Shader1", texture2));
	ASSERT_EQ(0, Core::ResourceManager::instance().texture("Shader2", texture2));
	ASSERT_EQ(0, Core::ResourceManager::instance().shader("Texture1", shader1Vert,
	                                                      shader1Frag));
	ASSERT_EQ(0, Core::ResourceManager::instance().shader("Texture2", shader1Vert,
	                                                      shader1Frag));

	ASSERT_EQ(0, Core::ResourceManager::instance().shaderByAlias("Texture1"));
	ASSERT_EQ(0, Core::ResourceManager::instance().shaderByAlias("Texture2"));
	ASSERT_EQ(0, Core::ResourceManager::instance().textureByAlias("Shader1"));
	ASSERT_EQ(0, Core::ResourceManager::instance().textureByAlias("Shader1"));

	ASSERT_EQ(shader1Id,
	          Core::ResourceManager::instance().shaderByAlias("Shader1"));
	ASSERT_EQ(shader2Id,
	          Core::ResourceManager::instance().shaderByAlias("Shader2"));
	ASSERT_EQ(texture1Id,
	          Core::ResourceManager::instance().textureByAlias("Texture1"));
	ASSERT_EQ(texture2Id,
	          Core::ResourceManager::instance().textureByAlias("Texture2"));
}

TEST_F(ResourceManagerTest, ModelFromFileTest)
{
	std::string metalBox = "Data/Models/box_metal.gltf";
	std::string teapot = "Data/Models/Teapot.gltf";

	Core::Mesh* metalBoxMesh = Core::ResourceManager::instance().mesh(metalBox);
	ASSERT_EQ(metalBoxMesh, Core::ResourceManager::instance().mesh(metalBox));
	ASSERT_EQ(metalBoxMesh,
	          Core::ResourceManager::instance().mesh("MetalBox", metalBox));
	ASSERT_EQ(metalBoxMesh,
	          Core::ResourceManager::instance().mesh("MetalBox", metalBox));
	ASSERT_EQ(metalBoxMesh, Core::ResourceManager::instance().mesh(metalBox));
	ASSERT_EQ(metalBoxMesh,
	          Core::ResourceManager::instance().meshByAlias("MetalBox"));

	Core::Mesh* teapotmesh =
	    Core::ResourceManager::instance().mesh("Teapot", teapot);
	ASSERT_NE(teapotmesh, metalBoxMesh);
	ASSERT_EQ(teapotmesh,
	          Core::ResourceManager::instance().mesh("Teapot", teapot));
	ASSERT_EQ(teapotmesh, Core::ResourceManager::instance().mesh(teapot));

	// Attempt alias change (will report error but still return resource)
	ASSERT_EQ(teapotmesh,
	          Core::ResourceManager::instance().mesh("Teapot_new", teapot));
	// Fail to find resource under the "new" alias without specifying path
	ASSERT_EQ(nullptr,
	          Core::ResourceManager::instance().meshByAlias("Teapot_new"));
	// The original alias remains unchanged
	ASSERT_EQ(teapotmesh,
	          Core::ResourceManager::instance().meshByAlias("Teapot"));

	// Loading existing resource with a different existing alias
	ASSERT_EQ(0, Core::ResourceManager::instance().mesh("MetalBox", teapot));
	ASSERT_EQ(metalBoxMesh,
	          Core::ResourceManager::instance().mesh("MetalBox", metalBox));
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
	ASSERT_EQ(shape1Triangles->m_primitiveType,
	          Core::Mesh::PrimitiveType::TRIANGLES);
	ASSERT_EQ(shape1Triangles->m_nVertices, 6);
	ASSERT_EQ(shape1Triangles->m_useNormals, false);
	ASSERT_EQ(shape1Triangles->m_useTexcoords, false);
	ASSERT_EQ(shape1Triangles->m_useTangents, false);
	ASSERT_EQ(shape1Triangles->m_useColors, true);

	ASSERT_NE(shape1Lines, shape1Triangles);
	ASSERT_NE(shape1Lines->m_vao, shape1Triangles->m_vao);

	ASSERT_EQ(shape1Lines,
	          Core::ResourceManager::instance().meshByAlias("Shape1Lines"));
	ASSERT_EQ(shape1Triangles,
	          Core::ResourceManager::instance().meshByAlias("Shape1Triangles"));
	ASSERT_NE(Core::ResourceManager::instance().meshByAlias("Shape1Lines"),
	          Core::ResourceManager::instance().meshByAlias("Shape1Triangles"));

	ASSERT_EQ(shape1Lines, shaper.createLineMesh("Shape1Lines"));

	// TODO: (DR) As a "temporary" measure, aliases are part of direct mesh hashes
	//	 thus the following call will create a new mesh under GIBBERISH alias,
	//	 the mesh content will be the same but with a different pointer
	// ASSERT_EQ(shape1Lines, shaper.createLineMesh("GIBBERISH"));
	// ASSERT_EQ(nullptr,
	// Core::ResourceManager::instance().meshByAlias("GIBBERISH"));
	ASSERT_NE(shape1Lines, shaper.createLineMesh("GIBBERISH"));
	ASSERT_NE(nullptr,
	          Core::ResourceManager::instance().meshByAlias("GIBBERISH"));

	ASSERT_EQ(shape1Lines,
	          Core::ResourceManager::instance().meshByAlias("Shape1Lines"));
}