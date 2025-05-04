#include "ViewportWindowSettings.h"

#include "rttr/registration.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<ViewportWindowSettings>("ViewportWindowSettings")
	    .property("manipulator_enabled", &ViewportWindowSettings::manipulator_enabled)
	    .property("camera", &ViewportWindowSettings::camera);
}