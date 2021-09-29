#include "Theme.h"

//#include "imgui_node_editor.h"

#include "Config.h"
#include "Core/API.h"

static Theme::CategoryNames							g_CategoryNames;
static std::map<EColor, const char*>		g_ColorNames;
static std::map<ESize, const char*>			g_SizeNames;
static std::map<ESizeVec2, const char*> g_SizeVecNames;

Theme Theme::createDefaultModern()
{
	Theme theme;

	theme.initModernProperties();
	theme.initFonts();

	theme.m_name = "modern";

	return theme;
}

Theme Theme::createDefaultClassic()
{
	Theme theme;

	theme.initClassicProperties();
	theme.initFonts();

	theme.m_name = "classic";

	return theme;
}

Theme::Theme(std::string name, const Theme::Colors& colors, const Theme::Sizes& sizes, const Theme::SizesVec& sizesVec)
{
	initClassicProperties();

	m_name = std::move(name);
	copyProperties(m_colors, colors);
	copyProperties(m_sizes, sizes);
	copyProperties(m_sizesVec2, sizesVec);
}

void Theme::initFonts()
{
	auto& io = ImGui::GetIO();

	m_fontsAssoc.insert(std::pair(EFont::MenuLarge, 0));
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, 0));
	m_fontsAssoc.insert(std::pair(EFont::Button, 1));
	m_fontsAssoc.insert(std::pair(EFont::Tab, 1));
	m_fontsAssoc.insert(std::pair(EFont::Node, 2));
	m_fontsAssoc.insert(std::pair(EFont::MenuSmall, 2));
	m_fontsAssoc.insert(std::pair(EFont::TutorialTitle, 3));

	io.FontDefault = get(EFont::MenuLarge);
}

void Theme::initClassicProperties()
{
	set(EColor::Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::WindowBackground, ImVec4(0.439f, 0.439f, 0.455f, 1.00f));
	set(EColor::PopupBackground, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	set(EColor::MenuBarBackground, ImVec4(0.40f, 0.38f, 0.369f, 1.00f));
	set(EColor::Border, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::PrimaryColor, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::ActiveColor, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
	set(EColor::FloatBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FloatBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.4f));
	set(EColor::FloatBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));

	set(EColor::TutorialBgColor, createColor(232, 232, 232, 255));

	// Node Editor
	set(EColor::Nodes_FloatText, ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.00f));
	set(EColor::NodeEditorBg, ImVec4(112.0f / 255.0f, 112.0f / 255.0f, 116.0f / 255.0f, 1.00f));
	set(EColor::Nodes_ConnectionPossible, createColor(0, 255, 0, 255));
	set(EColor::Nodes_ConnectionNotPossible, createColor(255, 0, 0, 255));
	set(EColor::Nodes_CreateNode, createColor(32, 45, 32, 180));

	set(EColor::Builder_NodePadding, ImVec4(0, 0, 0, 0));

	//pin colors
	set(EColor::PulsePin, createColor(164, 58, 190, 255));
	set(EColor::FloatPin, createColor(58, 144, 66, 255));
	set(EColor::MatrixPin, createColor(178, 71, 66, 255));
	set(EColor::QuatPin, createColor(178, 144, 66, 255));
	set(EColor::Vec3Pin, createColor(58, 84, 187, 255));
	set(EColor::Vec4Pin, createColor(106, 96, 67, 255));
	set(EColor::ScreenPin, createColor(51, 150, 215, 255));
	set(EColor::MatrixMulPin, createColor(68, 201, 156, 255));

	//pin inner colors
	set(EColor::InnerPulsePin, createColor(255, 255, 255, 255));
	set(EColor::InnerFloatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixPin, createColor(255, 255, 255, 255));
	set(EColor::InnerQuatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec3Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec4Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerScreenPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixMulPin, createColor(255, 255, 255, 255));


	// Folta operator color set
	set(EColor::NodeBgOperator, ImVec4(83.0f / 255.0f, 101.0f / 255.0f, 146.0f / 255.0f, 1.00f));
	set(EColor::NodeHeaderOperator, ImVec4(73.0f / 255.0f, 91.0f / 255.0f, 136.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperator, ImVec4(77.0f / 255.0f, 85.0f / 255.0f, 106.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperatorActive, ImVec4(97.0f / 255.0f, 105.0f / 255.0f, 126.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperatorHovered, ImVec4(87.0f / 255.0f, 95.0f / 255.0f, 116.0f / 255.0f, 1.00f));

	// Folta transformation color set
	set(EColor::NodeBgTransformation, ImVec4(137.0f / 255.0f, 115.0f / 255.0f, 59.0f / 255.0f, 1.00f));
	set(EColor::NodeHeaderTranformation, ImVec4(127.0f / 255.0f, 105.0f / 255.0f, 49.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformation, ImVec4(91.0f / 255.0f, 84.0f / 255.0f, 67.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformationActive, ImVec4(111.0f / 255.0f, 104.0f / 255.0f, 87.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformationHovered, createColor(101, 94, 77, 255));


	m_fontsAssoc.insert(std::pair(EFont::MenuLarge, 0));
	m_fontsAssoc.insert(std::pair(EFont::Button, 1));
	m_fontsAssoc.insert(std::pair(EFont::Tab, 1));
	m_fontsAssoc.insert(std::pair(EFont::Node, 2));
	m_fontsAssoc.insert(std::pair(EFont::MenuSmall, 2));
	m_fontsAssoc.insert(std::pair(EFont::Header, 5));

	// Tutorial fonts.
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, 9));
	m_fontsAssoc.insert(std::pair(EFont::TutorialTitle, 3));
	m_fontsAssoc.insert(std::pair(EFont::TutorialAssignment, 4));
	m_fontsAssoc.insert(std::pair(EFont::TutorialHint, 10));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeTitle, 7));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeDescription, 8));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeItemTitle, 6));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeItemDescription, 2));


	m_sizes[ESize::Nodes_FloatMargin]						= 1.0f;
	m_sizes[ESize::Nodes_FloatWidth]						= 25.0f;
	m_sizes[ESize::Nodes_Rounding]							= 0.0f;
	m_sizes[ESize::Nodes_BorderWidth]						= 0.0f;
	m_sizes[ESize::Nodes_LabelIndent]						= 3.0f;
	m_sizes[ESize::Nodes_HeaderLabelIndent]			= 2.0f;
	m_sizes[ESize::Nodes_LinkThickness]					= 3.0f;
	m_sizes[ESize::Nodes_trackballButtonHeight] = 20.0f;
	m_sizes[ESize::Nodes_TrackBallSensitivity]	= 5.0f;

	m_sizes[ESize::Nodes_FloatInnerPadding] = 1.0f;

	m_sizes[ESize::Nodes_dragSpeedDefaulrRatio] = 0.015f;
	m_sizes[ESize::Nodes_CtrlMultiplicator]			= 0.1f;
	m_sizes[ESize::Nodes_SHIFTMultiplicator]		= 10.0f;
	m_sizes[ESize::Nodes_ALTMultiplicator]			= 0.01f;

	m_sizes[ESize::Nodes_InputsAlignment]	 = 0.0f;
	m_sizes[ESize::Nodes_MiddleAlignment]	 = 0.0f;
	m_sizes[ESize::Nodes_OutputsAlignment] = 0.0f;

	m_sizes[ESize::Nodes_leftSideSpacing]	 = 3.0f;
	m_sizes[ESize::Nodes_rightSideSpacing] = 3.0f;

	m_sizesVec2[ESizeVec2::Window_FramePadding] = ImVec2(4.0f, 4.0f);

	m_sizesVec2[ESizeVec2::Nodes_ItemsSpacing]	 = ImVec2(2.0f, 3.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatPadding]	 = ImVec2(0.0f, 1.0f);
	m_sizesVec2[ESizeVec2::Nodes_PinSpacing]		 = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_PivotAlignment] = ImVec2(0.0f, 0.5f);
	m_sizesVec2[ESizeVec2::Nodes_PivotSize]			 = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_InputsSize] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_MiddleSize] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_OutputSize] = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_IconSize]						 = ImVec2(12.0f, 12.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatCycleButtonSize] = ImVec2(32.0f, 32.0f);
	m_sizesVec2[ESizeVec2::Nodes_ScreenTextureSize]		 = ImVec2(130.0f, 130.0f);

	m_sizesVec2[ESizeVec2::Builder_ItemSpacing] = ImVec2(0.0f, 0.0f);
}

void Theme::initModernProperties()
{
	set(EColor::Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	set(EColor::WindowBackground, ImVec4(0.439f, 0.439f, 0.455f, 1.00f));
	set(EColor::PopupBackground, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	set(EColor::MenuBarBackground, ImVec4(0.40f, 0.38f, 0.369f, 1.00f));
	set(EColor::Border, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::PrimaryColor, ImVec4(0.278f, 0.278f, 0.286f, 1.00f));
	set(EColor::ActiveColor, ImVec4(0.259f, 0.588f, 0.980f, 1.00f));
	set(EColor::FloatBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
	set(EColor::FloatBgHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.4f));
	set(EColor::FloatBgActive, ImVec4(0.26f, 0.59f, 0.98f, 0.67f));

	set(EColor::TutorialBgColor, createColor(232, 232, 232, 255));

	// Node Editor
	set(EColor::Nodes_FloatText, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));
	set(EColor::NodeEditorBg, ImVec4(0.298f, 0.298f, 0.298f, 1.00f));
	set(EColor::Nodes_ConnectionPossible, createColor(0, 255, 0, 255));
	set(EColor::Nodes_ConnectionNotPossible, createColor(255, 0, 0, 255));
	set(EColor::Nodes_CreateNode, createColor(32, 45, 32, 180));

	set(EColor::Builder_NodePadding, ImVec4(0, 0, 0, 0));

	// Pin colors
	set(EColor::PulsePin, createColor(164, 58, 190, 255));
	set(EColor::FloatPin, createColor(58, 144, 66, 255));
	set(EColor::MatrixPin, createColor(178, 71, 66, 255));
	set(EColor::QuatPin, createColor(178, 144, 66, 255));
	set(EColor::Vec3Pin, createColor(58, 84, 187, 255));
	set(EColor::Vec4Pin, createColor(106, 96, 67, 255));
	set(EColor::ScreenPin, createColor(51, 150, 215, 255));
	set(EColor::MatrixMulPin, createColor(68, 201, 156, 255));

	// Pin inner colors
	set(EColor::InnerPulsePin, createColor(255, 255, 255, 255));
	set(EColor::InnerFloatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixPin, createColor(255, 255, 255, 255));
	set(EColor::InnerQuatPin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec3Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerVec4Pin, createColor(255, 255, 255, 255));
	set(EColor::InnerScreenPin, createColor(255, 255, 255, 255));
	set(EColor::InnerMatrixMulPin, createColor(255, 255, 255, 255));


	// Sequence
	set(EColor::NodeBgSequence, ImVec4(0.541f, 0.541f, 0.541f, 1.0f));
	set(EColor::NodeHeaderSequence, ImVec4(0.431f, 0.431f, 0.431f, 1.0f));

	// Operator
	set(EColor::NodeBgOperator, ImVec4(0.816f, 0.816f, 0.816f, 1.00f));
	set(EColor::NodeHeaderOperator, ImVec4(73.0f / 255.0f, 91.0f / 255.0f, 136.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperator, createColor(255, 255, 255, 255));
	set(EColor::FloatBgOperatorActive, ImVec4(97.0f / 255.0f, 105.0f / 255.0f, 126.0f / 255.0f, 1.00f));
	set(EColor::FloatBgOperatorHovered, ImVec4(87.0f / 255.0f, 95.0f / 255.0f, 116.0f / 255.0f, 1.00f));

	// Transform
	set(EColor::NodeBgTransformation, ImVec4(0.816f, 0.816f, 0.816f, 1.00f));
	set(EColor::NodeHeaderTranformation, ImVec4(0.698f, 0.498f, 0.0f, 1.00f));
	set(EColor::FloatBgTransformation, createColor(255, 255, 255, 255));
	set(EColor::FloatBgTransformationActive, ImVec4(111.0f / 255.0f, 104.0f / 255.0f, 87.0f / 255.0f, 1.00f));
	set(EColor::FloatBgTransformationHovered, createColor(101, 94, 77, 255));


	m_fontsAssoc.insert(std::pair(EFont::MenuLarge, 0));
	m_fontsAssoc.insert(std::pair(EFont::Button, 1));
	m_fontsAssoc.insert(std::pair(EFont::Tab, 1));
	m_fontsAssoc.insert(std::pair(EFont::Node, 2));
	m_fontsAssoc.insert(std::pair(EFont::MenuSmall, 2));
	m_fontsAssoc.insert(std::pair(EFont::Header, 5));

	// Tutorial fonts.
	m_fontsAssoc.insert(std::pair(EFont::TutorialText, 9));
	m_fontsAssoc.insert(std::pair(EFont::TutorialTitle, 3));
	m_fontsAssoc.insert(std::pair(EFont::TutorialAssignment, 4));
	m_fontsAssoc.insert(std::pair(EFont::TutorialHint, 10));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeTitle, 7));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeDescription, 8));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeItemTitle, 6));
	m_fontsAssoc.insert(std::pair(EFont::WelcomeItemDescription, 2));


	m_sizes[ESize::Nodes_FloatMargin]						= 1.0f;
	m_sizes[ESize::Nodes_FloatWidth]						= 25.0f;
	m_sizes[ESize::Nodes_Rounding]							= 0.0f;
	m_sizes[ESize::Nodes_BorderWidth]						= 0.0f;
	m_sizes[ESize::Nodes_LabelIndent]						= 3.0f;
	m_sizes[ESize::Nodes_HeaderLabelIndent]			= 2.0f;
	m_sizes[ESize::Nodes_LinkThickness]					= 3.0f;
	m_sizes[ESize::Nodes_trackballButtonHeight] = 20.0f;
	m_sizes[ESize::Nodes_TrackBallSensitivity]	= 5.0f;

	m_sizes[ESize::Nodes_FloatInnerPadding] = 1.0f;

	m_sizes[ESize::Nodes_dragSpeedDefaulrRatio] = 0.015f;
	m_sizes[ESize::Nodes_CtrlMultiplicator]			= 0.1f;
	m_sizes[ESize::Nodes_SHIFTMultiplicator]		= 10.0f;
	m_sizes[ESize::Nodes_ALTMultiplicator]			= 0.01f;

	m_sizes[ESize::Nodes_InputsAlignment]	 = 0.0f;
	m_sizes[ESize::Nodes_MiddleAlignment]	 = 0.0f;
	m_sizes[ESize::Nodes_OutputsAlignment] = 0.0f;

	m_sizes[ESize::Nodes_leftSideSpacing]	 = 3.0f;
	m_sizes[ESize::Nodes_rightSideSpacing] = 3.0f;

	m_sizesVec2[ESizeVec2::Window_FramePadding] = ImVec2(4.0f, 4.0f);

	m_sizesVec2[ESizeVec2::Nodes_ItemsSpacing]	 = ImVec2(2.0f, 3.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatPadding]	 = ImVec2(0.0f, 1.0f);
	m_sizesVec2[ESizeVec2::Nodes_PinSpacing]		 = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_PivotAlignment] = ImVec2(0.0f, 0.5f);
	m_sizesVec2[ESizeVec2::Nodes_PivotSize]			 = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_InputsSize] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_MiddleSize] = ImVec2(0.0f, 0.0f);
	m_sizesVec2[ESizeVec2::Nodes_OutputSize] = ImVec2(0.0f, 0.0f);

	m_sizesVec2[ESizeVec2::Nodes_IconSize]						 = ImVec2(12.0f, 12.0f);
	m_sizesVec2[ESizeVec2::Nodes_FloatCycleButtonSize] = ImVec2(32.0f, 32.0f);
	m_sizesVec2[ESizeVec2::Nodes_ScreenTextureSize]		 = ImVec2(130.0f, 130.0f);

	m_sizesVec2[ESizeVec2::Builder_ItemSpacing] = ImVec2(0.0f, 0.0f);
}

void Theme::initNames()
{
	// All category keys must be I3T_PROPERTY_NAME_OFFSET characters long.
	g_CategoryNames["glob_"] = "Global";
	g_CategoryNames["ngen_"] = "Node Editor General";
	g_CategoryNames["npin_"] = "Node Editor Pins";
	g_CategoryNames["nops_"] = "Node Editor Operators";
	g_CategoryNames["ntrs_"] = "Node Editor Transformations";
	g_CategoryNames["npop_"] = "Node Editor Popups";


	// Global colors.
	g_ColorNames[EColor::Text]							= "glob_Text";
	g_ColorNames[EColor::Border]						= "glob_Border";
	g_ColorNames[EColor::WindowBackground]	= "glob_Window Background";
	g_ColorNames[EColor::PopupBackground]		= "glob_Popup Background";
	g_ColorNames[EColor::MenuBarBackground] = "glob_Menu Bar Background";
	g_ColorNames[EColor::PrimaryColor]			= "glob_Primary Color (tabs, tiles, ...)";
	g_ColorNames[EColor::ActiveColor]				= "glob_Active Color";
	g_ColorNames[EColor::TabColor]					= "glob_Tab Color";

	// Node editor colors.
	// 1. General
	g_ColorNames[EColor::NodeEditorBg]		= "ngen_Node Editor Background";
	g_ColorNames[EColor::NodeHeader]			= "ngen_Node Editor Header";
	g_ColorNames[EColor::Nodes_FloatText] = "ngen_Text in cells";

	// 2. Pins
	g_ColorNames[EColor::PulsePin]	= "npin_Pulse Pin";
	g_ColorNames[EColor::FloatPin]	= "npin_Float Pin";
	g_ColorNames[EColor::MatrixPin] = "npin_Matrix Pin";
	g_ColorNames[EColor::QuatPin]		= "npin_Quaternion Pin";
	g_ColorNames[EColor::Vec3Pin]		= "npin_Vector 3 Pin";
	g_ColorNames[EColor::Vec4Pin]		= "npin_Vector 4 Pin";
	g_ColorNames[EColor::ScreenPin] = "npin_Screen Pin";

	g_ColorNames[EColor::InnerPulsePin]			= "npin_Inner Color Pulse";
	g_ColorNames[EColor::InnerFloatPin]			= "npin_Inner Color Float";
	g_ColorNames[EColor::InnerVec3Pin]			= "npin_Inner Color Vec3";
	g_ColorNames[EColor::InnerVec4Pin]			= "npin_Inner Color Vec4";
	g_ColorNames[EColor::InnerMatrixPin]		= "npin_Inner Color Matrix";
	g_ColorNames[EColor::InnerQuatPin]			= "npin_Inner Color Quat";
	g_ColorNames[EColor::InnerMatrixMulPin] = "npin_Inner Color MatrixMul";
	g_ColorNames[EColor::InnerScreenPin]		= "npin_Inner Color Screen";

	// 3. Operator
	g_ColorNames[EColor::NodeBgOperator]				 = "nops_Operator Background";
	g_ColorNames[EColor::NodeHeaderOperator]		 = "nops_Operator Header";
	g_ColorNames[EColor::FloatBgOperator]				 = "nops_Operator Float Background";
	g_ColorNames[EColor::FloatBgOperatorActive]	 = "nops_Operator Active Float Background";
	g_ColorNames[EColor::FloatBgOperatorHovered] = "nops_Operator Hovered Float Background";

	// 4. Transforms
	g_ColorNames[EColor::NodeBgTransformation]				 = "ntrs_Transform Background";
	g_ColorNames[EColor::NodeHeaderTranformation]			 = "ntrs_Transform Header";
	g_ColorNames[EColor::FloatBgTransformation]				 = "ntrs_Transform Float Background";
	g_ColorNames[EColor::FloatBgTransformationActive]	 = "ntrs_Transform Float Active";
	g_ColorNames[EColor::FloatBgTransformationHovered] = "ntrs_Transform Float Hovered";

	// 5. Popups
	g_ColorNames[EColor::Nodes_ConnectionPossible]		= "npop_Connection is possible";
	g_ColorNames[EColor::Nodes_ConnectionNotPossible] = "npop_Connection is not possible";
	g_ColorNames[EColor::Nodes_CreateNode]						= "npop_Create node popup";

	/// \todo ???
	// g_ColorNames[EColor::Builder_NodePadding] = "node_Builder Padding (Not a color!)";


	// Global spacing and other size properties.
	g_SizeNames[ESize::Nodes_Rounding]		= "ngen_Nodes Rounding";
	g_SizeNames[ESize::Nodes_FloatWidth]	= "ngen_Nodes Float Width";
	g_SizeNames[ESize::Nodes_FloatMargin] = "ngen_Nodes Float Margin";
	g_SizeNames[ESize::Nodes_BorderWidth] = "ngen_Nodes BorderWidth";

	g_SizeNames[ESize::Nodes_LabelIndent]						= "ngen_Pin labels Indent";
	g_SizeNames[ESize::Nodes_HeaderLabelIndent]			= "ngen_Header Label Indent";
	g_SizeNames[ESize::Nodes_LinkThickness]					= "ngen_Link Thickness";
	g_SizeNames[ESize::Nodes_trackballButtonHeight] = "ngen_Trackball button height";
	g_SizeNames[ESize::Nodes_TrackBallSensitivity]	= "ngen_Trackball Sensitivity";

	g_SizeNames[ESize::Nodes_FloatInnerPadding] = "ngen_Float inner padding";

	g_SizeNames[ESize::Nodes_dragSpeedDefaulrRatio] = "ngen_Drag speed fo float";
	g_SizeNames[ESize::Nodes_CtrlMultiplicator]			= "ngen_CTRL Multiplicator";
	g_SizeNames[ESize::Nodes_SHIFTMultiplicator]		= "ngen_SHIFT Multiplicator";
	g_SizeNames[ESize::Nodes_ALTMultiplicator]			= "ngen_ALT Multiplicator";

	g_SizeNames[ESize::Nodes_InputsAlignment]	 = "ngen_Nodes Inputs Alignment";
	g_SizeNames[ESize::Nodes_MiddleAlignment]	 = "ngen_Nodes Middle Alignment";
	g_SizeNames[ESize::Nodes_OutputsAlignment] = "ngen_Nodes Outputs Alignment";

	g_SizeNames[ESize::Nodes_leftSideSpacing]	 = "ngen_Nodes left side spacing";
	g_SizeNames[ESize::Nodes_rightSideSpacing] = "ngen_Nodes right side spacing";


	g_SizeVecNames[ESizeVec2::Window_FramePadding] = "glob_Windows Frame Padding";

	g_SizeVecNames[ESizeVec2::Nodes_IconSize]				= "ngen_Nodes Icon Size";
	g_SizeVecNames[ESizeVec2::Nodes_PivotAlignment] = "ngen_Nodes Pivot Alignment";
	g_SizeVecNames[ESizeVec2::Nodes_PinSpacing]			= "ngen_Nodes Pin Spacing";
	g_SizeVecNames[ESizeVec2::Nodes_ItemsSpacing]		= "ngen_Nodes Items Spacing";
	g_SizeVecNames[ESizeVec2::Nodes_FloatPadding]		= "ngen_Nodes Float Padding";

	g_SizeVecNames[ESizeVec2::Nodes_PivotSize] = "ngen_Nodes Pivot size";

	g_SizeVecNames[ESizeVec2::Nodes_InputsSize] = "ngen_Nodes Inputs Size";
	g_SizeVecNames[ESizeVec2::Nodes_MiddleSize] = "ngen_Nodes Middle Size";
	g_SizeVecNames[ESizeVec2::Nodes_OutputSize] = "ngen_Nodes Output Size";

	g_SizeVecNames[ESizeVec2::Nodes_FloatCycleButtonSize] = "ngen_Nodes Float Cycle button size";
	g_SizeVecNames[ESizeVec2::Nodes_ScreenTextureSize]		= "ngen_Nodes Screen Texture Size";

	g_SizeVecNames[ESizeVec2::Builder_ItemSpacing] = "ngen_Builder Item Spacing";
}

void Theme::apply()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = m_colors[EColor::Text];

	style.Colors[ImGuiCol_WindowBg]					= m_colors[EColor::WindowBackground];
	style.Colors[ImGuiCol_Border]						= m_colors[EColor::Border];
	style.Colors[ImGuiCol_PopupBg]					= m_colors[EColor::PopupBackground];
	style.Colors[ImGuiCol_TitleBg]					= m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TitleBgActive]		= m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TitleBgCollapsed] = m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_MenuBarBg]				= m_colors[EColor::MenuBarBackground];

	style.Colors[ImGuiCol_Tab]								= m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TabActive]					= m_colors[EColor::ActiveColor];
	style.Colors[ImGuiCol_TabUnfocused]				= m_colors[EColor::PrimaryColor];
	style.Colors[ImGuiCol_TabUnfocusedActive] = m_colors[EColor::PrimaryColor];

	style.Colors[ImGuiCol_Separator] = m_colors[EColor::PrimaryColor];

	style.Colors[ImGuiCol_PlotHistogram] = m_colors[EColor::ActiveColor];

	style.FramePadding.x = m_sizesVec2[ESizeVec2::Window_FramePadding].x;
	style.FramePadding.y = m_sizesVec2[ESizeVec2::Window_FramePadding].y;
	style.TabRounding		 = 2.0f;

	// Show borders.
	style.ChildBorderSize	 = 0.0f;
	style.PopupBorderSize	 = 1.0f;
	style.WindowBorderSize = 1.0f;

	// SS, MH please check this
	//---------------------------------------------------------------------------------------
	/* \todo JH repaire
	auto& nodesStyle = ax::NodeEditor::GetStyle();

	nodesStyle.NodeRounding																					 = m_sizes[ESize::Nodes_Rounding];
	nodesStyle.NodeBorderWidth																			 = m_sizes[ESize::Nodes_BorderWidth];
	nodesStyle.Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg] = m_colors[EColor::NodeBgOperator];

	nodesStyle.Colors[ax::NodeEditor::StyleColor::StyleColor_Bg]		 = m_colors[EColor::NodeEditorBg];
	nodesStyle.Colors[ax::NodeEditor::StyleColor::StyleColor_Grid] 		 = m_colors[EColor::NodeEditorBg];
    */
	//---------------------------------------------------------------------------------------
}

ImFont* Theme::get(EFont font) { return I3T::getUI()->getFonts()[m_fontsAssoc[font]]; }

const char* Theme::getCategoryName(const std::string& key)
{
	auto name = Theme::getCategoryNames()[key];
	assert(name && "Unknown category, did you create entry in g_CategoryNames?");

	return name;
}

Theme::CategoryNames& Theme::getCategoryNames() { return g_CategoryNames; }

std::map<EColor, const char*>& Theme::getColorNames() { return g_ColorNames; }

std::map<ESize, const char*>& Theme::getSizeNames() { return g_SizeNames; }

std::map<ESizeVec2, const char*>& Theme::getSizeVecNames() { return g_SizeVecNames; }

void Theme::operatorColorTheme()
{
    /* \todo JH repaire
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg] = m_colors[EColor::NodeBgOperator];

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_FrameBg]				= m_colors[EColor::FloatBgOperator];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FloatBgOperatorHovered];
	style.Colors[ImGuiCol_FrameBgActive]	= m_colors[EColor::FloatBgOperatorActive];
	*/
}

void Theme::transformationColorTheme()
{
    /* \todo JH repaire
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg] =
			m_colors[EColor::NodeBgTransformation];

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_FrameBg]				= m_colors[EColor::FloatBgTransformation];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FloatBgTransformationHovered];
	style.Colors[ImGuiCol_FrameBgActive]	= m_colors[EColor::FloatBgTransformationActive];
	*/
}

bool isVectorsEqual(ImVec4 a, ImVec4 b)
{
	/// \todo Comparing float values!
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w ? true : false;
}

ImVec4 Theme::getHeader()
{
    /* \todo JH repaire
	if (isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
										 m_colors[EColor::NodeBgOperator]))
	{
		return m_colors[EColor::NodeHeaderOperator];
	}
	if (isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
										 m_colors[EColor::NodeBgTransformation]))
	{
		return m_colors[EColor::NodeHeaderTranformation];
	}
	*/
	return m_colors[EColor::NodeHeaderTranformation];
	//return ImVec4(0, 0, 0, 0);
}

ImVec4 Theme::getBg()
{
    /* \todo JH repaire
	if (isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
										 m_colors[EColor::NodeBgOperator]))
	{
		return m_colors[EColor::NodeHeaderOperator];
	}
	if (isVectorsEqual(ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor::StyleColor_NodeBg],
										 m_colors[EColor::NodeBgTransformation]))
	{
		return m_colors[EColor::NodeHeaderTranformation];
	}
	*/
	return m_colors[EColor::NodeBgTransformation];
	//return ImVec4(0, 0, 0, 0);
}

void Theme::returnFloatColorToDefault()
{
	ImGuiStyle& style											= ImGui::GetStyle();
	style.Colors[ImGuiCol_FrameBg]				= m_colors[EColor::FloatBg];
	style.Colors[ImGuiCol_FrameBgHovered] = m_colors[EColor::FloatBgHovered];
	style.Colors[ImGuiCol_FrameBgActive]	= m_colors[EColor::FloatBgHovered];
}
