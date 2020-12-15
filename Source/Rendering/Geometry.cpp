#include "Geometry.h"

#include <iostream>

;

#include "Rendering/ShaderProvider.h"
#include "Utils/ObjReader.h"
#include "Utils/TMshReader.h"

#include "Config.h"

/**
    Constructor.
	Accepts given vertices and indices and stores them in VBOs.

    \param [in]		vertices			If non-null, the vertices.
    \param [in]		indices				If non-null, the indices.
    \param 		   	verticesCount   	Number of vertices.
    \param 		   	indicesCount		Number of indices.
    \param 		   	attribsPerVertex	Number of attributes per vertex.
 */

Geometry::Geometry(float* vertices, unsigned int* indices, GLuint verticesCount, GLuint indicesCount,
                   GLuint attribsPerVertex)
{
  //actShaderId = -1;
  actShaderId = 0; // This glCreateShader returns 0 if an error occurs creating the shader object.

  this->vertices = vertices;
  this->indices = indices;
  this->verticesCount = verticesCount;
  this->indicesCount = indicesCount;
  this->attribsPerVertex = attribsPerVertex;

  calculateAABB();

  bindVBOs();
}

Geometry::~Geometry()
{
  delete[] vertices;
  delete[] indices;

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo_vertices);
  glDeleteBuffers(1, &vbo_indices);
}

void Geometry::calculateAABB()
{
  if (verticesCount == 0)
  {
    AABBMin = glm::vec3();
    AABBMax = glm::vec3();
    return;
  }

  glm::vec3 low = glm::vec3(vertices[0], vertices[1], vertices[2]);
  glm::vec3 high = glm::vec3(vertices[0], vertices[1], vertices[2]);

  for (unsigned int i = 1; i < verticesCount; i++)
  {
    const int index = i * attribsPerVertex; // Trinagle first vertex index 

    if (vertices[index] < low.x) low.x = vertices[index];
    else
      if (vertices[index] > high.x) high.x = vertices[index];

    if (vertices[index + 1] < low.y) low.y = vertices[index + 1];
    else
      if (vertices[index + 1] > high.y) high.y = vertices[index + 1];

    if (vertices[index + 2] < low.z) low.z = vertices[index + 2];
    else
      if (vertices[index + 2] > high.z) high.z = vertices[index + 2];
  }

  AABBMin = low;
  AABBMax = high;
}


/** Bind VBO for vertices and indices and copy data there. Generate a VAO name \a vao - nothing more.*/
void Geometry::bindVBOs()
{
  // buffer for vertices
  glGenBuffers(1, &vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesCount * attribsPerVertex, vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // buffer for triangle indices - ELEMENT_ARRAY
  glGenBuffers(1, &vbo_indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesCount, indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vao);
}


void Geometry::print()
{
  std::cout << "Geometry : " << std::endl;
  std::cout << "verticesCount : " << verticesCount << std::endl;
  std::cout << "indicesCount : " << indicesCount << std::endl;
  std::cout << "attribsPerVertex : " << attribsPerVertex << std::endl;

  /*
  for (int i=0; i<6; i++) cout << vertices[i] << ", ";
  cout << std::endl;

  for (int i=0; i<6; i++) cout << indices[i] << ", ";
  cout << std::endl;
  */

  /*
  for (int i=0; i<verticesCount * attribsPerVertex; i+=attribsPerVertex) {

    cout << vertices[i+8] << ", ";
    cout << vertices[i+9] << ", ";
    cout << vertices[i+10] << ", ";
    cout << vertices[i+11] << "   :   ";

    cout << vertices[i+12] << ", ";
    cout << vertices[i+13] << ", ";
    cout << vertices[i+14] << ", ";
    cout << vertices[i+15] << ", ";
    cout << std::endl;


  }
  */
}


int GeometryMap::addGeometry(const std::string& name, const std::string& filename)
{
  if (geometries.find(name) != geometries.end())
  {
    geometries[name]->flag = true;
    return 0;
  }

  if (filename.substr(filename.find_last_of(".") + 1) == "tmsh")
  {
    Geometry* g = TMshReader::geometryFromTMsh(filename);
    if (!g) return 1;
    g->flag = true;
    geometries[name] = g;
  }
  else if (filename.substr(filename.find_last_of(".") + 1) == "obj")
  {
    Geometry* g = ObjReader::geometryFromOBJ(filename.c_str());
    if (!g) return 1;
    g->flag = true;
    geometries[name] = g;
  }
  return 0;
  // geometry was loaded correctly and flag is set to true, OR the geometry file format was different and the geometry was not loaded and the flag is set to false or not changed at all
}

int GeometryMap::addGeometry(const std::string& name, const std::string& filename, const float scale)
{
  if (geometries.find(name) != geometries.end())
  {
    geometries[name]->flag = true;
    return 0; // geometry was found
  }

  if (filename.substr(filename.find_last_of(".") + 1) == "tmsh")
  {
    Geometry* g = TMshReader::geometryFromTMsh(filename, scale);
    if (!g) return 1;
    g->flag = true;
    geometries[name] = g;
  }
  else if (filename.substr(filename.find_last_of(".") + 1) == "obj")
  {
    Geometry* g = ObjReader::geometryFromOBJ(filename.c_str(), scale);
    if (!g) return 1;
    g->flag = true;
    geometries[name] = g;
  }

  return 0;
  // geometry was loaded correctly and flag is set to true, OR the geometry file format was different and the geometry was not loaded and the flag is set to false or not changed at all
}

int GeometryMap::addHCGeometry(std::string name, std::string filename)
{
  if (hcGeometries.find(name) != hcGeometries.end())
  {
    hcGeometries[name]->flag = true;
    return 0;
  }

  if (filename.substr(filename.find_last_of(".") + 1) == "tmsh")
  {
    Geometry* g = TMshReader::geometryFromTMsh(filename);
    if (!g) return 1;
    g->flag = true;
    hcGeometries[name] = g;
  }
  else if (filename.substr(filename.find_last_of(".") + 1) == "obj")
  {
    Geometry* g = ObjReader::geometryFromOBJ(filename.c_str());
    if (!g) return 1;
    g->flag = true;
    hcGeometries[name] = g;
  }
  return 0;
  // geometry was loaded correctly and flag is set to true, OR the geometry file format was different and the geometry was not loaded and the flag is set to false or not changed at all
}


int GeometryMap::addHCGeometry(std::string name, std::string filename, float scale)
{
  if (hcGeometries.find(name) != hcGeometries.end())
  {
    hcGeometries[name]->flag = true;
    return 0;
  }

  if (filename.substr(filename.find_last_of(".") + 1) == "tmsh")
  {
    Geometry* g = TMshReader::geometryFromTMsh(filename, scale);
    if (!g) return 1;
    g->flag = true;
    hcGeometries[name] = g;
  }
  else if (filename.substr(filename.find_last_of(".") + 1) == "obj")
  {
    Geometry* g = ObjReader::geometryFromOBJ(filename.c_str(), scale);
    if (!g) return 1;
    g->flag = true;
    hcGeometries[name] = g;
  }
  return 0;
  // geometry was loaded correctly and flag is set to true, OR the geometry file format was different and the geometry was not loaded and the flag is set to false or not changed at all
}

void GeometryMap::connectAllToShader()
{
  for (std::map<std::string, Geometry*>::const_iterator it = geometries.begin(); it != geometries.end(); ++it)
  {
    ShaderProvider::connectGeometry(it->second);
  }
}

void GeometryMap::connectAllHCToShader()
{
  for (std::map<std::string, Geometry*>::const_iterator it = hcGeometries.begin(); it != hcGeometries.end(); ++it)
  {
    //cout << "Geometry name = " << it->first << std::endl;
    const std::string geomName = it->first;
    if (geomName == "frustrum")
      ShaderProvider::connectGeometry(it->second, 2); ///< \todo hack + add enum for shaders
    else
      ShaderProvider::connectGeometry(it->second);
    //cout << "actShaderID = " << it->second->actShaderId << std::endl;
  }
}

// statics
std::map<std::string, Geometry*> GeometryMap::geometries;
std::map<std::string, Geometry*> GeometryMap::hcGeometries;
