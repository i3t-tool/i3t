
#include <map>
#include <iostream>
#include <string>

#include "pgr.h"

namespace pgr {
namespace sg {

class MeshGeometry;
class BasicShaderProgram;

/// map of reference-counted resources
template <class T, class Loader, class Deleter>
class ResourceManager {
  struct SharedResource {
    SharedResource(): resource(), count(0) {}
    explicit SharedResource(const T & r): resource(r), count(1) {}

    T resource;
    unsigned count;
  };

  typedef std::map<std::string, SharedResource> ResourceMap;
  typedef std::pair<std::string, SharedResource> ResourcePair;
  typedef typename ResourceMap::iterator ResourceIterator;

public:
  T get(const std::string & name) {
    ResourceIterator it = m_resources.find(name);
    if(it != m_resources.end()) {
      it->second.count++;
      return it->second.resource;
    }

    T resource = m_loader(name);
    insert(name, resource);
    return resource;
  }

  bool exists(const std::string & name) const {
    return m_resources.find(name) != m_resources.end();
  }

  void release(const std::string & name) {
    ResourceIterator it = m_resources.find(name);
    if(it == m_resources.end()) {
      std::cerr << "release called on non existing resource (already freed?!?) " << name << std::endl;
      return;
    }

    if(it->second.count == 0) {
      std::cerr << "count is 0, that should not happen!" << std::endl;
      return;
    }

    it->second.count--;
    if(it->second.count == 0)
      remove(it);
  }

  void insert(const std::string & name, const T & resource) {
    std::pair<ResourceIterator, bool> ret = m_resources.insert(ResourcePair(name, SharedResource(resource)));
    if(ret.second == false)
      std::cerr << "cannot insert " << name << " resource with the same name already exists" << std::endl;
  }

protected:

  ResourceManager(void) {}
  ~ResourceManager() {}

  void remove(ResourceIterator it) {
    m_deleter(it->second.resource);
    m_resources.erase(it);
  }

private:
  ResourceMap m_resources;
  Loader m_loader;
  Deleter m_deleter;
};

/// functor for texture loading
struct TextureLoader {
  virtual GLuint operator()(const std::string & name);
};

/// functor for texture deleting
struct TextureDeleter {
  void operator()(GLuint texture);
};

/// functor for mesh loading
struct MeshLoader {
  MeshGeometry * operator()(const std::string & path);
};

/// functor for mesh deleting
struct MeshDeleter {
  void operator()(MeshGeometry * mesh);
};

/// functor for shader loading
struct ShaderLoader {
  BasicShaderProgram * operator()(const std::string & path);
};

/// functor for shader deleting
struct ShaderDeleter {
  void operator()(BasicShaderProgram * shader);
};

/// declarations for singleton classes
#define SINGLETON_DECL(cl) \
  private: cl() {} cl(cl const&) {} cl& operator=(cl const&) {} \
  public: static cl * Instance(); \
  private: static cl * m_instance;

/// definitions for singleton classes
#define SINGLETON_DEF(cl) \
  cl* cl::m_instance = 0; \
  cl* cl::Instance() { \
    if(m_instance == 0) m_instance = new cl(); \
    return m_instance; \
  }

/// Singleton class for texture loading
class TextureManager : public ResourceManager<GLuint, TextureLoader, TextureDeleter> {
  SINGLETON_DECL(TextureManager)
};

/// Singleton class for mesh loading
class MeshManager : public ResourceManager<MeshGeometry*, MeshLoader, MeshDeleter> {
  SINGLETON_DECL(MeshManager)
};

/// Singleton class for shader loading
class ShaderManager : public ResourceManager<BasicShaderProgram*, ShaderLoader, ShaderDeleter> {
  SINGLETON_DECL(ShaderManager)
};

} // end namespace sg
} // end namespace pgr

