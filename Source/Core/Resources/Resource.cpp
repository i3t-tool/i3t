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
#include "Resource.h"

#include "Config.h"

using namespace Core;

Resource::Resource(std::string alias, std::string path, ResourceType type)
    : alias(std::move(alias)), path(path), resourceType(type)
{
	// Empty
}

Resource::Resource(std::string alias, size_t hashId, std::string path, ResourceType type, std::shared_ptr<void> data)
    : alias(std::move(alias)), hashId(hashId), path(path), resourceType(type), data(data)
{
	// Empty
}
