/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Core/Module.h"

constexpr const char* I3T_TEST_AUTO_RUN = "I3T_TEST_AUTO_RUN";

class TestModule : public Module
{
public:
	bool& getShowTestWindow()
	{
		return m_showTestWindow;
	}

	struct ImGuiTest* createTest(const char* group, const char* name);

private:
	void onInit() override;
	void onBeginFrame() override;
	void onUpdate(double deltaSeconds) override;
	void onEndFrame() override;
	void onClose() override;

	/// Based on I3T_TEST_AUTO_RUN environment variable.
	/// If set to 1, the tests will run automatically and the application will close after the tests are done.
	bool m_autoRun = false;
	bool m_showTestWindow = false;
	struct ImGuiTestEngine* m_engine;
};
