#include "GUIImage.h"
#include <iostream>
#include "pgr.h"
#include "stb_image.h"


// based on image.cpp from pgr framework

GUIImage::GUIImage(const std::string& path)
{
	const bool mipmap = true; // todo decide and remove, also, multisampling?
	// also, dival jsem se na anisotropic filtering, ale to se pro tutorial nehodi, vzhledem k tomu ze scale je uniformni (krom thumbnailu, ale tam to neva)

	// SET PASSED ARGUMENT
	m_path = path;

	// PREPARE OPENGL
	// generate and bind one texture
	m_texID = 0;
	glGenTextures(1, &m_texID);
	glBindTexture(GL_TEXTURE_2D, m_texID);
	// set linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// LOAD DATA USING STBI IMAGE
	stbi_set_flip_vertically_on_load(false);
	int            components;
	unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &components, STBI_rgb_alpha);
	//std::cerr << path << "\nchannels: " << std::to_string(components) << std::endl; // todo logging
	if (!data)
	{
		std::cerr << "Failed to load texture " << m_path << std::endl; // todo logging
		glDeleteTextures(1, &m_texID); // also binds texture 0 if the deleted one is this one
		stbi_image_free(data);
		//m_texID = 0; // - to be sure, that the destructor doesnt accidentally delete any other texture with the same id this one originally had (before being deleted) EDIT unnecessary when throwing ex
		throw std::runtime_error("Failed to load texture " + m_path);
	}
	// upload our image data to OpenGL
	//glTexImage2D(GL_TEXTURE_2D, 0, components == 4 ? GL_RGBA : GL_RGB, m_width, m_height, 0, components == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data); // todo does not work why?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// create mipmaps
	if (mipmap) { glGenerateMipmap(GL_TEXTURE_2D); }
	// unbind the texture (just in case someone will mess up with texture calls later)
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();
	// free data from stbi
	stbi_image_free(data);
}

GUIImage::GUIImage(GUIImage&& other) noexcept
{
	m_path   = std::move(other.m_path);
	m_texID  = other.m_texID;
	m_width  = other.m_width;
	m_height = other.m_height;

	other.m_texID = 0; // imporant, so that when moved, then the destructor does nothing
}

GUIImage::~GUIImage()
{
	if (m_texID > 0)
	{
		glDeleteTextures(1, &m_texID); // doesnt do anything if texID is 0, but we do check anyway for speed
	}
}
