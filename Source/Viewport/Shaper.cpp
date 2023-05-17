#include "Shaper.h"

#include "Core/Resources/Mesh.h"
#include "Core/Resources/ResourceManager.h"
#include "Utils/Color.h"

using namespace Vp;

std::string Shaper::xyzAxes;
std::string Shaper::unitLineCube;
std::string Shaper::unitCube;
std::string Shaper::plane;
std::string Shaper::screenQuad;

void Shaper::initDefaultShapes()
{
	{
		Shaper shaper;
		glm::vec3 pos = glm::vec3(0, 0, 0);
		float size = 1.0f;
		shaper.line(pos, pos + glm::vec3(1.0f, 0.0f, 0.0f) * size, Color::RED);
		shaper.line(pos, pos + glm::vec3(0.0f, 1.0f, 0.0f) * size, Color::GREEN);
		shaper.line(pos, pos + glm::vec3(0.0f, 0.0f, 1.0f) * size, Color::BLUE);
		xyzAxes = "shape_xyzAxes";
		shaper.createLineMesh(xyzAxes);
	}
	{
		glm::vec3 unitCubeArr[24] = {
		    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 1.0f, -1.0f),  glm::vec3(1.0f, 1.0f, -1.0f),
		    glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f),
		    glm::vec3(-1.0f, 1.0f, 1.0f),   glm::vec3(1.0f, 1.0f, 1.0f),   glm::vec3(1.0f, -1.0f, 1.0f),
		    glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),
		    glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),
		    glm::vec3(1.0f, -1.0f, 1.0f),   glm::vec3(-1.0f, 1.0f, 1.0f),  glm::vec3(-1.0f, -1.0f, 1.0f),
		    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, -1.0f, 1.0f),  glm::vec3(1.0f, 1.0f, -1.0f),
		    glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),
		};

		Shaper shaper;
		for (int i = 0; i < 24; i += 2)
		{
			shaper.line(glm::vec3(glm::vec4(unitCubeArr[i], 1.0f)), glm::vec3(glm::vec4(unitCubeArr[i + 1], 1.0f)));
		}
		unitLineCube = "shape_unitCube";
		shaper.createLineMesh(unitLineCube);
	}
	{
		Shaper shaper;
		glm::vec3 c1 = glm::vec3(-1, 1, -1);
		glm::vec3 c2 = glm::vec3(1, 1, -1);
		glm::vec3 c3 = glm::vec3(-1, 1, 1);
		glm::vec3 c4 = glm::vec3(1, 1, 1);
		glm::vec3 c5 = glm::vec3(-1, -1, -1);
		glm::vec3 c6 = glm::vec3(1, -1, -1);
		glm::vec3 c7 = glm::vec3(-1, -1, 1);
		glm::vec3 c8 = glm::vec3(1, -1, 1);

		shaper.quad(c2, c1, c6, c5);
		shaper.quad(c4, c3, c2, c1);
		shaper.quad(c3, c7, c1, c5);
		shaper.quad(c6, c5, c8, c7);
		shaper.quad(c4, c8, c3, c7);
		shaper.quad(c4, c2, c8, c6);

		unitCube = "shape_unitCubeFilled";
		shaper.createMesh(unitCube);
	}
	{
		Shaper shaper;
		glm::vec3 c1 = glm::vec3(-1, 1, 0);
		glm::vec3 c2 = glm::vec3(1, 1, 0);
		glm::vec3 c3 = glm::vec3(-1, -1, 0);
		glm::vec3 c4 = glm::vec3(1, -1, 0);
		shaper.quad(c1, c2, c3, c4);

		plane = "shape_plane";
		shaper.createMesh(plane);
	}
	{
		Shaper s;
		s.quad(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0), Color::GREEN);
		screenQuad = "shape_screenQuad";
		s.createMesh(screenQuad);
	}
}

void Shaper::addVector(const float x, const float y, const float z)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

void Shaper::addVector(const glm::vec3 v)
{
	vertices.push_back(v.x);
	vertices.push_back(v.y);
	vertices.push_back(v.z);
}

void Shaper::addColor(const glm::vec3 c)
{
	colors.push_back(c.x);
	colors.push_back(c.y);
	colors.push_back(c.z);
}

void Shaper::addColor(const glm::vec3 c, int count)
{
	for (int i = 0; i < count; i++)
	{
		addColor(c);
	}
}

void Shaper::reserveBuffers(const int newCapacity)
{
	vertices.reserve(newCapacity);
	colors.reserve(newCapacity);
}

void Shaper::setColor(const glm::vec3 color)
{
	currentColor[0] = color[0];
	currentColor[1] = color[1];
	currentColor[2] = color[2];
}

void Shaper::line(const glm::vec3 a, const glm::vec3 b)
{
	addVector(a.x, a.y, a.z);
	addVector(b.x, b.y, b.z);
	addColor(currentColor);
	addColor(currentColor);
}

void Shaper::line(const glm::vec3 a, const glm::vec3 b, const glm::vec3 color)
{
	addVector(a.x, a.y, a.z);
	addVector(b.x, b.y, b.z);
	addColor(color);
	addColor(color);
}

void Shaper::line(const glm::vec3 a, const glm::vec3 b, const glm::vec3 color_a, const glm::vec3 color_b)
{
	addVector(a.x, a.y, a.z);
	addVector(b.x, b.y, b.z);
	addColor(color_a);
	addColor(color_b);
}

void Shaper::quad(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 d)
{
	quad(a, b, c, d, currentColor);
}

void Shaper::quad(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 d, const glm::vec3 color)
{
	// Bottom left tris
	addVector(a);
	addVector(c);
	addVector(d);
	addColor(color, 3);

	// Top right tris
	addVector(a);
	addVector(d);
	addVector(b);
	addColor(color, 3);
}

void Shaper::lineBox(glm::vec3 from, glm::vec3 to) {
	glm::vec3 unitCubeArr[24] = {
	    glm::vec3(to.x, to.y, to.z),    glm::vec3(to.x, to.y, from.z),  glm::vec3(to.x, to.y, from.z),
	    glm::vec3(from.x, to.y, from.z),  glm::vec3(from.x, to.y, from.z), glm::vec3(from.x, to.y, to.z),
	    glm::vec3(from.x, to.y, to.z),   glm::vec3(to.x, to.y, to.z),   glm::vec3(to.x, from.y, to.z),
	    glm::vec3(to.x, from.y, from.z),  glm::vec3(to.x, from.y, from.z), glm::vec3(from.x, from.y, from.z),
	    glm::vec3(from.x, from.y, from.z), glm::vec3(from.x, from.y, to.z), glm::vec3(from.x, from.y, to.z),
	    glm::vec3(to.x, from.y, to.z),   glm::vec3(from.x, to.y, to.z),  glm::vec3(from.x, from.y, to.z),
	    glm::vec3(to.x, to.y, to.z),    glm::vec3(to.x, from.y, to.z),  glm::vec3(to.x, to.y, from.z),
	    glm::vec3(to.x, from.y, from.z),  glm::vec3(from.x, to.y, from.z), glm::vec3(from.x, from.y, from.z),
	};
	for (int i = 0; i < 24; i += 2)
	{
		line(glm::vec3(glm::vec4(unitCubeArr[i], 1.0f)), glm::vec3(glm::vec4(unitCubeArr[i + 1], 1.0f)), currentColor);
	}
}

void Shaper::clear()
{
	vertices.clear();
	colors.clear();
}

const std::vector<float>& Shaper::getVertices() const { return vertices; }

const std::vector<float>& Shaper::getColors() const { return colors; }

Core::Mesh* Shaper::createLineMesh(const std::string& alias)
{
	if (getVertices().empty() || getColors().empty())
	{
		throw std::runtime_error("Shaper: Can't create line mesh with 0 vertices / colors!");
	}

	Core::Mesh* mesh;
	if (alias.empty())
	{
		mesh = Core::Mesh::create(Core::Mesh::LINES, &getVertices()[0], getVertices().size() / 3, &getColors()[0],
		                          getColors().size() / 3);
	}
	else
	{
		mesh = RMI.mesh(alias, Core::Mesh::LINES, &getVertices()[0], getVertices().size() / 3, &getColors()[0],
		                getColors().size() / 3);
	}
	return mesh;
}

Core::Mesh* Shaper::createMesh(const std::string& alias)
{
	if (getVertices().empty() || getColors().empty())
	{
		throw std::runtime_error("Shaper: Can't create mesh with 0 vertices / colors!");
	}

	Core::Mesh* mesh;
	if (alias.empty())
	{
		mesh = Core::Mesh::create(Core::Mesh::TRIANGLES, &getVertices()[0], getVertices().size() / 3, &getColors()[0],
		                          getColors().size() / 3);
	}
	else
	{
		mesh = RMI.mesh(alias, Core::Mesh::TRIANGLES, &getVertices()[0], getVertices().size() / 3, &getColors()[0],
		                getColors().size() / 3);
	}
	return mesh;
}
