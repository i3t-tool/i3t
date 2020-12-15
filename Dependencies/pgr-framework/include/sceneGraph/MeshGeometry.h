#ifndef MESHGEOMETRY_H
#define MESHGEOMETRY_H

#include "pgr.h"

namespace pgr {
namespace sg {

/** Container for the mesh data.
 *
 * Holds the complete geometry of the mesh, grouped to materialGroups with single material each.
 * Holds all vertices from all submeshes packed to buffer objects
 * called m_vertexBufferObject, m_normalBufferObject and m_texCoordBufferObject
 */
class MeshGeometry {
public:
  /// one material/vertex group - submesh
  struct SubMesh {
    /// material name, if available
    std::string name;
    /// material ambient component
    GLfloat ambient[3];
    /// material diffuse component
    GLfloat diffuse[3];
    /// material specular component
    GLfloat specular[3];
    /// material shininess
    GLfloat shininess;

    /// jak s texturou???? a co kdyz jich bude vic??? povolime pouze jednu
    // Diffuse texture[0] only
    std::string textureName;
    GLuint textureID;

    /// number of indices in this submesh
    GLuint nIndices;
    /// first index in array of indices
    GLuint startIndex;
    /// vertex in array of vertices added to index in the index buffer
    GLuint baseVertex;
  };

  typedef std::vector<SubMesh> SubMeshList;

  MeshGeometry(void);
  ~MeshGeometry();

  static MeshGeometry * LoadFromFile(const std::string & path);
  static MeshGeometry * LoadRawHeightMap(const std::string & path);

  GLuint getSubMeshCount(void) const {
    return static_cast<GLuint>(m_subMeshList.size());
  }

  const SubMesh* getSubMesh(unsigned index) const {
    if(index < m_subMeshList.size())
      return &m_subMeshList[index];
    else
      return NULL;
  }

  SubMesh* getSubMesh(unsigned index) {
    if(index < m_subMeshList.size())
      return &m_subMeshList[index];
    else
      return NULL;
  }

  GLuint getVertexBuffer(void) const {
    return m_vertexBufferObject;
  }

  GLuint getNormalBuffer(void) const {
    return m_normalBufferObject;
  }

  GLuint getTexCoordBuffer(void) const {
    return m_texCoordBufferObject;
  }

  GLuint getElementBuffer(void) const {
    return m_elementArrayBufferObject;
  }

  GLuint getVerticesCount(void) const {
    return m_nVertices;
  }

  GLuint getIndicesCount(void) const {
    return m_nIndices;
  }

  bool hasNormals(void) const {
    return m_hasNormals;
  }

  bool hasTexCoords(void) const {
    return m_hasTexCoords;
  }

protected:
  void setMesh(
      unsigned int verticesCount,
      float* vertices,
      float* normals,
      float* texCoords,
      unsigned int indicesCount,
      GLuint* indices
  );

  /// identifier for the buffer object for indices
  GLuint m_elementArrayBufferObject;
  /// identifier for the buffer object for vertices
  GLuint m_vertexBufferObject;
  /// identifier for the buffer object for normals
  GLuint m_normalBufferObject;
  /// identifier for the buffer object for texture coordinates
  GLuint m_texCoordBufferObject;

  /// list of sumbeshes (vertex/material groups)
  SubMeshList m_subMeshList;

  /// count of vertices af all the submeshes together
  GLuint m_nVertices;
  /// count of indices af all the submeshes together
  GLuint m_nIndices;

  ///
  bool m_hasNormals;
  bool m_hasTexCoords;
};

} // end namespace sg
} // end namespace pgr


#endif // MESHGEOMETRY_H
