#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "Core/Types.h"
#include "Logger/Logger.h"

#include "Viewport/SelectStencil.h"
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
	std::shared_ptr<SelectStencil> m_selectStencil;

protected:
	std::vector<std::shared_ptr<Entity>> m_entities;
	// TODO: (DR) Support for multiple selected entities might be nice, but perhaps not needed
	// TODO: (DR) It might make more sense to reuse unused ids to keep the below 255
	//	Also selectable entities should have the lowest id possible
	//	Right now quickly re-adding entities will make the id explode past 255!!
	Entity* m_selectedEntity = nullptr;

	std::vector<std::function<void(Entity*)>> m_selectionCallbacks; ///< Callbacks that get triggered on entity selection

	// Temporary lists for transparency sorting
	std::vector<Entity*> m_unorderedTransparentEntities;
	std::vector<Entity*> m_explicitTransparencyOrderEntitiesFirst;
	std::vector<Entity*> m_explicitTransparencyOrderEntitiesLast;

	// Temporary list for selection/highlighting
	std::vector<Entity*> m_highlightedEntities;

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

	/**
	 * Create and populates a SceneRenderTarget object with expected framebuffer objects for the scenes render pass.
	 * Individual framebuffers are only identified by their index in the render target framebuffer array.
	 * It is expected that these indexes are kept track of somewhere else, or just expected to be a certain way.
	 *
	 * @param options Relevant render options to take into account during render target creation
	 * @return Owning pointer to the created render target object
	 */
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
	 * Update selection logic.
	 *
	 * @param diwne
	 * @param renderTarget Scene's render target
	 * @param mousePos Current mouse position relative to the window
	 * @param windowSize Current window size
	 */
	void processSelection(SceneRenderTarget& renderTarget, glm::vec2 mousePos, glm::ivec2 windowSize);

	void addSelectionCallback(std::function<void(Entity*)> callback) { this->m_selectionCallbacks.push_back(callback); }
	void triggerSelectionCallbacks(Entity* entity);

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
		if (entity->m_selectable)
		{
			entity->m_selectionId = m_selectStencil->registerStencil();
		}
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
			if (entityPtr->m_selectionId != -1)
			{
				m_selectStencil->freeStencil(entityPtr->m_selectionId);
				entityPtr->m_selectionId = -1;
			}
		}
		else
		{
			LOG_ERROR("Scene: Cannot remove a NULL entity!");
		}
	}

	const std::vector<std::shared_ptr<Entity>>& getEntities() const { return m_entities; }

protected:
	void sortUnorderedTransparentEntities(glm::mat4 view, std::vector<Entity*>& entities);
	void sortExplicitlyOrderedTransparentEntities(std::vector<Entity*>& entities);

	void renderSortedTransparentEntities(glm::mat4 view, glm::mat4 projection,
	                                     const std::vector<Entity*>& entities) const;
};
} // namespace Vp
