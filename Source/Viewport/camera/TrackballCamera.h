#pragma once

#include "IOrbitCamera.h"

namespace Vp
{
/**
 * Camera turning around a point using a virtual screen-space trackball.
 */
class TrackballCamera : public IOrbitCamera
{
protected:
	glm::mat4 m_accumulatedRotation = glm::mat4(1);

	float m_zoomSpeed = 0.8f;
	float m_rotateSpeed = 0.29f;
	float m_translateSpeed = 0.02f;
	bool m_smoothScroll = true;

	float m_dScroll = 0.0f;

public:
	void update() override;
	void processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize) override;

	glm::mat4 getRotation() const;
	void setRotation(glm::mat4 rotation);

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
};

} // namespace Vp
