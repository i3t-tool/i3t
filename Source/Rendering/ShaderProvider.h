//---------------------------------------------------------------------------
/**
 * \file	shaderProvider.h
 *
 * Declares the shader provider class
 *
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _SHADER_PROVIDER_H_
#define _SHADER_PROVIDER_H_

#define SHADER_COUNT 3

#include <vector>

#include "pgr.h"
#include <glm/glm.hpp>

class Shader;
class RedShader;
class BaseShader;
class AlphaShader;

class Geometry;
class ModelInstance;

struct Environment;
class Camera;

/**
 * \brief Three vectors of models grouped together according to the shader they use for display
 */
struct RenderGroups final
{
  /**
   * \brief Three groups of vectors of models: groups[0] to groups[3], each for all models using the same shader for
   * display.
   *   - 0 red shader - lighting and diffuseColor
   *   - 1 base shader - lighting, diffuseColor, and texture
   *   - 2 alpha shader - No lighting, only diffuseColor and u_ambientColor plus set alpha to u_alpha
   */
  std::vector<std::vector<ModelInstance*>> groups;

  RenderGroups() { groups = std::vector<std::vector<ModelInstance*>>(SHADER_COUNT, std::vector<ModelInstance*>()); }
};

class ShaderProvider final
{
private:
  static RenderGroups* renderGroups;
  ///< models in three groups grouped according to the shader the model uses for display

  static std::vector<Shader*> shaders; ///< vector of all three shader pairs

  /**
   * \brief Based on alpha flag and number of attributes, returns the shader number (base, red, or alpha)
   *        \todo 1) different result than getShaderID(ModelInstance * m) and 2) not used with alphaFlag
   *        \todo 3) it is impossible to decide the shader from geometry only
   * \param m model (geometry and material (colors and textures))
   * \param g Geometry (vertices, buffers, ...)
   * \retval 0 red shader
   * \retval 1 base shader
   * \retval 2 alpha shader - if (alphaFlag==true), but NEVER USED
   */
  static int getShaderID(ModelInstance* m);

  /**
   * \brief Based on number of attributes, returns the shader number (base or red only, not alpha).
   *        Used in connectGeometry() only.
   * \param g Geometry (vertices, OpenGL buffer names, ...)
   * \retval 0 red shader
   * \retval 1 base shader - used if attribsPerVertes
   */
  static int getShaderID(Geometry* g);

public:
  /**
   * \brief Stores three sets of shaders in the \a shaders vector
   * \param redVert Transform both, the vertex position and the normal.
   * \param redFrag Modulates ambient + diffuse lighting by a diffuseColor uniform.
   * \param baseVert Transform both, the vertex position and the normal. Copies the texture coordinate.
   * \param baseFrag Modulates ambient + diffuse lighting by a texture color times diffuseColor uniform.
   * \param alphaVert Transform the vertex position and copies the normal
   * \param alphaFrag Computes color as u_diffuseColor * u_ambientColor and set alpha to u_alpha.
   * \return
   */
  static int init(const char* redVert, const char* redFrag, const char* baseVert, const char* baseFrag,
                  const char* alphaVert, const char* alphaFrag);

  /**
   * \brief Delete all six shaders
   */
  static void dispose();

  /**
   * \brief Set the internal renderGroup with three vectors of models to the provided one
   * \param rg RenderGroup to replace the stored render group
   */
  static void setRenderGroups(RenderGroups* rg);

  /**
   * \brief Get Shader for given model \a m
   * \param m Model used for query
   * \return Shader with OpenGL shade name (ID) and locations of variables
   */
  static Shader* provideShader(ModelInstance* m);

  /**
   * \brief Add the model \a m to one of three groups according to the shader it will use for display. The selection
   * is based on number of attributes and existence of the texture. \param m Model to be added to the appropriate
   * group
   */
  static void addToGroup(ModelInstance* m);
  /**
   * \brief Find the model in one of the groups and remove it from this group. Searches only one group with the
   * appropriate number of vertex attributes. \param m Model to be removed
   */
  static void removeFromGroup(ModelInstance* m);

  /**
   * \brief Render all models in all groups using the Camera object
   * \param camera	   Camera
   * \param environment  Ambient and directional light.
   */
  static void renderAll(Camera* camera, Environment* environment);
  /**
   * \brief Render all models in all groups using the projection and view matrices
   * \param proj			Projection matrix
   * \param view			View matrix
   * \param environment	Ambient and directional light.
   */
  static void renderAll(glm::mat4 proj, glm::mat4 view, Environment* environment);
  /**
   * \brief Render all models in all groups using the projection and view matrices omitting one model given by \a
   * noRender \param proj			Projection matrix \param view			View matrix \param environment	Ambient and
   * directional light. \param noRender      Single model instance excluded from rendering (not rendered)
   */
  static void renderAll(glm::mat4 proj, glm::mat4 view, Environment* environment, ModelInstance* noRender);

  /**
   * \brief Connects the attributes for given geometry - Select the shader based on number of model attributes
   * (8=>baseShader or other=>redShader) \param g Geometry to be rendered. Appropriate shader method
   * connectVertexAttributes() is called from shaders[]. Implemented for red and base shaders, NOT implemented for
   * AlphaShader
   */
  static void connectGeometry(Geometry* g);

  /**
   * \brief Connect the geometry to a given shader
   * \param g Geometry to be rendered.
   * \param shaderID Explicitely given Shader ID
   */
  static void connectGeometry(Geometry* g, const GLuint shaderID);

  /**
   * \brief Not used - should not be necessary - use geometries and hcGeometries
   * \return Returns the redShader instance
   */
  static Shader* getRedShader() { return shaders[0]; }

  /**
   * \brief Not used - should not be necessary - use geometries and hcGeometries
   * \return Returns the baseShader instance
   */
  static Shader* getBaseShader() { return shaders[1]; }

  /**
   * \brief Used only in Scene::render rot rendering of frustrumObject
   * \return Returns the alphaShader instance
   */
  static Shader* getAlphaShader() { return shaders[2]; }
};

#endif
