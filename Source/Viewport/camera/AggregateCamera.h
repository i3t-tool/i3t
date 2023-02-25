#pragma once

#include <memory>

#include "Viewport/data/DisplayOptions.h"

#include "ICamera.h"
#include "OrbitCamera.h"
#include "TrackballCamera.h"

// TODO: (DR) Add an FPS camera perhaps?

namespace Vp
{
/**
 * Camera with multiple modes that it can seamlessly switch between.
 * Delegates actual "camera work" to one of the cameras that corresponds to the current mode.
 */
class AggregateCamera : public ICamera
{
public:
	enum CameraMode {
		ORBIT, TRACKBALL, NONE
	};
protected:
	CameraMode m_activeMode = CameraMode::NONE;
	std::shared_ptr<ICamera> m_activeCamera = nullptr;

	std::shared_ptr<OrbitCamera> m_orbitCamera = nullptr;
	std::shared_ptr<TrackballCamera> m_trackballCamera = nullptr;

public:
	AggregateCamera();

	void switchMode(CameraMode newMode);

	void size(int width, int height) override;
	void update() override;
	void processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize) override;

	CameraMode getMode() const;
	const std::shared_ptr<ICamera>& getActiveCamera() const;
	const std::shared_ptr<OrbitCamera>& getOrbitCamera() const;
	const std::shared_ptr<TrackballCamera>& getTrackballCamera() const;

	const glm::mat4& getView() const override;
	const glm::mat4& getProjection() const override;
	float getZNear() const override;
	void setZNear(float zNear) override;
	float getZFar() const override;
	void setZFar(float zFar) override;
	float getFov() const override;
	void setFov(float fov) override;
	int getWidth() const override;
	int getHeight() const override;
	const glm::vec3& getPosition() const override;
	const glm::vec3& getDirection() const override;
	const glm::vec3& getUp() const override;
	const glm::vec3& getRight() const override;
};
} // namespace Vp
