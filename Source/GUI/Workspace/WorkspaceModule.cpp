#include "WorkspaceModule.h"

#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"
#include "State/StateManager.h"

#include "GUI/Workspace/Nodes/Basic/CoreNode.h"

#include "Viewport/Viewport.h"

static DIWNE::SettingsDiwne settingsDiwne;

using namespace Workspace;

Ptr<WorkspaceDiwne> WorkspaceModule::g_editor;
WorkspaceSettings WorkspaceModule::g_settings;

int WorkspaceModule::g_pinStyle = 1;
int WorkspaceModule::g_pinStyleMul = 0;
int WorkspaceModule::g_pinStylePulse = 0;
int WorkspaceModule::g_pinStyleScreen = 0;
int WorkspaceModule::g_pinStyleModelMatrix = 2;

WorkspaceModule::WorkspaceModule()
{
	settingsDiwne.useChildWindow = false;
	g_editor = std::make_shared<WorkspaceDiwne>("WorkspaceEditor", &settingsDiwne);

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
	visitor.dump(getNodeEditor().getCoreNodes().collect());

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

	g_editor->manipulatorStartCheck3D();
}

void WorkspaceModule::clearScene()
{
	getNodeEditor().clear();
}

Memento WorkspaceModule::saveGlobal()
{
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Document workspaceDoc(&doc.GetAllocator());
	auto result = JSON::serializeToDocument(g_settings, workspaceDoc);
	if (!result)
	{
		return emptyMemento();
	}
	doc.AddMember("workspace", workspaceDoc, doc.GetAllocator());

	return doc;
}

void WorkspaceModule::loadGlobal(const Memento& memento)
{
	if (!memento.HasMember("workspace"))
	{
		LOG_ERROR("Cannot load global workspace data! No 'workspace' entry found!");
		return;
	}
	JSON::deserializeDocument(memento["workspace"], g_settings);
}

void WorkspaceModule::clearGlobal()
{
	g_settings = WorkspaceSettings();
}

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
		App::getModule<StateManager>().requestSnapshot();
	}

	return result;
}
