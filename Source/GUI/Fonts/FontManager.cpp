#include "FontManager.h"

#include "Core/Application.h"
#include "Core/Defs.h"
#include "GUI/Fonts/Icons.h"

#include <imgui_impl_opengl3.h>

void FontManager::generateFonts(float dpiScale)
{
	auto& io = ImGui::GetIO();
	io.Fonts->Clear();
	ImGui_ImplOpenGL3_DestroyFontsTexture();

	// TODO: This serves no purpose, just to confuse us, the font sizes below can just be scaled by this constant
	constexpr float constScale = 1.2f;

	float fontScale = dpiScale * constScale;

	// https://www.unicode.org/roadmaps/bmp/
	// to show the atlas, use
	//  ImGuiIO& io = ImGui::GetIO();
	//  ImFontAtlas* atlas = io.Fonts;
	//  ImGui::ShowFontAtlas(atlas);
	// or
	// ImGui Demo / Configuration / Style / Fonts

	// (PF) minimalistic font set - 1024x2048 font atlas texture
	const ImWchar ranges[] = {
	    0x0020,
	    0x007F, // Basic Latin
	    0x0080,
	    0x017F, // Czech mini (Latin-1 Supplement + Latin extended A)
	    0x0400,
	    0x04FF, // Cyrillic
	    // 0x0080, 0x07FF, // Czech + lot of right to left characters -- too many never used characters
	    // 0x25FC, 0x2BC8, // Geometric shapes media buttons stop and play - are not in our fonts
	    // - this is a too long range instead of two characters
	    0,
	};
	const ImWchar symbolRanges[] = {
	    0x2300,
	    0x23E9,
	    0,
	};
	// https://symbl.cc/en/unicode/table/
	// These icons should be in unicode, but are not in our roboto fonts...
	// “User interface symbols” subblock of the “Miscellaneous Technical”
	//  ◼ 0x25FC
	//  ⯈ 0x2BC8 Black Medium Right-Pointing Triangle Centred
	// ▶ 25B6 Play Button, play.
	// ⏩ 23E9 Fast-Forward Button, fast forward.
	// ⏪ 23EA Fast Reverse Button, fast rewind, rewind.
	// ⏭ 23ED Next Track Button.
	// ⏮ 23EE Last Track Button.
	// ⏯ 23EF Play Or Pause Button.
	// ⏸ 23F8 Pause Button, pause.
	// ⏹ 23F9 Stop Button, stop.


	// Font v navrhu ma mensi mezery mezi pismeny - bez toho nevychazi na spravnou sirku
	ImFontConfig ubuntuBoldCfg;
	ubuntuBoldCfg.OversampleH = 2;
	ubuntuBoldCfg.OversampleV = 1;
	ubuntuBoldCfg.GlyphExtraSpacing.x = -0.5f;

	// NOTE: Oversampling is a technique that scales the loaded font by a specific factor in the X (horizontal) or Y
	// (vertical) direction (or both). In the process the font is also slightly blurred. Such modified font texture is
	// meant to be rendered at the original smaller size and the blurred edges of letters allow for subpixel
	// antialiasing. More info here: https://github.com/nothings/stb/blob/master/tests/oversample/README.md
	// This can be abused to make scaled up text look less pixelated when zoomed in. Although it also makes the zoomed
	// in text a little blurry.

	// Inspired by:
	// https://github.com/thedmd/imgui-node-editor/blob/af7fa51bb9d68c9b44477c341f13a7dadee3e359/examples/application/source/application.cpp#L97

	// In the future, dynamically replacing the font for a non-blurry bigger one would be the solution (or outright
	// always using a big font and scaling it down)
	// Another solution could be switching to freetype/vector-based fonts which ImGui might include soon. However, this
	// is a good enough solution for now. Note that using oversampling with high factors dramatically increases the size
	// of the font atlas texture, hogging video memory, especially if many fonts with large glyph ranges are used.

	// Default config, using lower horizontal oversampling (3 --> 2) than imgui to save some memory
	// (The difference is, as imgui docs mention, almost non-existent)
	ImFontConfig lqConfig;
	lqConfig.OversampleH = 2;
	lqConfig.OversampleV = 1;

	// More video memory intensive font config that uses high horizontal AND vertical oversampling.
	// This is ideal for fonts we zoom in on like in the workspace, albeit they become slightly blurry.
	ImFontConfig hqConfig;
	hqConfig.OversampleH = 4;
	hqConfig.OversampleV = 4;
	hqConfig.PixelSnapH = false;

	// Medium quality config
	ImFontConfig mqConfig;
	mqConfig.OversampleH = 3;
	mqConfig.OversampleV = 2;
	mqConfig.PixelSnapH = false;

	// NOTE: When possible, use a large font and scale it down for some occasional smaller text.
	//  Fonts can be scaled locally by wrapping an ImGui call with ImGui::SetWindowFontScale(x.xf) and back to (1.0f).
	//  Do not load an entire new font just because you need to draw text that is 1 or 2pts smaller than some other
	//  text. Generally we want to avoid scaling smaller text to be larger, but it CAN be done if the text is
	//  oversampled. Each new font potentially expands the final rasterized font atlas size, larger fonts take up more
	//  space, oversampled fonts take up even more space, icons take up a LOT of space, multiple language glyph ranges
	//  also take up more space. So ideally we want to use just a few higher quality fonts (with icons and extra
	//  glyphs).

	// As of right now the font atlas size is 2048x2048

	// Optimization possibilities:
	//  - Specific language glyph ranges can be dynamically loaded during language switch (avoids loading too many)
	//  - Taking this even further only the glyphs that are actually used can be loaded with ImFontGlyphRangesBuilder.
	//  - Icon fonts can be reduced to only contain icons that we actually use, the ttf file can be modifed
	//  - Tutorial and Welcome window still require their own fonts, realistically we could probably just use 4 variants

	// As of 21.03.2025 there is an ImGui WIP PR for dynamic fonts (https://github.com/ocornut/imgui/issues/8465)
	// This enables to load font once and let it automatically reload at requested scales
	// I assume this is a general effort to ease handling of DPI in ImGui which is ongoing and still in BETA

	std::unordered_map<std::string, ImFont*> fonts = {
	    {"Roboto14", loadFont("Data/Fonts/Roboto-Regular.ttf", 14.0f, fontScale, &hqConfig, ranges, true, true)}, //
	    {"Roboto16", loadFont("Data/Fonts/Roboto-Regular.ttf", 16.0f, fontScale, &lqConfig, ranges, false)},      //
	    {"RobotoBold12", loadFont("Data/Fonts/Roboto-Bold.ttf", 12.0f, fontScale, &lqConfig, ranges, false)},     //
	    {"RobotoBold16", loadFont("Data/Fonts/Roboto-Bold.ttf", 16.0f, fontScale, &mqConfig, ranges, false)},     //
	    {"RobotoMono14", loadFont("Data/Fonts/RobotoMono-Regular.ttf", 14.0f, fontScale, &lqConfig, ranges, false)},
	    {"RobotoItalic16", loadFont("Data/Fonts/Roboto-Italic.ttf", 16.0f, fontScale, &lqConfig, ranges, false)},    //
	    {"UbuntuBold18", loadFont("Data/Fonts/Ubuntu-Bold.ttf", 18.0f, fontScale, &lqConfig, ranges, false)},        //
	    {"UbuntuBold24", loadFont("Data/Fonts/Ubuntu-Bold.ttf", 24.0f, fontScale, &lqConfig, ranges, false)},        //
	    {"UbuntuBold33.5", loadFont("Data/Fonts/Ubuntu-Bold.ttf", 33.5f, fontScale, &ubuntuBoldCfg, ranges, false)}, //
	};
	m_fonts = std::move(fonts);
	io.Fonts->Build();
	io.FontDefault = getFont(m_defaultFont);

	ImGui_ImplOpenGL3_CreateFontsTexture();

	LOG_INFO("Generated fonts for UI scale of {}.", dpiScale);
}

ImFont* FontManager::loadFont(const char* filename, float size_pixels, float fontScale,
                              const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges, bool mergeIcons,
                              bool mergeNodeEditorIcons)
{
	ImFont* font =
	    ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size_pixels * fontScale, font_cfg_template, glyph_ranges);
	if (mergeNodeEditorIcons) // Node editor icons take precedence (must be merged first)
		loadNodeEditorIcons(size_pixels, fontScale);
	if (mergeIcons)
		loadFontAwesomeIcons(size_pixels, fontScale);
	return font;
}

void FontManager::loadFontAwesomeIcons(float size_pixels, float fontScale)
{
	ImGuiIO& io = ImGui::GetIO();

	// Merge in icon fonts
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	float iconFontSize = size_pixels * fontScale;

	icons_config.GlyphMinAdvanceX = (size_pixels + 1) * fontScale;
	//	icons_config.GlyphMaxAdvanceX = iconFontSize;
	icons_config.GlyphOffset = ImVec2(0, 0);

	io.Fonts->AddFontFromFileTTF("Data/Icons/" FONT_ICON_FILE_NAME_FA_I3T, iconFontSize, &icons_config,
	                             getFontRanges_faIcons());
	;
}

/// Special oversampled set of icons for use in the node editor. These icons are less blurry when scaled.
void FontManager::loadNodeEditorIcons(float size_pixels, float fontScale)
{
	ImGuiIO& io = ImGui::GetIO();

	// Merge in node editor icon fonts, using higher oversampling
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.OversampleH = 4;
	icons_config.OversampleV = 4;
	icons_config.PixelSnapH = false;

	float iconFontSize = size_pixels * fontScale;

	icons_config.GlyphMinAdvanceX = 0; // Don't align to allow alignment using narrow spaces
	icons_config.GlyphOffset = ImVec2(0, 0);
	// icons_config.RasterizerMultiply = 1.1f;

	io.Fonts->AddFontFromFileTTF("Data/Icons/" FONT_ICON_FILE_NAME_FA_I3T, iconFontSize, &icons_config,
	                             getFontRanges_nodeEditorIcons());
}

const ImWchar* FontManager::getFontRanges_faIcons()
{
	static const ImWchar icons_ranges_fa[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
	return &icons_ranges_fa[0];
}

const ImWchar* FontManager::getFontRanges_nodeEditorIcons()
{
	// Custom range to include as little glyphs as possible
	static ImVector<ImWchar> node_editor_icons_ranges;
	static bool loaded = false;
	if (!loaded)
	{
		node_editor_icons_ranges.clear();

		ImFontGlyphRangesBuilder builder;
		builder.AddText(
		    ICON_FA_CHEVRON_DOWN ICON_FA_CHEVRON_RIGHT ICON_FA_ANGLE_DOWN ICON_FA_ANGLE_RIGHT ICON_FA_CARET_DOWN
		        ICON_FA_CARET_RIGHT ICON_FA_PEN_TO_SQUARE ICON_FA_I3T_DOTS_3_1 ICON_FA_PEN ICON_FA_I3T_DOTS_3_2
		            ICON_FA_I3T_DOTS_3_3 ICON_FA_I3T_NSPACE_1 ICON_FA_I3T_NSPACE_2 ICON_FA_I3T_NSPACE_3
		                ICON_FA_I3T_NSPACE_4 ICON_I3T_TRACKING ICON_I3T_REF_SPACE ICON_FA_I3T_OGL ICON_FA_I3T_VULKAN);
		builder.BuildRanges(&node_editor_icons_ranges);
		loaded = true;
	}
	return node_editor_icons_ranges.Data;
}

void FontManager::setDefaultFont(std::string name)
{
	m_defaultFont = name;
}
