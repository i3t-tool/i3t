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

namespace Core
{
/**
 * Base class for objects that are the actual data managed by a ResourceManager.
 * This is meant to be used with multiple inheritance if necessary.
 */
class ManagedResource
{
public:
	size_t m_resourceId{0}; ///< Resource ID of an object
	                        ///< An identifier used by a ResourceManager
};
} // namespace Core