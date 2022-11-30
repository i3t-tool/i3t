#include "World.h"
#include "Components.h"
#include "HardcodedMeshes.h"
#include "Transforms.h"

#include "Config.h"
//#include "Source/GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"
//#include "Source/GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include "Core/Input/InputManager.h"
#include "Core/Nodes/GraphManager.h"
#include "GUI/Elements/Nodes/WorkspaceTransformation.h"

//
#include "sceneGraph/ShaderProgram.h"

#include "GUI/Elements/Windows/WorkspaceWindow.h"
//

#include <string.h>

glm::mat4 World::perspective =
    glm::mat4(1.0f); //< main camera perspective matrix
glm::mat4 World::mainCamera =
    glm::mat4(1.0f); //< most probably the main camera viewMatrix
glm::vec3 World::World::mainCamPos = glm::vec3(0.0f);
float World::height = 10;
float World::width = 10;
bool World::initializedRender = false;

Shader World::shader0;      ///< Default shader
Shader World::shaderHandle; ///< Handle shader
Shader World::shaderProj;   ///< preview projection matrices

std::map<std::string, GLuint> World::textures;
// std::map<std::string, pgr::MeshData > World::models;

World::World()
{
	if (!World::initializedRender)
	{
		printf("initialize render before creating World!\n");
	}
	this->started = false;

	TranslationManipulator* tm = new TranslationManipulator();
	ScaleManipulator* sm = new ScaleManipulator();
	LookAtManipulator* lm = new LookAtManipulator();
	OrthoManipulator* om = new OrthoManipulator();
	PerspectiveManipulator* pm = new PerspectiveManipulator();
	FrustumManipulator* fm = new FrustumManipulator();
	RotationManipulator* rm = new RotationManipulator();
	FreeManipulator* mm = new FreeManipulator();
	this->manipulators.emplace("Translation",
	                           Manipulator(&tm->m_editednode, &tm->m_parent, tm));
	this->manipulators.emplace("Scale",
	                           Manipulator(&sm->m_editednode, &sm->m_parent, sm));
	this->manipulators.emplace("LookAt",
	                           Manipulator(&lm->m_editednode, &lm->m_parent, lm));
	this->manipulators.emplace("Ortho",
	                           Manipulator(&om->m_editednode, &om->m_parent, om));
	this->manipulators.emplace("Perspective",
	                           Manipulator(&pm->m_editednode, &pm->m_parent, pm));
	this->manipulators.emplace("Frustum",
	                           Manipulator(&fm->m_editednode, &fm->m_parent, fm));
	this->manipulators.emplace("EulerX",
	                           Manipulator(&rm->m_editednode, &rm->m_parent, rm));
	this->manipulators.emplace("EulerY",
	                           Manipulator(&rm->m_editednode, &rm->m_parent, rm));
	this->manipulators.emplace("EulerZ",
	                           Manipulator(&rm->m_editednode, &rm->m_parent, rm));
	this->manipulators.emplace("AxisAngle",
	                           Manipulator(&rm->m_editednode, &rm->m_parent, rm));
	// this->manipulators.emplace("Quat",
	// Manipulator(&rm->m_editednode,&rm->m_parent,rm));
	this->manipulators.emplace("Free",
	                           Manipulator(&mm->m_editednode, &mm->m_parent, mm));
	GameObject* sceneHandles = new GameObject();

	for (std::map<std::string, Manipulator>::const_iterator i =
	         this->manipulators.cbegin();
	     i != this->manipulators.cend(); i++)
	{
		if (sceneHandles->getComponent(i->second.component->getComponentType()) ==
		    nullptr)
		{ // add each manipulator only once (rotation is stored multiple times under
			// different keys)
			i->second.component->m_isActive = false;
			sceneHandles->addComponent(i->second.component);
		}
	}

	this->sceneRoot = new GameObject(gridMesh, &World::shader0, 0);
	this->sceneRoot->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->sceneRoot->addComponent(new Renderer(Renderer::DRAW_LINES));

	GameObject* camera = new GameObject();
	camera->transform(glm::vec3(0.0f, 5.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f),
	                  glm::vec3(1.0f, 0.0f, 0.0f), -30.0f);
	camera->addComponent(new Camera(60.0f, this->sceneRoot));
	camera->addComponent(new CameraControl());
	this->camControl =
	    (CameraControl*)camera->getComponent(CameraControl::componentType());

	this->sceneRoot->addChild(camera, false);
	this->sceneRoot->addChild(sceneHandles, false);
}

bool World::init()
{
	World::shader0 = loadShader(
	    Config::getAbsolutePath("Data/Shaders/simple-vs.glsl").c_str(),
	    Config::getAbsolutePath("Data/Shaders/simple-fs.glsl").c_str());
	World::shaderHandle = loadShader(
	    Config::getAbsolutePath("Data/Shaders/handle-vs.glsl").c_str(),
	    Config::getAbsolutePath("Data/Shaders/handle-fs.glsl").c_str());
	World::shaderProj = loadShader(
	    Config::getAbsolutePath("Data/Shaders/viewproj-vs.glsl").c_str(),
	    Config::getAbsolutePath("Data/Shaders/viewproj-fs.glsl").c_str());

	if (World::shader0.program * World::shaderHandle.program *
	        World::shaderProj.program * World::shaderProj.program ==
	    0)
	{
		printf("World::init():cannot load shaders\n");
		return false;
	}

	World::textures.emplace("cube", pgr::createTexture(Config::getAbsolutePath(
	                                    "Data/textures/cube.png")));
	World::textures.emplace("cube_color",
	                        pgr::createTexture(Config::getAbsolutePath(
	                            "Data/textures/cube_color.png")));
	World::textures.emplace(
	    "color_grid",
	    pgr::createTexture(Config::getAbsolutePath("Data/textures/cGrid.png")));
	World::textures.emplace("axis", pgr::createTexture(Config::getAbsolutePath(
	                                    "Data/textures/axis.png")));
	World::textures.emplace("white", pgr::createTexture(Config::getAbsolutePath(
	                                     "Data/textures/white.png")));

	CHECK_GL_ERROR();
	World::initializedRender = true;

	return true;
}
void World::end()
{
	pgr::deleteProgramAndShaders(World::shader0.program);
	pgr::deleteProgramAndShaders(World::shaderHandle.program);
	pgr::deleteProgramAndShaders(World::shaderProj.program);

	for (std::map<std::string, GLuint>::const_iterator i =
	         World::textures.cbegin();
	     i != World::textures.cend(); i++)
	{
		glDeleteTextures(1, &(i->second));
	}
}

void GUIRecursive(GameObject* root)
{
	for (int i = 0; i < root->children.size(); i++)
	{
		GUIRecursive(root->children[i]);
	}
	for (int i = 0; i < root->components.size(); i++)
	{
		if (root->components[i]->m_isActive)
		{
			root->components[i]->GUI();
		}
	}
}
void updateRecursive(GameObject* root)
{
	for (int i = 0; i < root->children.size(); i++)
	{
		updateRecursive(root->children[i]);
	}
	for (int i = 0; i < root->components.size(); i++)
	{
		if (root->components[i]->m_isActive)
		{
			root->components[i]->update();
		}
	}
}
void startRecursive(GameObject* root)
{
	for (int i = 0; i < root->components.size(); i++)
	{
		if (root->components[i]->m_isActive)
		{
			root->components[i]->start();
		}
	}
	for (int i = 0; i < root->children.size(); i++)
	{
		startRecursive(root->children[i]);
	}
}
CameraControl* World::getCameraControl() { return camControl; }
void World::sceneSetView(glm::vec3 dir, bool world)
{
	this->camControl->setRotation(dir, world);
}
void World::sceneZoom(float val) { camControl->setScroll(val); }
void World::manipulatorsSetMatrix(
    std::shared_ptr<WorkspaceTransformation> matnode,
    std::shared_ptr<Core::Sequence> parent)
{
	// printf("manipulatorsSetMatrix 0x%p,0x%p\n",matnode,parent);
	if (activeManipulator != nullptr)
	{
		activeManipulator->component->m_isActive = false;
		activeManipulator = nullptr;
	}
	if (matnode == nullptr)
	{
		return;
	}
	// if(matnode->get()==nullptr){return;}

	Ptr<Core::NodeBase> nodebase = matnode->getNodebase();
	const Core::Transform::DataMap* data = nodebase->getDataMap(); // printf("a");
	const Operation* operation = nodebase->getOperation();         // printf("b");
	const char* keyword =
	    nodebase->getOperation()->keyWord.c_str(); // printf("c");

	if (this->manipulators.count(keyword) == 1)
	{
		activeManipulator = &(this->manipulators[keyword]);
		if (showManipulators)
		{
			activeManipulator->component->m_isActive = true;
		}
		*(activeManipulator->editedNode) = nodebase;
		if (parent != nullptr)
		{ /*if(parent->get()!=nullptr){*/
			(activeManipulator->parent) = &parent;
		} //} /* \todo maybe repaire pointers? */
	}
	// else{printf("No manipulators\n"); }
	// printf("operation %s\n",keyword);
}
void World::manipulatorsSetVisible(bool visible)
{
	showManipulators = visible;

	if (activeManipulator != nullptr)
	{
		activeManipulator->component->m_isActive = visible;
	}
}
bool World::manipulatorsGetVisible() { return showManipulators; }
GameObject* World::addModel(const char* name)
{
	GameObject* g = nullptr;
	bool lines = false;
	if (strcmp("CubeGray", name) == 0)
	{
		g = new GameObject(unitcubeMesh, &World::shader0, World::textures["cube"]);
	}
	else if (strcmp("CubeColor", name) == 0)
	{
		g = new GameObject(unitcubeMesh, &World::shader0,
		                   World::textures["cube_color"]);
	}
	else if (strcmp("CubeColorGrid", name) == 0)
	{
		g = new GameObject(unitcubeMesh, &World::shader0,
		                   World::textures["color_grid"]);
	}
	else if (strcmp("PlainAxis", name) == 0)
	{
		g = new GameObject(three_axisMesh, &World::shader0,
		                   World::textures["axis"]);
	}
	else if (strstr("Grid", name) == name)
	{
		g = new GameObject(gridMesh, &World::shader0, 0);
		g->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		g->scale(glm::vec3(0.125f));
		lines = true;
		if (strcmp("GridXY", name) == 0)
		{
			g->rotate(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
		}
		else if (strcmp("GridYZ", name) == 0)
		{
			g->rotate(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
		}
	}
	if (g != nullptr)
	{
		g->addComponent(new Renderer(Renderer::DRAW_LINES * lines));
		this->sceneRoot->addChild(g, true);
	}
	return g;
}
bool World::removeModel(GameObject* g)
{
	/// \todo PF, MH - scene root is not defined.
	if (sceneRoot == nullptr)
		return false;

	return this->sceneRoot->rmChild(g, true);
}
void World::onStart()
{
	this->started = true;
	startRecursive(this->sceneRoot);
}

void World::onUpdate()
{
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	// printf("%f %f %f %f\n", viewport[0], viewport[1], viewport[2],
	// viewport[3]);
	World::width = viewport[2];
	World::height = viewport[3];
	if (!this->started)
	{
		printf("World:call start() before update()!\n");
		return;
	}
	updateRecursive(this->sceneRoot);

	CHECK_GL_ERROR();

	/// \todo New approach
	/*
	for (const auto& node : g_workspaceDiwne->m_workspaceCoreNodes)
	{
	  const auto maybeModel = std::dynamic_pointer_cast<WorkspaceModel>(node);
	  if (maybeModel == nullptr)
	  {
	    continue;
	  }

	  const auto modelNode = (*maybeModel->getNodebase()).as<Core::Model>();
	  modelNode->mesh()->draw(World::mainCamera, World::perspective);
	}
	 */
}

void World::onGUI() { GUIRecursive(this->sceneRoot); }

Shader World::loadShader(const char* vs_name, const char* fs_name)
{
	Shader shader = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	GLuint gl_shader = 0;
	GLuint shaders[] = {pgr::createShaderFromFile(GL_VERTEX_SHADER, vs_name),
	                    pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fs_name),
	                    0};
	if (shaders[0] * shaders[1] == 0)
	{
		return shader;
	}

	shader.program = pgr::createProgram(shaders);
	shader.attr_pos = glGetAttribLocation(shader.program, "position");
	shader.attr_norm = glGetAttribLocation(
	    shader.program, "norm"); // printf("%d", this->attr_norm);
	shader.attr_uv = glGetAttribLocation(shader.program, "uv");
	shader.camera = glGetUniformLocation(shader.program, "camera");
	shader.color = glGetUniformLocation(shader.program, "color");
	shader.Mmatrix = glGetUniformLocation(shader.program, "Mmatrix");
	shader.PVMmatrix = glGetUniformLocation(shader.program, "PVMmatrix");
	shader.VNmatrix = glGetUniformLocation(shader.program, "VNmatrix");
	return shader;
}
