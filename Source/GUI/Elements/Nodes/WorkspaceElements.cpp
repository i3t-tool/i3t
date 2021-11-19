

#include "WorkspaceElements.h"


/// \todo Remove these.

/* DIWNE - \todo JH to remove, but I need something what use instead -> from Type get Shape and Color */
std::map<EValueType, EColor> WorkspacePinColorBackground = {
		{EValueType::Float,     EColor::FloatPin},      {EValueType::Matrix,EColor::MatrixPin},
		{EValueType::MatrixMul, EColor::MatrixMulPin},  {EValueType::Pulse, EColor::PulsePin},
		{EValueType::Quat,      EColor::QuatPin},       {EValueType::Screen,EColor::ScreenPin},
		{EValueType::Vec3,      EColor::Vec3Pin},       {EValueType::Vec4,  EColor::Vec4Pin}};

std::map<EValueType, DIWNE::IconType> WorkspacePinShapeBackground = {
		{EValueType::Float,     DIWNE::IconType::Rectangle},    {EValueType::Matrix,DIWNE::IconType::Rectangle},
		{EValueType::MatrixMul, DIWNE::IconType::Rectangle},    {EValueType::Pulse, DIWNE::IconType::Rectangle},
		{EValueType::Quat,      DIWNE::IconType::Rectangle},    {EValueType::Screen,DIWNE::IconType::Rectangle},
		{EValueType::Vec3,      DIWNE::IconType::Rectangle},	{EValueType::Vec4,  DIWNE::IconType::Rectangle}};

std::map<EValueType, DIWNE::IconType> WorkspacePinShapeForeground = {
		{EValueType::Float,     DIWNE::IconType::TriangleRight},  {EValueType::Matrix, DIWNE::IconType::TriangleRight},
		{EValueType::MatrixMul, DIWNE::IconType::Cross},          {EValueType::Pulse,  DIWNE::IconType::TriangleRight},
		{EValueType::Quat,      DIWNE::IconType::TriangleRight},  {EValueType::Screen, DIWNE::IconType::TriangleRight},
		{EValueType::Vec3,      DIWNE::IconType::TriangleRight},  {EValueType::Vec4,   DIWNE::IconType::TriangleRight}};

std::map<EValueType, EColor> WorkspacePinColorForeground = {
		{EValueType::Float,     EColor::InnerFloatPin},     {EValueType::Matrix,EColor::InnerMatrixPin},
		{EValueType::MatrixMul, EColor::InnerMatrixMulPin}, {EValueType::Pulse, EColor::InnerPulsePin},
		{EValueType::Quat,      EColor::InnerQuatPin},      {EValueType::Screen,EColor::InnerScreenPin},
		{EValueType::Vec3,      EColor::InnerVec3Pin},      {EValueType::Vec4,  EColor::InnerVec4Pin}};


std::map<WorkspaceLevelOfDetail, std::string> WorkspaceLevelOfDetailName = {
		{WorkspaceLevelOfDetail::Full, "Full"},
		{WorkspaceLevelOfDetail::SetValues, "Set values"},
		{WorkspaceLevelOfDetail::Label, "Label"}};

WorkspaceNode::WorkspaceNode(DIWNE::ID id, std::string const topLabel, std::string const middleLabel)
    :   DIWNE::Node(id)
    ,   m_topLabel(topLabel)
    ,   m_middleLabel(middleLabel)
    ,   m_removeFromWorkspaceWindow(false)
{}

bool WorkspaceNode::drawNodeBackground(DIWNE::Diwne &diwne)
{
    /* \todo JH background by settings in different type of nodes */
    /* whole node background */
    diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max,
                             ImGui::ColorConvertFloat4ToU32(ImVec4(0,50,255,255)), 5, ImDrawCornerFlags_Top); /* \todo JH 5 is rounding of corners -> take from Theme?*/
    return false;
}


bool WorkspaceNode::topContent(DIWNE::Diwne &diwne)
{
    bool interaction_happen = false;

    diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max,
                             ImGui::ColorConvertFloat4ToU32(I3T::getTheme().getHeader()), 5, ImDrawCornerFlags_Top); /* \todo JH 5 is rounding of corners -> take from Theme?*/

	// ImGui::Spring(0, I3T::getSize(ESize::Nodes_HeaderLabelIndent)); // 0 - spring will always have zero size - left align the header
	ImGui::TextUnformatted(m_topLabel.c_str());
	// ImGui::Spring(10);	 // 1 - power of the current spring = 1, use default spacing .x or .y

    return interaction_happen;
}

bool WorkspaceNode::middleContent(DIWNE::Diwne &diwne)
{
    bool interaction_happen = false;

    diwne.AddRectFilledDiwne(m_middleRectDiwne.Min, m_middleRectDiwne.Max,
                             ImGui::ColorConvertFloat4ToU32(I3T::getTheme().getBg()));

	ImGui::TextUnformatted(m_middleLabel.c_str());

    return interaction_happen;
}


void WorkspaceNode::drawMenuDelete()
{
    if (ImGui::MenuItem("Delete")) {
            m_removeFromWorkspaceWindow = true;
    }
}

void WorkspaceNode::nodePopupContent()
{
    drawMenuDelete();
}


WorkspacePin::WorkspacePin(DIWNE::ID id, std::string label)
    :   DIWNE::Pin(id)
    ,   m_label(label)
    ,   m_showLabel(false)
{}



/* >>>>> STATIC FUNCTIONS <<<<< */

int numberOfCharWithDecimalPoint(float value, int numberOfVisibleDecimal)
{
	int border = 10, result = 1, int_value;

	if (value < 0)
	{
		result++; /* sign */
		value = -value;
	}

	int_value = (int) value;
	while (int_value >= border)
	{
		result++;
		border *= 10;
	}

	return result + (numberOfVisibleDecimal > 0 ? numberOfVisibleDecimal + 1 : 0); /* +1 for decimal point */
}


