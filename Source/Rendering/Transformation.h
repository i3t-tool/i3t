/**
 * \file	transformation.h
 *
 * Declares the transformation class
 *
 * \date  	2014/11/16
 * \author 	Michal Folta, CTU Prague
 */

#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_

#include <vector>

#include <glm/glm.hpp>

/**
 * \brief Virtual interface class - defines two virtual interface methods getTransformationMatrix() and
 * getTransformationOut()
 */
class Transformation
{
public:
	virtual ~Transformation(){};

	virtual glm::mat4 getTransformationMatrix() = 0;
	virtual glm::mat4 getTransformationOut() = 0;
};

#endif
