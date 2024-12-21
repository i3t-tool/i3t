/**
 * \file
 * \brief
 * \authors Martin Herich
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "GUI/Elements/IWindow.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "TextEditor.h"

#include <array>
#include <functional>
#include <memory>

namespace Workspace
{
class ScriptingNode;
}

class ScriptEditorModal : public ModalWindow
{
public:
	explicit ScriptEditorModal(Workspace::ScriptingNode* node);

	void onImGui() override;

private:
	void onSave();

	Workspace::ScriptingNode* m_node;
	std::unique_ptr<TextEditor> m_editor;
	std::string m_message;
	std::string m_editedScript;
};
