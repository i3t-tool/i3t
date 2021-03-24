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

class WorkspaceMatrix4x4;
class GameObject;

struct Shader2{
    GLuint program;///< GL shader program
    GLuint camera;    ///< camera position before rotation
    GLuint Mmatrix;   ///< matrix of model's transform in shader
    GLuint PVMmatrix; ///< matrix view*model's transform in shader
    GLuint VNmatrix;  ///< matrix for adjusting model's normals in shader
    GLuint color;///<color
    GLint attr_pos;///<vertice position attribute
    GLint attr_norm; ///<vertice normal vector attribute
    GLint attr_uv;///<vertice texture coords attribute
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
    static World2* tmpAccess;
    /// calls start() on each component in scene
    /**
    
    */
    void onStart();
    /// Render scene
    /**
    Draws scene, updates global camera matricies
    */
    void onUpdate();

    void handlesSetMatrix(WorkspaceMatrix4x4*mat);
    GameObject* sceneRoot;///<root of scene of this world. Scene is a tree of GameObjects.
    GameObject* sceneHandles;///<control of transformation manipulators 

    ///load shaders
    /**
    load shaders
    */
    static bool initRender();           
    static float width;             ///< current viewport width
    static float height;            ///< current viewport width
    static glm::mat4x4 perspective; ///< Main camera perspective matrix, updated by Callbacks::onResize. Accessed by GameObject::draw(glm::mat4).
    static glm::mat4x4 mainCamera;  ///< Main camera transform, updated by Render::onDisplay. Accessed by GameObject::draw(glm::mat4).
    static glm::vec3 mainCamPos;    ///< Main camera position before rotation. Needed for reflection calculation in fs. Accessed by GameObject::draw(glm::mat4).

    static Shader2 shader0; ///< Default shader
    static Shader2 shaderHandle;   ///< Handle shader
    static Shader2 shaderProj; ///< preview projection matrices

    //static GLuint texture0;
private:
    static bool initializedRender; ///< Was render already initialized?
};