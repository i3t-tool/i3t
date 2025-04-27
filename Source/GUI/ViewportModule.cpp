#include "ViewportModule.h"

#include "Workspace/WorkspaceModule.h"

ViewportModule::ViewportModule()
{
	m_viewport = std::make_unique<Vp::Viewport>();
	App::getModule<StateManager>().addOriginator(this);
}
Vp::Viewport& ViewportModule::getViewport()
{
	return *m_viewport;
}
void ViewportModule::onInit()
{
	m_viewport->init();
}
void ViewportModule::onUpdate(double deltaSeconds)
{
	m_viewport->update(deltaSeconds);
}

Ptr<UI::ViewportWindow> ViewportModule::createViewportWindow()
{
	return m_viewportWindows.emplace_back(std::make_shared<UI::ViewportWindow>(true, m_viewport.get()));
}

/////////////////////////////////////////
// State save/load
/////////////////////////////////////////

Memento ViewportModule::saveScene(State::Scene* scene)
{
	// TODO: (DR) Other things aside from Viewport that could use the saveGlobal/Scene method:
	// - Default models are a global setting and are loaded using the outdated "Config" class
	// - Themes could be rewritten to use this global mechanism rather than the "ThemeLoader" class

	// Update current settings data with current values
	m_viewport->getMainScene()->saveSettings(m_viewport->getSettings(), true, false);

	rapidjson::Document doc;
	doc.SetObject();

	// Serialize scene settings
	rapidjson::Document viewportDoc(&doc.GetAllocator());
	auto result = JSON::serializeToDocument(m_viewport->getSettings().scene(), viewportDoc);
	if (!result)
	{
		return emptyMemento();
	}
	doc.AddMember("viewport", viewportDoc, doc.GetAllocator());

	return doc;
}

void ViewportModule::loadScene(const Memento& memento, State::Scene* scene)
{
	if (!memento.HasMember("viewport"))
	{
		LOG_ERROR("Cannot load scene viewport data! No 'viewport' entry found!");
		return;
	}
	JSON::deserializeDocument(memento["viewport"], m_viewport->getSettings().scene());

	// Update scene values
	m_viewport->getMainScene()->loadSettings(m_viewport->getSettings(), true, false);
}

void ViewportModule::clearScene(bool newScene)
{
	// Clear manipulators
	m_viewport->getManipulators().clearManipulators();

	// Clear the scene specific settings
	m_viewport->getSettings().scene() = Vp::ViewportSceneSettings();

	// Update scene values
	m_viewport->getMainScene()->loadSettings(m_viewport->getSettings(), true, false);
}

Memento ViewportModule::saveGlobal()
{
	rapidjson::Document doc;
	doc.SetObject();

	// Update current settings data with current values
	m_viewport->getMainScene()->saveSettings(m_viewport->getSettings(), false, true);

	// Serialize global settings
	rapidjson::Document viewportDoc(&doc.GetAllocator());
	auto result = JSON::serializeToDocument(m_viewport->getSettings().global(), viewportDoc);
	if (!result)
	{
		return emptyMemento();
	}
	doc.AddMember("viewport", viewportDoc, doc.GetAllocator());

	return doc;
}

void ViewportModule::loadGlobal(const Memento& memento)
{
	if (!memento.HasMember("viewport"))
	{
		LOG_ERROR("Cannot load global viewport data! No 'viewport' entry found!");
		return;
	}
	JSON::deserializeDocument(memento["viewport"], m_viewport->getSettings().global());

	// Update scene values
	m_viewport->getMainScene()->loadSettings(m_viewport->getSettings(), false, true);
}

void ViewportModule::clearGlobal()
{
	// Clear the global settings
	m_viewport->getSettings().global() = Vp::ViewportGlobalSettings();

	// Update scene values
	m_viewport->getMainScene()->loadSettings(m_viewport->getSettings(), false, true);
};
