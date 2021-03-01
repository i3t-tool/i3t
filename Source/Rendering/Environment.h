//---------------------------------------------------------------------------
/**
 * \file	Environment.h
 *
 * Declares the environment class
 *
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "glm/glm.hpp"

/**
 * \brief Stores the directinal light source and ambient color
 */
struct Environment
{
	glm::vec3 lightDirection; ///< direction of the directional light source
	glm::vec3 lightColor;     ///< color of the directional light source

	glm::vec3 ambientColor; ///< ambient coolor of the environment

	Environment()
	{
		lightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	}
};

#endif
