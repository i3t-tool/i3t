#pragma once
//----------------
/**
 * \file World2.h
 * \author Daniel Gruncl
 * \brief 3D rendering context
 * contains shader data, projection and view matricies, viewport dimensions,
 * as well as scene tree (tree of GameObjects).
 */
//---------------
#include "pgr.h"
#include "GameObject.h"

#include <map>
#include <memory>

class WorkspaceMatrix4x4;
class GameObject;
class Component;
namespace Core{class NodeBase;class Sequence;}

struct Shader2{
    GLuint program;     ///< GL shader program
    GLuint camera;      ///<uniform camera position before rotation
    GLuint Mmatrix;     ///<uniform matrix of model's transform in shader
    GLuint PVMmatrix;   ///<uniform matrix view*model's transform in shader
    GLuint VNmatrix;    ///<uniform matrix for adjusting model's normals in shader
    GLuint color;       ///<uniform color
    GLint attr_pos;     ///<vertice position attribute
    GLint attr_norm;    ///<vertice normal vector attribute
    GLint attr_uv;      ///<vertice texture coords attribute
};
struct Manipulator {
    Manipulator(std::shared_ptr<Core::NodeBase>*_editedNode,std::shared_ptr<Core::Sequence>*_parent,Component*_component){
        editedNode=_editedNode;parent=_parent;component=_component;
    }
    Manipulator(){editedNode=nullptr;parent=nullptr;component=nullptr;}
    std::shared_ptr<Core::NodeBase>*editedNode;
    std::shared_ptr<Core::Sequence>*parent;
    Component*component;
    //Component*m_gameObject;
};

class World2
{
public:
    World2();
    /// Load and compile shader program
    /**
    \param[in] vs_name Vertex shader file
    \param[in] fs_name Fragment shader file
    \return GL shader, 0 if failure
    */
    static Shader2 loadShader(const char* vs_name,const char* fs_name);
    /// Initialize scene
    /**
    Creates and initializes default scene
    */
    static World2* loadDefaultScene();
    /// calls start() on each component in scene
    /**
    
    */
    void onStart();
    /// Render scene
    /**
    Draws scene, updates global camera matricies
    */
    void onUpdate();
    ///Activate manipulators in scene (viewport) for givent type of workspace matrix
    void handlesSetMatrix(std::shared_ptr<WorkspaceMatrix4x4>*matnode,std::shared_ptr<Core::Sequence>*parent);

    void tmpDrawNode();
    void tmpSetNode();

    ///Add GameObject to scene (viewport window)
    GameObject* addModel(const char* name);
    ///Remove GameObject from scene (viewport window)
    bool removeModel(GameObject*);
    GameObject* sceneRoot;///<root of scene of this world. Scene is a tree of GameObjects.
    std::map<std::string,Manipulator>manipulators;///<Properites of manipulator components

    ///load HC shaders, textures
    static bool init();
    ///unload HC shaders, textures
    static void end();
    static float width;             ///< current viewport width
    static float height;            ///< current viewport width
    static glm::mat4x4 perspective; ///< Main camera perspective matrix, updated by Callbacks::onResize. Accessed by GameObject::draw(glm::mat4).
    static glm::mat4x4 mainCamera;  ///< Main camera transform, updated by Render::onDisplay. Accessed by GameObject::draw(glm::mat4).
    static glm::vec3 mainCamPos;    ///< Main camera position before rotation. Needed for reflection calculation in fs. Accessed by GameObject::draw(glm::mat4).

    //HC shaders
    static Shader2 shader0; ///< Default shader
    static Shader2 shaderHandle;   ///< Handle shader
    static Shader2 shaderProj; ///< preview projection matrices

    //HC textures
    static GLuint cubeTexture;
    static GLuint cubeColorTexture;
    static GLuint cGridTexture;
    static GLuint axisTexture;
    static GLuint whiteTexture;

    //static std::map<const char*, GLuint > textures;
    //static std::map<const char*, pgr::MeshData > models;
    
private:
    static bool initializedRender; ///< Was render already initialized?
};