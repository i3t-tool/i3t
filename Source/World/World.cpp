#include "World.h"
#include "HardcodedMeshes.h"
#include "Components.h"
#include "Transforms.h"

#include "Config.h"
#include "Source/GUI/Elements/Nodes/WorkspaceNodeWithCoreData.h"
#include "Source/Core/Nodes/GraphManager.h"
#include "Source/Core/Input/InputManager.h"

//#include "../Scripting/Scripting.h"
std::shared_ptr<Core::NodeBase>World::tmpNode;
Core::SequencePtr World::tmpSequence = Core::SequencePtr();
Core::SequencePtr World::tmpSequence2 = Core::SequencePtr();

glm::mat4 World::perspective = glm::mat4(1.0f);
glm::mat4 World::mainCamera = glm::mat4(1.0f);
glm::vec3 World::World::mainCamPos = glm::vec3(0.0f);
float World::height = 10;
float World::width = 10;
bool World::initializedRender = false;

Shader World::shader0; ///< Default shader
Shader World::shaderHandle;  ///< Handle shader
Shader World::shaderProj; ///< preview projection matrices

GLuint World::cubeTexture=0;
GLuint World::cubeColorTexture=0;
GLuint World::cGridTexture=0;
GLuint World::axisTexture=0;
GLuint World::whiteTexture=0;

World::World(){
    if (!World::initializedRender) { printf("initialize render before creating World!\n"); }
    this->started=false;

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
    this->manipulators.emplace("AxisAngle",     Manipulator(&rm->m_editednode,&rm->m_parent,rm));
    //this->manipulators.emplace("Quat",          Manipulator(&rm->m_editednode,&rm->m_parent,rm));
    this->manipulators.emplace("Free",          Manipulator(&mm->m_editednode,&mm->m_parent,mm));
    GameObject*sceneHandles = new GameObject();

    for(std::map<std::string,Manipulator>::const_iterator i=this->manipulators.cbegin();i!=this->manipulators.cend();i++){
        if(sceneHandles->getComponent(i->second.component->getComponentType())==nullptr){//add each manipulator only once (rotation is stored multiple times under different keys)
            i->second.component->m_isActive=false;
            sceneHandles->addComponent(i->second.component);
        }
    }

    this->sceneRoot = new GameObject(gridMesh,&World::shader0,0); 
    this->sceneRoot->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->sceneRoot->addComponent(new Renderer(Renderer::DRAW_LINES));

    GameObject* camera = new GameObject();
    camera->transform(glm::vec3(0.0f, 5.0f, 10.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f, 0.0f, 0.0f),-30.0f);
    camera->addComponent(new Camera(60.0f, this->sceneRoot)); 
    camera->addComponent(new CameraControl());

    this->sceneRoot->addChild(camera, false);
    this->sceneRoot->addChild(sceneHandles, false);
}

bool World::init(){
    World::shader0 =         loadShader(Config::getAbsolutePath("/Data/shaders/simple-vs.glsl").c_str(),  Config::getAbsolutePath("/Data/shaders/simple-fs.glsl").c_str()); 
    World::shaderHandle =    loadShader(Config::getAbsolutePath("/Data/shaders/handle-vs.glsl").c_str(),  Config::getAbsolutePath("/Data/shaders/handle-fs.glsl").c_str()); 
    World::shaderProj =      loadShader(Config::getAbsolutePath("/Data/shaders/viewproj-vs.glsl").c_str(),Config::getAbsolutePath("/Data/shaders/viewproj-fs.glsl").c_str());

    if (World::shader0.program * World::shaderHandle.program *World::shaderProj.program * World::shaderProj.program == 0){
        printf("World::init():cannot load shaders\n");return false;
    }

    World::cubeTexture =       pgr::createTexture(Config::getAbsolutePath("/Data/textures/cube.png"));
    World::cubeColorTexture =  pgr::createTexture(Config::getAbsolutePath("/Data/textures/cube_color.png"));
    World::cGridTexture =      pgr::createTexture(Config::getAbsolutePath("/Data/textures/cGrid.png"));
    World::axisTexture =       pgr::createTexture(Config::getAbsolutePath("/Data/textures/axis.png"));
    World::whiteTexture =      pgr::createTexture(Config::getAbsolutePath("/Data/textures/white.png"));

    CHECK_GL_ERROR();
    World::initializedRender = true;
    return true;
}
void World::end() {
    pgr::deleteProgramAndShaders(World::shader0.program);
    pgr::deleteProgramAndShaders(World::shaderHandle.program);
    pgr::deleteProgramAndShaders(World::shaderProj.program);
    glDeleteTextures(1,&World::cubeTexture);
    glDeleteTextures(1,&World::cubeColorTexture);
    glDeleteTextures(1,&World::cGridTexture);
    glDeleteTextures(1,&World::axisTexture);
    glDeleteTextures(1,&World::whiteTexture);
}
World* World::loadDefaultScene(){
    if (!World::initializedRender){printf("initialize render before creating World!\n");return nullptr;}
    GLuint renderTexture;
    RenderTexture* rend =     new RenderTexture(&renderTexture,256,256);

    GameObject* objhandles =  new GameObject(unitcubeMesh,    &World::shader0, World::cubeTexture);
    GameObject* screen =      new GameObject(unitquadMesh,    &World::shader0, renderTexture);
    GameObject* camhandles =  new GameObject(unitcubeMesh,    &World::shader0, 0);

    camhandles->transform(  glm::vec3(0.0f, 5.0f, 2.0f),    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
    objhandles->transform(  glm::vec3(0.0f, 0.0f, 1.41f),   glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);//objhandles->transformation[0][3]=1.0f;
    screen->transform(      glm::vec3(-4.0f, 4.0f, 0.0f),   glm::vec3(2.0f, 2.0f, 0.4f),    glm::vec3(0.0f, 1.0f, 0.0f), 225.0f);

    World* w = new World();

    w->sceneRoot->addChild(objhandles, false);       objhandles->addComponent(new Renderer(Renderer::USE_STENCIL));
                                                     objhandles->addComponent(new TmpTransform());
    w->sceneRoot->addChild(camhandles, true);        //camhandles->addComponent(new TmpCamera());
                                                     camhandles->addComponent(new Camera(60.0f, w->sceneRoot, rend));
    w->sceneRoot->addChild(screen, false);           screen->addComponent(new Renderer());

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
void World::handlesSetMatrix(std::shared_ptr<WorkspaceMatrix4x4>*matnode,std::shared_ptr<Core::Sequence>*parent) {
    printf("handlesSetMatrix 0x%p,0x%p\n",matnode,parent);
    for(std::map<std::string,Manipulator>::const_iterator i=this->manipulators.cbegin();i!=this->manipulators.cend();i++){
        i->second.component->m_isActive=false;
        //*(i->second.editedNode)=nullptr;
    }

    if(matnode==nullptr){return;}
    if(matnode->get()==nullptr){return;}
    WorkspaceNodeWithCoreData*  nodebasedata= (WorkspaceNodeWithCoreData*)(matnode->get()); 
    Ptr<Core::NodeBase>	        nodebase    = nodebasedata->getNodebase();

    //nodebase    = &tmpNode;
    tmpNode=nodebase;//tmp

    WorkspaceNode*              node        = (WorkspaceNode*)nodebasedata; 
    const Core::Transform::DataMap*	data	= nodebase->getDataMap(); //printf("a");
	const Operation*			operation	= nodebase->getOperation(); //printf("b");
	const char*					keyword		= nodebase->getOperation()->keyWord.c_str(); //printf("c");
    
    if(this->manipulators.count(keyword)==1){
        Manipulator m=this->manipulators[keyword];
        m.component->m_isActive=true;
        *m.editedNode=nodebase;
    }
    else{printf("No manipulators\n"); }

    printf("operation %s\n",keyword);

}
void World::tmpDrawNode() {//this tends to cause crash
	if(tmpNode.get()==nullptr){return;}
	WorkspaceNodeWithCoreData* nodebasedata = (WorkspaceNodeWithCoreData*)(tmpNode.get());
    const Operation* operation = nodebasedata->getNodebase()->getOperation();
    const Core::Transform::DataMap* coreMap = tmpNode->getDataMap();
	glm::mat4 coreData = glm::mat4(1.0f);
    //const std::string s= operation->keyWord;
    
    //if(strcmp(s.c_str(), "Quat") != 0){
        const glm::mat4& cp = tmpNode->getData().getMat4();
        coreData=cp;
    //}
    //printf("type %d\n",nodebasedata->getNodebase()->getData().getOpValType());
	
	//int idOfNode = nodebasedata->getId().Get();
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
				ValueSetResult vsr=tmpNode->setValue(localData, { columns, rows });
			}

			if (inactive){ImGui::PopItemFlag();ImGui::PopStyleVar();}
		}
		ImGui::NewLine();
	}
	ImGui::PopItemWidth();
	ImGui::End();
}

void World::tmpSetNode() {
    if(tmpSequence.get()==nullptr){
        tmpSequence = Core::Builder::createSequence();
        tmpSequence->addMatrix(Core::Builder::createTransform<Core::Translation>());
        tmpSequence->addMatrix(Core::Builder::createTransform<Core::EulerRotX>());
        tmpSequence->addMatrix(Core::Builder::createTransform<Core::EulerRotY>());
        tmpSequence->addMatrix(Core::Builder::createTransform<Core::EulerRotZ>());
        tmpSequence->addMatrix(Core::Builder::createTransform<Core::AxisAngleRot>());
        tmpSequence->addMatrix(Core::Builder::createTransform<Core::Scale>());
        tmpSequence->addMatrix(Core::Builder::createTransform<Core::Free>());

        ((Core::AxisAngleRot*)tmpSequence->getMatrices().at(4).get())->setAxis(glm::vec3(0.0f,1.0f,0.0f));
        ((Core::AxisAngleRot*)tmpSequence->getMatrices().at(4).get())->setRot(0);
    }
    if (tmpSequence2.get() == nullptr) {
        tmpSequence2 = Core::Builder::createSequence();
        tmpSequence2->addMatrix(Core::Builder::createTransform<Core::LookAt>());
        tmpSequence2->addMatrix(Core::Builder::createTransform<Core::OrthoProj>());

        ((Core::LookAt*)tmpSequence2->getMatrices().at(0).get())->setEye(glm::vec3(0.0f));
        ((Core::LookAt*)tmpSequence2->getMatrices().at(0).get())->setCenter(glm::vec3(0.0f,0.0f,-1.0f));
    }

    if(InputManager::isKeyPressed(Keys::t))     {tmpNode=tmpSequence->getMatrices().at(0);}
    else if(InputManager::isKeyPressed(Keys::x)){tmpNode=tmpSequence->getMatrices().at(1);}
    else if(InputManager::isKeyPressed(Keys::y)){tmpNode=tmpSequence->getMatrices().at(2);}
    else if(InputManager::isKeyPressed(Keys::z)){tmpNode=tmpSequence->getMatrices().at(3);}
    else if(InputManager::isKeyPressed(Keys::w)){tmpNode=tmpSequence->getMatrices().at(4);}
    //else if(InputManager::isKeyPressed(Keys::q)){tmpNode=Core::Builder::createTransform<Core::QuatRot>();}
    else if(InputManager::isKeyPressed(Keys::s)){tmpNode=tmpSequence->getMatrices().at(5);tmpNode->setDataMap(&Core::Transform::g_Scale);}
    else if(InputManager::isKeyPressed(Keys::g)){tmpNode=tmpSequence->getMatrices().at(6);}
    

    else if(InputManager::isKeyPressed(Keys::o)){
        if(tmpSequence2->getMatrices().size()==2){tmpSequence2->popMatrix(1);}
        tmpSequence2->addMatrix(Core::Builder::createTransform<Core::OrthoProj>());
        tmpNode=tmpSequence2->getMatrices().at(1);
    }
    else if(InputManager::isKeyPressed(Keys::p)){
        if(tmpSequence2->getMatrices().size()==2){tmpSequence2->popMatrix(1);}
        tmpSequence2->addMatrix(Core::Builder::createTransform<Core::PerspectiveProj>());
        tmpNode=tmpSequence2->getMatrices().at(1);
    }
    else if(InputManager::isKeyPressed(Keys::f)){
        if(tmpSequence2->getMatrices().size()==2){tmpSequence2->popMatrix(1);}
        tmpSequence2->addMatrix(Core::Builder::createTransform<Core::Frustum>());
        tmpNode=tmpSequence2->getMatrices().at(1);
    }
    
    else if(InputManager::isKeyPressed(Keys::l)){tmpNode=tmpSequence2->getMatrices().at(0);}
    else if (tmpNode.get() == nullptr){tmpNode=tmpSequence->getMatrices().at(5);}

    for(std::map<std::string,Manipulator>::const_iterator i=this->manipulators.cbegin();i!=this->manipulators.cend();i++){
        i->second.component->m_isActive=false;
        *(i->second.editedNode)=nullptr;
    }

	const char*keyword=tmpNode->getOperation()->keyWord.c_str();

    if(this->manipulators.count(keyword)==1){
        Manipulator m=this->manipulators[keyword];
        m.component->m_isActive=true;
        *m.editedNode=tmpNode;
        *m.parent=tmpSequence;
        if(strcmp(keyword,"Ortho")==0||strcmp(keyword,"Frustum")==0||strcmp(keyword,"Perspective")==0){*m.parent=tmpSequence2;}
    }
    else{
        printf("no manipulators\n");
    }
}
GameObject* World::addModel(const char* name) {
    GameObject* g=nullptr;
    if(strcmp("CubeGray",name)==0){             g=new GameObject(unitcubeMesh,  &World::shader0,World::cubeTexture);}
    else if (strcmp("CubeColor",name)==0) {     g=new GameObject(unitcubeMesh,  &World::shader0,World::cubeColorTexture); }
    else if (strcmp("CubeColorGrid",name)==0) { g=new GameObject(unitcubeMesh,  &World::shader0,World::cGridTexture); }
    else if (strcmp("PlainAxis",name)==0) {     g=new GameObject(three_axisMesh,&World::shader0,World::axisTexture); }
    if(g!=nullptr){this->sceneRoot->addChild(g,true);}
    return g;
}
bool World::removeModel(GameObject*g) {
    return this->sceneRoot->rmChild(g,true);
}
void World::onStart(){
    this->started=true;
    startRecursive(this->sceneRoot);
}
void World::onUpdate(){
    float viewport[4];
    glGetFloatv(GL_VIEWPORT, viewport);
    //printf("%f %f %f %f\n", viewport[0], viewport[1], viewport[2], viewport[3]);
    World::width = viewport[2];
    World::height= viewport[3];
    if(!this->started){ printf("World:call start() before update()!\n"); return;}
    updateRecursive(this->sceneRoot);

    CHECK_GL_ERROR();
}
Shader World::loadShader(const char* vs_name, const char* fs_name){
    Shader shader = {0,0,0,0,0,0,0,0,0};
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
