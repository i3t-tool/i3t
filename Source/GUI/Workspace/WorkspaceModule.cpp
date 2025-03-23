#include "WorkspaceModule.h"

#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"

#include "GUI/Workspace/Nodes/Basic/CoreNode.h"

#include "Viewport/Viewport.h"

static DIWNE::SettingsDiwne settingsDiwne;

using namespace Workspace;

Ptr<WorkspaceDiwne> WorkspaceModule::g_editor;

WorkspaceModule::WorkspaceModule()
{
	g_editor = std::make_shared<WorkspaceDiwne>(&settingsDiwne);

	App::getModule<StateManager>().addOriginator(this);

	// Setup viewport selection callback
	I3T::getViewport()->getMainScene().lock()->addSelectionCallback([](Vp::Entity* newlySelectedEntity) {
		// Save information about this callback and perform actions based on it later while in workspace window context.
		// This is a workaround due to viewport selection occurring in unknown order at unknown time.
		g_editor->m_viewportSelectionChanged = true;
		g_editor->m_viewportLastSelectedEntity = newlySelectedEntity;
	});
}

WorkspaceDiwne& WorkspaceModule::getNodeEditor()
{
	return *g_editor;
}

/////////////////////////////////////////
// State save/load
/////////////////////////////////////////

Memento WorkspaceModule::saveScene(Scene* scene)
{
	Memento memento;
	rapidjson::Value::AllocatorType& a = memento.GetAllocator();

	SerializationVisitor visitor(memento);
	visitor.dump(getNodeEditor().getAllCoreNodes().collect());

	JSON::addFloat(memento["workspace"], "zoom", g_editor->canvas().getZoom(), a);
	JSON::addRect(memento["workspace"], "workArea", g_editor->canvas().getViewportRectDiwne(), a);

	return memento;
}

void WorkspaceModule::loadScene(const Memento& memento, Scene* scene)
{
	clearScene();

	if (!memento.HasMember("workspace"))
	{
		LOG_ERROR("Failed to load workspace! No 'workspace' member found.");
		return;
	}

	NodeDeserializer::createFrom(memento);

	if (memento["workspace"].HasMember("zoom"))
		g_editor->setZoom(memento["workspace"]["zoom"].GetFloat());

	if (memento["workspace"].HasMember("workArea"))
		g_editor->canvas().setViewportRectDiwne(JSON::getRect(memento["workspace"]["workArea"]));
}

void WorkspaceModule::clearScene()
{
	getNodeEditor().clear();
}

Memento WorkspaceModule::saveGlobal()
{
	return emptyMemento();
}

void WorkspaceModule::loadGlobal(const Memento& memento) {}

void WorkspaceModule::clearGlobal() {}

/////////////////////////////////////////
// NodeEditor management
/////////////////////////////////////////

bool WorkspaceModule::connectNodesNoSave(Ptr<CoreNode> lhs, Ptr<CoreNode> rhs, int lhsPin, int rhsPin)
{
	bool success = std::static_pointer_cast<CoreNodeWithPins>(rhs)->getInputs().at(rhsPin)->plug(
	    std::static_pointer_cast<CoreNodeWithPins>(lhs)->getOutputs().at(lhsPin).get(), false);
	if (!success)
	{
		LOG_INFO("Cannot connect pin{} to pin{} of nodes {} and {}", lhs->getNodebase()->getSignature(),
		         rhs->getNodebase()->getSignature(), lhsPin, rhsPin);
	}
	rhs->updateDataItemsWidth();
	lhs->updateDataItemsWidth();
	return success;
}

bool WorkspaceModule::connectNodes(Ptr<CoreNode> lhs, Ptr<CoreNode> rhs, int lhsPin, int rhsPin)
{
	const auto result = connectNodesNoSave(lhs, rhs, lhsPin, rhsPin);
	if (result)
	{
		App::getModule<StateManager>().takeSnapshot();
	}

	return result;
}
