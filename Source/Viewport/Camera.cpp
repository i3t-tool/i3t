#include "Camera.h"

#include "Core/Input/InputManager.h"
#include "Logger/Logger.h"

#include "imgui.h"

using namespace Vp;

Camera::Camera(glm::mat4 view, glm::mat4 projection)
{
	this->m_view = view;
	this->m_projection = projection;
}

Camera::Camera(int width, int height, glm::vec3 pivot, float radius)
{
	this->pivot = pivot;
	this->radius = radius;

	this->width = width;
	this->height = height;
}

void Camera::size(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Camera::update()
{
	glm::mat4 cameraTransform = glm::mat4(1.0f);

	if (fpsMode)
	{
		cameraTransform = glm::rotate(cameraTransform, glm::radians(rotationX),
		                              glm::vec3(0.0f, 1.0f, 0.0f));
		cameraTransform = glm::rotate(cameraTransform, glm::radians(rotationY),
		                              glm::vec3(0.0f, 0.0f, 1.0f));

		const glm::vec4 cameraDir = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0);
		const glm::vec4 cameraUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0);
		const glm::vec3 cameraDirTransformed =
		    glm::vec3(cameraTransform * cameraDir);
		const glm::vec3 cameraUpTransformed = glm::vec3(cameraTransform * cameraUp);
		const glm::vec3 cameraCenter = position + cameraDirTransformed;

		// No parent transform, replaced with identity matrices //TODO: (DR) This is
		// redundant
		const glm::vec3 cameraPosWorld =
		    glm::vec3(glm::mat4(1) * glm::vec4(position, 1.0));
		const glm::vec3 cameraCenterWorld =
		    glm::vec3(glm::mat4(1) * glm::vec4(cameraCenter, 1.0));
		const glm::vec3 cameraUpWorld =
		    glm::vec3(glm::mat4(1) * glm::vec4(cameraUpTransformed, 0.0));

		m_projection = createProjectionMatrix(false);
		m_view = glm::lookAt(cameraPosWorld, cameraCenterWorld, cameraUpWorld);

		up = glm::normalize(cameraUpTransformed);
		direction = glm::normalize(cameraDirTransformed);
		right = glm::cross(up, direction);
	}
	else
	{
		cameraTransform = glm::translate(cameraTransform, pivot);
		cameraTransform = glm::rotate(cameraTransform, glm::radians(rotationX),
		                              glm::vec3(0.0f, 1.0f, 0.0f));
		cameraTransform = glm::rotate(cameraTransform, glm::radians(rotationY),
		                              glm::vec3(0.0f, 0.0f, 1.0f));
		cameraTransform =
		    glm::translate(cameraTransform, glm::vec3(radius, 0.0f, 0.0f));

		const glm::vec4 cameraPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0);
		const glm::vec4 cameraUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0);
		const glm::vec3 cameraPosTransformed =
		    glm::vec3(cameraTransform * cameraPos);
		const glm::vec3 cameraUpTransformed = glm::vec3(cameraTransform * cameraUp);

		const glm::vec3 cameraPosWorld =
		    glm::vec3(glm::mat4(1) * glm::vec4(cameraPosTransformed, 1.0));
		const glm::vec3 cameraPivotWorld =
		    glm::vec3(glm::mat4(1) * glm::vec4(pivot, 1.0));
		const glm::vec3 cameraUpWorld =
		    glm::vec3(glm::mat4(1) * glm::vec4(cameraUpTransformed, 0.0));

		m_projection = createProjectionMatrix(true);

		m_view = glm::lookAt(cameraPosWorld, cameraPivotWorld, cameraUpWorld);

		position = cameraPosTransformed;
		up = glm::normalize(cameraUpTransformed);
		direction = glm::normalize(pivot - glm::vec3(cameraPosTransformed));
		right = glm::cross(up, direction);
	}
}

glm::mat4 Camera::createProjectionMatrix(bool nonShrinking)
{
	if (nonShrinking)
	{
		// Non shrinking resizing
		// Method 1
		float scale = atan(glm::radians(fov)) * zNear;
		float l = -scale;
		float r = scale;
		float b = -scale;
		float t = scale;
		float aspect = std::min(width, height) / (float)std::max(width, height);
		if (width > height)
		{
			t *= aspect;
			b *= aspect;
		}
		else
		{
			l *= aspect;
			r *= aspect;
		}
		return glm::frustum(l, r, b, t, zNear, zFar);

		//		// Method 2
		//		if (width > height) {
		//			glm::mat4 projection = glm::perspective(glm::radians(fov), height /
		//(float)width, zNear, zFar); 			float tmp = projection[0][0]; 			projection[0][0]
		//= projection[1][1]; 			projection[1][1] = tmp; 			return projection; 		} else {
		//			return glm::perspective(glm::radians(fov), width / (float)height,
		//zNear, zFar);
		//		}
	}
	else
	{
		return glm::perspective(glm::radians(fov), width / (float)height, zNear,
		                        zFar);
	}
}

void Camera::processInput()
{
	float dx = InputManager::m_mouseXDelta;
	float dy = InputManager::m_mouseYDelta;

	if (InputManager::m_mouseWheelOffset != 0)
	{
		dScroll = InputManager::m_mouseWheelOffset;
	}

	// TODO: (DR) Make pan speed dependent on window size

	mouseDrag(dx, dy, InputManager::isAxisActive("rotate"),
	          InputManager::isAxisActive("pan"));
	mouseWheel(dScroll);

	dScroll *= 0.88f; // TODO: (DR) Scroll smoothing is fps dependent
	if (dScroll * dScroll < 0.0005f)
	{
		dScroll = 0.0f;
	}
}

void Camera::mouseDrag(float dx, float dy, bool rotate, bool pan)
{
	// Rotate
	if (rotate)
	{
		if (fpsMode)
		{
			rotationX += -dx * fpsRotateSpeed;
			rotationY += dy * fpsRotateSpeed;
		}
		else
		{
			// The code below flips the horizontal rotate direction when the camera
			// turns "upside-down" That is, when vertical rotation is in the range of >
			// 90 and < 270 However in case of a continuous mouse drag the direction
			// isn't flipped until the next drag as to not suddenly change direction
			float rot = fmodf(abs(rotationY), 360.0f);
			bool upsideDown = rot > 90.0f && rot < 270.0f;
			if (rotate && !isRotating)
			{
				// Rotation just started
				rotateStartedUpsideDown = upsideDown;
			}
			if (upsideDown && rotateStartedUpsideDown)
			{
				dx *= -1;
			}
			rotationX += -dx * rotateSpeed;
			rotationY += dy * rotateSpeed;
		}
		isRotating = true;
	}
	else
	{
		isRotating = false;
	}
	// Pan
	if (pan)
	{
		glm::vec3 oldPivot = glm::vec3(pivot);

		const float ratio = radius / zNear / 100.0f;
		pivot += glm::vec3(right) * (translateSpeed * dx * ratio);
		pivot += glm::vec3(up) * (translateSpeed * dy * ratio);
	}
}

// void Camera::mouseMoved(int dx, int dy)
//{
//	/*if (fpsMode) {
//		rotationX += -dx * fpsRotateSpeed;
//		rotationY += -dy * fpsRotateSpeed;
//	}*/
//}

void Camera::mouseWheel(float scroll)
{
	if (scroll == 0)
		return;
	if (fpsMode)
	{
		fpsTranslateSpeed += fpsMouseWheelSpeedDelta * scroll;
		fpsTranslateSpeed = std::abs(fpsTranslateSpeed);
		fpsTranslateSpeed = std::max(fpsTranslateSpeed, 0.001f);
	}
	else
	{
		const float ratio = radius / zNear / 100.0f;
		radius = radius - scroll * zoomSpeed * ratio;
		if (radius < 0.01f)
		{
			radius = 0.01f;
		}
	}
}

void Camera::keyboard(bool w, bool s, bool a, bool d, bool shift)
{
	if (fpsMode)
	{
		glm::vec3 oldPos = glm::vec3(position);

		float speed = fpsTranslateSpeed;
		if (shift)
		{
			speed *= fpsSpeedBoostMultiplier;
		}
		if (w)
		{
			position += direction * speed;
		}
		if (s)
		{
			position -= direction * speed;
		}
		if (a)
		{
			position += right * speed;
		}
		if (d)
		{
			position -= right * speed;
		}
	}
}

void Camera::toggleFpsMode() { enableFpsMode(!fpsMode); }

void Camera::enableFpsMode(bool b)
{
	fpsMode = b;
	if (!fpsMode)
	{
		pivot = position + (direction * radius);
	}
}

glm::vec3 Camera::getPivot() const { return pivot; }
void Camera::setPivot(const glm::vec3& pivot) { this->pivot = pivot; }
float Camera::getZNear() const { return zNear; }
void Camera::setZNear(float zNear) { this->zNear = zNear; }
float Camera::getZFar() const { return zFar; }
void Camera::setZFar(float zFar) { this->zFar = zFar; }
float Camera::getFov() const { return fov; }
void Camera::setFov(float fov) { this->fov = fov; }
float Camera::getRadius() const { return radius; }
void Camera::setRadius(float radius) { Camera::radius = radius; }
float Camera::getRotationX() const { return rotationX; }
void Camera::setRotationX(float rotationX) { Camera::rotationX = rotationX; }
float Camera::getRotationY() const { return rotationY; }
void Camera::setRotationY(float rotationY) { Camera::rotationY = rotationY; }
