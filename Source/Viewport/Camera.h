#pragma once

#include "pgr.h"

namespace Vp
{
/**
 * The camera is a simple container for the view and projection matrices.
 * It can calculate its own view and projection matrices using the update()
 * method. The matrices are generated using usual camera parameters which can
 * also be updated dynamically on user input if the processInput() method is
 * called.
 *
 * The camera has two modes:
 * 	- Rotate and pan around a pivot point
 * 	- First person mode // TODO: (DR) Either remove or actually properly implement
 */
class Camera
{
public:
	// Matrices
	glm::mat4 m_view;
	glm::mat4 m_projection;

private:
	// Controls
	bool fpsMode = false;

	int width{200};
	int height{200};

	float zNear = 0.2f;
	float zFar = 270.0f;
	float fov = 90.0f;

	glm::vec3 pivot = glm::vec3(0.0f, 0.0f, 0.0f);

	float radius = 7.0f;
	float rotationX = 0.0f;
	float rotationY = 30.0f;

	float dScroll = 0.0f;
	float zoomSpeed = 0.8f;
	float rotateSpeed = 0.29f;
	float translateSpeed = 0.02f;

	bool isRotating = false;
	bool rotateStartedUpsideDown = false;

	float fpsTranslateSpeed = 0.05f;
	float fpsMouseWheelSpeedDelta = 0.001f;
	float fpsRotateSpeed = 0.2f;
	float fpsSpeedBoostMultiplier = 3.0f;

	// Info
	glm::vec3 position = glm::vec3(3.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right = glm::vec3(0.0f, 1.0f, 0.0f);

public:
	/**
	 * Creates the camera with all default values.
	 * Needs to be supplied with view and projection matrices or these
	 * matrices need to be calculated by calling size() and update().
	 */
	Camera() = default;

	/**
	 * Creates the camera from supplied matrices.
	 * @param view
	 * @param projection
	 */
	Camera(glm::mat4 view, glm::mat4 projection);

	/**
	 * Creates the camera in pivot mode using supplied arguments.
	 * @param width Width in pixels
	 * @param height  Height in pixels
	 * @param pivot Pivot point position
	 * @param radius Distance between camera and the pivot point
	 */
	Camera(int width, int height, glm::vec3 pivot, float radius);

	/**
	 * Sets the camera resolution in pixels.
	 */
	void size(int width, int height);

	/**
	 * Recalculates the view and projection matrices based on the internal camera
	 * state.
	 */
	void update();

	/**
	 * Updates the internal camera state with user input.
	 */
	void processInput(glm::vec2 mousePos, glm::ivec2 windowSize);

private:
	void mouseDrag(float dx, float dy, bool left, bool middle);
	// void mouseMoved(int x, int y);
	void mouseWheel(float scroll);

	void keyboard(bool w, bool s, bool a, bool d, bool shift);

	glm::mat4 createProjectionMatrix(bool nonShrinking);

public:
	void enableFpsMode(bool b);
	void toggleFpsMode();

	// Getters and setters
	glm::vec3 getPivot() const;
	void setPivot(const glm::vec3& pivot);
	float getZNear() const;
	void setZNear(float zNear);
	float getZFar() const;
	void setZFar(float zFar);
	float getFov() const;
	void setFov(float fov);
	void setRadius(float radius);
	float getRadius() const;
	float getRotationX() const;
	void setRotationX(float rotationX);
	float getRotationY() const;
	void setRotationY(float rotationY);
};
} // namespace Vp