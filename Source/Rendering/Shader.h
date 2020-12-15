//---------------------------------------------------------------------------
/**
 * \file	shader.h
 *
 * Header for representing shader data 
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>

#include <glm/glm.hpp>

#include "Rendering/Camera.h"

#include "Environment.h"
#include "Geometry.h"
#include "ModelInstance.h"

/**
 * \brief Base class for all three shaders
 */
class Shader
{
protected:

  int errorCode; ///< 0-no error, 1-shader files error, 2-shader compile error

  GLuint id; ///< openGL program name (ID) 

  // atributes
  GLint positionLoc; ///< Locaition of a_position attribute
  GLint normalLoc; ///< Location of  a_normal attribute 

  // transformations 
  GLint PVMLoc; ///< Location of the PVM matrix uniform u_PVM - does not store the u_normalTrans location

public:

  Shader(const char* vertexShader, const char* fragmentShader)
  {
    std::cout << "shader init" << std::endl;

    GLuint shaders[] = {
      pgr::createShaderFromFile(GL_VERTEX_SHADER, vertexShader),
      pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShader),
      0,
    };

    if (shaders[0] == 0 || shaders[1] == 0)
    {
      std::cout << "shader files error" << std::endl;
      errorCode = 1;
      return;
    }

    id = pgr::createProgram(shaders);

    if (id == 0)
    {
      std::cout << "shader compile error" << std::endl;
      errorCode = 2;
      return;
    }
    //else {
    //	std::cout << "compiled... id = " << id << std::endl;
    //}

    errorCode = 0;
  }

  /**
   * \brief Return status of shader compilation and program linking
   * \retval  0 no error
   * \retval  1 shader files error
   * \retval  2 shader compile error 
   */
  int getErrorCode() const
  {
    return errorCode;
  }

  /**
   * \brief Get locations of all shader variables (implemented for each shader)
   */
  virtual void getLocations()
  {
    // attributes
    positionLoc = glGetAttribLocation(id, "a_position");
    normalLoc = glGetAttribLocation(id, "a_normal");


    // uniforms
    PVMLoc = glGetUniformLocation(id, "u_PVM");
  }

  ~Shader()
  {
    pgr::deleteProgramAndShaders(id);
  }


  /**
   * \brief Get compiled shader program ID
   * \return OpenGL shader program name
   */
  GLuint getId() const
  {
    return id;
  }

  // matrices
  GLint getPVMLoc() const
  {
    return PVMLoc;
  }

  // attributes
  GLint getPositionLoc() const
  {
    return positionLoc;
  }

  GLint getNormalLoc() const
  {
    return normalLoc;
  }

  /*
  void begin(GLuint texId) {

    begin();

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(getDiffuseSamplerLoc(), 0);
    glBindTexture(GL_TEXTURE_2D, texId);

  }
  */

  /**
   * \brief Set OpenGL state for drawing non-transparent objects (no blending, enabled depthTest) and useProgram(id)
   */
  virtual void begin()
  {
    //PFXX 
    glEnable(GL_CULL_FACE);     // side effect - see inside 
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable( GL_BLEND );

    glUseProgram(id);

    // global uniforms place
    // ...
  }

  /**
   * \brief useProgram(0)
   */
  virtual void end()
  {
    glUseProgram(0);
  }

  /**
   * \brief Draw the object using the appropriate shader. Calls draw() with four parameters - using projection and view matrices extracted from camera instead of camera object itself
   * \param modelInstance Model to draw
   * \param camera		Camera object (stores projection and view matrices)
   * \param environment   Directional light source and ambient color
   */
  virtual void draw(ModelInstance* modelInstance, Camera* camera, Environment* environment) = 0;

  /**
   * \brief Draw the object using the appropriate shader - variant with projection and view matrix parameters
   * \param modelInstance Model to draw
   * \param projection  Camera projection matrix
   * \param view        Camera view matrix
   * \param environment Directional light source and ambient color
   */
  virtual void draw(ModelInstance* modelInstance, glm::mat4 projection, glm::mat4 view, Environment* environment) = 0;

  /**
   * \brief Binds all VBOs into the VAO, sets VertexAttribPointers and enables all atribute arrays
   * \param geometry 
   */
  virtual void connectVertexAttributes(Geometry* geometry) = 0;

  /**
   * \brief Get and print the shader compile log - not used... and probably duplicates the code in pgr lib \todo remove or call the routine from pgr.lib
   */
  //void shaderLog() const
  //{
  //	GLint result;
  //	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  //	if (result == GL_FALSE) {
  //		fprintf( stderr, "Vertex shader compilation failed!\n" );
  //		GLint logLen;
  //		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen );
  //		if ( logLen > 0 ) {
  //			char * log = (char *)malloc(logLen);
  //			GLsizei written;
  //			glGetShaderInfoLog(id, logLen, &written, log);
  //			fprintf(stderr, "Shader log:\n%s", log);
  //			free(log);
  //		}
  //	}
  //}
};

#endif
