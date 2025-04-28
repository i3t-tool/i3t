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

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "Core/Types.h"
#include "Logger/Logger.h"

#include "Viewport/SelectStencil.h"
#include "Viewport/data/DisplayOptions.h"
#include "Viewport/data/ViewportSettings.h"
#include "Viewport/entity/GameObject.h"
#include "Viewport/framebuffer/Framebuffer.h"
#include "Viewport/scene/Lighting.h"
#include "Viewport/shader/Shaders.h"

namespace Vp
{
class Viewport;
class Entity;
class AbstractCamera;
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

	std::shared_ptr<AbstractCamera> m_camera;
	std::shared_ptr<Lighting> m_lighting;
	std::shared_ptr<SelectStencil> m_selectStencil;

protected:
	std::vector<std::shared_ptr<Entity>> m_entities;
	// TODO: (DR) Support for multiple selected entities might be nice, but perhaps not needed
	// TODO: (DR) It might make more sense to reuse unused ids to keep the below 255
	//	Also selectable entities should have the lowest id possible
	//	Right now quickly re-adding entities will make the id explode past 255!!
	Entity* m_selectedEntity = nullptr;

	std::vector<std::function<void(Entity*)>>
	    m_selectionCallbacks; ///< Callbacks that get triggered on entity selection

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
	 * @param width Desired framebuffer pixel width
	 * @param height Desired framebuffer pixel height
	 * @param model Implicit model matrix, multiplies all model transforms from the left.
	 */
	virtual void draw(int width, int height, const glm::mat4& model, SceneRenderTarget& renderTarget,
	                  const DisplayOptions& displayOptions);

	/**
	 * Draw the scene using a provided camera.
	 * @param width Desired framebuffer pixel width
	 * @param height Desired framebuffer pixel height
	 * @param camera The camera to render the scene with
	 * @param model Implicit model matrix, multiplies all model transforms from the left.
	 */
	virtual void draw(int width, int height, const std::shared_ptr<AbstractCamera>& camera, const glm::mat4& model,
	                  SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions);

	/**
	 * Draw the scene using the provided view and projection matrices.
	 * @param width Desired framebuffer pixel width
	 * @param height Desired framebuffer pixel height
	 * @param model Implicit model matrix, multiplies all model transforms from the left.
	 */
	virtual void draw(int width, int height, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	                  SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions);

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

	void addSelectionCallback(std::function<void(Entity*)> callback)
	{
		this->m_selectionCallbacks.push_back(callback);
	}
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

	const std::vector<std::shared_ptr<Entity>>& getEntities() const
	{
		return m_entities;
	}

	/////////////////////////////////////////
	// State save/load
	/////////////////////////////////////////

protected:
	void drawStandard(int width, int height, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	                  SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions,
	                  RenderOptions renderOptions, bool alpha, glm::vec3 clearColor, const Ptr<Framebuffer>& mainFBO);

	void drawWboit(int width, int height, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	               SceneRenderTarget& renderTarget, const DisplayOptions& displayOptions, RenderOptions renderOptions,
	               bool alpha, glm::vec3 clearColor, const Ptr<Framebuffer>& mainFBO,
	               const Ptr<Framebuffer>& transparentFBO);

	void drawHighlight(int width, int height, const glm::mat4& model, const glm::mat4& view,
	                   const glm::mat4& projection, SceneRenderTarget& renderTarget,
	                   const DisplayOptions& displayOptions, ViewportSettings& stg, const Ptr<Framebuffer>& mainFBO,
	                   const Ptr<Framebuffer>& selectionFBO, const Ptr<Framebuffer>& selectionBlurFBO,
	                   const Ptr<Framebuffer>& selectionBlurSecondPassFBO);

	void sortUnorderedTransparentEntities(const glm::mat4& model, const glm::mat4& view,
	                                      std::vector<Entity*>& entities);
	void sortExplicitlyOrderedTransparentEntities(std::vector<Entity*>& entities);

	void renderSortedTransparentEntities(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	                                     const std::vector<Entity*>& entities) const;
};
} // namespace Vp
