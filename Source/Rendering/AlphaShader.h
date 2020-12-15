//---------------------------------------------------------------------------
/**
 * \file	alphaShader.h
 *
 * Declares the alpha shader class
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague		  
 */
//---------------------------------------------------------------------------

#ifndef _ALPHA_SHADER_H_
#define _ALPHA_SHADER_H_

#include "RedShader.h"

#include <glm/gtx/norm.hpp>

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

/**
 * \brief Structure for storage of depth of triangles or orientation to the
 * camera dot(n,e), each triangle stores index and depth or orientation, used in
 * AlphaShader for back to front display of transparent triangles
 */
struct DepthIndex
{
  unsigned int index; ///< indices to triangles (vertex indices will then be 3i, 3i+1, and 3i+2 )
  float depth2; ///< distance of the triagle centroid to the camera

  bool operator<(const DepthIndex& p) const { return (depth2 < p.depth2); }
  bool operator==(const DepthIndex& p) const { return (depth2 == p.depth2); }
  bool operator!=(const DepthIndex& p) const { return (depth2 != p.depth2); }

  DepthIndex()
  {
  }

  DepthIndex(const int _index, const float _depth2) : index(_index), depth2(_depth2)
  {
  }
};

/**
 * \brief Shader that uses alpha blending, where VS transforms the vertex position and copies the normal 
 *        and FS Computes color as u_diffuseColor * u_ambientColor and set alpha to u_alpha.
 */
class AlphaShader : public RedShader
{
private:

  GLint alphaLoc; ///< location of uniform u_alpha

  float alpha;

  /**
   * \brief Sort triangles according to their distance to camera (their centroid to camera) ignoring a perspective division by \a w. NOT USED
   * \param geometry      Vertices and indices of the triangles
   * \param trans         Compound transformation matrix - PVM - for transforming of individual vertices
   * \param camPosition   position of the camera
   * \param sortedIndices [out] List of indices to triangles sorted according to their distance to camera
   */
  void sortIndices(Geometry* geometry, glm::mat4 trans, glm::vec3 camPosition, std::vector<DepthIndex>& sortedIndices)
  {
    int first; ///< index of the first vertex attribute of vertex[i] in the array of vertices

    for (unsigned int i = 0; i < geometry->getIndicesCount(); i += 3)
    {
      //index of the first triangle-vertex index

      // point A = vertex[indices(i)].xyz
      first = geometry->getIndices()[i] * geometry->getAttribsPerVertex();
      // index of position x = first attribute of vertex[i]
      glm::vec4 A = trans * glm::vec4(geometry->getVertices()[first], geometry->getVertices()[first + 1],
                                      geometry->getVertices()[first + 2], 1.0f);

      // point B = vertex[indices(i+1)].xyz
      first = geometry->getIndices()[i + 1] * geometry->getAttribsPerVertex();
      glm::vec4 B = trans * glm::vec4(geometry->getVertices()[first], geometry->getVertices()[first + 1],
                                      geometry->getVertices()[first + 2], 1.0f);

      // point C = vertex[indices(i+2)].xyz
      first = geometry->getIndices()[i + 2] * geometry->getAttribsPerVertex();
      glm::vec4 C = trans * glm::vec4(geometry->getVertices()[first], geometry->getVertices()[first + 1],
                                      geometry->getVertices()[first + 2], 1.0f);

      glm::vec3 tCenter = glm::vec3((A + B + C)) / 3.0f; //centroid

      sortedIndices.push_back(DepthIndex(i, glm::length2(tCenter - camPosition)));
    }

    std::sort(sortedIndices.begin(), sortedIndices.end());
  }

  /**
   * \brief Sort triangles according to their orientation to camera (dot product of their normal to the direction to camera) including perspective division by \a w
   * \param geometry       Vertices and indices of the triangles
   * \param trans          Compound transformation matrix - PVM - for transforming of individual vertices
   * \param camDirection   Vector to the camera
   * \param sortedIndices  [out] List of indices to triangles sorted according to their orientation to camera dot(n,e) in perspective projection
   */
  void sortIndicesPerspective(Geometry* geometry, glm::mat4 trans, glm::vec3 camDirection,
                              std::vector<DepthIndex>& sortedIndices)
  {
    int first;

    for (unsigned int i = 0; i < geometry->getIndicesCount(); i += 3)
    {
      first = geometry->getIndices()[i] * geometry->getAttribsPerVertex();
      glm::vec4 A = trans * glm::vec4(geometry->getVertices()[first], geometry->getVertices()[first + 1],
                                      geometry->getVertices()[first + 2], 1.0f);
      A /= A.w;

      first = geometry->getIndices()[i + 1] * geometry->getAttribsPerVertex();
      glm::vec4 B = trans * glm::vec4(geometry->getVertices()[first], geometry->getVertices()[first + 1],
                                      geometry->getVertices()[first + 2], 1.0f);
      B /= B.w;

      first = geometry->getIndices()[i + 2] * geometry->getAttribsPerVertex();
      glm::vec4 C = trans * glm::vec4(geometry->getVertices()[first], geometry->getVertices()[first + 1],
                                      geometry->getVertices()[first + 2], 1.0f);
      C /= C.w;

      glm::vec3 tn = glm::normalize(glm::cross(glm::vec3(B - A), glm::vec3(C - A))); // triangle normal


      sortedIndices.push_back(DepthIndex(i, glm::dot(camDirection, tn)));
    }

    std::sort(sortedIndices.begin(), sortedIndices.end());
  }

public:

  AlphaShader(const char* vertexShader, const char* fragmentShader) : RedShader(vertexShader, fragmentShader),
                                                                      alpha(0.5f)
  {
  }

  void getLocations() override
  {
    RedShader::getLocations();

    alphaLoc = glGetUniformLocation(id, "u_alpha");
  }


  /**
   * \brief Set OpenGL state for drawing transparent objects (blending, alpha, 1-srcAlpha, masked depth) and useProgram(id)
   */
  void begin() override
  {
    //glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);

    glDepthMask(GL_FALSE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glUseProgram(id);
  }

  /**
   * \brief Stop depth masking and useProgram(0)
   */
  void end() override
  {
    glDepthMask(GL_TRUE);
    RedShader::end();  
  }

  void draw(ModelInstance* modelInstance, Camera* camera, Environment* environment) override
  {
    draw(modelInstance, camera->getProjection(), camera->getView(), environment);
  }

  void draw(ModelInstance* modelInstance, glm::mat4 projection, glm::mat4 view, Environment* environment) override
  {
    /*
    // Should be set in ShaderProvider::connectGeometry(), but AlphaShader has no connectVertexAttributes() method
    if (modelInstance->getGeometry()->actShaderId != id) {
      modelInstance->getGeometry()->connectVertexAttributes(this);
    }
    */

    // camera combined matrix
    const glm::mat4 combined = projection * view;

    // sorted the semi-transparent triangles back to front according their orientation to camera
    std::vector<DepthIndex> di;
    sortIndicesPerspective(modelInstance->getGeometry(), modelInstance->getTrans(), -glm::vec3(glm::inverse(view)[2]),
                           di);

    // PVM
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(combined * modelInstance->getTrans()));

    // environment
    glm::vec3 lightDirection = glm::vec3(
      glm::inverse(modelInstance->getTrans()) * glm::vec4(environment->lightDirection, 0.0f));

    if (lightDirectionLoc != -1)
      glUniform3fv(lightDirectionLoc, 1, glm::value_ptr(lightDirection));
    if (lightColorLoc != -1)
      glUniform3fv(lightColorLoc, 1, glm::value_ptr(environment->lightColor));
    glUniform3fv(ambientColorLoc, 1, glm::value_ptr(environment->ambientColor));

    // material
    glUniform3fv(diffuseColorLoc, 1, glm::value_ptr(modelInstance->material->diffuseColor));

    // alpha
    glUniform1f(alphaLoc, alpha);

    // draw
    glBindVertexArray(modelInstance->getGeometry()->getVao());

    for (auto i = di.size() - 1; i >= 0; i--)
    {
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(di[i].index * sizeof(unsigned int)));
    }

    CHECK_GL_ERROR();

    glBindVertexArray(0);
  }
};

#endif
