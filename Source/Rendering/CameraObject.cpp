#include "CameraObject.h"

#include "Rendering/Shaper.h"
#include "ShaperElements.h"
#include "Transformation.h"

CameraObject::CameraObject(Geometry* _geometry, Material* _material, Transformation* _transformation,
                           Transformation* _projection)
		: Object(_geometry, _material, _transformation), projection(_projection), drawFrustum(true)
{
}

Transformation* CameraObject::getTransformationProjection()
{
	return projection;
}

glm::mat4 CameraObject::getProjection()
{
	return projection->getTransformationMatrix();
}

glm::mat4 CameraObject::getView()
{
	return transformation->getTransformationMatrix();
}

void CameraObject::update()
{
	setTrans(glm::inverse(transformation->getTransformationMatrix()));
}

float CameraObject::getAspect()
{
	glm::mat4 p = getProjection();
	return p[1][1] / p[0][0];
}

void CameraObject::debug(Shaper* shaper)
{
	assert(shaper == ShaperElements::getShaper());
	// ShaperElements::setShaper(shaper);  // variant used in Object

	if (drawFrustum)
	{
		ShaperElements::getShaper()->setColor(
				Color::RED); // \todo presumes the dhaper was set elsewhere, ignores the parameter
		ShaperElements::drawFrustum(getProjection(), getTrans());
	}

	ShaperElements::drawOrientation(getTrans(), 1.0f); // camera coordinate frame
}
