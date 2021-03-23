/**
 * \file	camera.h
 *
 * Class for representing renderer camera projection and viewport data
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Config.h"
#include "Core/Input/InputManager.h"
#include "Logger/LoggerInternal.h"

/**
 * \brief Storage for lookAt parameters (position, center and up vector)
 */
class ViewPort final
{
protected:
	glm::vec3 position; ///< camera position
	glm::vec3 lookAt;   ///< camera center - point camera looks at
	glm::vec3 up;       ///< up vector

public:
	ViewPort() = default;
	;

	ViewPort(const glm::vec3 position, const glm::vec3 lookAt, const glm::vec3 up)
	{
		this->position = position;
		this->lookAt = lookAt;
		this->up = up;
	}

	void setTrans(glm::mat4 trans)
	{
		position = glm::vec3(trans[3]);
		up = glm::vec3(trans[1]);
		lookAt = position + glm::vec3(trans[2]);
	}

	void setPosition(const glm::vec3 pos) { this->position = pos; }

	void setPosition(const float x, const float y, const float z) { this->position = glm::vec3(x, y, z); }

	glm::vec3 getPosition() const { return position; }

	void setLookAt(const glm::vec3 lat) { this->lookAt = lat; }

	void setLookAt(const float x, const float y, const float z) { this->lookAt = glm::vec3(x, y, z); }

	/**
	 * \brief Get the center (point camera looks at)
	 * \return Point camera looks at
	 */
	glm::vec3 getLookAt() const { return lookAt; }

	void setUp(const glm::vec3 vup) { this->up = vup; }

	void setUp(const float x, const float y, const float z) { this->up = glm::vec3(x, y, z); }

	glm::vec3 getUp() const { return up; }

	glm::vec3 getDirection() const { return glm::normalize(lookAt - position); }
};

/**
 * \brief Camera viewport, FOV, lerp state, and view and projection matrices
 */
class Camera final
{
private:
	// lerping data
	ViewPort* lerpViewPort;
	float lerpAlpha;    ///< current lerp parameter
	float lerpDelta;    ///< lerpAlha increment
	bool continualLerp; ///< true while camera is lerping

	// old camera position
	glm::vec3 backupPosition;
	glm::vec3 backupLookAt;
	glm::vec3 backupUp;

	// current camera position and field of view
	glm::vec3 position; ///< camera position - eye
	glm::vec3 lookAt;   ///< camera center - point camera looks at
	glm::vec3 up;       ///< up vector
	float fov;          ///< camera field of view

	glm::mat4 view;       ///< view matrix V = E^(-1)
	glm::mat4 projection; ///< projection matrix P
	glm::mat4 combined;   ///< projection * view

	glm::mat4 trackballRotationSum;

	void lerp();

public:
	Camera()
	{
		lerpViewPort = nullptr;
		lerpAlpha = 0.0f;
		lerpDelta = 0.0f;
		continualLerp = false;

		position = glm::vec3(0.0f, 0.0f, 10.0f);
		lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);

		fov = 70.0f;

		view = glm::lookAt(position, lookAt, up);

		trackballRotationSum = glm::mat4();
	}

	Camera(const glm::vec3 _pos, const glm::vec3 _up, const glm::vec3 _lookAt)
	{
		lerpViewPort = nullptr;
		lerpAlpha = 0.0f;
		lerpDelta = 0.0f;
		continualLerp = false;

		position = _pos;
		lookAt = _lookAt;
		up = _up;

		fov = 70.0f;

		view = glm::lookAt(position, lookAt, up);

		trackballRotationSum = glm::mat4();
	}

	Camera(ViewPort* port)
	{
		lerpViewPort = nullptr;
		lerpAlpha = 0.0f;
		lerpDelta = 0.0f;

		position = port->getPosition();
		lookAt = port->getLookAt();
		up = port->getUp();

		continualLerp = false; // PF
		fov = 70.0f;           // PF

		view = glm::lookAt(position, lookAt, up);
	}

	void setContinualLerp(bool enable) { continualLerp = enable; }

	glm::vec3 getUpVector() const { return up; }

	glm::vec3 getRealUpVector() const { return glm::cross(getDirection(), getSide()); }

	/**
	 * \brief Get the camera position.
	 * \return point the camera position (eye)
	 */
	glm::vec3 getPosition() const { return position; }

	/**
	 * \brief Get the camera lookAt point.
	 * \return point the camera looks at (center)
	 */
	glm::vec3 getLookAt() const { return lookAt; }

	/**
	 * \brief Get camera direction
	 * \return Direction of the camera (normalized vector from camera position to the center), i.e., camera z-
	 */
	glm::vec3 getDirection() const { return glm::normalize(lookAt - position); }

	/**
	 * \brief Get the camera right vector
	 * \return Vector of camera x
	 */
	glm::vec3 getSide() const { return glm::normalize(glm::cross(getDirection(), up)); }

	void setPosition(const float x, const float y, const float z) { position = glm::vec3(x, y, z); }

	void setPosition(const glm::vec3 vec) { position = vec; }

	void setLookAt(const float x, const float y, const float z)
	{
		lookAt.x = x;
		lookAt.y = y;
		lookAt.z = z;
	}

	void setLookAt(glm::vec3 vec) { lookAt = vec; }

	void setUp(const float x, const float y, const float z)
	{
		up.x = x;
		up.y = y;
		up.z = z;
	}

	void setUp(const glm::vec3 vec) { up = vec; }

	/**
	 * \brief Set parallel projection of given width and height
	 * \param width   Width
	 * \param height  Height
	 * \param near    Not used, set to 1.0
	 * \param far     Not used, set to 100.0
	 */
	void setOrtho(const int width, const int height, int near, int far)
	{
		const float wh = (float)width / 2.0f;
		const float hh = (float)height / 2.0f;
		const float n = 1.0f;
		const float f = 100.0f;

		projection = glm::ortho(-wh, wh, -hh, hh, n, f);
	}

	void setFov(const float _fov) { fov = _fov; }

	float getFov() const { return fov; }

	void setPerspective(const float _fov, const float width, const float height)
	{
		projection = glm::perspective(_fov, width / height, Config::CAM_NEAR, Config::CAM_FAR);
		this->fov = _fov;
	}

	void setPerspective(const float width, const float height)
	{
		projection = glm::perspective(fov, width / height, Config::CAM_NEAR, Config::CAM_FAR);
	}

	// matrices
	glm::mat4 getCombined() const { return combined; }

	glm::mat4 getProjection() const { return projection; }

	glm::mat4 getView() const { return view; }

	/// special translation.
	/// translation perpendicular to the view direction (translation to the left)
	// void strafe(const float delta)
	//{
	//  //std::cout << "strafe" << std::endl;
	//  glm::vec3 dir = lookAt - position; // same as getDirection()
	//  glm::vec3 d = delta * glm::normalize(glm::cross(up, dir));
	//  position = position + d;
	//  lookAt = lookAt + d;
	//}

	/**
	 * \brief  Translate camera in the view direction about delta
	 * Translates both - camera position (eye) and camera lookAt (center)
	 *
	 * \param  delta step in camera view direction
	 */
	void pedal(const float delta)
	{
		const glm::vec3 dir = delta * glm::normalize(lookAt - position);

		// const float len = glm::length(lookAt - position);
		// const glm::vec3 dir = delta * (lookAt - position) / len;

		position = position + dir;
		lookAt = lookAt + dir;
	}

	// void WSAD(const Keys::Code forward, const Keys::Code back, const Keys::Code left, const Keys::Code right,
	//          const float delta)
	//{
	//  if (InputController::isKeyPressed(forward))
	//  {
	//    pedal(delta);
	//  }
	//  if (InputController::isKeyPressed(back))
	//  {
	//    pedal(-delta);
	//  }
	//  if (InputController::isKeyPressed(left))
	//  {
	//    strafe(delta);
	//  }
	//  if (InputController::isKeyPressed(right))
	//  {
	//    strafe(-delta);
	//  }
	//  //cout << "CAM_MOTION_SENSITIVITY : " << Config::CAM_MOTION_SENSITIVITY << std::endl;
	//}

	// LERPING
	void lerpTo(ViewPort* port, const bool continual)
	{
		this->continualLerp = continual;

		lerpViewPort = port;
		lerpAlpha = 0.0f;
		lerpDelta = 1 / (Config::CAM_LERP_DISTTOSTEP_KOEF * glm::distance(port->getPosition(), position));

		if (lerpDelta > 0.1f)
		{
			lerpDelta = 0.1f;
		}

		backupPosition = position;
		backupLookAt = lookAt;
		backupUp = up;
	}

	bool isLerping() const { return lerpViewPort != nullptr; }

	// rotations
	/*
	void rotateYCenter(float angle) {
	  //glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	  //position = glm::vec3(R * glm::vec4(position, 1.0f));
	}
	*/

	/**
	 * \brief Rotate camera direction around camera up vector (turning left - right)
	 * \param angle Rotation angle around upVector
	 */
	void rotateY(const float angle)
	{
		glm::vec3 dir = lookAt - position;
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle, up);
		dir = glm::vec3(R * glm::vec4(dir, 1.0));

		lookAt = position + dir;
	}

	/**
	 * \brief Tilt the camera - rotate around camera X-axis
	 * \param angle Rotation angle around camera X
	 */
	void rotateX(const float angle)
	{
		glm::vec3 dir = lookAt - position;
		glm::vec3 cross = glm::cross(up, dir);
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle, cross);
		dir = glm::vec3(R * glm::vec4(dir, 1.0));

		lookAt = position + dir;
	}

	/**
	 * \brief Roll the camera - rotate around the view direction
	 * \param angle
	 */
	void roll(const float angle)
	{
		const glm::vec3 dir = glm::normalize(lookAt - position);
		const glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle, dir);
		up = glm::vec3(R * glm::vec4(up, 1.0));
	}

	// freeLook
	void freeLook()
	{
		if (InputManager::m_mouseX != (float)Config::WIN_WIDTH / 2)
		{
			const float angleDelta = Config::MOUSE_SENSITIVITY * (InputManager::m_mouseX - (float)Config::WIN_WIDTH / 2);
			rotateY((float)-angleDelta);
		}
		if (InputManager::m_mouseY != (float)Config::WIN_HEIGHT / 2)
		{
			const float angleDelta =
					Config::MOUSE_SENSITIVITY * (InputManager::m_mouseY - (float)Config::WIN_HEIGHT / 2);
			rotateX((float)angleDelta);
		}
	}

	glm::vec3 snapToSphere(const float radius, const float x, const float y) const
	{
		float d, z;
		d = sqrt(x * x + y * y);

		if (d < radius) /* Inside sphere */
			z = sqrt(radius * radius - d * d);
		else
			z = 0;

		return radius * glm::normalize(glm::vec3(x, y, (float)z));
	}

	glm::vec3 projectToSphere(const float radius, const float x, const float y) const
	{
		double d, t, z;

		d = sqrt(x * x + y * y);
		if (d < radius * 0.70710678118654752440)
		{
			/* Inside r*cos45� circle -> project to sphere */
			z = sqrt(radius * radius - d * d);
		}
		else
		{
			/* Outside the circle - project to hyperbola */
			t = radius / 1.41421356237309504880;
			z = t * t / d;
		}

		return radius * glm::normalize(glm::vec3(x, y, (float)z));
	}

	// trackball
	void trackball(glm::vec3 center, float sens)
	{
		glm::vec2 end = glm::vec2((float)InputManager::m_mouseX / (float)Config::WIN_WIDTH,
		                          (float)InputManager::m_mouseY / (float)Config::WIN_HEIGHT) *
		                    2.0f -
		                1.0f;
		glm::vec2 start = glm::vec2((float)InputManager::m_mouseXPrev / (float)Config::WIN_WIDTH,
		                            (float)InputManager::m_mouseYPrev / (float)Config::WIN_HEIGHT) *
		                      2.0f -
		                  1.0f;

		if (start != end)
		{
			glm::vec3 p1 = projectToSphere(1.0f, start.x, -start.y);
			glm::vec3 p2 = projectToSphere(1.0f, end.x, -end.y);

			glm::vec3 axis = glm::normalize(glm::cross(p1, p2));

			/* Figure out how much to rotate around that axis. */
			glm::vec3 d = p1 - p2; // points p1, p2 on the unit circle

			double t = glm::length(d) / 2.0f; // (2.0 * trackballScreenRadius);

			/* Avoid problems with out-of-control values of sinus...s */
			if (t > 1.0)
				t = 1.0;
			else if (t < -1.0)
				t = -1.0;

			// angle = float(RADTODEG(2.0f * asin(t))); /* how much to rotate about axis (in degrees) */
			auto angle = float(2.0f * asin(t)); /* how much to rotate about axis (in radians) */

			glm::mat4 trackBallRotation = glm::rotate(glm::mat4(1.0f), angle * Config::MOUSE_SENSITIVITY * 1000.0f, axis);

			glm::mat4 viewRotation = glm::inverse(view); // INVERSE!!! - we need E, not E^(-1)
			glm::mat4 viewTranslation = glm::mat4(1.0f);
			viewTranslation[3] = viewRotation[3];
			viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // without translation
			glm::mat4 viewRotationInv = view;
			viewRotationInv[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // without translation
			glm::mat4 TC = glm::mat4(1.0);
			TC[3] = view * glm::vec4(lookAt, 1.0f); // camera center in camera space
			glm::mat4 nv = glm::inverse(TC * trackBallRotation * glm::inverse(TC) * view);
			trackballRotationSum = viewRotation * trackBallRotation * viewRotationInv * trackballRotationSum;

			// new view matrix parameters
			position = glm::vec3(nv[3]);
			up = glm::vec3(nv[1]);
			lookAt = center;
		}
	}

	void moveOrbitCenter(const Keys::Code forward, const Keys::Code back, const Keys::Code left, const Keys::Code right,
	                     const float delta, glm::vec3& center) const
	{
		if (InputManager::isKeyPressed(forward))
		{
			center += up * delta;
		}
		if (InputManager::isKeyPressed(back))
		{
			center -= up * delta;
		}
		if (InputManager::isKeyPressed(left))
		{
			center -= getSide() * delta;
		}
		if (InputManager::isKeyPressed(right))
		{
			center += getSide() * delta;
		}
		// cout << "CAM_MOTION_SENSITIVITY : " << Config::CAM_MOTION_SENSITIVITY << std::endl;
	}

	void moveOrbitCenter(const float dx, const float dy, const float delta, glm::vec3& center)
	{
		const glm::vec3 d = (getRealUpVector() * dy + getSide() * dx) * delta * glm::length(position);

		center += d;
		position += d;
		lookAt += d;
	}

	void orbit(const glm::vec3 center)
	{
		glm::vec3 toCam = position - center;

		if (InputManager::m_mouseY != Config::WIN_HEIGHT / 2)
		{
			// const float angleDelta = Config::MOUSE_SENSITIVITY * (-InputController::mouseY + Config::WIN_HEIGHT / 2);
			const float angleDelta = Config::MOUSE_SENSITIVITY * -InputManager::m_mouseYDelta;

			toCam = glm::rotate(toCam, angleDelta, getSide());

			glm::vec3 rUp = glm::cross(-glm::normalize(toCam), -getSide());
			if (glm::dot(rUp, up) < 0.0f)
			{
				const float len = glm::length(toCam);

				toCam = glm::dot(up, toCam) * up;

				toCam = glm::normalize(toCam) * len;
				toCam = glm::rotate(toCam, angleDelta > 0 ? -0.001f : 0.001f, getSide());
			}
		}

		if (InputManager::m_mouseX != (float)Config::WIN_WIDTH / 2)
		{
			// const float angleDelta = Config::MOUSE_SENSITIVITY * (InputController::mouseX - Config::WIN_WIDTH / 2);
			const float angleDelta = Config::MOUSE_SENSITIVITY * InputManager::m_mouseXDelta;

			toCam = glm::rotate(toCam, -angleDelta, up);
		}

		position = toCam + center;
		lookAt = center;
	}

	/**
	 * \brief Update the view and combined matrices
	 */
	void update()
	{
		if (lerpViewPort != nullptr)
			lerp();

		view = glm::lookAt(position, lookAt, up);
		combined = projection * view;
	}
};
