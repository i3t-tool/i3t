#pragma once

#include <string>
#include <vector>

#include "pgr.h"

namespace Core
{
class Mesh;
}

/**
 * \brief A utility class for constructing simple shapes.
 *
 * Used in Viewport for drawing of world axes, axes of models and wireframe
 * objects (such as projection frustrum)
 */
class Shaper
{
public:
	static std::string xyzAxes;
	static std::string unitCube;
	static std::string unitCubeFilled;

	static void initDefaultShapes();

private:
	glm::vec3 currentColor{0.0f}; ///< variable for storage of current draw color

	std::vector<float> vertices; ///< XYZ points prepared for copying to buffers and drawing
	std::vector<float> colors;   ///< RGB colors prepared for copying to buffers and

	void addVector(const float x, const float y, const float z);
	void addVector(const glm::vec3 v);

	void addColor(const glm::vec3 c);
	void addColor(const glm::vec3 c, int count);

public:
	void reserveBuffers(const int newCapacity);

	/**
	 * \brief Set the current drawing color.
	 * \param color New drawing color "currentColor"
	 */
	void setColor(const glm::vec3 color);

	/**
	 * \brief Prepare geometry for line from \a a to \a b using the single global
	 * color \a currentColor \param a point from \param b point to
	 */
	void line(const glm::vec3 a, const glm::vec3 b);

	/**
	 * \brief Prepare geometry for line from \a a to \a b using the single color
	 * \a color for the whole line (both line ends) \param a Start point \param b
	 * End point \param color Line color
	 */
	void line(const glm::vec3 a, const glm::vec3 b, const glm::vec3 color);

	/**
	 * \brief Prepare geometry for line from \a a to \a b using the single color
	 * \a color for the whole line (both line ends) \param a Start point \param b
	 * End point \param color_a Start-point color \param color_b End-point color
	 */
	void line(const glm::vec3 a, const glm::vec3 b, const glm::vec3 color_a, const glm::vec3 color_b);

	/**
	 * Creates a quad with two triangles. Triangles are CCW acd and adb.
	 * a --- b
	 * |  \  |
	 * c --- d
	 *
	 * @param a Top left corner
	 * @param b Top right corner
	 * @param c Bottom left corner
	 * @param d Bottom right corner
	 */
	void quad(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 d);

	/**
	 * Creates a quad with two triangles. Triangles are CCW acd and adb.
	 * a --- b
	 * |  \  |
	 * c --- d
	 *
	 * @param a Top left corner
	 * @param b Top right corner
	 * @param c Bottom left corner
	 * @param d Bottom right corner
	 * @param color Color of the vertices
	 */
	void quad(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 d, const glm::vec3 color);

	/**
	 * \brief Clear prepared geometry (vertices and colors).
	 */
	void clear();

	Core::Mesh* createLineMesh(const std::string& alias);
	Core::Mesh* createMesh(const std::string& alias);

public:
	const std::vector<float>& getVertices() const;
	const std::vector<float>& getColors() const;
};