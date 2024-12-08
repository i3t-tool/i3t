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

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"
#include "Scripting/Environment.h"

#include <memory>
#include <string>

static inline const Core::Operation DEFAULT_SCRIPTING_NODE_OPERATION = {"Script", "script"};

namespace Workspace
{
struct ScriptInterface
{
	Core::ID id;
	Core::Operation operation;
	sol::function onInit = sol::nil;
	sol::function onUpdate = sol::nil;
};

Result<std::unique_ptr<ScriptInterface>, Error> createScript(Core::ID id, const std::string& script);

void removeScript(Core::ID id);

class ScriptingNode : public CoreNodeWithPins
{
public:
	ScriptingNode(DIWNE::Diwne& diwne);

	/// Constructor for swapping the script
	ScriptingNode(DIWNE::Diwne& diwne, const std::string& script, std::unique_ptr<ScriptInterface> interface);

public:
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<ScriptingNode>(shared_from_this()));
	}

	void popupContent() override;
	int maxLengthOfData() override;
	void drawMenuLevelOfDetail() override;

	Core::ID getScriptId() const
	{
		return m_interface->id;
	}
	void reloadScript();

private:
	std::string m_script;
	ScriptInterface* m_interface = nullptr;
};
} // namespace Workspace
