#pragma once

#include <chrono>

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
class ICamera
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
	virtual ~ICamera() = default;

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
	virtual const glm::mat4 getView() const;

	/**
	 *
	 * @return The camera projection matrix
	 */
	virtual const glm::mat4 getProjection() const;

	virtual void viewpoint(ICamera::Viewpoint viewpoint);

	void interpolate(glm::mat4 from, glm::mat4 to);
	bool isInterpolating(float& progress) const;

	virtual int getWidth() const;
	virtual int getHeight() const;

	virtual const glm::vec3 getPosition() const;
	virtual const glm::vec3 getDirection() const;
	virtual const glm::vec3 getUp() const;
	virtual const glm::vec3 getRight() const;

	virtual float getZNear() const;
	virtual void setZNear(float zNear);
	virtual float getZFar() const;
	virtual void setZFar(float zFar);
	virtual float getFov() const;
	virtual void setFov(float fov);

protected:
	glm::mat4 createProjectionMatrix(bool nonShrinking) const;
};

} // namespace Vp
