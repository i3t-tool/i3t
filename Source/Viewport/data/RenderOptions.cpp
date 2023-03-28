#include "RenderOptions.h"

using namespace Vp;

RenderOptions::RenderOptions()
{
	multisample = false;
	samples = 4;
	framebufferAlpha = false;
	wboit = false;
	wboitFunc = 0;
	selection = false;

	clearColor = glm::vec3(0.2f, 0.2f, 0.2f);
}
