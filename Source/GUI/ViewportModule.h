#pragma once

#include "Core/Defs.h"
#include "Core/Module.h"
#include "Elements/Windows/ViewportWindow.h"
#include "State/Stateful.h"
#include "Viewport/Viewport.h"

/**
 * Module managing the I3T scene view. Scene view is displayed in ViewportWindow.h.
 */
class ViewportModule : public Module, public IStateful
{
	UPtr<Vp::Viewport> m_viewport;

public:
	std::vector<Ptr<UI::ViewportWindow>> m_viewportWindows;

	ViewportModule();

	Vp::Viewport& getViewport();

	void onInit() override;
	void onUpdate(double deltaSeconds) override;

	Ptr<UI::ViewportWindow> createViewportWindow();

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
