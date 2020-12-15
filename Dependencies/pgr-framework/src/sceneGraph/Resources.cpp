
#include "sceneGraph/Resources.h"
#include "sceneGraph/MeshGeometry.h"
#include "sceneGraph/ShaderProgram.h"

namespace pgr {
namespace sg {

SINGLETON_DEF(TextureManager)
SINGLETON_DEF(MeshManager)
SINGLETON_DEF(ShaderManager)

MeshGeometry *MeshLoader::operator ()(const std::string &path) {
  return MeshGeometry::LoadFromFile(path);
}

void MeshDeleter::operator ()(MeshGeometry *mesh) {
  delete mesh;
}

GLuint TextureLoader::operator ()(const std::string &name) {
  return pgr::createTexture(name);
}

void TextureDeleter::operator ()(GLuint texture) {
  glDeleteTextures(1, &texture);
}

void ShaderDeleter::operator ()(BasicShaderProgram *shader) {
  delete shader;
}

BasicShaderProgram * ShaderLoader::operator ()(const std::string &path) {
  GLuint shaderList[] = {
    pgr::createShaderFromFile(GL_VERTEX_SHADER, path + ".vert"),
    pgr::createShaderFromFile(GL_FRAGMENT_SHADER, path + ".frag"),
    0
  };

  GLuint shp = pgr::createProgram(shaderList);
  return new BasicShaderProgram(shp);
}

} // end namespace sg
} // end namespace pgr

