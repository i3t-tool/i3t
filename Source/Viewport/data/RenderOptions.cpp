#include "RenderOptions.h"
#include "Viewport/shader/PhongShader.h"

using namespace Vp;

RenderOptions::RenderOptions()
{
	multisample = false;
	samples = 4;
	framebufferAlpha = false;
	wboit = false;
	wboitFunc = 0;
	selection = false;
	lightingModel = PhongShader::LightingModel::BLINN_PHONG;

	clearColor = glm::vec3(0.2f, 0.2f, 0.2f);
}
