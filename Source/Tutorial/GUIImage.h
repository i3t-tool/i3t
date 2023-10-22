/**
 * \file
 * \brief
 * \author Miroslav Müller
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once
#include <functional>
#include <string>

/**
 * @brief A class for holding an image texture id with properties of the image.
 * Goes by the RAII standard by loading the image at start and deleting it at
 * its end.
 */
struct GUIImage
{
	/**
	 * @brief Constructor which loads the specified image into OpenGl memory.
	 * WARNING: Throws an exception when file not found or texture loading fails!
	 * @param path Whole path to the image file to be loaded
	 */
	GUIImage(const std::string& path);

	/**
	 * @brief Move constructor which makes sure the previous instance doesn't have
	 * the GLuint texture id, which would then be deleted with the destructor
	 */
	GUIImage(GUIImage&& other) noexcept;

	/**
	 * @brief Copy constructor is NOT ALLOWED - use pointers (preferably smart) if
	 * you want to have the image referable from multiple places
	 */
	GUIImage(const GUIImage& other) = delete;

	/**
	 * @brief Destructor which deletes the image from OpenGL memory
	 */
	~GUIImage();

	std::string m_path;
	unsigned int m_texID;
	int m_width;
	int m_height;

	// int m_preferedWidth; // -1 = always stretch to contents, 0 = prefer
	// original width, other = stretch up to this max width
	// std::function<void(GUIImage)> destructorFn; // possible use case when there
	// would be an external object eg for opengl destructing, but it was decided
	// to do it here
};
