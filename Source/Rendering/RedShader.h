//---------------------------------------------------------------------------
/**
 * \file	redShader.h
 *
 * Declares the red shader class
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _RED_SHADER_H_
#define _RED_SHADER_H_

#include "Rendering/Shader.h"
#include "World2/World2.h"
#include "World2/HardcodedMeshes.h"
#include <glm/gtc/type_ptr.hpp>

//#include "geometry.h"

/**
 * \brief Shader program, where VS transforms both, the vertex position and the normal and
 *        FS modulates ambient + diffuse lighting by a diffuseColor uniform.
 */
class RedShader : public Shader
{
protected:
  GLint normalTransLoc;
  GLint lightDirectionLoc;
  GLint lightColorLoc;
  GLint diffuseColorLoc;
  GLint ambientColorLoc;

public:
  RedShader(const char* vertexShader, const char* fragmentShader) : Shader(vertexShader, fragmentShader) {}

  void getLocations() override
  {
    Shader::getLocations();

    normalTransLoc = glGetUniformLocation(id, "u_normalTrans");
    lightDirectionLoc = glGetUniformLocation(id, "u_lightDirection");
    lightColorLoc = glGetUniformLocation(id, "u_lightColor");
    diffuseColorLoc = glGetUniformLocation(id, "u_diffuseColor");
    ambientColorLoc = glGetUniformLocation(id, "u_ambientColor");
  }

  GLint getLightDirectionLoc() const { return lightDirectionLoc; }

  GLint getLightColorLoc() const { return lightColorLoc; }

  GLint getDiffuseColorLoc() const { return diffuseColorLoc; }

  GLint getAmbientColorLoc() const { return ambientColorLoc; }

  void draw(ModelInstance* modelInstance, Camera* camera, Environment* environment) override
  {
    draw(modelInstance, camera->getProjection(), camera->getView(), environment);
  }

  // PF2018-03-05
  void printMatrix(std::string s, glm::mat4 m)
  {
    std::cout << s << std::endl;
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        std::cout << m[j][i] << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  void draw(ModelInstance* modelInstance, glm::mat4 projection, glm::mat4 view, Environment* environment) override
  {
    /*
    // Set in ShaderProvider::connectGeometry()
    if (modelInstance->getGeometry()->actShaderId != id) {
      modelInstance->getGeometry()->connectVertexAttributes(this);
    }
    */

    // PVM
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection * view * modelInstance->getTrans()));

    // PF2018-03-05
    // printMatrix("Projection:", projection);
    // printMatrix("View", view);
    // printMatrix("Model", modelInstance->getTrans());
    // printMatrix("PVM", projection * view * modelInstance->getTrans());
    // normal trans
    glUniformMatrix3fv(normalTransLoc, 1, GL_FALSE,
                       glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(modelInstance->getTrans())))));
    // glm::vec3 lightDirection = glm::vec3(glm::inverse(modelInstance->getTrans()) *
    // glm::vec4(environment->lightDirection, 0.0f));

    glUniform3fv(lightDirectionLoc, 1, glm::value_ptr(environment->lightDirection));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(environment->lightColor));
    glUniform3fv(ambientColorLoc, 1, glm::value_ptr(environment->ambientColor));

    // material
    glUniform3fv(diffuseColorLoc, 1, glm::value_ptr(modelInstance->material->diffuseColor));

    // draw
    glBindVertexArray(modelInstance->getGeometry()->getVao());
    //glDrawElements(GL_TRIANGLES, modelInstance->getGeometry()->getIndicesCount(), GL_UNSIGNED_INT, (void*)0);
    //glDrawElements(GL_TRIANGLES, modelInstance->getGeometry()->getIndicesCount(), GL_UNSIGNED_INT, (void*)0);

    CHECK_GL_ERROR();

    glBindVertexArray(0);
    //World2::tmpAccess->sceneRoot->children[2]->draw(glm::mat4(1.0f));
    printf("tttttttt\n");
  }

  /**
   * \brief Connect vertex data to locations (glVertexAttribPointer and enable them.
   *        Called by ShaderProvider::connectGeometry
   * \param geometry Model (vertices an indices) + OpenGL names of buffers
   */
  void connectVertexAttributes(Geometry* geometry) override
  {
    geometry->actShaderId = id;

    glBindVertexArray(geometry->getVao());

    glBindBuffer(GL_ARRAY_BUFFER, geometry->getVerticesBuffer());

    // position
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, geometry->getAttribsPerVertex() * sizeof(float),
                          (void*)0);

    // normal
    if (normalLoc > 0)
    {
      glEnableVertexAttribArray(normalLoc);
      glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, geometry->getAttribsPerVertex() * sizeof(float),
                            (void*)(3 * sizeof(float)));
    }
    /*
    // uv
    glEnableVertexAttribArray(texCoord0Loc);
    glVertexAttribPointer(texCoord0Loc, 2, GL_FLOAT, GL_FALSE, geometry->getAttribsPerVertex() * sizeof(float), (void
    *) (6 * sizeof(float)));
    */

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->getIndicesBuffer());

    glBindVertexArray(0);

    CHECK_GL_ERROR();
  }
};

#endif
