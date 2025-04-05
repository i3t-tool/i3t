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
#include "GUI/Workspace/Nodes/ScriptingNode.h"

#include "GUI/I3TGui.h"

#include <regex>

#include "GUI/Elements/Modals/ScriptEditor.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "Scripting/ScriptingModule.h"

constexpr const char* SCRIPTING_NODE_TEMPLATE = R"(
I3T.workspace.__scripts[{}] = {{}}
do
    local self = I3T.workspace.__scripts[{}]
    {}
end)";

constexpr const auto ERROR_LINE_OFFSET = 4;
/// This is somehow magic number based on the observed behavior of the error message.
constexpr const auto ONE_LINE_SCRIPT_ERROR_LINE_OFFSET = 6;

//------------------------------------------------------------------------------------------------//

class ScriptingNode : public Core::Node
{
public:
	ScriptingNode() : Node(DEFAULT_SCRIPTING_NODE_OPERATION) {}

	explicit ScriptingNode(std::unique_ptr<Workspace::ScriptInterface> interface,
	                       Workspace::ScriptingNode* workspaceNode)
	    : Node(interface->operation), m_workspaceNode(workspaceNode), m_interface(std::move(interface))
	{}

	~ScriptingNode() override
	{
		if (m_interface)
		{
			Workspace::removeScript(m_interface->id);
		}
	}

	/// Cannot be used Node::onInit function because it is called before the node is added to the workspace.
	void performInit()
	{
		if (m_workspaceNode != nullptr && m_interface != nullptr)
		{
			auto& lua = Application::getModule<ScriptingModule>().environment();
			auto script = lua["I3T"]["workspace"]["__scripts"][m_interface->id];

			script["node"] = std::dynamic_pointer_cast<Workspace::CoreNode>(m_workspaceNode->shared_from_this());

			if (m_interface->onInit)
			{
				m_interface->onInit();
			}
			m_isInitialized = true;
		}
	}

	void updateValues(int inputIndex) override
	{
		if (m_isInitialized && m_interface->onUpdate)
		{
			m_interface->onUpdate();
		}
	}

	Workspace::ScriptInterface* getInterface() const
	{
		return m_interface.get();
	}

private:
	Workspace::ScriptingNode* m_workspaceNode = nullptr;
	std::unique_ptr<Workspace::ScriptInterface> m_interface = nullptr;

	bool m_isInitialized = false;
};

//------------------------------------------------------------------------------------------------//

namespace Workspace
{
Result<std::unique_ptr<Workspace::ScriptInterface>, ScriptError> createScript(Core::ID id, const std::string& script)
{
	auto& scripting = Application::getModule<ScriptingModule>();

	auto toExecute = fmt::format(SCRIPTING_NODE_TEMPLATE, id, id, script);
	auto maybeError = scripting.runScript(toExecute.c_str());

	if (maybeError)
	{
		auto& err = maybeError.value();

		if (err.line)
		{
			auto type = err.type.value();
			auto line = err.line.value();

			// must contains max 1 newline character
			const auto newlinesCount = std::ranges::count(script, '\n');
			const auto isOneLine = newlinesCount == 0 || newlinesCount == 1;

			if (isOneLine)
			{
				line -= ONE_LINE_SCRIPT_ERROR_LINE_OFFSET;
			}
			else
			{
				line -= ERROR_LINE_OFFSET;
			}

			err.line = line;
		}

		return Err(err);
	}

	// auto node = getNode(scripting, id);
	auto& lua = scripting.environment();
	auto node = lua["I3T"]["workspace"]["__scripts"][id];

	if (node["operation"] != sol::nil && !node["operation"].is<Core::Operation>())
	{
		return Err(ScriptError("node Operation is invalid"));
	}

	Core::Operation operation = node["operation"].get_or<Core::Operation>(DEFAULT_SCRIPTING_NODE_OPERATION);

	auto onInit = node["on_init"].get_or<sol::function>(sol::nil);
	if (onInit)
	{
		onInit.set_error_handler(lua["print"]);
	}

	auto onUpdate = node["on_update_values"].get_or<sol::function>(sol::nil);
	if (onUpdate)
	{
		onUpdate.set_error_handler(lua["print"]);
	}

	return std::make_unique<Workspace::ScriptInterface>(id, operation, std::move(onInit), std::move(onUpdate));
}

void removeScript(Core::ID id)
{
	auto& scripting = Application::getModule<ScriptingModule>();
	auto scripts = scripting.environment()["I3T"]["workspace"]["__scripts"];
	if (scripts != sol::nil)
	{
		scripts[id]["node"] = sol::nil;
		scripts[id] = sol::nil;
	}
}

//------------------------------------------------------------------------------------------------//

ScriptingNode::ScriptingNode(DIWNE::NodeEditor& diwne)
    : CoreNodeWithPins(diwne, Core::GraphManager::createCustomNode<::ScriptingNode>())
{}

ScriptingNode::ScriptingNode(DIWNE::NodeEditor& diwne, const std::string& script,
                             std::unique_ptr<ScriptInterface> interface)
    : CoreNodeWithPins(diwne, Core::GraphManager::createCustomNode<::ScriptingNode>(std::move(interface), this)),
      m_script(script)
{
	m_interface = getNodebase()->as<::ScriptingNode>()->getInterface();
}

ScriptingNode::~ScriptingNode()
{
	// TODO: (DR) Cannot use onDestroy(), because the DIWNE node is stored in Lua context? i don't really know
	m_nodebase->finalize();
}

void ScriptingNode::popupContent(DIWNE::DrawInfo& context)
{
	auto& nodeEditor = I3T::getWorkspace().getNodeEditor();

	CoreNode::drawMenuSetEditable();

	ImGui::Separator();

	CoreNode::drawMenuDuplicate(context);

	ImGui::Separator();

	if (I3TGui::MenuItemWithLog(_t("Edit script")))
	{
		auto modal = std::make_unique<ScriptEditorModal>((Workspace::ScriptingNode*) shared_from_this().get());
		I3T::getUI()->getWindowManager().openModal(std::move(modal));
	}

	if (I3TGui::MenuItemWithLog(_t("Reload script")))
	{
		reloadScript();
	}
}

int ScriptingNode::maxLengthOfData()
{
	return 0;
}

void ScriptingNode::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}

Ptr<ScriptingNode> ScriptingNode::setScript(const std::string& script)
{
	m_script = script;

	return reloadScript();
}

Ptr<ScriptingNode> ScriptingNode::reloadScript()
{
	auto self = std::static_pointer_cast<ScriptingNode>(shared_from_this());
	auto coreNode = std::static_pointer_cast<::ScriptingNode>(m_nodebase);
	const auto id = Core::IdGenerator::next();
	auto interface = createScript(id, m_script);
	if (!interface)
	{
		LOG_ERROR("Failed to reload script");
		return nullptr;
	}

	auto newNode = std::make_shared<ScriptingNode>(diwne, m_script, std::move(interface.value()));

	auto& nodeEditor = I3T::getWorkspace().getNodeEditor();

	bool success = nodeEditor.replaceAndReplugNode(self, newNode);
	if (!success)
		LOG_INFO("Replugging of scripting node didn't replug all pins perfectly. Perhaps there was a pin mismatch.");

	self->m_interface = nullptr;

	newNode->getNodebase()->as<::ScriptingNode>()->performInit();
	newNode->setPosition(getPosition());
	return newNode;
}

std::vector<std::tuple<Core::ID, int, Core::ID, int>> ScriptingNode::collectNeighbours(Core::ID overrideId) const
{
	std::vector<std::tuple<Core::ID, int, Core::ID, int>> neighbours;
	const auto node = getNodebase();
	for (auto i = 0L; i < node->getInputPins().size(); i++)
	{
		auto& pin = node->getInputPins()[i];
		if (pin.isPluggedIn())
		{
			auto* parentPin = pin.getParentPin();
			auto fromId = parentPin->Owner.getId();
			auto fromIndex = parentPin->Index;
			auto toId = overrideId;
			auto toIndex = i;
			neighbours.emplace_back(fromId, fromIndex, toId, toIndex);
		}
	}

	for (auto i = 0L; i < node->getOutputPins().size(); i++)
	{
		auto& pin = node->getOutputPins()[i];
		for (auto* outputPin : pin.getOutComponents())
		{
			auto fromId = overrideId;
			auto fromIndex = i;
			auto toId = outputPin->Owner.getId();
			auto toIndex = outputPin->Index;
			neighbours.emplace_back(fromId, fromIndex, toId, toIndex);
		}
	}

	return neighbours;
}
} // namespace Workspace
