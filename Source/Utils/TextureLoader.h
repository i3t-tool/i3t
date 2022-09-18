//---------------------------------------------------------------------------
/**
 * \file	textureLoader.h
 *
 * Class for loading and holding texture
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _TEXTURE_LOADER_H_
#define _TEXTURE_LOADER_H_

#include "pgr.h"

#include <iostream>
#include <map>
#include <string>

#include "Config.h"

/**
 * \brief Map storing the OpenGL texture names \a textureId
 *
 * \todo I have no idea why two types of textures are used - textures and hard
 * coded textures.
 */
struct TextureRecord
{
	TextureRecord() : flag(false), textureId(0) // PF initialization added
	{
	}

	TextureRecord(const bool _flag, const GLuint _textureId)
	{
		flag = _flag;
		textureId = _textureId;
	}

	bool flag;
	///< Meaning texture needed. Either texture just loaded correctly or texture
	///< from the repository used by name.
	GLuint textureId; ///< OpenGL texture name
};

/**
 * \brief Texture manager, two maps: one of user loaded and one of hard code
 * loaded textures. Each texture is described in its TextureRecord
 *
 */
class TextureLoader final
{
private:
	static std::map<std::string, TextureRecord>
	    textures; ///< user loaded textures
	static std::map<std::string, TextureRecord>
	    hcTextures; ///< hard coded textures

public:
	/**
	 * \brief Delete all (user) textures from the repository (\a textures map)
	 */
	static void endTextures()
	{
		for (std::map<std::string, TextureRecord>::const_iterator it =
		         textures.begin();
		     it != textures.end(); ++it)
		{
			glDeleteTextures(1, &(it->second.textureId));
		}
		textures.clear();
	}

	/**
	 * \brief Delete all hard coded textures from the repository (\a hcTextures
	 * map)
	 */
	static void endHCTextures()
	{
		for (std::map<std::string, TextureRecord>::const_iterator it =
		         hcTextures.begin();
		     it != hcTextures.end(); ++it)
		{
			glDeleteTextures(1, &(it->second.textureId));
		}
		hcTextures.clear();
	}

	/**
	 * \brief Check, if the texture \a name is already in the texture map \a
	 * textures, load it if not, and return its OpenGL ID. In both cases flag it
	 * as needed now. It is used in Reader::readTexture() only.
	 *
	 * \param name Name (ID) of the texture, as defined in textures.cfg and used
	 * in the materials.cfg. \param path The absolute path to the texture file
	 * (working directory + filename). \return OpenGL texture object name or 0 on
	 * failure
	 */
	static int loadTexture(const std::string& name, const std::string& path)
	{
		if (textures.find(name) != textures.end())
		{
			textures[name].flag = true; // already in the repository, flag as used now
			return textures[name].textureId;
		}

		GLuint t = pgr::createTexture(path); // load texture from file
		if (t == 0)
		{
			std::cout << "texture load error: " << path << std::endl;
		}
		else
		{
			textures[name] = TextureRecord(true, t);
		}
		return t;
	}

	/**
	 * \brief Check, if the texture \a name is already in the texture map \a
	 * hcTextures, load it if not, and return its OpenGL ID. It is used in
	 * main.cpp:init() only.
	 *
	 * \param name OpenGL name (ID) of the texture
	 * \param path The absolute path to the texture file (working directory +
	 * filename). \return OpenGL texture object name or 0 on failure
	 */
	static int loadHCTexture(const std::string& name, const std::string& path)
	{
		if (hcTextures.find(name) != hcTextures.end())
		{
			hcTextures[name].flag = true;
			return hcTextures[name].textureId;
		}

		GLuint t = pgr::createTexture(path);
		if (t == 0)
		{
			std::cout << "texture load error: " << path << std::endl;
		}
		else
		{
			hcTextures[name] = TextureRecord(true, t);
		}
		return t;
	}

	/**
	 * \brief Get OpenGL texture object name (OpenGL texture ID) from the
	 * repository \param name Name (key) of the texture in the repository \return
	 * OpenGL texture object name
	 */
	static GLuint getId(const std::string& name)
	{
		return textures[name].textureId;
	}

	/**
	 * \brief Replace the OpenGL texture object name in the repository
	 * \param name Name (key) of the texture in the repository
	 * \param id New OpenGL texture object name
	 */
	static void setTextureID(const std::string& name, const int id)
	{
		textures[name].textureId = id;
	}

	static GLuint getHCId(const std::string& name)
	{
		return hcTextures[name].textureId;
	}

	static void setHCTextureID(const std::string& name, const int id)
	{
		hcTextures[name].textureId = id;
	}

	/**
	 * \brief Set the flag of all textures in the repository (\a textures) to
	 * false. Used for resetting the texture repository before reading the config
	 * files with currently used textures.
	 */
	static void markAllUnflag()
	{
		for (std::map<std::string, TextureRecord>::iterator it = textures.begin();
		     it != textures.end(); ++it)
		{
			it->second.flag = false;
		}
	}

	/**
	 * \brief Set the flag of all textures in the repository (\a hcTextures) to
	 * false. NOT USED.
	 */
	static void markAllHCUnflag()
	{
		for (std::map<std::string, TextureRecord>::iterator it = hcTextures.begin();
		     it != hcTextures.end(); ++it)
		{
			it->second.flag = false;
		}
	}

	/**
	 * \brief Delete textures with flag = false from the repository (\a textures)
	 */
	static void disposeUnflag()
	{
		for (std::map<std::string, TextureRecord>::const_iterator it =
		         textures.begin();
		     it != textures.end();)
		{
			if (!(it->second).flag)
			{
				std::cout << "delete unflagged texture : " << it->first << std::endl;
				glDeleteTextures(1, &(it->second.textureId));
				it = textures.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	/**
	 * \brief Delete textures with flag = false from the repository (\a
	 * hcTextures)
	 */
	static void disposeHCUnflag()
	{
		for (std::map<std::string, TextureRecord>::const_iterator it =
		         hcTextures.begin();
		     it != hcTextures.end();)
		{
			if (!(it->second).flag)
			{
				std::cout << "delete unflagged HC texture : " << it->first << std::endl;
				glDeleteTextures(1, &(it->second.textureId));
				it = textures.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
};

#endif
