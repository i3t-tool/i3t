#pragma once

#include "Core/Defs.h"
#include "Core/Module.h"
#include "GUI/Elements/Windows/ViewportWindow.h"
#include "State/Stateful.h"
#include "Viewport/Viewport.h"
#include "ViewportModuleSettings.h"

/**
 * Module managing the I3T scene view. Scene view is displayed in ViewportWindow.h.
 */
class ViewportModule : public Module, public IStateful
{
	UPtr<Vp::Viewport> m_viewport;

public:
	/// Vector containing all active viewport windows. New windows are added dynamically but there will always be at
	/// least one window. The windows are also ordered, the first window is always the primary one with index 1, the
	/// next one will be index 2 and so on. Closing a window will decrement the index of ones after it.
	std::vector<Ptr<UI::ViewportWindow>> m_viewportWindows;
	std::vector<UPtr<ViewportWindowSettings>> m_windowSettings;
	ViewportModuleSettings m_settings;

	// TODO: Implement this properly, manipulators are disabled in secondary viewport windows for now
	/// Index of the window that is being interacted with, 0 for none
	int m_interactingWindowIndex = 0;

	ViewportModule();

	Vp::Viewport& getViewport();

	void onInit() override;
	void onUpdate(double deltaSeconds) override;

	Ptr<UI::ViewportWindow> getOrCreateViewportWindow(int index);
	Ptr<UI::ViewportWindow> showNewViewportWindow();

	void collectScene(UI::ViewportWindow* window); ///< Update current settings with data from the current scene
	void updateScene(UI::ViewportWindow* window);  ///< Update current scene from current settings.

	void collectGlobal(); ///< Update current settings with global data
	void updateGlobal();  ///< Update global values from current settings.

private:
	/////////////////////////////////////////
	// State save/load
	/////////////////////////////////////////

	Memento saveScene(State::Scene* scene) override;
	void loadScene(const Memento& memento, State::Scene* scene) override;
	void appendScene(const Memento& memento, State::Scene* scene) override {}
	void clearScene(bool newScene) override;

	Memento saveGlobal() override;
	void loadGlobal(const Memento& memento) override;
	void clearGlobal() override;
};
