#pragma once

#include <memory>
#include <unordered_map>

#include "Core/Defs.h"

#include "Viewport/shader/Shader.h"

namespace Vp
{
class ColorShader;
class PhongShader;
class GridShader;
class FrustumShader;
class WBOITCompositeShader;

class Shaders
{
public:
	std::shared_ptr<PhongShader> m_phongShader;
	std::shared_ptr<ColorShader> m_colorShader;
	std::shared_ptr<GridShader> m_gridShader;
	std::shared_ptr<FrustumShader> m_frustumShader;
	std::shared_ptr<WBOITCompositeShader> m_wboitCompositeShader;

	void create();
};

} // namespace Vp
