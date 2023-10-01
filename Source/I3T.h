#pragma once

#include "Core/Application.h"

class I3TApplication : public Application
{
public:
	bool m_debugWindowManager = false; ///< Debug switch for WindowManager, toggled in Help > Debug window manager
	bool m_debugTrackball = false;     ///< Debug switch for WindowManager, toggled in Help > Debug trackball

protected:
	void onInit() override;
	void onBeginFrame() override{};
	void onEndFrame() override{};
	void onUpdate(double delta) override;
	void onClose() override;
};

// Forward declarations
namespace Vp
{
class Viewport;
}
class UIModule;

// Static util methods
namespace I3T
{
I3TApplication& app();
UIModule* getUI();
Vp::Viewport* getViewport();
} // namespace I3T