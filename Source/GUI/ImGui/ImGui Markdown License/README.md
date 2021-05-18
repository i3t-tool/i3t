This will become an altered/extended version of imgui_markdown to meet the requirements of the i3t project.

# imgui_markdown_extended

## Markdown for Dear ImGui

A permissively licensed markdown single-header library for [Dear ImGui](https://github.com/ocornut/imgui).

Requires C++11 or above

imgui_markdown currently supports the following markdown functionality:

  * Wrapped text
  * Headers H1, H2, H3
  * Indented text, multi levels
  * Unordered lists and sub-lists
  * Links
  * Images

![imgui_markdown demo live editing](https://github.com/juliettef/Media/blob/master/imgui_markdown_demo_live_editing.gif)

*Note - the gif above is heavily compressed due to GitHub limitations. There's a [(slightly) better version of it on twitter](https://twitter.com/juulcat/status/1090996799266000898).*

## Syntax

### Wrapping
Text wraps automatically. To add a new line, use 'Return'.
### Headers
```
# H1
## H2
### H3
```
### Lists
#### Indents
On a new line, at the start of the line, add two spaces per indent.
```
Normal text
··Indent level 1
····Indent level 2
······Indent level 3
Normal text
```
#### Unordered lists
On a new line, at the start of the line, add two spaces, an asterisks and a space. For nested lists, add two additional spaces in front of the asterisk per list level increment.
```
Normal text
··*·Unordered List level 1
····*·Unordered List level 2
······*·Unordered List level 3
······*·Unordered List level 3
··*·Unordered List level 1
Normal text
```
### Links
```
[link description](https://...)
```
### Images
```
![image alt text](image identifier e.g. filename)
```


![Example use of imgui_markdown with icon fonts](https://github.com/juliettef/Media/blob/master/imgui_markdown_icon_font.jpg)

## Example use on Windows with links opening in a browser

```Cpp

#include "ImGui.h"                // https://github.com/ocornut/imgui
#include "imgui_markdown.h"       // https://github.com/juliettef/imgui_markdown
#include "IconsFontAwesome5.h"    // https://github.com/juliettef/IconFontCppHeaders

// Following includes for Windows LinkCallback
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Shellapi.h"
#include <string>

void LinkCallback( ImGui::MarkdownLinkCallbackData data_ );
inline ImGui::MarkdownImageData ImageCallback( ImGui::MarkdownLinkCallbackData data_ );

static ImFont* H1 = NULL;
static ImFont* H2 = NULL;
static ImFont* H3 = NULL;

static ImGui::MarkdownConfig mdConfig; 


void LinkCallback( ImGui::MarkdownLinkCallbackData data_ )
{
    std::string url( data_.link, data_.linkLength );
    if( !data_.isImage )
    {
        ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
    }
}

inline ImGui::MarkdownImageData ImageCallback( ImGui::MarkdownLinkCallbackData data_ )
{
    // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
    ImTextureID image = ImGui::GetIO().Fonts->TexID;
    // > C++14 can use ImGui::MarkdownImageData imageData{ true, false, image, ImVec2( 40.0f, 20.0f ) };
    ImGui::MarkdownImageData imageData;
    imageData.isValid =         true;
    imageData.useLinkCallback = false;
    imageData.user_texture_id = image;
    imageData.size =            ImVec2( 40.0f, 20.0f );
    return imageData;
}

void LoadFonts( float fontSize_ = 12.0f )
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    // Base font
    io.Fonts->AddFontFromFileTTF( "myfont.ttf", fontSize_ );
    // Bold headings H2 and H3
    H2 = io.Fonts->AddFontFromFileTTF( "myfont-bold.ttf", fontSize_ );
    H3 = mdConfig.headingFormats[ 1 ].font;
    // bold heading H1
    float fontSizeH1 = fontSize_ * 1.1f;
    H1 = io.Fonts->AddFontFromFileTTF( "myfont-bold.ttf", fontSizeH1 );
}

void ExampleMarkdownFormatCallback( const ImGui::MarkdownFormatInfo& markdownFormatInfo_, bool start_ )
{
    // Call the default first so any settings can be overwritten by our implementation.
    // Alternatively could be called or not called in a switch statement on a case by case basis.
    // See defaultMarkdownFormatCallback definition for furhter examples of how to use it.
    ImGui::defaultMarkdownFormatCallback( markdownFormatInfo_, start_ );        
       
    switch( markdownFormatInfo_.type )
    {
    // example: change the colour of heading level 2
    case ImGui::MarkdownFormatType::HEADING:
    {
        if( markdownFormatInfo_.level == 2 )
        {
            if( start_ )
            {
                ImGui::PushStyleColor( ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] );
            }
            else
            {
                ImGui::PopStyleColor();
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

void Markdown( const std::string& markdown_ )
{
    // You can make your own Markdown function with your prefered string container and markdown config.
    // > C++14 can use ImGui::MarkdownConfig mdConfig{ LinkCallback, NULL, ImageCallback, ICON_FA_LINK, { { H1, true }, { H2, true }, { H3, false } }, NULL };
    mdConfig.linkCallback =         LinkCallback;
    mdConfig.tooltipCallback =      NULL;
    mdConfig.imageCallback =        ImageCallback;
    mdConfig.linkIcon =             ICON_FA_LINK;
    mdConfig.headingFormats[0] =    { H1, true };
    mdConfig.headingFormats[1] =    { H2, true };
    mdConfig.headingFormats[2] =    { H3, false };
    mdConfig.userData =             NULL;
    mdConfig.formatCallback =       ExampleMarkdownFormatCallback;
    ImGui::Markdown( markdown_.c_str(), markdown_.length(), mdConfig );
}

void MarkdownExample()
{
    const std::string markdownText = u8R"(
# H1 Header: Text and Links
You can add [links like this one to enkisoftware](https://www.enkisoftware.com/) and lines will wrap well.
## H2 Header: indented text.
  This text has an indent (two leading spaces).
    This one has two.
### H3 Header: Lists
  * Unordered lists
    * Lists can be indented with two extra spaces.
  * Lists can have [links like this one to Avoyd](https://www.avoyd.com/)
)";
    Markdown( markdownText );
}
```

## Projects using imgui_markdown

### [Avoyd](https://www.enkisoftware.com/avoyd)
Avoyd is an abstract 6 degrees of freedom voxel game.  
[www.avoyd.com](https://www.avoyd.com)  

The game and the voxel editor's help and tutorials use imgui_markdown with Dear ImGui.  

![Avoyd screenshot](https://github.com/juliettef/Media/blob/master/imgui_markdown_Avoyd_about_OSS.png?raw=true)

### [bgfx](https://github.com/bkaradzic/bgfx)
Cross-platform rendering library.  
[bkaradzic.github.io/bgfx/overview](https://bkaradzic.github.io/bgfx/overview.html)

### [Imogen](https://github.com/CedricGuillemet/Imogen)
GPU/CPU Texture Generator  
[skaven.fr/imogen](http://skaven.fr/imogen/)

![Imogen screenshot](https://camo.githubusercontent.com/28347bc0c1627aa4f289e1b2b769afcb3a5de370/68747470733a2f2f692e696d6775722e636f6d2f7351664f3542722e706e67)

### [Light Tracer](https://lighttracer.org/)
Experimental GPU ray tracer for web

![Light Tracer screenshot](https://github.com/juliettef/Media/blob/master/imgui_markdown_Light_Tracer.png?raw=true)

### [Visual 6502 Remix](https://github.com/floooh/v6502r)
Transistor level 6502 Hardware Simulation  
[hfloooh.github.io/visual6502remix](https://floooh.github.io/visual6502remix/)  

Using imgui_markdown as help viewer for Visual 6502 Remix with internal and external links: 
 
[![Using imgui_markdown as help viewer for Visual 6502 Remix with internal and external links - animated gif](https://user-images.githubusercontent.com/1699414/69185510-320baa00-0b17-11ea-9fd5-82ed6e02a05c.gif)
![Using imgui_markdown as help viewer for Visual 6502 Remix - screenshot](https://user-images.githubusercontent.com/1699414/69185626-67b09300-0b17-11ea-85a8-fed54a0082b4.png)](https://github.com/ocornut/imgui/issues/2847#issuecomment-555710973)  

![Using imgui_markdown in the About page for Visual 6502 Remix - screenshot](https://github.com/juliettef/Media/blob/master/imgui_markdown_Visual_6502_Remix_About.png?raw=true)

## Credits

Design and implementation - [Doug Binks](http://www.enkisoftware.com/about.html#doug) - [@dougbinks](https://github.com/dougbinks)  
Implementation and maintenance - [Juliette Foucaut](http://www.enkisoftware.com/about.html#juliette) - [@juliettef](https://github.com/juliettef)  
Thanks to [Omar Cornut for Dear ImGui](https://github.com/ocornut/imgui).

## License (zlib)

Copyright (c) 2019 Juliette Foucaut and Doug Binks

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
