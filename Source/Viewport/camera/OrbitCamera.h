#pragma once

#include "IOrbitCamera.h"

namespace Vp
{
/**
 * Camera turning around a point using an azimuth and elevation angle.
 * It doesn't roll.
 */
class OrbitCamera : public IOrbitCamera
{
protected:
	float m_rotationX = -90.0f;
	float m_rotationY = 30.0f;

	float m_zoomSpeed = 0.8f;
	float m_rotateSpeed = 0.29f;
	float m_translateSpeed = 0.02f;
	bool m_smoothScroll = true;

	float m_dScroll = 0.0f;

	bool m_isRotating = false;
	bool m_rotateStartedUpsideDown = false;

public:
	void update() override;
	void processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize) override;

	virtual float getRotationX() const;
	virtual void setRotationX(float rotationX);

	virtual float getRotationY() const;
	virtual void setRotationY(float rotationY);

	void viewpoint(ICamera::Viewpoint viewpoint) override;

	float getZoomSpeed() const;
	void setZoomSpeed(float zoomSpeed);
	float getRotateSpeed() const;
	void setRotateSpeed(float rotateSpeed);
	float getTranslateSpeed() const;
	void setTranslateSpeed(float translateSpeed);
	bool getSmoothScroll() const;
	void setSmoothScroll(bool b);

protected:
	virtual void mouseWheel(float scroll);
	virtual void mouseDrag(float dx, float dy, bool rotate, bool pan);
};

} // namespace Vp
