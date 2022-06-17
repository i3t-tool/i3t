#pragma once
#include <functional>
#include <string>

/**
 * @brief A class for holding an image texture id with properties of the image. Goes by the RAII standard by loading the image at start and deleting it at its end.
*/
struct GUIImage
{
	/**
   * @brief Constructor which loads the specified image into OpenGl memory. WARNING: Throws an exception when file not found or texture loading fails!
   * @param path Whole path to the image file to be loaded
  */
	GUIImage(const std::string& path);

	/**
   * @brief Move constructor which makes sure the previous instance doesn't have the GLuint texture id, which would then be deleted with the destructor
  */
	GUIImage(GUIImage&& other) noexcept;

	/**
   * @brief Copy constructor is NOT ALLOWED - use pointers (preferably smart) if you want to have the image referable from multiple places
  */
	GUIImage(const GUIImage& other) = delete;

	/**
   * @brief Destructor which deletes the image from OpenGL memory
   */
	~GUIImage();

	std::string  m_path;
	unsigned int m_texID;
	int          m_width;
	int          m_height;


	//int m_preferedWidth; // -1 = always stretch to contents, 0 = prefer original width, other = stretch up to this max width
	//std::function<void(GUIImage)> destructorFn; // possible use case when there would be an external object eg for opengl destructing, but it was decided to do it here
};
