//---------------------------------------------------------------------------
/**
 * \file	baseShader.h
 *
 * Declares the base shader class
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _BASESHADER_H_
#define _BASESHADER_H_

#include "RedShader.h"
#include <glm/gtc/type_ptr.hpp>

//#include "modelInstance.h"
//#include "Environment.h"

/**
 * \brief Shader program, where VS transforms both, the vertex position and the normal and copies the texture
 * coordinate and FS modulates ambient + diffuse lighting by a texture color times diffuseColor uniform.
 */
class BaseShader : public RedShader
{
protected:
	// textures
	GLint texCoord0Loc;      ///< location of the v_texCoord0 attribute variable
	GLint diffuseSamplerLoc; ///< location of uniform sampler u_diffuse

public:
	BaseShader(const char* vertexShader, const char* fragmentShader) : RedShader(vertexShader, fragmentShader) {}

	void getLocations() override
	{
		RedShader::getLocations();

		texCoord0Loc = glGetAttribLocation(id, "a_texCoord0");
		diffuseSamplerLoc = glGetUniformLocation(id, "u_diffuse");
	}

	GLint getTexCoord0Loc() { return texCoord0Loc; }

	GLint getDiffuseSamplerLoc() { return diffuseSamplerLoc; }

	void draw(ModelInstance* modelInstance, Camera* camera, Environment* environment) override
	{
		draw(modelInstance, camera->getProjection(), camera->getView(), environment);
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

		// environment
		// glm::vec3 lightDirection = glm::vec3( modelInstance->getTrans() * glm::vec4(environment->lightDirection, 0.0f)
		// );

		// normal trans
		glUniformMatrix3fv(normalTransLoc, 1, GL_FALSE,
		                   glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(modelInstance->getTrans())))));

		glUniform3fv(lightDirectionLoc, 1, glm::value_ptr(environment->lightDirection));
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(environment->lightColor));
		glUniform3fv(ambientColorLoc, 1, glm::value_ptr(environment->ambientColor));

		// material
		glUniform3fv(diffuseColorLoc, 1, glm::value_ptr(modelInstance->material->diffuseColor));

		// textures
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(diffuseSamplerLoc, 0);
		glBindTexture(GL_TEXTURE_2D, modelInstance->material->getTextureId());

		// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		// draw
		glBindVertexArray(modelInstance->getGeometry()->getVao());
		glDrawElements(GL_TRIANGLES, modelInstance->getGeometry()->getIndicesCount(), GL_UNSIGNED_INT, (void*)0);

		CHECK_GL_ERROR();

		glBindVertexArray(0);
	}

	/**
	 * \brief Called by ShaderProvider::connectGeometry
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
		glEnableVertexAttribArray(normalLoc);
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, geometry->getAttribsPerVertex() * sizeof(float),
		                      (void*)(3 * sizeof(float)));

		// uv
		glEnableVertexAttribArray(texCoord0Loc);
		glVertexAttribPointer(texCoord0Loc, 2, GL_FLOAT, GL_FALSE, geometry->getAttribsPerVertex() * sizeof(float),
		                      (void*)(6 * sizeof(float)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->getIndicesBuffer());

		glBindVertexArray(0);

		CHECK_GL_ERROR();
	}
};

#endif
