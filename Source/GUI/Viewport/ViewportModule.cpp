#include "ViewportModule.h"

#include "GUI/Fonts/Bindings/BindingFontAwesome.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "Viewport/entity/GridObject.h"

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
	m_interactingWindowIndex = 0;

	// React to active tracking
	if (Core::GraphManager::isTracking())
	{
		// Scene view 1 must be visible
		m_viewportWindows[0]->show();
	}

	Theme& theme = I3T::getTheme();
	m_viewport->getMainScene()->m_worldGrid->m_gridColor = &theme.get(EColor::SceneViewGridColor).x;
	m_viewport->getMainScene()->m_worldGrid->m_axisXColor = &theme.get(EColor::SceneViewGridX).x;
	m_viewport->getMainScene()->m_worldGrid->m_axisYColor = &theme.get(EColor::SceneViewGridY).x;
	m_viewport->getMainScene()->m_worldGrid->m_axisZColor = &theme.get(EColor::SceneViewGridZ).x;

	m_viewport->getMainScene()->m_localGrid->m_gridColor = &theme.get(EColor::SceneViewGridLocalColor).x;
	m_viewport->getMainScene()->m_localGrid->m_axisXColor = &theme.get(EColor::SceneViewGridLocalX).x;
	m_viewport->getMainScene()->m_localGrid->m_axisYColor = &theme.get(EColor::SceneViewGridLocalY).x;
	m_viewport->getMainScene()->m_localGrid->m_axisZColor = &theme.get(EColor::SceneViewGridLocalZ).x;
}

Ptr<UI::ViewportWindow> ViewportModule::getOrCreateViewportWindow(int index)
{
	if (m_viewportWindows.size() <= index - 1)
	{
		collectGlobal();
		auto& ptr = m_viewportWindows.emplace_back(std::make_shared<UI::ViewportWindow>(this, index, true));
		I3T::getUI()->getWindowManager().addWindow(ptr);
		updateGlobal();
		return ptr;
	}
	return m_viewportWindows[index - 1];
}

Ptr<UI::ViewportWindow> ViewportModule::showNewViewportWindow()
{
	return getOrCreateViewportWindow(m_viewportWindows.size() + 1);
}
ViewportModuleSettings& ViewportModule::getSettings()
{
	return m_settings;
}
int ViewportModule::getWindowCount() const
{
	return m_viewportWindows.size();
}
const std::vector<Ptr<UI::ViewportWindow>> ViewportModule::getWindows()
{
	return m_viewportWindows;
}
const Ptr<UI::ViewportWindow>& ViewportModule::getWindow(int index)
{
	if (index < 0 || index >= m_viewportWindows.size())
		return {};
	return m_viewportWindows[index];
}

void ViewportModule::collectScene(UI::ViewportWindow* window)
{
	window->m_camera->saveSettings(window->m_settings.camera);

	// Common
	m_settings.lightFollowsCamera = m_viewport->getMainScene()->m_lightFollowsCamera;
}

void ViewportModule::updateScene(UI::ViewportWindow* window)
{
	window->m_camera->loadSettings(window->m_settings.camera);

	// Common
	m_viewport->getMainScene()->m_lightFollowsCamera = m_settings.lightFollowsCamera;
}
void ViewportModule::collectGlobal() {}
void ViewportModule::updateGlobal()
{
	for (auto& window : m_viewportWindows)
	{
		window->m_camera->loadSettings(m_viewport->getSettings().global().camera);
	}
}

/////////////////////////////////////////
// State save/load
/////////////////////////////////////////

Memento ViewportModule::saveScene(State::Scene* scene)
{
	// TODO: (DR) Other things aside from Viewport that could use the saveGlobal/Scene method:
	// - Default models are a global setting and are loaded using the outdated "Config" class
	// - Themes could be rewritten to use this global mechanism rather than the "ThemeLoader" class

	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Value viewport(rapidjson::kObjectType);

	// Serialize settings for each window
	rapidjson::Value windowArray(rapidjson::kArrayType);
	assert(!m_viewportWindows.empty() && "There must always be at least one ViewportWindow instance.");
	for (auto& window : m_viewportWindows)
	{
		// Update current settings data with current values
		collectScene(window.get());

		rapidjson::Document windowSettingsDoc(&doc.GetAllocator());
		auto result = JSON::serializeToDocument(window->m_settings, windowSettingsDoc);
		windowArray.PushBack(windowSettingsDoc, doc.GetAllocator());
	}
	viewport.AddMember("views", windowArray, doc.GetAllocator());

	rapidjson::Document sceneSettingsDoc(&doc.GetAllocator());
	auto result = JSON::serializeToDocument(m_settings, sceneSettingsDoc);
	viewport.AddMember("scene", sceneSettingsDoc, doc.GetAllocator());

	// TODO: (DR) Display options are not saved

	doc.AddMember("viewport", viewport, doc.GetAllocator());

	return doc;
}

void ViewportModule::loadScene(const Memento& memento, State::Scene* scene)
{
	if (!memento.HasMember("viewport"))
	{
		LOG_ERROR("Cannot load scene viewport data! No 'viewport' entry found!");
		return;
	}

	const rapidjson::Value& viewport = memento["viewport"];

	// Load common scene settings
	if (viewport.HasMember("scene"))
	{
		JSON::deserializeDocument(viewport["scene"], m_settings);
	}

	// Load individual scene view windows
	if (viewport.HasMember("views"))
	{
		int i = 0;
		for (auto& view : viewport["views"].GetArray())
		{
			ViewportWindowSettings viewSettings;
			JSON::deserializeDocument(view, viewSettings);

			auto window = getOrCreateViewportWindow(i + 1);
			window->m_settings = viewSettings;

			// Update viewport with loaded settings
			updateScene(window.get());
			i++;
		}
	}
}

void ViewportModule::clearScene(bool newScene)
{
	// Clear manipulators
	m_viewport->getManipulators().clearManipulators();

	m_settings = ViewportModuleSettings();

	// Clear the scene specific settings
	for (int i = 0; i < m_viewportWindows.size(); i++)
	{
		auto& window = m_viewportWindows[i];
		window->m_settings = ViewportWindowSettings();
		updateScene(window.get());
	}
}

Memento ViewportModule::saveGlobal()
{
	rapidjson::Document doc;
	doc.SetObject();

	// Update current settings data with current values
	collectGlobal();

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

	// Update values
	updateGlobal();
}

void ViewportModule::clearGlobal()
{
	// Clear the global settings
	m_viewport->getSettings().global() = Vp::ViewportGlobalSettings();

	// Update scene values
	updateGlobal();
};
