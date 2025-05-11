#pragma once

// Custom I3T icons, inserted into fa-6-solid FontAwesome6 ttf file. Starting at codepoint U+ee00.
// Icon literals are written in UTF-8 encoding, hence sometimes you can't just increment the hex number, as the first
// few bits might need to remain static to idenfity the UTF-8 byte.
// You can use compart.com to search a particular unicode hex value and get its UTF-8 encoding (or an online converter)

#define ICON_MIN_FA_I3T 0xee00
#define ICON_MAX_FA_I3T 0xee06

#define FONT_ICON_FILE_NAME_FA_I3T "fa-6-solid-900-i3t.ttf"

#define ICON_FA_I3T_GIZMO "\xee\xb8\x80"      // U+ee00
#define ICON_FA_I3T_FRUSTUM "\xee\xb8\x81"    // U+ee01
#define ICON_FA_I3T_TWO_DOTS_1 "\xee\xb8\x82" // U+ee02
#define ICON_FA_I3T_TWO_DOTS_2 "\xee\xb8\x83" // U+ee03
#define ICON_FA_I3T_DOTS_3_1 "\xee\xb8\x84"   // U+ee04
#define ICON_FA_I3T_DOTS_3_2 "\xee\xb8\x85"   // U+ee05
#define ICON_FA_I3T_DOTS_3_3 "\xee\xb8\x86"   // U+ee06

// Narrow spaces, thin spaces used for alignment, Unicode has these but we can't really rely on that with all fonts
// Nspace 1 is a "quarter" of a space, Nspace 2 a half etc.
#define ICON_FA_I3T_NSPACE_1 "\xee\xb8\x87" // U+ee07
#define ICON_FA_I3T_NSPACE_2 "\xee\xb8\x88" // U+ee08
#define ICON_FA_I3T_NSPACE_3 "\xee\xb8\x89" // U+ee09
#define ICON_FA_I3T_NSPACE_4 "\xee\xb8\x8A" // U+ee0A

#define ICON_FA_I3T_MAT_DECOMPOSE "\xee\xb9\x8C" // U+ee4c
#define ICON_FA_I3T_OGL "\xee\xb9\xb2"           // U+ee72
#define ICON_FA_I3T_VULKAN "\xee\xb9\xb3"        // U+ee73
#define ICON_FA_I3T_COORD_SYSTEM "\xee\xb8\x8b"  // U+ee0b