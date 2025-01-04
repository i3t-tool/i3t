/**
 * \file
 * \brief
 * \authors Sofie Sasorina, Jaroslav Holecek
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "glm/glm.hpp"

#include "Config.h"
#include "Core/Result.h"
#include "I3T.h"
#include "Logger/Logger.h"

#include "GUI/Elements/IWindow.h"

#include "GUI/Workspace/WorkspaceDiwne.h"

/**
 * \brief Class for Workspace window object, stores everything what Workspace window need
 */
class WorkspaceWindow : public IWindow, public IStateful
{
public:
	I3T_WINDOW(WorkspaceWindow)

	explicit WorkspaceWindow(bool show);
	~WorkspaceWindow() override;

private:
	Memento saveScene(Scene* scene) override;

	void loadScene(const Memento& memento, Scene* scene) override;

	void clearScene() override;

public:
	Memento saveGlobal() override;
	void loadGlobal(const Memento& memento) override;
	void clearGlobal() override;

private:
	void initDiwneFromTheme();

public:
	Workspace::WorkspaceDiwne& getNodeEditor();

	Application& m_wholeApplication;

	void render();

private:
	void showEditMenu();
};
