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

#include "Core/Application.h"

class I3TApplication : public Application
{
public:
	bool m_debugWindowManager = false; ///< Debug switch for WindowManager, toggled in Help > Debug window manager
	bool m_debugTrackball = false;     ///< Debug switch for WindowManager, toggled in Help > Debug trackball

protected:
	void onInit() override;
	void onBeginFrame() override{};
	void onEndFrame() override{};
	void onUpdate(double delta) override;
	void onClose() override;
};

// Forward declarations
namespace Vp
{
class Viewport;
}
class UIModule;

// Static util methods
namespace I3T
{
I3TApplication& app();
UIModule* getUI();
Vp::Viewport* getViewport();
} // namespace I3T