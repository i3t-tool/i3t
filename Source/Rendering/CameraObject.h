//---------------------------------------------------------------------------
/**
 * \file	cameraObject.h
 *
 * Header for representing camera object in scene
 *
 * \date  	2014/11/16
 * \author 	Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _CAMERA_OBJECT_H_
#define _CAMERA_OBJECT_H_

#include "Object.h"

// class Transformation;
// class Shaper;

/**
 * \brief Geometry of the camera
 */
class CameraObject : public Object
{
protected:
	Transformation* projection; ///< projection matrix

	bool drawFrustum; ///< true to show the camera frustrum in the 3D scene preview

public:
	CameraObject(Geometry* _geometry, Material* _material, Transformation* _transformation,
	             Transformation* _projection);

	bool getDrawFrustum() const { return drawFrustum; }

	void setDrawFrustum(const bool val) { drawFrustum = val; }

	Transformation* getTransformationProjection();

	/**
	 * \brief  Get the camera porjection matrix P
	 * \return P - a camera projection matrix
	 */

	glm::mat4 getProjection();
	/**
	 * \brief Get the camera E = v^(-1) matrix --- \todo check, if not reversed
	 * \return E - the inverse view matrix (model matrix of the camera object i 3D scene)
	 */
	glm::mat4 getView();

	/**
	 * \brief Aspect of the projection
	 * \return aspect = w/h
	 */
	float getAspect();

	void update() override;

	/**
	 * \brief Prepare the camera coordinate frame and frustrum into the Shaper geometry (vertices and colors)
	 *  \notused Parameter \a shaper NOT USED - already set in static object Shaper - differs from overloaded method of
	 * Object::debug() \param shaper Parameter \a shaper NOT USED - already set in static object Shaper
	 */
	void debug(Shaper* shaper) override;
};

#endif
