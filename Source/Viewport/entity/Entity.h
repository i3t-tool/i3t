/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "pgr.h"

namespace Vp
{
class Scene;
class Shader;

enum class DisplayType
{
	Default,
	Axes,
	Grid,
	Camera,
	Frustum
};

/**
 * \brief A scene entity base class.
 */
class Entity
{
protected:
	DisplayType m_displayType{DisplayType::Default};

public:
	glm::mat4 m_modelMatrix{1}; ///< Model transformation of the entity

	// Some basic properties
	bool m_backFaceCull;
	bool m_visible;
	bool m_opaque;
	bool m_wboit;    ///< Whether this entity should be rendered using order independent transparency,
	                 ///< relevant if using a shader that supports wboit
	int m_wboitFunc; ///< Index of the weight function to be used in wboit
	unsigned int m_explicitTransparencyOrder; ///< Explicit order for ordered transparency

	// Selection properties
	bool m_selectable;     ///< Whether this entity should be written into the selection stencil buffer
	int m_selectionId{-1}; ///< Id of the object, -1 if not assigned, for selection purposes
	bool m_highlight; ///< Whether to paint this entity with an outline, can be used for selection or just highlighting
	bool m_highlightUseDepth; ///< Whether the highlight outline should respect the scene's depth buffer
	glm::vec3 m_highlightColor;

	Shader* m_shader{nullptr};

	Entity();
	virtual ~Entity() = default;

	virtual void update(Scene& scene) = 0;

	void render(glm::mat4 view, glm::mat4 projection)
	{
		render(view, projection, false);
	}
	void render(glm::mat4 view, glm::mat4 projection, bool silhouette)
	{
		render(m_shader, view, projection, silhouette);
	}

protected:
	void render(Shader* shader, glm::mat4 view, glm::mat4 projection)
	{
		render(shader, view, projection, false);
	}

	/**
	 * IMPLEMENTATION NOTE:
	 * Derived entity classes should expect a certain kind of shader and be able to directly static cast to it.
	 * Currently there is no error checking and wrongly passed shader type will CAUSE UNDEFINED BEHAVIOUR!
	 *
	 * Using dynamic casts instead might be undesirable (slow) here (this might just be a premature optimalization)
	 * Generally the shader passed to this method is the m_shader of this entity.
	 * However we need functionality that allows us to change the shader temporarily (like for drawing silhouettes)
	 * Because of this the shader can also sometimes be something else, usually some derived type of the one the entity
	 * expects. I couldn't find a way to inform what kinds of shaders an entity is able to use in its render methods.
	 * The type of the shader must be the base Shader class and so some kind of casting must occur.
	 *
	 * Another option could be to use some kind of a typeid/enum system that tells us what type a shader is (rather than
	 * use dynamic cast). But that approach ignores inheritance which we need and having multiple types of shaders and
	 * entity member variable seems very inflexible.
	 *
	 * The true elegant solution probably doesn't involve inheritance at all and rather uses composition, together with
	 * an ECS or something. But it was decided to not dive into that realm and so we are stuck with this seemingly
	 * simple but not very flexible inheritance structure.
	 */
	virtual void render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette) = 0;

public:
	virtual void dispose() = 0;

	/**
	 * Called when the entity is added to a scene.
	 * @param scene Scene to which it has been added
	 */
	virtual void onSceneAdd(Scene& scene) {}

	/**
	 * Called when the entity is removed from a scene.
	 * @param scene Scene from which it has been removed
	 */
	virtual void onSceneRemove(Scene& scene) {}

	virtual void setDisplayType(DisplayType type)
	{
		m_displayType = type;
	}
	virtual DisplayType getDisplayType() const
	{
		return m_displayType;
	}

	virtual void setHighlighted(bool highlight)
	{
		m_highlight = highlight;
	}
	virtual bool isHighlighted() const
	{
		return m_highlight;
	}

	virtual void setHighlightColor(glm::vec3 highlightColor)
	{
		m_highlightColor = highlightColor;
	}
	virtual glm::vec3 getHighlightColor() const
	{
		return m_highlightColor;
	}
};
} // namespace Vp