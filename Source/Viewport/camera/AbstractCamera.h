#pragma once

#include <chrono>

#include "Viewport/entity/GameObject.h"
#include <glm/glm.hpp>

// TODO: (DR) Perhaps add option to use orthographic projection
// TODO: (DR) Cameras should have an ability to recieve a view matrix and reconstruct parameters from it, or force using
// it for some time, would allow view changing using buttons or interpolated view rotation.

namespace Vp
{
/**
 * Abstract camera base class.
 * Holds a view matrix and projection information.
 * Also contains further information about its view space (position, direction, up and right vectors)
 */
class AbstractCamera
{
public:
	enum class Viewpoint
	{
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		FRONT,
		BACK
	};

protected:
	glm::mat4 m_view{1.0f};
	glm::mat4 m_projection{1.0f};

	int m_width{200};  ///< Camera resolution width in pixels
	int m_height{200}; ///< Camera resolution height in pixels

	float m_zNear = 0.2f;  ///< Distance to the near clipping pane
	float m_zFar = 270.0f; ///< Distance to the far clipping pane
	float m_fov = 90.0f;   ///< Vertical field of vision in degrees

	// Read only view space information updated in the update() method
	glm::vec3 m_position = glm::vec3(3.0f, 0.0f, 0.0f);
	glm::vec3 m_direction = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_right = glm::vec3(0.0f, 1.0f, 0.0f);

	std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> interpolationStart{};
	double interpolationPeriod{0.14f};
	glm::mat4 interpolationFrom;
	glm::mat4 interpolationTo;

public:
	virtual ~AbstractCamera() = default;

	/**
	 * Sets the camera resolution in pixels.
	 * Should be called prior to the Camera::update() method.
	 * @param width Screen width in pixels
	 * @param height Screen height in pixels
	 */
	virtual void size(int width, int height);

	/**
	 * Recalculates the view and projection matrices based on the internal camera state.
	 * Generally this method should build up a new view matrix from scratch using some arbitrary internal variables.
	 * From the calculated view matrix, direction, up, right and position information should be updated.
	 */
	virtual void update() = 0;

	/**
	 * Updates the internal camera state with user input.
	 * @param mousePos Mouse position relative to the window size.
	 * @param windowSize The window size in pixels.
	 */
	virtual void processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize) = 0;

	/**
	 * @return The camera view matrix
	 */
	virtual glm::mat4 getView() const;

	/**
	 *
	 * @return The camera projection matrix
	 */
	virtual glm::mat4 getProjection() const;

	/**
	 * Moves the camera to the specified viewpoint. Subclasses need to provide their own implementations.
	 * @param viewpoint
	 */
	virtual void viewpoint(AbstractCamera::Viewpoint viewpoint);

	/**
	 * Moves camera so that all objects in the scene are visible.
	 */
	virtual void centerOnScene(const Scene& scene);

	/**
	 * Moves camera so that all selected objects in the scene are visible.
	 */
	virtual void centerOnSelection(const Scene& scene);

	/**
	 * Moves camera so that the specified objects are visible.
	 */
	virtual void centerOnObjects(const std::vector<const GameObject*> objects);

	/**
	 * Moves camera so that the specified axis aligned bounding box is visible.
	 */
	virtual void centerOnBox(glm::vec3 boxMin, glm::vec3 boxMax, bool interpolate);

	/**
	 * Begin interpolation from one view matrix to another, the interpolation will take m_interpolationPeriod seconds in
	 * realtime. During that period the getView() method will be returning the interpolated view matrix rather than the
	 * one actually stored. This interpolation is independent from any internal parameters of the camera and does not
	 * modify it.
	 * @param from Start view matrix
	 * @param to End view matrix
	 */
	void interpolate(glm::mat4 from, glm::mat4 to);

	/**
	 * Checks whether interpolation between view matrices is currently happening.
	 * If interpolating, the progress argument is set to the current progress in range <0, 1>.
	 * @param progress Reference to which progress is written.
	 * @return true if interpolating, false otherwise
	 */
	bool isInterpolating(float& progress) const;

	virtual int getWidth() const;
	virtual int getHeight() const;

	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getDirection() const;
	virtual glm::vec3 getUp() const;
	virtual glm::vec3 getRight() const;

	virtual float getZNear() const;
	virtual void setZNear(float zNear);
	virtual float getZFar() const;
	virtual void setZFar(float zFar);
	virtual float getFov() const;
	virtual void setFov(float fov);

protected:
	glm::mat4 createProjectionMatrix(bool nonShrinking) const;
	std::vector<glm::vec3> createBoundingBoxWorldPoints(glm::vec3 boxMin, glm::vec3 boxMax, glm::mat4 modelMatrix);
};

} // namespace Vp
