#include "ViewportModuleSettings.h"

#include "rttr/registration.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<ViewportModuleSettings>("ViewportModuleSettings")
	    .property("lightFollowsCamera", &ViewportModuleSettings::lightFollowsCamera);
}