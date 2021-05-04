
#include "WorkspaceElements.h"
#include "spdlog/fmt/fmt.h"
#include <string>



// #include <format> // not as standard library yet

std::map<EValueType, ImColor> WorkspacePinColor = {
		{EValueType::Float, ImColor(58, 144, 66)},    {EValueType::Matrix, ImColor(178, 71, 66)},
		{EValueType::MatrixMul, ImColor(68, 201, 156)}, {EValueType::Pulse, ImColor(147, 226, 74)},
		{EValueType::Quat, ImColor(178, 144, 66)},      {EValueType::Screen, ImColor(51, 150, 215)},
		{EValueType::Vec3, ImColor(58, 84, 187)},       {EValueType::Vec4, ImColor(106, 96, 67)}};

std::map<EValueType, IconType> WorkspacePinShape = {
		{EValueType::Float, IconType::Arrow},     {EValueType::Matrix, IconType::Arrow},
		{EValueType::MatrixMul, IconType::Circle}, {EValueType::Pulse, IconType::Circle},
		{EValueType::Quat, IconType::Arrow},      {EValueType::Screen, IconType::Circle},
		{EValueType::Vec3, IconType::Arrow},      {EValueType::Vec4, IconType::Arrow}};

std::map<EValueType, ImColor> WorkspaceInnerPinColor = {
		{EValueType::Float, ImColor(164, 191, 168)},    {EValueType::Matrix, ImColor(201, 169, 168)},
		{EValueType::MatrixMul, ImColor(68, 201, 156)}, {EValueType::Pulse, ImColor(147, 226, 74)},
		{EValueType::Quat, ImColor(201, 191, 168)},      {EValueType::Screen, ImColor(51, 150, 215)},
		{EValueType::Vec3, ImColor(164, 172, 205)},       {EValueType::Vec4, ImColor(179, 176, 168)} };

std::map<WorkspaceLevelOfDetail, std::string> WorkspaceLevelOfDetailName = {
    {WorkspaceLevelOfDetail::Full, "Full"},
    {WorkspaceLevelOfDetail::SetValues, "Set values"},
    {WorkspaceLevelOfDetail::Label, "Label"}
};

/* \todo JH not use constant values here */
WorkspaceNode::WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, WorkspaceNodeArgs const& args)
    :   m_id(id), m_headerBackground(headerBackground), m_headerLabel(args.headerLabel), m_label(args.nodeLabel)
{
	/* \todo Some better default values - take from Const.h*/
	m_color = ImColor(255, 255, 255);
	m_size = ImVec2(100, 100);
	m_touchTime = 1.0;
}

/* \todo JH not use constant values here */
WorkspaceNode::WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    :   m_id(id), m_headerBackground(headerBackground), m_headerLabel(headerLabel), m_label(nodeLabel)
{
	/* \todo Some better default values - take from Const.h*/
	m_color = ImColor(89, 134, 179);
	m_size = ImVec2(100, 100);
	m_touchTime = 1.0;
}

ne::NodeId const WorkspaceNode::getId() const
{
    return m_id;
}

std::string WorkspaceNode::getHeaderLabel()
{
    return m_headerLabel;
}

ImTextureID WorkspaceNode::getHeaderBackground()
{
    return m_headerBackground;
}

void WorkspaceNode::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin, bool withPins)
{

	builder.Begin(m_id);

	drawHeader(builder);

    if (withPins)
    {
        drawInputs(builder, newLinkPin);
    }


	//drawData(builder);

    //if (withPins)
    //{
        drawOutputs(builder, newLinkPin);
    //}

	builder.End();
}

void WorkspaceNode::drawHeader(util::NodeBuilder& builder)
{

    builder.Header(m_color);
	ImGui::Spring(0);     // 0 - spring will always have zero size - left align the header
	ImGui::TextUnformatted(m_headerLabel.c_str());
	ImGui::Spring(1);     // 1 - power of the current spring = 1, use default spacing .x or .y
	//ImGui::Dummy(ImVec2(0, 28));
	//ImGui::Spring(0);

	builder.EndHeader();
}

/* \todo JH time-functions are taken from example */
void WorkspaceNode::TouchNode(float const constTouchTime)
{
	m_touchTime = constTouchTime;
}

void WorkspaceNode::UpdateTouch(float const constDeltaTime)
{
	if (m_touchTime > 0)
	{
		m_touchTime -= constDeltaTime;
	}
}

float WorkspaceNode::GetTouchProgress(float const constTouchTime)
{
	if (m_touchTime > 0.0f)
		return (constTouchTime - m_touchTime) / constTouchTime;
	else
		return 0.0f;
}


WorkspaceLinkProperties::WorkspaceLinkProperties(const ne::LinkId id)
    : m_id(id)
    , m_color(ImColor(100.0, 200.0, 10.0, 1.0f)) /* \todo JH not constant here... */ //SS what is this?
    , m_thickness(3) /* \todo JH not constant here... */
{}

ne::LinkId const WorkspaceLinkProperties::getId() const	{return m_id; }
ImColor const WorkspaceLinkProperties::getColor() const {return m_color; }
float const WorkspaceLinkProperties::getThickness() const {return m_thickness; }

WorkspacePinProperties::WorkspacePinProperties(ne::PinId const id, std::string label)
		: m_id(id), m_label(label), m_showLabel(false), m_iconSize(24), m_color(ImColor(100.0, 200.0, 10.0, 1.0f)) /* \todo JH no constants here... */
{}


ne::PinId const WorkspacePinProperties::getId() const {return m_id;}
int const WorkspacePinProperties::getIconSize() const {return m_iconSize;}
ImColor const WorkspacePinProperties::getColor() const {return m_color;}

bool WorkspacePinProperties::getShowLabel() const {return m_showLabel;}
std::string const WorkspacePinProperties::getLabel() const {return m_label;}


/* >>>>> STATIC FUNCTIONS <<<<< */

int numberOfCharWithDecimalPoint(float value, int numberOfVisibleDecimal)
{
    int border = 10, result = 1, int_value;

    if (value < 0)
    {
        result++; /* sign */
        value = -value;
    }

    int_value = (int)value;
    while (int_value >= border)
    {
        result++;
        border *=10;
    }

    return result + (numberOfVisibleDecimal > 0 ? numberOfVisibleDecimal+1 : 0); /* +1 for decimal point */

}

