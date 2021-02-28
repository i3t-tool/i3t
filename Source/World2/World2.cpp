#include "World2.h"
#include "HardcodedMeshes.h"
#include "Components.h"

#include "Config.h"

GameObject* test=NULL;

glm::mat4 World2::perspective = glm::mat4(1.0f);
glm::mat4 World2::mainCamera = glm::mat4(1.0f);
glm::vec3 World2::World2::mainCamPos = glm::vec3(0.0f);
float World2::height = 10;
float World2::width = 10;
bool World2::initializedRender = false;

GLuint World2::shader0; ///< Default shader
GLuint World2::shaderTerrain; ///< Terrain shader
GLuint World2::shaderSky;     ///< Skybox shader
GLuint World2::shaderHandle;  ///< Handle shader
GLuint World2::shaderTest;  ///< Handle shader
// GLint World2::shaderHandleColor;///<Handle shader color attribute
GLuint World2::shaderProj; ///< preview projection matrices

World2*World2::tmpAccess=NULL;

World2::World2(){
}
bool World2::initRender(){
  World2::shaderSky =       loadShader(Config::getAbsolutePath("/Data/shaders2/skybox-vs.glsl").c_str(), Config::getAbsolutePath("/Data/shaders2/skybox-fs.glsl").c_str()); 
  World2::shader0 =         loadShader(Config::getAbsolutePath("/Data/shaders2/simple-vs.glsl").c_str(), Config::getAbsolutePath("/Data/shaders2/simple-fs.glsl").c_str()); 
  World2::shaderTerrain =   loadShader(Config::getAbsolutePath("/Data/shaders2/simple-vs.glsl").c_str(), Config::getAbsolutePath("/Data/shaders2/terrain-fs.glsl").c_str()); 
  World2::shaderHandle =    loadShader(Config::getAbsolutePath("/Data/shaders2/handle-vs.glsl").c_str(), Config::getAbsolutePath("/Data/shaders2/handle-fs.glsl").c_str()); 
  World2::shaderProj =      loadShader(Config::getAbsolutePath("/Data/shaders2/viewproj-vs.glsl").c_str(),Config::getAbsolutePath("/Data/shaders2/viewproj-fs.glsl").c_str());
  World2::shaderTest =      loadShader(Config::getAbsolutePath("/Data/shaders2/test-vs.glsl").c_str(),   Config::getAbsolutePath("/Data/shaders2/test-fs.glsl").c_str());
  if (World2::shaderSky * World2::shader0 * World2::shaderTerrain * World2::shaderHandle *World2::shaderProj * World2::shaderProj*World2::shaderTest == 0){
    getchar();printf("World2::initRender():cannot load shaders\n");return false;
  }

  CHECK_GL_ERROR();
  World2::initializedRender = true;

  test = new GameObject(unitquadMesh, World2::shaderTest, 0);
  return true;
}
World2* World2::loadDefaultScene(){
  if (!World2::initializedRender){printf("initialize render before meshes!\n");return false;}
  GLuint rocksTexture, grassTexture, grassBigTexture, skyboxTexture,renderTexture;
  RenderTexture* rend;
  GameObject *terrainGrass, *terrainRocks, *skybox, *objhandles, *camhandles, *lookat, *camera, *scene, *testparent, *testchild;

  grassTexture =    pgr::createTexture(Config::getAbsolutePath("/Data/textures2/grass.jpg"));
  grassBigTexture = pgr::createTexture(Config::getAbsolutePath("/Data/textures2/grassbig.jpg"));
  rocksTexture =    pgr::createTexture(Config::getAbsolutePath("/Data/textures2/rocks.jpg"));
  skyboxTexture =   pgr::createTexture(Config::getAbsolutePath("/Data/textures2/skybox.png"));

  rend =        new RenderTexture(&renderTexture,256,256);

  terrainGrass =new GameObject(plane_grassMesh, World2::shaderTerrain,  grassTexture);      terrainGrass->textures.push_back(grassBigTexture);
  terrainRocks =new GameObject(plane_rocksMesh, World2::shader0,        rocksTexture);
  skybox =      new GameObject(skyboxMesh,      World2::shaderSky,      skyboxTexture);
  objhandles =  new GameObject(unitcubeMesh,    World2::shader0,        grassTexture);
  lookat =      new GameObject(unitcubeMesh,    World2::shader0,        renderTexture);
  camhandles =  new GameObject(unitcubeMesh,    World2::shader0,        0);
  testchild =   new GameObject(unitcubeMesh,    World2::shader0,        rocksTexture);
  testparent =  new GameObject(unitcubeMesh,    World2::shader0,        grassBigTexture);
  camera =      new GameObject();
  scene =       new GameObject();

  terrainGrass->transform(   glm::vec3(0.0f, -5.0f, 0.0f),    glm::vec3(48.0f, 48.0f, 48.0f),     glm::vec3(1.0f, 0.0f, 0.0f),-90.0f);
  terrainRocks->transform(   glm::vec3(0.0f, -5.0f, 0.0f),    glm::vec3(48.0f, 48.0f, 48.0f),     glm::vec3(1.0f, 0.0f, 0.0f),-90.0f);
  skybox->transform(         glm::vec3(0.0f, -5.0f, 0.0f),    glm::vec3(10.0f, 10.0f, 10.0f),     glm::vec3(1.0f, 0.0f, 0.0f),90.0f);
  camera->transform(         glm::vec3(0.0f, 0.0f, 10.0f),    glm::vec3(1.0f, 1.0f, 1.0f),        glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
  objhandles->transform(     glm::vec3(0.0f, -5.0f, -0.0f),   glm::vec3(1.0f, 1.0f, 1.0f),        glm::vec3(1.0f, 0.0f, 0.0f),0.0f);
  camhandles->transform(     glm::vec3(-0.0f, -1.0f, -0.0f),  glm::vec3(1.0f, 1.0f, 1.0f),        glm::vec3(0.0f, 1.0f, 0.0f),-0.0f);
  lookat->transform(         glm::vec3(-4.0f, -1.0f, 0.0f),   glm::vec3(2.0f, 2.0f, 0.4f),        glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
  testparent->transform(     glm::vec3(2.0f, -4.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 0.5f),        glm::vec3(0.0f, 1.0f, 0.0f),45.0f);
  testchild->transform(      glm::vec3(2.0f, -0.5f, 0.0f),    glm::vec3(0.5f, 1.5f, 0.5f),        glm::vec3(0.0f, 0.0f, 1.0f),45.0f);

  scene->addComponent(new TransformHandles(objhandles));
  scene->addChild(terrainGrass, false);     terrainGrass->addComponent(new Renderer());
  scene->addChild(terrainRocks, false);     terrainRocks->addComponent(new Renderer());
  scene->addChild(skybox, false);           skybox->addComponent(new Renderer());
  scene->addChild(camera, false);           camera->addComponent(new Camera2(60.0f, scene)); 
                                            camera->addComponent(new CameraControl());
  scene->addChild(testparent, false);       testparent->addComponent(new Renderer());
    testparent->addChild(testchild, false);     testchild->addComponent(new Renderer());
    testparent->addChild(objhandles, true);     objhandles->addComponent(new Renderer(Renderer::USE_STENCIL));
        objhandles->addChild(camhandles, true);     camhandles->addComponent(new CameraHandles());
                                                    camhandles->addComponent(new Camera2(60.0f, scene, rend));
  scene->addChild(lookat, false);           lookat->addComponent(new Renderer());

  World2* w = new World2();w->sceneRoot = scene;w->onStart();
  return w;
}
void updateRecursive(GameObject* root){
  for (int i = 0; i < root->children.size(); i++){updateRecursive(root->children[i]);}
  for (int i = 0; i < root->components.size(); i++){if (root->components[i]->isActive){root->components[i]->update();}}
}

void startRecursive(GameObject* root){
  for (int i = 0; i < root->components.size(); i++){if (root->components[i]->isActive){root->components[i]->start();}}
  for (int i = 0; i < root->children.size(); i++){startRecursive(root->children[i]);}
}

void World2::onStart(){startRecursive(this->sceneRoot);}

void World2::onUpdate(){
  float viewport[4];
  glGetFloatv(GL_VIEWPORT, viewport);
  //printf("%f %f %f %f\n", viewport[0], viewport[1], viewport[2], viewport[3]);
  World2::width = viewport[2];
  World2::height= viewport[3];
  updateRecursive(this->sceneRoot);
  CHECK_GL_ERROR();
}
GLuint World2::loadShader(const char* vs_name, const char* fs_name){
  GLuint shader = 0;
  GLuint shaders[] = {pgr::createShaderFromFile(GL_VERTEX_SHADER, vs_name),pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fs_name), 0};
  if (shaders[0] == 0 || shaders[1] == 0){return 0;}
  shader = pgr::createProgram(shaders);
  return shader;
}
