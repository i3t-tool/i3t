#pragma once

#include <string>
#include <unordered_map>

#include <imgui.h>

/**
 * DPI aware font manager. Loads and caches fonts for any requested DPI scale.
 */
class FontManager
{
	// TODO: (DR) Perhaps in the future implement per window DPI scaling
	// struct CachedFontAtlas
	// {
	// 	ImFontAtlas* atlas;
	// 	std::unordered_map<std::string, ImFont*> m_fonts;
	// };
	// /// Mapping between dpi scales and already built font atlases.
	// std::unordered_map<float, CachedFontAtlas> m_fontCache;

	std::unordered_map<std::string, ImFont*> m_fonts;
	std::string m_defaultFont = "Roboto14";

public:
	/**
	 * Get a particular font.
	 * Such a font must have been previously loaded by generateFonts().
	 * @param name String identifier of the font (hardcoded currently).
	 */
	ImFont* getFont(std::string name)
	{
		return m_fonts[name];
	}

	/// Load and generate font atlas for the specific dpi scaling factor
	void generateFonts(float dpiScale);

	/// Set which font acts as the ImGui default font.
	void setDefaultFont(std::string name);

private:
	static ImFont* loadFont(const char* filename, float size_pixels, float fontScale,
	                        const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges, bool mergeIcons,
	                        bool mergeNodeEditorIcons = false);


	// Icon rendering
	// ============================================================================================================
	// fa-6-solid-900-i3t.ttf contains some custom I3T icons, see IconsFontAwesome6_I3T.h
	// The I3T icons begin at Private Use Area code point 0xEE00

	/// Icons are loaded from a custom icon font file, rasterized and merged into the last loaded font
	static void loadFontAwesomeIcons(float size_pixels, float fontScale);
	static void loadNodeEditorIcons(float size_pixels, float fontScale);

	static const ImWchar* getFontRanges_faIcons();
	static const ImWchar* getFontRanges_nodeEditorIcons();
};
