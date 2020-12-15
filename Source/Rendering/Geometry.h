//---------------------------------------------------------------------------
/**
 * \file	geometry.h
 *
 * Class representing model geometry data (vertices, indices, vbo objects/arrays)
 *
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "pgr.h"

#include <glm/vec3.hpp>
#include <iostream>
#include <map>
#include <string>

/** Class representing model geometry data (vertices, indices, vbo
 * objects/arrays). */
class Geometry
{
private:
  // data arrays of vertex face set
  float* vertices;       ///< array with vertices
  unsigned int* indices; ///< indices of the triangles

  // VBO's id's
  GLuint vbo_vertices; ///< OpenGL name
  GLuint vbo_indices;  ///< OpenGL name
  GLuint vao;          ///< OpenGL name

  /// number of float attributes per one vertex including the coordinates (also called \a stride)
  GLuint attribsPerVertex;

  /// number of vertices (length of vertices array is verticesCount * attribsPerVertex)
  GLuint verticesCount;

  /// number of indices (length of indices array is indicesCount, number of triangles is indicesCount / 3)
  GLuint indicesCount;

  /// aabb of geometry calculated from vertices in constructor, [minX, minY, minZ], [maxX, maxY, maxZ]
  glm::vec3 AABBMin;
  glm::vec3 AABBMax;

  /** Calculates an axis alligned bounding box (AABB), asuming vertices are 3D, i.e. with x, y, z and with stride
   * equal to the attrivsPerVertex value*/
  void calculateAABB();

  void bindVBOs();

public:
  bool flag; ///< geometry was found or loaded correctly during geometryAdd()

  GLuint actShaderId; /// id of shader program connected to this geometry

  Geometry(float* vertices, unsigned int* indices, GLuint verticesCount, GLuint indicesCount,
           GLuint attribsPerVertex);

  ~Geometry();

  float* getVertices() const { return vertices; }

  unsigned int* getIndices() const { return indices; }

  GLuint getVao() const { return vao; }

  GLuint getVerticesBuffer() const { return vbo_vertices; }

  GLuint getIndicesBuffer() const { return vbo_indices; }

  GLuint getIndicesCount() const { return indicesCount; }

  GLuint getVerticesCount() const { return verticesCount; }

  GLuint getAttribsPerVertex() const { return attribsPerVertex; }

  glm::vec3 getAABBMin() const { return AABBMin; }

  glm::vec3 getAABBMax() const { return AABBMax; }

  void print();
};

/** A geometry map - stores single instances of each loaded geometry.
  Manages two map containers (key is the given name).
  \todo What is the difference between geometries and hcGeometries?
*/
class GeometryMap
{
private:
  // hard coded
  static std::map<std::string, Geometry*> hcGeometries; ///< loaded hard coded geometries

  /// users
  static std::map<std::string, Geometry*> geometries; ///< loaded user defined geometries

public:
  static Geometry* get(std::string name) { return geometries[name]; }

  static Geometry* getHC(std::string name) { return hcGeometries[name]; }

  //@{
  /** \name   Geometry loaders */

  /**
      Load geometry from a file. First, check if the geometry of the
      \a name is already loaded. If not, load it from the given file.
      Accepts <A href="https://en.wikipedia.org/wiki/Wavefront_.obj_file">.obj</A> and .tmsh file formats.


      \param	name		The name.
      \param	filename	Filename of the file.

      \retval 0	OK - or a different file format was specified and nothing was loaded.
      \retval	1	File loading error.
   */

  static int addGeometry(const std::string& name, const std::string& filename);

  /**
      Load geometry from a file. First, check if the geometry of the
      \a name is already loaded. If not, load it from the given file.
    Accepts <A href="https://en.wikipedia.org/wiki/Wavefront_.obj_file">.obj</A> and .tmsh file formats.


      \param	name		The name.
      \param	filename	Filename of the file.
      \param	scale   	The scale, that multiplies the vertex coordinates.
              Ignored if the \a name already present in the map.

      \retval 0	OK - or a different file format was specified and nothing was loaded.
      \retval	1	File loading error.
     */

  static int addGeometry(const std::string& name, const std::string& filename, float scale);

  static int addHCGeometry(std::string name, std::string filename);
  static int addHCGeometry(std::string name, std::string filename, float scale);
  //@}

  static void connectAllToShader();
  static void connectAllHCToShader();

  /**
   * \brief Set the flag of all geometries in the repository (\a geometries) to false.
   */
  static void markAllUnflag()
  {
    for (std::map<std::string, Geometry*>::const_iterator it = geometries.begin(); it != geometries.end(); ++it)
    {
      (it->second)->flag = false;
    }
  }

  /**
   * \brief Set the flag of all hard coded geometries in the repository (\a hcGeometries) to false. NOT USED.
   */
  static void markAllHCUnflag()
  {
    for (std::map<std::string, Geometry*>::const_iterator it = hcGeometries.begin(); it != hcGeometries.end(); ++it)
    {
      (it->second)->flag = false;
    }
  }

  static void disposeUnflag()
  {
    for (std::map<std::string, Geometry*>::const_iterator it = geometries.begin(); it != geometries.end();)
    {
      if (!(it->second)->flag)
      {
        // cout << "delete unflag : " << it->first << std::endl;
        delete (it->second);
        it = geometries.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  static void disposeHCUnflag()
  {
    for (std::map<std::string, Geometry*>::const_iterator it = hcGeometries.begin(); it != hcGeometries.end();)
    {
      if (!(it->second)->flag)
      {
        // cout << "delete unflag : " << it->first << std::endl;
        delete (it->second);
        it = geometries.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  static void dispose()
  {
    for (std::map<std::string, Geometry*>::const_iterator it = geometries.begin(); it != geometries.end(); ++it)
    {
      delete (it->second);
    }
    geometries.clear();
  }

  static void disposeHC()
  {
    for (std::map<std::string, Geometry*>::const_iterator it = hcGeometries.begin(); it != hcGeometries.end(); ++it)
    {
      delete (it->second);
    }
    hcGeometries.clear();
  }
};

#endif
