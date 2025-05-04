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

#include "Viewport/data/RenderOptions.h"
#include "Viewport/framebuffer/Framebuffer.h"

namespace Vp
{
// TODO: In Dengine, framebuffers are indexed by a string in a hashmap, so that reacting to changes
//  in code is easier, it does however bring a runtime cost, likely negligible though
/**
 * A set of framebuffers and their settings into which a Scene can draw.
 * This is a separate class to allow rendering a single scene with different settings
 * (requiring framebuffer(s) with different settings)
 */
class SceneRenderTarget
{
private:
	std::vector<std::shared_ptr<Framebuffer>> m_framebuffers;
	std::weak_ptr<Framebuffer> outputFramebuffer;

	RenderOptions renderOptions;

public:
	std::weak_ptr<Framebuffer> getOutputFramebuffer();
	void setOutputFramebuffer(std::weak_ptr<Framebuffer> framebuffer);

	std::weak_ptr<Framebuffer> getFramebuffer(unsigned int index);
	void addFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

	const RenderOptions& getRenderOptions() const;
	void setRenderOptions(const RenderOptions& renderOptions);
};

} // namespace Vp
