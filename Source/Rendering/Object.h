//---------------------------------------------------------------------------
/**
 * \file	object.h
 *
 * Header for representing base object in scene 
 * 
 * \date  	2014/11/16
 * \author 	Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Rendering/ModelInstance.h"
#include <string>
#include <vector>

class Transformation;
class Shaper;
class Shader;

class Object : public ModelInstance
{
protected:

  Transformation* transformation; ///< object model transformation or view transformation for CameraObject

public:

  Object(Geometry* _geometry, Material* _material, Transformation* _transformation);

  Transformation* getTransformation() const;

  void update() override;

  /**
   * \brief Prepare the object coordinate frame into the Shaper geometry (vertices and colors)
   * \param shaper [out] Shaper object for accumulating the geometric data (vertices and colors) to be rendered
   */
  virtual void debug(Shaper* shaper);
};

struct ObjectDefinitionTree;

class ObjectDefinition
{
private:

  std::string name; ///< object name
  Geometry* geometry; ///< object geometry (vertices, VBOs, ...)
  Material* material; ///< object material 

  ObjectDefinitionTree* node; ///< probably parent node of this definition \todo rename to parentNode

public:

  ObjectDefinition(std::string _name, ObjectDefinitionTree* _node, Geometry* _geometry, Material* _material)
    : name(_name), node(_node), geometry(_geometry), material(_material)
  {
  }

  std::string getName() const
  {
    return name;
  }

  Geometry* getGeometry() const
  {
    return geometry;
  }

  Material* getMaterial() const
  {
    return material;
  }

  /**
   * \brief Get the space separated node names from the root to this node 
   *        This method is used for writing full object name with path to the .scn file
   * \return 
   */
  std::string getObjectDefinitionTreePath() const;

  /**
   * \brief Get new instance of the same object (same geometry and material) as this with new transformation
   *		  A copy.
   * \param _transformation Transformation matrix for the copy of this object
   * \return New object copy
   */
  Object* getInstance(Transformation* _transformation) const
  {
    return new Object(geometry, material, _transformation);
  }
};

/**
 * \brief A (sub) tree describing one level of the object menus. Defined in World
 */
struct ObjectDefinitionTree
{
  static ObjectDefinitionTree* tree; ///< tree with complete object menu hierarchy

  static ObjectDefinition* getObjectDefinitionFromTreePath(const std::string& path);

  std::string name; ///< name of this tree node

  ObjectDefinitionTree* parent; ///< parent in the tree

  std::vector<ObjectDefinitionTree*> nodes; ///< subMenus - vector of attached subtrees
  std::vector<ObjectDefinition*> definitions; ///< menu items - definition of geometric objects

  /**
   * \brief Constructor of the objectMenuHierarchy tree root with name "objects"
   * \param _parent parent of this tree node
   */
  ObjectDefinitionTree(ObjectDefinitionTree* _parent) : parent(_parent), name("objects")
  {
  }

  ~ObjectDefinitionTree()
  {
    // delete the subtrees
    for (std::vector<ObjectDefinitionTree*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
      delete (*it);
    }
    nodes.clear();

    // delete the menu items
    for (std::vector<ObjectDefinition*>::iterator it = definitions.begin(); it != definitions.end(); ++it)
    {
      delete (*it);
    }
    definitions.clear();
  }

  /**
   * \brief Search of the \a subtree of given \a name in this submenu level
   * \param name Name of the subTree found in this menu level 
   * \return object or NULL
   */
  ObjectDefinitionTree* getNode(const std::string& name)
  {
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
      if (name == nodes[i]->name)
        return nodes[i];
    }
    return NULL;
  }

  /**
  * \brief Search of the \a menu item (objectDefinition) of given \a name in this submenu level
  * \param name Name of the object to be found in this menu level (menuItems)
  * \return object or NULL
  */
  ObjectDefinition* getDefiniton(const std::string& name)
  {
    for (unsigned int i = 0; i < definitions.size(); i++)
    {
      if (name == definitions[i]->getName())
        return definitions[i];
    }
    return NULL;
  }
};

/*
class ObjectDefinitionMap {

private:

	static std::map<std::string, ObjectDefinition*> definitions;

public:

	static ObjectDefinition * get(string name) {
		return definitions[name];
	}

	void addObject(string name, ObjectDefinition * def) {
		definitions[name] = def;
	}
	
	static void dispose() {
		for(std::map<std::string, ObjectDefinition*>::const_iterator it = definitions.begin(); it != definitions.end(); ++it) {			
			delete (it->second);
		}
		definitions.clear();
	}
};
*/

#endif
