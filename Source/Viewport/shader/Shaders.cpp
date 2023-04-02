#include "Shaders.h"

using namespace Vp;

#include "Core/Resources/ResourceManager.h"

#include "Viewport/shader/BoxBlurShader.h"
#include "Viewport/shader/ColorShader.h"
#include "Viewport/shader/FrustumShader.h"
#include "Viewport/shader/GridShader.h"
#include "Viewport/shader/PhongShader.h"
#include "Viewport/shader/ScreenOverlayShader.h"
#include "Viewport/shader/SelectionCompositeShader.h"
#include "Viewport/shader/WBOITCompositeShader.h"

void Shaders::load()
{
	if (loaded)
		return;

	// clang-format off
	m_phongShader = std::make_shared<PhongShader>(
		RMI.shader("Data/Shaders/phongVert.glsl", "Data/Shaders/phongFrag.glsl")
	);
	m_colorShader = std::make_shared<ColorShader>(
		RMI.shader("Data/Shaders/colorVert.glsl", "Data/Shaders/colorFrag.glsl")
	);
	m_gridShader = std::make_shared<GridShader>(
		RMI.shader("Data/Shaders/gridVert.glsl", "Data/Shaders/gridFrag.glsl")
	);
	m_frustumShader = std::make_shared<FrustumShader>(
		RMI.shader("Data/Shaders/frustumVert.glsl", "Data/Shaders/colorFrag.glsl")
	);
	m_wboitCompositeShader = std::make_shared<WBOITCompositeShader>(
		RMI.shader("Data/Shaders/basicVert.glsl", "Data/Shaders/wboitCompositeFrag.glsl")
	);
	m_boxBlurShader = std::make_shared<BoxBlurShader>(
		RMI.shader("Data/Shaders/basicVert.glsl", "Data/Shaders/boxBlurFrag.glsl")
	);
	m_selectionCompositeShader = std::make_shared<SelectionCompositeShader>(
		RMI.shader("Data/Shaders/basicVert.glsl", "Data/Shaders/selectionCompositeFrag.glsl")
	);
	m_screenOverlayShader = std::make_shared<ScreenOverlayShader>(
		RMI.shader("Data/Shaders/basicVert.glsl", "Data/Shaders/screenOverlayFrag.glsl")
	);
	// clang-format on

	loaded = true;
}
