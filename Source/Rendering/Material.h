//---------------------------------------------------------------------------
/**
 * \file	material.h
 *
 * Declares the material class
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "pgr.h"
#include <glm/vec3.hpp>
#include <map>
#include <string>

struct Material
{
private:
	std::string name; ///< material reference name - used in config files
	GLuint textureId; ///< OpenGL texture name

public:
	float ns;    ///< \todo  Probably read in reader.cpp but never used
	float ni;    ///< \todo  Probably read in reader.cpp but never used
	float d;     ///< \todo  Probably read in reader.cpp but never used
	float illum; ///< \todo  Probably read in reader.cpp but never used

	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

	Material(const std::string& name, const GLuint textureId)
	{
		assert(textureId < 10000); // something from assigning -1 into unsignedInt

		this->name = name;
		this->textureId = textureId;
		diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
		specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
		ns = 1.0;
		ni = 1.0f;
		d = 1.0f;
		illum = 1.0f;
	}

	Material(const std::string& name, const GLuint textureId, const glm::vec3 diffuseColor,
	         const glm::vec3 specularColor, const float ns, const float ni, const float illum, const float d)
	{
		this->name = name;
		this->textureId = textureId;
		this->diffuseColor = diffuseColor;
		this->specularColor = specularColor;
		this->ns = ns;
		this->ni = ni;
		this->d = d;
		this->illum = illum;
	}

	std::string getName() const { return name; }

	void setName(const std::string& _name) { name = _name; }

	GLuint getTextureId() const { return textureId; }

	void setTextureId(const int id) { textureId = id; }

	glm::vec3 getDiffuseColor() const { return diffuseColor; }

	void setDiffuseColor(const float r, const float g, const float b)
	{
		diffuseColor.x = r;
		diffuseColor.y = g;
		diffuseColor.z = b;
	}

	glm::vec3 getSpecularColor() const { return specularColor; }
};

class MaterialMap
{
private:
	static std::map<std::string, Material*> materials;
	static std::map<std::string, Material*> hcMaterials;

public:
	static Material* get(const std::string name) { return materials[name]; }

	// HC means Hard coded
	static Material* getHC(const std::string name) { return hcMaterials[name]; }

	static void addMaterial(Material* mat) { materials[mat->getName()] = mat; }

	static void addHCMaterial(Material* mat) { hcMaterials[mat->getName()] = mat; }

	static void dispose()
	{
		for (std::map<std::string, Material*>::const_iterator it = materials.begin(); it != materials.end(); ++it)
		{
			delete (it->second);
		}
		materials.clear();
		for (std::map<std::string, Material*>::const_iterator it = hcMaterials.begin(); it != hcMaterials.end(); ++it)
		{
			delete (it->second);
		}
		hcMaterials.clear();
	}
};

#endif
