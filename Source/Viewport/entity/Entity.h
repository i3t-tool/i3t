#pragma once

#include "pgr.h"

namespace Vp
{
class Scene;

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
	// TODO: (DR) Figure out if ids need to be used or not
	/** Id of the object, -1 if not assigned, a positive integer otherwise */
	long m_id{-1};

	glm::mat4 m_modelMatrix{1}; ///< Model transformation of the entity

	// Some basic properties
	bool m_opaque = true;
	bool m_backFaceCull = false;
	bool m_visible = true;

	virtual ~Entity() = default;

	virtual void update(Scene& scene) = 0;
	virtual void render(glm::mat4 view, glm::mat4 projection) = 0;
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

	virtual void setDisplayType(DisplayType type) { m_displayType = type; }
	virtual DisplayType getDisplayType() const { return m_displayType; }
};
} // namespace Vp