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

#include "Core/Nodes/Id.h"

#include "Viewport/entity/TexturedObject.h"

// For debug purposes, enables bounding box drawing
#define SHOW_BOUNDING_BOX 0

namespace Vp
{
class PhongShader;
class ColoredObject;

/**
 * Viewport entity representing a Model node in Core/GUI
 */
class SceneModel : public TexturedObject
{
public:
	Core::ID m_guiNodeId; ///< ID of the gui node object is representing

	bool m_showAxes{true};

	std::string m_modelAlias{};

	std::weak_ptr<ColoredObject> m_axes; ///< Visualisation of the basis vectors

#if SHOW_BOUNDING_BOX
	std::weak_ptr<ColoredObject> m_boundingBox; ///< For debug purposes
#endif

	SceneModel(Core::Mesh* mesh, PhongShader* shader);

	/**
	 * Load mesh using an alias.
	 */
	SceneModel(std::string modelAlias, PhongShader* shader);

	void setModel(std::string modelAlias);
	std::string getModel();

	void render(Shader* shader, glm::mat4 view, glm::mat4 projection, bool silhouette) override;
	void update(Scene& scene) override;

#if SHOW_BOUNDING_BOX
	void updateBoundingBox();
#endif

	void onSceneAdd(Scene& scene) override;
	void onSceneRemove(Scene& scene) override;
};
} // namespace Vp
