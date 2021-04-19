#include "World2.h"
#include "HardcodedMeshes.h"
#include "Components.h"
#include "Transforms.h"

#include "Config.h"
#include "Source/GUI/Elements/Nodes/WorkspaceNodeWithCoreData.h"
#include "Source/Core/Nodes/GraphManager.h"
#include "Source/Core/Input/InputManager.h"

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

GLuint World2::cubeTexture=0;
GLuint World2::cubeColorTexture=0;
GLuint World2::cGridTexture=0;
GLuint World2::axisTexture=0;
GLuint World2::whiteTexture=0;

World2::World2(){
    TranslationManipulator* tm =    new TranslationManipulator();
    ScaleManipulator* sm =          new ScaleManipulator();
    LookAtManipulator*lm =          new LookAtManipulator();
    OrthoManipulator*om =           new OrthoManipulator();
    PerspectiveManipulator*pm =     new PerspectiveManipulator();
    FrustumManipulator*fm =         new FrustumManipulator();
    RotationManipulator*rm =        new RotationManipulator();
    FreeManipulator*mm =            new FreeManipulator();
    this->manipulators.emplace("Translation",   Manipulator(&tm->m_editednode,&tm->m_parent,tm));
    this->manipulators.emplace("Scale",         Manipulator(&sm->m_editednode,&sm->m_parent,sm));
    this->manipulators.emplace("LookAt",        Manipulator(&lm->m_editednode,&lm->m_parent,lm));
    this->manipulators.emplace("Ortho",         Manipulator(&om->m_editednode,&om->m_parent,om));
    this->manipulators.emplace("Perspective",   Manipulator(&pm->m_editednode,&pm->m_parent,pm));
    this->manipulators.emplace("Frustum",       Manipulator(&fm->m_editednode,&fm->m_parent,fm));
    this->manipulators.emplace("EulerX",        Manipulator(&rm->m_editednode,&rm->m_parent,rm));
    this->manipulators.emplace("EulerY",        Manipulator(&rm->m_editednode,&rm->m_parent,rm));
    this->manipulators.emplace("EulerZ",        Manipulator(&rm->m_editednode,&rm->m_parent,rm));
    this->manipulators.emplace("Free",          Manipulator(&mm->m_editednode,&mm->m_parent,mm));
    //this->manipulators.emplace("AxisAngle",     Manipulator(&rm->m_editednode,&rm->m_parent,rm));//not editable
    GameObject*sceneHandles = new GameObject();

    for(std::map<std::string,Manipulator>::const_iterator i=this->manipulators.cbegin();i!=this->manipulators.cend();i++){
        if(sceneHandles->getComponent(i->second.component->getComponentType())==nullptr){//add each manipulator only once (rotation is stored multiple times under different keys)
            i->second.component->m_isActive=false;
            sceneHandles->addComponent(i->second.component);
        }
    }

    this->sceneRoot = new GameObject(gridMesh,&World2::shader0,0); 
    this->sceneRoot->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->sceneRoot->addComponent(new Renderer(Renderer::DRAW_LINES));

    GameObject* camera = new GameObject();
    camera->transform(glm::vec3(0.0f, 5.0f, 10.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f, 0.0f, 0.0f),-30.0f);
    camera->addComponent(new Camera2(60.0f, this->sceneRoot)); 
    camera->addComponent(new CameraControl());

    this->sceneRoot->addChild(camera, false);
    this->sceneRoot->addChild(sceneHandles, false);

    /*auto root = Builder::createSequence();
    auto branch1 = Builder::createSequence();
    auto branch2 = Builder::createSequence();

    std::vector<Ptr<Core::NodeBase>> matrices = {
        // sequence 1
        Builder::createTransform<Core::EulerRotX>(),
        Builder::createTransform<Core::Scale>(),
        Builder::createTransform<Core::Translation>(),
        // sequence 2
        Builder::createTransform<Core::Scale>(),
        Builder::createTransform<Core::Translation>(),
        // sequence 3
        Builder::createTransform<Core::Scale>(),
        Builder::createTransform<Core::Translation>(),
    };
    ValueSetResult v = matrices[0].get()->setValue(3.14159f);
    printf("ValueSetResult %d\n", v.status);

    Core::GraphManager::plug(root, branch1, 0, 0);
    Core::GraphManager::plug(root, branch2, 0, 0);

    // Add matrices to the sequences.
    root->addMatrix(matrices[0]); root->addMatrix(matrices[1]); root->addMatrix(matrices[2]);
    branch1->addMatrix(matrices[3]); branch1->addMatrix(matrices[4]);
    branch2->addMatrix(matrices[5]); branch2->addMatrix(matrices[6]);

    // Create sequence–root path from "branch1" sequence to root sequence.
    Core::SequenceTree tree(branch1);

    glm::mat4 m = getNodeTransform(&(Ptr<Core::NodeBase>)matrices[4], &branch1);*/
    //glm::mat4 m2=
}
bool World2::init(){
    World2::shader0 =         loadShader(Config::getAbsolutePath("/Data/shaders/simple-vs.glsl").c_str(),  Config::getAbsolutePath("/Data/shaders/simple-fs.glsl").c_str()); 
    World2::shaderHandle =    loadShader(Config::getAbsolutePath("/Data/shaders/handle-vs.glsl").c_str(),  Config::getAbsolutePath("/Data/shaders/handle-fs.glsl").c_str()); 
    World2::shaderProj =      loadShader(Config::getAbsolutePath("/Data/shaders/viewproj-vs.glsl").c_str(),Config::getAbsolutePath("/Data/shaders/viewproj-fs.glsl").c_str());
    if (World2::shader0.program * World2::shaderHandle.program *World2::shaderProj.program * World2::shaderProj.program == 0){
        getchar();printf("World2::init():cannot load shaders\n");return false;
    }//World2::shaderHandle=World2::shader0;

    World2::cubeTexture =       pgr::createTexture(Config::getAbsolutePath("/Data/textures/cube.png"));
    World2::cubeColorTexture =  pgr::createTexture(Config::getAbsolutePath("/Data/textures/cube_color.png"));
    World2::cGridTexture =      pgr::createTexture(Config::getAbsolutePath("/Data/textures/cGrid.png"));
    World2::axisTexture =       pgr::createTexture(Config::getAbsolutePath("/Data/textures/axis.png"));
    World2::whiteTexture =      pgr::createTexture(Config::getAbsolutePath("/Data/textures/white.png"));

    CHECK_GL_ERROR();
    World2::initializedRender = true;
    return true;
}
void World2::end() {
    pgr::deleteProgramAndShaders(World2::shader0.program);
    pgr::deleteProgramAndShaders(World2::shaderHandle.program);
    pgr::deleteProgramAndShaders(World2::shaderProj.program);
    glDeleteTextures(1,&World2::cubeTexture);
    glDeleteTextures(1,&World2::cubeColorTexture);
    glDeleteTextures(1,&World2::cGridTexture);
    glDeleteTextures(1,&World2::axisTexture);
    glDeleteTextures(1,&World2::whiteTexture);
}
World2* World2::loadDefaultScene(){
    if (!World2::initializedRender){printf("initialize render before creating World2!\n");return nullptr;}
    GLuint renderTexture;
    RenderTexture* rend;
    GameObject *objhandles, *camhandles, *lookat, *testparent, *testchild;

    rend =        new RenderTexture(&renderTexture,256,256);

    objhandles =  new GameObject(unitcubeMesh,    &World2::shader0, World2::cubeTexture);
    lookat =      new GameObject(unitquadMesh,    &World2::shader0, renderTexture);
    camhandles =  new GameObject(unitcubeMesh,    &World2::shader0, 0);
    testchild =   new GameObject(unitcubeMesh,    &World2::shader0, World2::cubeColorTexture);
    testparent =  new GameObject(three_axisMesh,  &World2::shader0, World2::axisTexture);          testparent->color = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);

    camhandles->transform(     glm::vec3(0.0f, 5.0f, 2.0f),    glm::vec3(1.0f, 1.0f, 1.0f),        glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
    objhandles->transform(     glm::vec3(0.0f, 0.0f, 1.41f),   glm::vec3(1.0f, 1.0f, 1.0f),        glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);//objhandles->transformation[0][3]=1.0f;
    lookat->transform(         glm::vec3(-4.0f, 4.0f, 0.0f),   glm::vec3(2.0f, 2.0f, 0.4f),        glm::vec3(0.0f, 1.0f, 0.0f), 225.0f);
    testparent->transform(     glm::vec3(2.0f, 1.0f,-3.0f),    glm::vec3(1.0f, 1.0f, 0.5f),        glm::vec3(0.0f, 1.0f, 0.0f),45.0f);//testparent->transformation[0][1]=0.5f;
    testchild->transform(      glm::vec3(0.0f, 6.0f,-8.0f),    glm::vec3(2.5f, 2.5f, 1.5f),        glm::vec3(0.0f, 0.0f, 1.0f),5.0f);

    World2* w = new World2();

    w->sceneRoot->addComponent(new TransformHandles(objhandles));
    w->sceneRoot->addChild(testchild, false);        testchild->addComponent(new Renderer());
    w->sceneRoot->addChild(testparent, false);       testparent->addComponent(new Renderer(Renderer::DRAW_LINES));
        testparent->addChild(objhandles, true);         objhandles->addComponent(new Renderer(Renderer::USE_STENCIL));
            objhandles->addChild(camhandles, true);         camhandles->addComponent(new CameraHandles());
                                                            camhandles->addComponent(new Camera2(60.0f, w->sceneRoot, rend));
    w->sceneRoot->addChild(lookat, false);           lookat->addComponent(new Renderer());

    w->onStart();
    return w;
}
void updateRecursive(GameObject* root){
    for (int i = 0; i < root->children.size(); i++){updateRecursive(root->children[i]);}
    for (int i = 0; i < root->components.size(); i++){if (root->components[i]->m_isActive){root->components[i]->update();}}
}

void startRecursive(GameObject* root){
    for (int i = 0; i < root->components.size(); i++){if (root->components[i]->m_isActive){root->components[i]->start();}}
    for (int i = 0; i < root->children.size(); i++){startRecursive(root->children[i]);}
}
Ptr<Core::NodeBase>op;
void World2::handlesSetMatrix(std::shared_ptr<WorkspaceMatrix4x4>*matnode,std::shared_ptr<Core::Sequence>*parent) {
    printf("handlesSetMatrix 0x%p,0x%p\n",matnode,parent);
    for(std::map<std::string,Manipulator>::const_iterator i=this->manipulators.cbegin();i!=this->manipulators.cend();i++){
        i->second.component->m_isActive=false;
        //*(i->second.editedNode)=nullptr;
    }

    if(matnode==nullptr){return;}
    if(matnode->get()==nullptr){return;}
    WorkspaceNodeWithCoreData*  nodebasedata= (WorkspaceNodeWithCoreData*)(matnode->get()); 
    const Ptr<Core::NodeBase>*	nodebase    = &nodebasedata->getNodebase();

    //op=Builder::createTransform<Core::Frustum>();
    //op=Builder::createTransform<Core::OrthoProj>();
    //op=Builder::createTransform<Core::EulerRotX>();
    //op=Builder::createTransform<Core::AxisAngleRot>();
    //op=Builder::createTransform<Core::LookAt>(glm::vec3{-0.0f, 1.0f, 0.0f}, glm::vec3{-0.1f, 0.5f, 0.0f },glm::vec3{0.0f, 1.0f, 0.0f});
    //nodebase    = &op;
    op=*nodebase;

    WorkspaceNode*              node        = (WorkspaceNode*)nodebasedata; 
    const Core::Transform::DataMap*	data		= nodebase->get()->getDataMap(); //printf("a");
	const Operation*			operation	= nodebase->get()->getOperation(); //printf("b");
	const char*					keyword		= nodebase->get()->getOperation()->keyWord.c_str(); //printf("c");
    DataStore                   datastore   = nodebase->get()->getData(); //printf("d");
    glm::mat4                   mat         = datastore.getMat4(); //printf("e\n");
    
    printf("nodebase 0x%p ", &nodebase); printf("get 0x%p\n", nodebase->get());
    if(this->manipulators.count(keyword)==1){
        Manipulator m=this->manipulators[keyword];
        m.component->m_isActive=true;
        Ptr<Core::NodeBase> nodebase2 = nodebasedata->getNodebase();
        *m.editedNode=nodebase2;
    }
    else{printf("No manipulators\n"); }

    printf("operation %s\n",keyword);
    printf("\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\t%0.3f %0.3f %0.3f %0.3f\n\n",
        mat[0][0], mat[1][0], mat[2][0], mat[3][0],
        mat[0][1], mat[1][1], mat[2][1], mat[3][1],
        mat[0][2], mat[1][2], mat[2][2], mat[3][2],
        mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
    printf("------------\n");

}
void World2::tmpDrawNode() {
	if(op.get()==nullptr){op= Core::Builder::createTransform<Core::EulerRotX>();}
	WorkspaceNodeWithCoreData* nodebasedata = (WorkspaceNodeWithCoreData*)(op.get());
	const glm::mat4& coreData = op->getData().getMat4();
	const Core::Transform::DataMap* coreMap = op->getDataMap();
	int idOfNode = nodebasedata->getId().Get();
	char label[]={0,0};
    float localData=0.0f;

	ImGui::PushItemWidth(50);
	for (int rows = 0; rows < 4; rows++){
		for (int columns = 0; columns < 4; columns++){
			localData = coreData[columns][rows];
			bool inactive = ((*coreMap)[columns * 4 + rows] == 0 || (*coreMap)[columns * 4 + rows] == 255);
			if (inactive){
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			ImGui::SameLine();
			label[0]='a'+ columns * 4 + rows;
			if(ImGui::DragFloat(label, &localData, 0.02f, 0.0f, 0.0f, fmt::format("% .{}f", 3).c_str(), 1.0f)){
				op->setValue(localData, { columns, rows });
			}

			if (inactive){ImGui::PopItemFlag();ImGui::PopStyleVar();}
		}
		ImGui::NewLine();
	}
	ImGui::PopItemWidth();
	ImGui::End();
}
void World2::tmpSetNode() {
    if(InputManager::isKeyPressed(Keys::x))     {op=Core::Builder::createTransform<Core::EulerRotX>();}
    else if(InputManager::isKeyPressed(Keys::y)){op=Core::Builder::createTransform<Core::EulerRotY>();}
    else if(InputManager::isKeyPressed(Keys::z)){op=Core::Builder::createTransform<Core::EulerRotZ>();}
    else if(InputManager::isKeyPressed(Keys::w)){op=Core::Builder::createTransform<Core::AxisAngleRot>();}
    else if(InputManager::isKeyPressed(Keys::s)){op=Core::Builder::createTransform<Core::Scale>();}
    else if(InputManager::isKeyPressed(Keys::t)){op=Core::Builder::createTransform<Core::Translation>();}
    else if(InputManager::isKeyPressed(Keys::o)){op=Core::Builder::createTransform<Core::OrthoProj>();}
    else if(InputManager::isKeyPressed(Keys::p)){op=Core::Builder::createTransform<Core::PerspectiveProj>();}
    else if(InputManager::isKeyPressed(Keys::f)){op=Core::Builder::createTransform<Core::Frustum>();}
    else if(InputManager::isKeyPressed(Keys::g)){op=Core::Builder::createTransform<Core::Free>();}
    else if(InputManager::isKeyPressed(Keys::l)){op=Core::Builder::createTransform<Core::LookAt>();}

    for(std::map<std::string,Manipulator>::const_iterator i=this->manipulators.cbegin();i!=this->manipulators.cend();i++){
        i->second.component->m_isActive=false;
        //*(i->second.editedNode)=nullptr;
    }

	const char*keyword=op->getOperation()->keyWord.c_str();

    if(this->manipulators.count(keyword)==1){
        Manipulator m=this->manipulators[keyword];
        m.component->m_isActive=true;
        *m.editedNode=op;
    }
    else{
        printf("no manipulators\n");
    }
}
GameObject* World2::addModel(const char* name) {
    GameObject* g=nullptr;
    if(strcmp("CubeGray",name)==0){             g=new GameObject(unitcubeMesh,  &World2::shader0,World2::cubeTexture);}
    else if (strcmp("CubeColor",name)==0) {     g=new GameObject(unitcubeMesh,  &World2::shader0,World2::cubeColorTexture); }
    else if (strcmp("CubeColorGrid",name)==0) { g=new GameObject(unitcubeMesh,  &World2::shader0,World2::cGridTexture); }
    else if (strcmp("PlainAxis",name)==0) {     g=new GameObject(three_axisMesh,&World2::shader0,World2::axisTexture); }
    if(g!=nullptr){this->sceneRoot->addChild(g,true);}
    return g;
}
bool World2::removeModel(GameObject*g) {
    return this->sceneRoot->rmChild(g,true);
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
