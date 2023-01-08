#pragma once

#include <memory>
#include <vector>

#include "Viewport/Camera.h"
#include "Viewport/entity/GameObject.h"
#include "Viewport/scene/DisplayOptions.h"
#include "Viewport/scene/Lighting.h"

#include "Logger/Logger.h"

#include "glm/glm.hpp"

namespace Vp
{
class Viewport;
class Entity;
class Camera;
class Lighting;

/**
 * A scene representing a "game world" that contains entities and is viewed by a
 * camera. Handles drawing and logic updates.
 */
class Scene
{
public:
	friend class Viewport;
	Viewport* m_viewport;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Lighting> m_lighting;

protected:
	std::vector<std::shared_ptr<Entity>> m_entities;
	std::vector<Entity*> m_delayedRenderEntities; // Temporary list

public:
	explicit Scene(Viewport* viewport);
	virtual ~Scene() = default;

	virtual void init(){};

	/**
	 * Draw the scene.
	 * @param width
	 * @param height
	 */
	virtual void draw(int width, int height);
	virtual void draw(glm::mat4 view, glm::mat4 projection, const DisplayOptions& displayOptions = DisplayOptions());

	/**
	 * Update entity logic.
	 */
	virtual void update();

	void processInput();

	/**
	 * Adds entity to the scene.
	 *
	 * Entity is added via a shared pointer which is copied and stored.
	 * The original pointer should be discarded after this call and the weak
	 * reference can be further used.
	 *
	 * @tparam T Any derived type of Entity
	 * @param entity A shared pointer to the entity to add. This pointer should be
	 * destroyed afterwards.
	 * @return A weak pointer to the added entity of said type.
	 */
	template <typename T, typename std::enable_if<std::is_base_of<Entity, T>::value, bool>::type = true>
	std::weak_ptr<T> addEntity(std::shared_ptr<T> entity)
	{
		m_entities.push_back(entity);
		entity->onSceneAdd(*this);
		return entity;
	}

	/**
	 * Remove entity from the scene.
	 * The entity is passed as a weak pointer previously acquired by addEntity()
	 *
	 * @tparam T Any derived type of Entity
	 * @param entity
	 */
	template <typename T, typename std::enable_if<std::is_base_of<Entity, T>::value, bool>::type = true>
	void removeEntity(std::weak_ptr<T> entity)
	{
		if (auto entityPtr = entity.lock())
		{
			std::erase(m_entities, entityPtr);
			entityPtr->onSceneRemove(*this);
		}
		else
		{
			LOG_ERROR("Scene: Cannot remove a NULL entity!");
		}
	}
};
} // namespace Vp
