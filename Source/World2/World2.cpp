#include "World2.h"
#include "HardcodedMeshes.h"
#include "Components.h"

#include "Config.h"
#include "Source/GUI/Elements/Nodes/WorkspaceNodeWithCoreData.h"
#include "Source/Core/Nodes/GraphManager.h"

//#include "../Scripting/Scripting.h"

glm::mat4 World2::perspective = glm::mat4(1.0f);
glm::mat4 World2::mainCamera = glm::mat4(1.0f);
glm::vec3 World2::World2::mainCamPos = glm::vec3(0.0f);
float World2::height = 10;
float World2::width = 10;
bool World2::initializedRender = false;

Shader2 World2::shader0; ///< Default shader
Shader2 World2::shaderHandle;  ///< Handle shader
Shader2 World2::shaderProj; ///< preview projection matrices

World2*World2::tmpAccess=NULL;

World2::World2(){
    /*this->sceneRoot = new GameObject(gridMesh,&World2::shader0,0);
    this->sceneRoot->addComponent(new Renderer(Renderer::DRAW_LINES));
    this->sceneHandles = new GameObject();
    this->onStart();*/
}
bool World2::initRender(){
    World2::shader0 =         loadShader(Config::getAbsolutePath("/Data/shaders/simple-vs.glsl").c_str(),  Config::getAbsolutePath("/Data/shaders/simple-fs.glsl").c_str()); 
    World2::shaderHandle =    loadShader(Config::getAbsolutePath("/Data/shaders/handle-vs.glsl").c_str(),  Config::getAbsolutePath("/Data/shaders/handle-fs.glsl").c_str()); 
    World2::shaderProj =      loadShader(Config::getAbsolutePath("/Data/shaders/viewproj-vs.glsl").c_str(),Config::getAbsolutePath("/Data/shaders/viewproj-fs.glsl").c_str());
    if (World2::shader0.program * World2::shaderHandle.program *World2::shaderProj.program * World2::shaderProj.program == 0){
        getchar();printf("World2::initRender():cannot load shaders\n");return false;
    }

    CHECK_GL_ERROR();
    World2::initializedRender = true;
    return true;
}
World2* World2::loadDefaultScene(){
    if (!World2::initializedRender){printf("initialize render before creating World2!\n");return nullptr;}
    GLuint cubeGrayTexture, cubeTexture,renderTexture,axisTexture;
    RenderTexture* rend;
    GameObject *objhandles, *camhandles, *lookat, *camera, *scene, *testparent, *testchild,*handles;

    cubeGrayTexture = pgr::createTexture(Config::getAbsolutePath("/Data/textures/cube.png"));
    cubeTexture =     pgr::createTexture(Config::getAbsolutePath("/Data/textures/cube_color.png"));
    axisTexture =     pgr::createTexture(Config::getAbsolutePath("/Data/textures/axis.png"));

    rend =        new RenderTexture(&renderTexture,256,256);

    objhandles =  new GameObject(unitcubeMesh,    &World2::shader0, cubeGrayTexture);
    lookat =      new GameObject(unitquadMesh,    &World2::shader0, renderTexture);
    camhandles =  new GameObject(unitcubeMesh,    &World2::shader0, 0);
    testchild =   new GameObject(unitcubeMesh,    &World2::shader0, cubeTexture);
    testparent =  new GameObject(three_axisMesh,  &World2::shader0, axisTexture);   testparent->color = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
    camera =      new GameObject();
    scene =       new GameObject(gridMesh,        &World2::shader0, 0);             scene->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    handles =     new GameObject();

    camera->transform(         glm::vec3(0.0f, 7.0f, 10.0f),   glm::vec3(1.0f, 1.0f, 1.0f),        glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
    camhandles->transform(     glm::vec3(0.0f, 5.0f, 2.0f),    glm::vec3(1.0f, 1.0f, 1.0f),        glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
    objhandles->transform(     glm::vec3(0.0f, 0.0f, 1.41f),   glm::vec3(1.0f, 1.0f, 1.0f),        glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);//objhandles->transformation[0][3]=1.0f;
    lookat->transform(         glm::vec3(-4.0f, 4.0f, 0.0f),   glm::vec3(2.0f, 2.0f, 0.4f),        glm::vec3(0.0f, 1.0f, 0.0f), 225.0f);
    testparent->transform(     glm::vec3(2.0f, 1.0f,-3.0f),    glm::vec3(1.0f, 1.0f, 0.5f),        glm::vec3(0.0f, 1.0f, 0.0f),45.0f);//testparent->transformation[0][1]=0.5f;
    testchild->transform(      glm::vec3(0.0f, 6.0f,-8.0f),    glm::vec3(2.5f, 2.5f, 1.5f),        glm::vec3(0.0f, 0.0f, 1.0f),5.0f);

    scene->addComponent(new TransformHandles(objhandles)); scene->addComponent(new Renderer(Renderer::DRAW_LINES));
    scene->addChild(camera, false);           camera->addComponent(new Camera2(60.0f, scene)); 
                                              camera->addComponent(new CameraControl());
    scene->addChild(testchild, false);        testchild->addComponent(new Renderer());
    scene->addChild(testparent, false);       testparent->addComponent(new Renderer(Renderer::DRAW_LINES));
        testparent->addChild(objhandles, true);     objhandles->addComponent(new Renderer(Renderer::USE_STENCIL));
            objhandles->addChild(camhandles, true);     camhandles->addComponent(new CameraHandles());
                                                        camhandles->addComponent(new Camera2(60.0f, scene, rend));
    scene->addChild(lookat, false);           lookat->addComponent(new Renderer());
    scene->addChild(handles,false);           handles->addComponent(new TranslationHandles());
                                              handles->addComponent(new ScaleHandles());

    World2* w = new World2();
    w->sceneRoot = scene;
    w->sceneHandles = handles;
    w->onStart();
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
void World2::handlesSetMatrix(WorkspaceMatrix4x4* matnode) {
    for(int i=0;i<this->sceneHandles->components.size();i++){this->sceneHandles->components[i]->isActive=false;}
    if(matnode==NULL){

        return;
    }
    WorkspaceNodeWithCoreData*  nodebasedata= (WorkspaceNodeWithCoreData*)matnode;
    Ptr<Core::NodeBase>		    nodebase    = nodebasedata->Nodebase;
    WorkspaceNode*              node        = (WorkspaceNode*)nodebasedata;
    Core::Transform::DataMap	data		= nodebase->getDataMap();
	const Operation*			operation	= nodebase->getOperation();
	const char*					keyword		= operation->keyWord.c_str();
    DataStore                   datastore   = nodebase->getData();
    glm::mat4                   mat         = datastore.getMat4();

    if(strcmp("Scale",keyword)==0){
        printf("Scale %f %f %f\n",mat[0][0], mat[1][1], mat[2][2]);
        ScaleHandles* handles = (ScaleHandles*)this->sceneHandles->getComponent(ScaleHandles::componentType());
        if(handles==NULL){return;}
        handles->editedobj=nodebase.get();
    }
    else if(strcmp(keyword,"Translation")==0){
        printf("Translation %f %f %f\n", mat[3][0], mat[3][1], mat[3][2]);
        TranslationHandles*handles=(TranslationHandles*)this->sceneHandles->getComponent(TranslationHandles::componentType());
        if(handles==NULL){return;}
        handles->editedobj=nodebase.get();
    }
    printf("-\n");
    printf("operation %s\n",keyword);
    printf("\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\n",
        mat[0][0], mat[1][0], mat[2][0], mat[3][0],
        mat[0][1], mat[1][1], mat[2][1], mat[3][1],
        mat[0][2], mat[1][2], mat[2][2], mat[3][2],
        mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
    printf("-\n");

}
void World2::onStart(){
    startRecursive(this->sceneRoot);
}
void World2::onUpdate(){
    float viewport[4];
    glGetFloatv(GL_VIEWPORT, viewport);
    //printf("%f %f %f %f\n", viewport[0], viewport[1], viewport[2], viewport[3]);
    World2::width = viewport[2];
    World2::height= viewport[3];
    updateRecursive(this->sceneRoot);
    CHECK_GL_ERROR();
}
Shader2 World2::loadShader(const char* vs_name, const char* fs_name){
    Shader2 shader = {0,0,0,0,0,0,0,0,0};
    GLuint gl_shader = 0;
    GLuint shaders[] = {pgr::createShaderFromFile(GL_VERTEX_SHADER, vs_name),pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fs_name), 0};
    if (shaders[0]*shaders[1] == 0){return shader;}

    shader.program =  pgr::createProgram(shaders);
    shader.attr_pos = glGetAttribLocation(shader.program, "position");
    shader.attr_norm =glGetAttribLocation(shader.program, "norm"); // printf("%d", this->attr_norm);
    shader.attr_uv =  glGetAttribLocation(shader.program, "uv");
    shader.camera =   glGetUniformLocation(shader.program, "camera");
    shader.color =    glGetUniformLocation(shader.program, "color");
    shader.Mmatrix =  glGetUniformLocation(shader.program, "Mmatrix");
    shader.PVMmatrix =glGetUniformLocation(shader.program, "PVMmatrix");
    shader.VNmatrix = glGetUniformLocation(shader.program, "VNmatrix");
    return shader;
}
