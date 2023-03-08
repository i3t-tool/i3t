#include "WorkspaceElements.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

/// \todo Remove these.

/* DIWNE - \todo JH to remove, but I need something what use instead -> from
 * Type get Shape and Color */
/* \todo MH is it possible to store std::map in Theme? */
std::map<EValueType, EColor> WorkspacePinColorBackground = {
    {EValueType::Float, EColor::FloatPin},         {EValueType::Matrix, EColor::MatrixPin},
    {EValueType::MatrixMul, EColor::MatrixMulPin}, {EValueType::Pulse, EColor::PulsePin},
    {EValueType::Quat, EColor::QuatPin},           {EValueType::Screen, EColor::ScreenPin},
    {EValueType::Vec3, EColor::Vec3Pin},           {EValueType::Vec4, EColor::Vec4Pin}};

std::map<EValueType, DIWNE::IconType> WorkspacePinShapeBackground = {
    {EValueType::Float, DIWNE::IconType::Rectangle},     {EValueType::Matrix, DIWNE::IconType::Rectangle},
    {EValueType::MatrixMul, DIWNE::IconType::Rectangle}, {EValueType::Pulse, DIWNE::IconType::Rectangle},
    {EValueType::Quat, DIWNE::IconType::Rectangle},      {EValueType::Screen, DIWNE::IconType::Rectangle},
    {EValueType::Vec3, DIWNE::IconType::Rectangle},      {EValueType::Vec4, DIWNE::IconType::Rectangle}};

std::map<EValueType, DIWNE::IconType> WorkspacePinShapeForeground = {
    {EValueType::Float, DIWNE::IconType::TriangleRight}, {EValueType::Matrix, DIWNE::IconType::TriangleRight},
    {EValueType::MatrixMul, DIWNE::IconType::Cross},     {EValueType::Pulse, DIWNE::IconType::TriangleRight},
    {EValueType::Quat, DIWNE::IconType::TriangleRight},  {EValueType::Screen, DIWNE::IconType::TriangleRight},
    {EValueType::Vec3, DIWNE::IconType::TriangleRight},  {EValueType::Vec4, DIWNE::IconType::TriangleRight}};

std::map<EValueType, EColor> WorkspacePinColorForeground = {
    {EValueType::Float, EColor::InnerFloatPin},         {EValueType::Matrix, EColor::InnerMatrixPin},
    {EValueType::MatrixMul, EColor::InnerMatrixMulPin}, {EValueType::Pulse, EColor::InnerPulsePin},
    {EValueType::Quat, EColor::InnerQuatPin},           {EValueType::Screen, EColor::InnerScreenPin},
    {EValueType::Vec3, EColor::InnerVec3Pin},           {EValueType::Vec4, EColor::InnerVec4Pin}};

std::map<WorkspaceLevelOfDetail, std::string> WorkspaceLevelOfDetailName = {
    {WorkspaceLevelOfDetail::Full, "Full"},
    {WorkspaceLevelOfDetail::SetValues, "Set values"},
    {WorkspaceLevelOfDetail::Label, "Label"},
    {WorkspaceLevelOfDetail::LightCycle, "Light cycle"}};

WorkspaceNode::WorkspaceNode(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const topLabel,
                             std::string const middleLabel)
    : DIWNE::Node(diwne, id), m_topLabel(topLabel), m_middleLabel(middleLabel), m_removeFromWorkspaceWindow(false)
{
}

WorkspaceNode::~WorkspaceNode() { diwne.m_takeSnap = true; }

bool WorkspaceNode::beforeContent()
{
	/* whole node background */
	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max, I3T::getTheme().get(EColor::NodeBg),
	                         I3T::getTheme().get(ESize::Nodes_Rounding), ImDrawCornerFlags_All);
	return false;
}

bool WorkspaceNode::topContent()
{
	bool interaction_happen = false;

	diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max, I3T::getTheme().get(EColor::NodeHeader),
	                         I3T::getTheme().get(ESize::Nodes_Rounding), ImDrawCornerFlags_Top);
	ImGui::Dummy(ImVec2(ImGui::GetStyle().ItemSpacing.x, 1));
	ImGui::SameLine();
	ImGui::TextUnformatted(m_topLabel.c_str());

	return interaction_happen;
}

bool WorkspaceNode::middleContent()
{
	bool interaction_happen = false;

	ImGui::TextUnformatted(m_middleLabel.c_str());

	return interaction_happen;
}

bool WorkspaceNode::leftContent() { return false; }
bool WorkspaceNode::rightContent() { return false; }
bool WorkspaceNode::bottomContent() { return false; }

bool WorkspaceNode::bypassFocusForInteractionAction()
{
	return (m_isHeld || m_topRectDiwne.Contains(diwne.screen2diwne(diwne.bypassGetMousePos())));
}

void WorkspaceNode::deleteAction() { m_removeFromWorkspaceWindow = true; }

void WorkspaceNode::drawMenuDelete()
{
	if (ImGui::MenuItem("Delete", "Delete"))
	{
		deleteActionDiwne();
	}
}

void WorkspaceNode::popupContent() { drawMenuDelete(); }

WorkspacePin::WorkspacePin(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const label)
    : DIWNE::Pin(diwne, id), m_label(label), m_showLabel(false)
{
}

/* >>>>> STATIC FUNCTIONS <<<<< */

int numberOfCharWithDecimalPoint(float value, int numberOfVisibleDecimal)
{
	int border = 10, result = 1, int_value;

	if (value < 0)
	{
		value = -value;
	}
	result++; /* always space for sign to avoid changing size of / alternatively
	             move it inside if above */

	int_value = (int)value;
	while (int_value >= border)
	{
		result++;
		border *= 10;
	}

	return result + (numberOfVisibleDecimal > 0 ? numberOfVisibleDecimal + 1 : 0); /* +1 for decimal point */
}
