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
#include "SceneRenderTarget.h"

using namespace Vp;

std::weak_ptr<Framebuffer> SceneRenderTarget::getOutputFramebuffer()
{
	return outputFramebuffer;
}

void SceneRenderTarget::setOutputFramebuffer(std::weak_ptr<Framebuffer> framebuffer)
{
	outputFramebuffer = framebuffer;
}

std::weak_ptr<Framebuffer> SceneRenderTarget::getFramebuffer(unsigned int index)
{
	try
	{
		return std::weak_ptr<Framebuffer>(m_framebuffers.at(index));
	}
	catch (const std::out_of_range& e)
	{
		return std::weak_ptr<Framebuffer>();
	}
}

const RenderOptions& SceneRenderTarget::getRenderOptions() const
{
	return renderOptions;
}

void SceneRenderTarget::setRenderOptions(const RenderOptions& renderOptions)
{
	this->renderOptions = renderOptions;
}

void SceneRenderTarget::addFramebuffer(std::shared_ptr<Framebuffer> framebuffer)
{
	this->m_framebuffers.push_back(framebuffer);
}
