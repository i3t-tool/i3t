/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <memory>

#include "Viewport/data/DisplayOptions.h"

#include "AbstractCamera.h"
#include "OrbitCamera.h"
#include "TrackballCamera.h"

// TODO: (DR) Add an FPS camera perhaps?

namespace Vp
{
/**
 * Camera with multiple modes that it can seamlessly switch between.
 * Delegates actual "camera work" to one of the cameras that corresponds to the current mode.
 */
class AggregateCamera : public AbstractCamera
{
public:
	enum CameraMode
	{
		ORBIT,
		TRACKBALL,
		NONE
	};

protected:
	CameraMode m_activeMode = CameraMode::NONE;
	std::shared_ptr<AbstractCamera> m_activeCamera = nullptr;

	std::shared_ptr<OrbitCamera> m_orbitCamera = nullptr;
	std::shared_ptr<TrackballCamera> m_trackballCamera = nullptr;

public:
	AggregateCamera();

	void switchMode(CameraMode newMode);

	void size(int width, int height) override;
	void update() override;
	void processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize) override;

	CameraMode getMode() const;
	const std::shared_ptr<AbstractCamera>& getActiveCamera() const;
	const std::shared_ptr<OrbitCamera>& getOrbitCamera() const;
	const std::shared_ptr<TrackballCamera>& getTrackballCamera() const;

	void viewpoint(AbstractCamera::Viewpoint viewpoint) override;

	void centerOnScene(const Scene& scene) override;
	void centerOnSelection(const Scene& scene) override;
	void centerOnObjects(const std::vector<const GameObject*> objects) override;
	void centerOnBox(glm::vec3 boxMin, glm::vec3 boxMax, bool interpolate) override;

	glm::mat4 getView() const override;
	glm::mat4 getProjection() const override;

	glm::vec3 getPosition() const override;
	glm::vec3 getDirection() const override;
	glm::vec3 getUp() const override;
	glm::vec3 getRight() const override;

	float getZNear() const override;
	void setZNear(float zNear) override;
	float getZFar() const override;
	void setZFar(float zFar) override;
	float getFov() const override;
	void setFov(float fov) override;
	int getWidth() const override;
	int getHeight() const override;
};
} // namespace Vp
