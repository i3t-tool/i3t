#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "Logger/Logger.h"

#include "Viewport/data/DisplayOptions.h"
#include "Viewport/entity/GameObject.h"
#include "Viewport/framebuffer/Framebuffer.h"
#include "Viewport/scene/Lighting.h"

namespace Vp
{
class Viewport;
class Entity;
class ICamera;
class Lighting;
class SceneRenderTarget;
class RenderOptions;

/**
 * A scene representing a "game world" that contains entities and is viewed by a
 * camera. Handles drawing and logic updates.
 */
class Scene
{
public:
	friend class Viewport;
	Viewport* m_viewport;

	std::shared_ptr<ICamera> m_camera;
	std::shared_ptr<Lighting> m_lighting;

protected:
	std::vector<std::shared_ptr<Entity>> m_entities;

private:
	// Temporary lists for transparency sorting
	std::vector<Entity*> m_unorderedTransparentEntities;
	std::vector<Entity*> m_explicitTransparencyOrderEntitiesFirst;
	std::vector<Entity*> m_explicitTransparencyOrderEntitiesLast;

public:
	explicit Scene(Viewport* viewport);
	virtual ~Scene() = default;

	/**
	 * Initialises the scene. Should be called before any draw operations.
	 */
	virtual void init(){};

	/**
	 * Draw the scene using the scene's camera.
	 * \param width Width of the
	 */
	virtual void draw(int width, int height, SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions);

	/**
	 * Draw the scene using the provided view and projection matrices.
	 */
	virtual void draw(int width, int height, glm::mat4 view, glm::mat4 projection, SceneRenderTarget& renderTarget,
	                  const DisplayOptions& displayOptions);

	virtual Ptr<SceneRenderTarget> createRenderTarget(const RenderOptions& options);

	/**
	 * Update entity logic.
	 */
	virtual void update(double dt);

	/**
	 * Update input logic.
	 *
	 * @param dt Time since last frame
	 * @param mousePos Current mouse position relative to the window
	 * @param windowSize Current window size
	 */
	void processInput(double dt, glm::vec2 mousePos, glm::ivec2 windowSize);

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

protected:
	void sortUnorderedTransparentEntities(glm::mat4 view, std::vector<Entity*>& entities);
	void sortExplicitlyOrderedTransparentEntities(std::vector<Entity*>& entities);

	void renderSortedTransparentEntities(glm::mat4 view, glm::mat4 projection,
	                                     const std::vector<Entity*>& entities) const;
};
} // namespace Vp
