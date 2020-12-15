
#include <iostream>

#include "sceneGraph/MeshGeometry.h"
#include "sceneGraph/Resources.h"

namespace pgr {
namespace sg {

MeshGeometry::MeshGeometry(void) : m_nVertices(0), m_nIndices(0), m_hasNormals(false), m_hasTexCoords(false) {
  glGenBuffers(1, &m_vertexBufferObject);
  glGenBuffers(1, &m_normalBufferObject);
  glGenBuffers(1, &m_texCoordBufferObject);
  glGenBuffers(1, &m_elementArrayBufferObject);
}

MeshGeometry::~MeshGeometry() {
  glDeleteBuffers(1, &m_vertexBufferObject);
  glDeleteBuffers(1, &m_normalBufferObject);
  glDeleteBuffers(1, &m_texCoordBufferObject);
  glDeleteBuffers(1, &m_elementArrayBufferObject );
}

// pass the data as blocks of bytes to OpenGL buffers
void MeshGeometry::setMesh(unsigned int verticesCount, float* vertices, float* normals, float* texCoords, unsigned int indicesCount, GLuint* indices) {

  // TODO: asssert if vertices == NULL or indices == NULL

  m_nVertices = verticesCount;

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, m_nVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);    // xyz
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if(normals != NULL) {
    m_hasNormals = true;
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_nVertices * 3 * sizeof(float), normals, GL_STATIC_DRAW);   // nor
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  if(texCoords != NULL) {
    m_hasTexCoords = true;
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_nVertices * 2 * sizeof(float), texCoords, GL_STATIC_DRAW); // st
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  m_nIndices = indicesCount;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementArrayBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

MeshGeometry *MeshGeometry::LoadFromFile(const std::string &path) {
  MeshGeometry* ret = 0;
  Assimp::Importer importer;   // asset loader

  //importer.SetExtraVerbose(true);

  // Unitize object in size
  importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1); //used by aiProcess_PreTransformVertices to normalize vertices to fit into (-1..1)^3

  // Load asset from the file - you can play with various processing steps
  const aiScene * scn = importer.ReadFile(path.c_str(), 0
      //| aiProcess_SortByPType             // Sort by primitive type into groups. Remove unrequested (all except triangles and polygons).
      | aiProcess_Triangulate             // Triangulate polygons (if any).
      | aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
      | aiProcess_GenSmoothNormals        // Calculate normals per vertex.
      | aiProcess_JoinIdenticalVertices
      //| aiProcess_RemoveComponent         // Removes above defined components
      //| aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
      //| aiProcess_ImproveCacheLocality
      //| aiProcess_RemoveRedundantMaterials
      //| aiProcess_FindDegenerates         // Find degenerated polys and convert them into lines/points, which are then removed.
      //| aiProcess_FindInvalidData         // In this step we can lost normals if they are incorrectly exported from the modeler.
      //                                    // Thats why we generate them.
      //| aiProcess_GenUVCoords             // Force regenerate spherical/cylindrical/planar... coords into UVs.
      | aiProcess_TransformUVCoords       // Pretransform UV coords with texture matrix. Like in vertex shaders using gl_TextureMatrix.
      //| aiProcess_OptimizeMeshes
      //| aiProcess_CalcTangentSpace        // Calculate tangents/bitangents per vertex.
                                         );

  if(!scn) {
    std::cerr << importer.GetErrorString() << std::endl;
    return ret;
  }

  // Collapse obtained (postprocessed) hierarchy into one root node with array of meshes.
  // This step must be separated because of use [aiProcess_PreTransformVertices] which creates
  // only one root with leafs children - one mesh per child.
  // importer.ApplyPostProcessing(aiProcess_OptimizeGraph);
  if(scn->mNumMeshes < 1) {
    std::cerr << "no meshes found in scene " << path << std::endl;
    return ret;
  }

  std::cout << "loaded " << scn->mNumMeshes << " meshes" << std::endl;

  // merge all sub-meshes to one big mesh
  const unsigned FACE_VERT_COUNT = 3;  // triangles

  unsigned int nVertices = 0;  // vertex counter
  unsigned int nIndices  = 0;  // indices counter

  // sum all vertices and indices for all meshes
  for(unsigned m = 0; m < scn->mNumMeshes; ++m) {
    // count the overall
    nVertices += scn->mMeshes[m]->mNumVertices;                 //   number of vertices
    nIndices  += scn->mMeshes[m]->mNumFaces * FACE_VERT_COUNT;  //   and indices in all THE meshes
  }

  if((nVertices == 0) || (nIndices < FACE_VERT_COUNT)) {
    std::cerr << "no triangles found in scene " << path << std::endl;
    return ret;
  }

  ////////// create MeshGeometry /////
  ret = new MeshGeometry();          // complete geometry (vertices with normals and indices for all subMeshes)

  // vertices
  glBindBuffer(GL_ARRAY_BUFFER, ret->m_vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), 0, GL_STATIC_DRAW);    // xyz
  for(unsigned m = 0, offset = 0; m < scn->mNumMeshes; ++m) {
    unsigned size = scn->mMeshes[m]->mNumVertices * 3 * sizeof(float);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, scn->mMeshes[m]->mVertices);
    offset += size;
  }

  // normal vectors
  glBindBuffer(GL_ARRAY_BUFFER, ret->m_normalBufferObject);
  glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), 0, GL_STATIC_DRAW);    // xyz
  for(unsigned m = 0, offset = 0; m < scn->mNumMeshes; ++m) {
    unsigned size = scn->mMeshes[m]->mNumVertices * 3 * sizeof(float);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, scn->mMeshes[m]->mNormals);
    offset += size;
  }

  //TODO: just texture 0 for now
  float *textureCoords = new float[2 * nVertices];  //2 floats per vertex (str)
  float *cur_textureCoord = textureCoords;

  unsigned *indices = new unsigned[nIndices];   // indices to the vertices of the faces

  ret->m_subMeshList.resize(scn->mNumMeshes);

  unsigned startIndex = 0;  // for face indexing - index in the array of indexes
  unsigned baseVertex = 0;  // for vertices block (base vertex is added to rellative index in the submesh to get absolute index in the array of vertices)

  for(unsigned m = 0; m < scn->mNumMeshes; ++m) {
    const aiMesh *mesh = scn->mMeshes[m];

    // copy texture coordinates
    aiVector3D vect;
    for(unsigned i=0; i<1; ++i) {   // TODO> up to 4 textures can be loaded
      if(mesh->HasTextureCoords(i)) {
        // we use 2D textures with 2 coordinates and ignore the third coordinate
        for(unsigned idx=0; idx<mesh->mNumVertices; idx++) {
          vect = (mesh->mTextureCoords[i])[idx];
          *cur_textureCoord++ = vect.x;
          *cur_textureCoord++ = vect.y;
        }
      }
    }

    // copy the face index tripplets (we use triangular faces) to indices
    for(unsigned j = 0; j < mesh->mNumFaces; ++j) {
      memcpy( &indices[startIndex + j*FACE_VERT_COUNT], mesh->mFaces[j].mIndices, FACE_VERT_COUNT * sizeof(unsigned) );
    }

    // copy the material info to SSubMesh structure
    const aiMaterial *mat = scn->mMaterials[mesh->mMaterialIndex];
    // the material vertices are grouped together (done by mesh processing step), so we cycle through all meshes and add their materials

    MeshGeometry::SubMesh* subMesh_p = ret->getSubMesh(m);
    aiColor4D color;
    aiString name;
    aiReturn retValue = AI_SUCCESS;

    //for(unsigned i=0; i< mat->mNumProperties; i++ )
    //{
    //  std::cout << "Property[" << i << "] = " << mat->mProperties[i]->mKey.data << " = " << mat->mProperties[i]->mData << std::endl;
    //}

    //Get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 | aiReturn_OUTOFMEMORY -3
    mat->Get(AI_MATKEY_NAME, name); // may be "" after the input mesh processing. Must be aiString type!
    subMesh_p->name = name.data;

    if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color)) != AI_SUCCESS)
      color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);

    subMesh_p->ambient[0] = color.r;
    subMesh_p->ambient[1] = color.g;
    subMesh_p->ambient[2] = color.b;

    if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS)
      color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);

    subMesh_p->diffuse[0] = color.r;
    subMesh_p->diffuse[1] = color.g;
    subMesh_p->diffuse[2] = color.b;

    if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color)) != AI_SUCCESS)
      color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);

    subMesh_p->specular[0] = color.r;
    subMesh_p->specular[1] = color.g;
    subMesh_p->specular[2] = color.b;

    ai_real shininess, strength;
    unsigned int max;	// changed: to unsigned

    max = 1;
    if ((retValue = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, &max)) != AI_SUCCESS)
      shininess = 1.0f;
    max = 1;
    if ((retValue = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS_STRENGTH, &strength, &max)) != AI_SUCCESS)
      strength = 1.0f;

    subMesh_p->shininess = shininess * strength;

    subMesh_p->textureID = 0;

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

      aiString texPath; // filename

      aiReturn texFound = mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath); // AI_SUCCESS
      subMesh_p->textureName = texPath.data;

      size_t found = path.find_last_of("/\\");

      if(found != std::string::npos) { // not found
        //subMesh_p->textureName.insert(0, "/");
        subMesh_p->textureName.insert(0, path.substr(0, found+1));
      }

      // cout << " file: " << str.substr(found+1) << endl;
      std::cout << "Loading texture file: " << subMesh_p->textureName << std::endl;

      GLuint texture = TextureManager::Instance()->get(subMesh_p->textureName);
      subMesh_p->textureID = texture;
    }

    // we ignore AI_MATKEY  OPACITY, REFRACTI, SHADING_MODEL

    // - indices to the element array
    subMesh_p->nIndices = mesh->mNumFaces * FACE_VERT_COUNT;
    subMesh_p->startIndex = startIndex;
    subMesh_p->baseVertex = baseVertex;

    startIndex += mesh->mNumFaces * FACE_VERT_COUNT;
    baseVertex += mesh->mNumVertices;

  }

  ret->m_nIndices = nIndices;
  ret->m_nVertices = nVertices;
  ret->m_hasTexCoords = true;
  ret->m_hasNormals = true;

  glBindBuffer(GL_ARRAY_BUFFER, ret->m_texCoordBufferObject);
  glBufferData(GL_ARRAY_BUFFER, nVertices * 2 * sizeof(float), textureCoords, GL_STATIC_DRAW); // st
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->m_elementArrayBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  delete [] textureCoords;
  delete [] indices;

  return ret;
}

MeshGeometry *MeshGeometry::LoadRawHeightMap(const std::string &path) {
  MeshGeometry* meshGeometry_p = NULL;

  long int m_nVertices;
  FILE* rawFile;
  char file[256];

  // resolution of raw file -> 513 x 513 grid
  const int _resX = 513;
  const int _resZ = 513;

  // distances between neighbour grid points along x, y, and z axis
  const float _deltaX = 1.0f / (_resX - 1);
  const float _deltaZ = 1.0f / (_resZ - 1);
  const float _deltaY = 1.0f / ((_resX - 1) * (_resZ - 1)); //256.0f; // height modulation

  // size of the grid
  const float _sizeX = 1.0f;
  const float _sizeZ = 1.0f;

  // origin of the grid - move the grid to the center of coordinate system
  const float _originX = -0.5f;
  const float _originZ = -0.5f;

  // the number of floats to read for heights and colors
  m_nVertices = _resX*_resZ;

  float *m_pVertices = NULL;
  m_pVertices = new float[3*m_nVertices];
  assert(m_pVertices);

  //float *_colors = NULL;
  //_colors = new float[size];
  //assert(_colors);

  // read the heights of terrain
  sprintf(file, "%s.raw", path.c_str());
  std::cout << "Loading terrain map file " << file << " [" << _resX << "x" << _resZ << "] ";
  std::cout.flush();

  rawFile = fopen(file, "rb");
  if (rawFile == NULL) {
    std::cerr << "MeshManager::LoadTerrain(): Can't open input raw file" << file << std::endl;
    return meshGeometry_p;
  }

  typedef unsigned char BYTE;
  BYTE *buffer = new BYTE[3*_resX]; // 2 bytes per one height, 3 bytes per one color
  int counter;
  float height;//, *cPtr;
  BYTE low, high;

  // read a terragen file that contains grid heights
  for(int z=0; z<_resZ; z++) {
    counter = 0;
    fread(buffer, 1, 2*_resX, rawFile);

    for(int x=0; x<_resX; x++) {
      low = buffer[counter++];   // fread((char*)&low, 1, 1, rawFile);
      high = buffer[counter++];  // fread((char*)&high, 1, 1, rawFile);
      height = (float) (high * 0xFFL + low);
      // index for vertex in the array of floats
      int idx = (_resX * z + x) * 3;

      m_pVertices[idx] = _originX + x*_deltaX;
      m_pVertices[idx+1] = height * _deltaY;
      m_pVertices[idx+2] = _originZ + z*_deltaZ;
      //_colors[i] = _colors[i+1] = _colors[i+2] = 0.5; // color is grey by default
    }
    std::cout << ".";
    std::cout.flush();
  }
  std::cout << std::endl;
  fclose(rawFile);

  delete [] buffer;

  sprintf(file, "%s.tga", path.c_str());
  std::cout << "Loading terrain texture " << file << " [" << _resX << "x" << _resZ << "] ";
  std::cout.flush();

  GLuint texture = TextureManager::Instance()->get(file);

  unsigned int m_nIndices = 2 * 3 * (_resX-1) * (_resZ-1);
  unsigned int* m_pIndices = NULL;
  m_pIndices = new unsigned int[m_nIndices];

  int triIndex;

  // build array of indices
  // asi by to chtelo zvazit, zda nepouzit tringle strip (mit na vyber jak kreslit mesh - tringles nebo strip)
  counter = 0;
  for (int iz = 0; iz < _resZ-1; iz++) {
    triIndex = iz*_resX;
    for (int ix = 0; ix < _resX-1; ix++) {

      m_pIndices[counter++] = triIndex;
      m_pIndices[counter++] = triIndex+_resX;
      m_pIndices[counter++] = triIndex+1;

      m_pIndices[counter++] = triIndex+1;
      m_pIndices[counter++] = triIndex+_resX;
      m_pIndices[counter++] = triIndex+_resX+1;

      triIndex++;
    }
  }


  // generate texture coords & normals
  GLfloat* m_pTexCoords = new GLfloat[2*m_nVertices];
  GLfloat* m_pNormals = new GLfloat[3*m_nVertices];

#define X 0
#define Y 1
#define Z 2

  float length;//, *ptr;

  int texCoordIndex = 0;
  int normalIndex = 0;

  float texCoordS = 0.0f;
  float texCoordT = 0.0f;

  float *point0, *point1, *point2;
  float mul = _deltaX * _deltaZ;
  float nx, ny, nz;

  int count;

  // For now we will do it the hard way - by generating the normals individually for each elevation point
  for (int iz = 0; iz < _resZ; iz++) {
    texCoordS = 0.0f;
    for (int ix = 0; ix < _resX; ix++) {

      // texture coordinates
      m_pTexCoords[texCoordIndex++] = texCoordS;
      m_pTexCoords[texCoordIndex++] = texCoordT;

      nx = ny = nz = 0.0f;

      // count of normals added to current vertex
      count = 0;

      point0 = m_pVertices + 3L*(iz*_resX + ix) + Y;

      if(iz<_resZ-1 && ix<_resX-1) {
        point1 = point0 + 3L*_resX; //m_pVertices + 3L*((iz+1)*_resX + ix) + Y;
        point2 = point0 + 3L;

        //nx += -_deltaZ * (*point2 - *point0);
        //ny += mul;
        //nz += -_deltaX * (*point1 - *point0);
        nx -= _deltaZ * (*point2 - *point0);
        ny += mul;
        nz -= _deltaX * (*point1 - *point0);

        count++;
      }

      if(iz>0 && ix<_resX-1) {
        point1 = point0 + 3L;
        point2 = point0 - 3L*_resX; //m_pVertices + 3L*((iz-1)*_resX + ix) + Y;

        //nx -= _deltaZ * (*point1 - *point0);
        //ny += mul;
        //nz += _deltaX * (*point2 - *point0);
        nx -= _deltaZ * (*point1 - *point0);
        ny += mul;
        nz += _deltaX * (*point2 - *point0);

        count++;
      }

      if(iz>0 && ix>0) {
        point1 = point0 - 3L*_resX; //m_pVertices + 3L*((iz-1)*_resX+ix) + Y;
        point2 = point0 - 3L;

        //nx += _deltaZ * (*point2 - *point0);
        //ny += mul;
        //nz += _deltaX * (*point1 - *point0);
        nx += _deltaZ * (*point2 - *point0);
        ny += mul;
        nz += _deltaX * (*point1 - *point0);

        count++;
      }

      if(iz<_resZ-1 && ix>0) {
        point1 = point0 - 3;
        point2 = point0 + 3L*_resX; //m_pVertices + 3L*((iz+1)*_resX + ix) + Y;

        //nx += _deltaZ * (*point1 - *point0);
        //ny += mul;
        //nz += -_deltaX * (*point2 - *point0);
        nx += _deltaZ * (*point1 - *point0);
        ny += mul;
        nz -= _deltaX * (*point2 - *point0);

        count++;
      }

      if(count == 0) {
        char errorStr[256];
        sprintf(errorStr, "MeshManager::LoadTerrain(): Failed to find any normals at: [%d, %d]", ix, iz);
        std::cerr << errorStr << std::endl;
        exit(1);
      }

      // normalize it
      length = sqrt( nx*nx +	ny*ny +	nz*nz );

      if(length < 1.0E-5f) {
        nx = 0.0f;
        ny = 1.0f;
        nz = 0.0f;
      } else {
        nx /= length;
        ny /= length;
        nz /= length;
      }

      //char errorStr[256];
      //sprintf(errorStr, "Normal at: [%d, %d] = [%lg, %lg, %lg]", ix, iz, nx, ny, nz);
      //std::cout << errorStr << std::endl;

      //normalIndex = 3*(iz*_resX + ix);

      m_pNormals[normalIndex+X] = nx;
      m_pNormals[normalIndex+Y] = ny;
      m_pNormals[normalIndex+Z] = nz;

      normalIndex += 3;

      texCoordS += _deltaX;
    }
    texCoordT += _deltaZ;
  }


#undef X
#undef Y
#undef Z


  ////////// create MeshGeometry /////
  meshGeometry_p = new MeshGeometry();          // complete geometry (vertices with normals and indices for all subMeshes)
  meshGeometry_p->m_subMeshList.resize(1);

  MeshGeometry::SubMesh* subMesh_p = meshGeometry_p->getSubMesh(0);

  subMesh_p->ambient[0] = 0.5f;
  subMesh_p->ambient[1] = 0.5f;
  subMesh_p->ambient[2] = 0.5f;

  subMesh_p->diffuse[0] = 0.7f;
  subMesh_p->diffuse[1] = 0.7f;
  subMesh_p->diffuse[2] = 0.7f;

  subMesh_p->specular[0] = 0.3f;
  subMesh_p->specular[1] = 0.3f;
  subMesh_p->specular[2] = 0.3f;

  subMesh_p->shininess = 10.0f;

  // indices to the element array
  subMesh_p->nIndices = m_nIndices;
  subMesh_p->startIndex = 0;
  subMesh_p->baseVertex = 0;

  subMesh_p->textureID = texture;

  // Finish the mesh by creating the buffer objects holding all vertices and indices
  meshGeometry_p->setMesh(m_nVertices, m_pVertices, m_pNormals, m_pTexCoords, m_nIndices, m_pIndices);

  delete [] m_pVertices;
  delete [] m_pNormals;
  delete [] m_pTexCoords;
  delete [] m_pIndices;

  return meshGeometry_p;
}

} // end namespace sg
} // end namespace pgr

