/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

class Module
{
	friend class Application;

public:
	Module() = default;
	virtual ~Module() = default;
	virtual void onInit() {}
	virtual void onBeforeFrame() {}
	virtual void onBeginFrame() {}
	virtual void onEndFrame() {}
	virtual void onUpdate(double deltaSeconds) {}
	virtual void onClose() {}
};
