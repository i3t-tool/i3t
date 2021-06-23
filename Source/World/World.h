#pragma once
//----------------
/**
 * \file World.h
 * \author Daniel Gruncl
 * \brief 3D rendering context
 * \date 2.3.2021
 * contains hardcoded textures, hardcoded shaders, shader data, projection and view matricies, viewport dimensions,
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
class CameraControl;
namespace Core{class NodeBase;class Sequence;}

struct Shader{
    GLuint program;     ///< GL shader program
    GLuint camera;      ///<uniform camera position before rotation
    GLuint Mmatrix;     ///<uniform matrix of model's transform in shader
    GLuint PVMmatrix;   ///<uniform matrix view*model's transform in shader
    GLuint VNmatrix;    ///<uniform matrix for adjusting model's normals in shader
    GLuint color;       ///<uniform color (vec4)
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

class World
{
public:
    World();
    /// Load and compile shader program
    /**
    \param[in] vs_name Vertex shader file
    \param[in] fs_name Fragment shader file
    \return GL shader, 0 if failure
    */
    static Shader loadShader(const char* vs_name,const char* fs_name);

    /// calls start() on each component in scene
    /**
    
    */
    void onStart();
    /// Render scene
    /**
    Draws scene, updates global camera matricies, updates scene logic.
    */
    void onUpdate();

    void onGUI();

    void sceneSetView(glm::vec3 dir,bool world);
    void sceneZoom(float val);

    ///Activate manipulators in scene (viewport) for givent type of workspace matrix
    void manipulatorsSetMatrix(std::shared_ptr<WorkspaceMatrix4x4>*matnode,std::shared_ptr<Core::Sequence>*parent);

    void manipulatorsSetVisible(bool visible);
    bool manipulatorsGetVisible();

    ///Add GameObject to scene (viewport window)
    GameObject* addModel(const char* name);
    ///Remove GameObject from scene (viewport window)
    bool removeModel(GameObject*);
    GameObject* sceneRoot;///<root of scene of this world. Scene is a tree of GameObjects.
    std::map<std::string,Manipulator>manipulators;///<Properites of manipulator components

    ///load HC shaders, textures. Must be called before any instance of World is created.
    static bool init();
    ///unload HC shaders, textures
    static void end();
    static float width;             ///< current viewport width
    static float height;            ///< current viewport width
    static glm::mat4x4 perspective; ///< Main camera perspective matrix, updated by Callbacks::onResize. Accessed by GameObject::draw(glm::mat4).
    static glm::mat4x4 mainCamera;  ///< Main camera transform, updated by Render::onDisplay. Accessed by GameObject::draw(glm::mat4).
    static glm::vec3 mainCamPos;    ///< Main camera position before rotation. Needed for reflection calculation in fs. Accessed by GameObject::draw(glm::mat4).

    //HC shaders
    static Shader shader0; ///< Default shader
    static Shader shaderHandle;   ///< Handle shader
    static Shader shaderProj; ///< preview projection matrices

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
    bool started=false;///<Was start() already called on this instance of World?
    CameraControl*camControl=nullptr;
    bool showManipulators=true;
    Manipulator*activeManipulator=nullptr;
};