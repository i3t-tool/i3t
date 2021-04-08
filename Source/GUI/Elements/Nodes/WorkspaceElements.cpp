
#include "WorkspaceElements.h"
#include "spdlog/fmt/fmt.h"
#include <string>

// #include <format> // not as standard library yet

std::map<EValueType, ImColor> WorkspacePinColor = {
		{EValueType::Float, ImColor(255, 255, 255)},    {EValueType::Matrix, ImColor(220, 48, 48)},
		{EValueType::MatrixMul, ImColor(68, 201, 156)}, {EValueType::Pulse, ImColor(147, 226, 74)},
		{EValueType::Quat, ImColor(124, 21, 153)},      {EValueType::Screen, ImColor(51, 150, 215)},
		{EValueType::Vec3, ImColor(218, 0, 183)},       {EValueType::Vec4, ImColor(255, 48, 48)}};

std::map<EValueType, IconType> WorkspacePinShape = {
		{EValueType::Float, IconType::Circle},     {EValueType::Matrix, IconType::Flow},
		{EValueType::MatrixMul, IconType::Circle}, {EValueType::Pulse, IconType::Circle},
		{EValueType::Quat, IconType::Circle},      {EValueType::Screen, IconType::Circle},
		{EValueType::Vec3, IconType::Circle},      {EValueType::Vec4, IconType::Square}};

std::map<WorkspaceLevelOfDetail, std::string> WorkspaceLevelOfDetailName = {
    {WorkspaceLevelOfDetail::Full, "Full"},
    {WorkspaceLevelOfDetail::SetValues, "Set values"},
    {WorkspaceLevelOfDetail::Label, "Label"}
};

/* \todo JH not use constant values here */
WorkspaceNode::WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, WorkspaceNodeArgs const& args)
    :   m_id(id), m_headerBackground(headerBackground), m_headerLabel(args.headerLabel), m_label(args.nodeLabel), m_levelOfDetail(args.levelOfDetail)
{
	/* \todo Some better default values - take from Const.h*/
	m_state = "default WorkspaceNode state";
	m_color = ImColor(255, 255, 255);
	m_size = ImVec2(100, 100);
	m_touchTime = 1.0;
}

/* \todo JH not use constant values here */
WorkspaceNode::WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
    :   m_id(id), m_headerBackground(headerBackground), m_headerLabel(headerLabel), m_label(nodeLabel), m_levelOfDetail(WorkspaceLevelOfDetail::SetValues)
{
	/* \todo Some better default values - take from Const.h*/
	m_state = "default WorkspaceNode state";
	m_color = ImColor(255, 255, 255);
	m_size = ImVec2(100, 100);
	m_touchTime = 1.0;
}

void WorkspaceNode::drawMenuLevelOfDetail()
{
    if (ImGui::BeginMenu("Level of detail")) {
        ImGui::Text(fmt::format("Actual level: {}", WorkspaceLevelOfDetailName[m_levelOfDetail]).c_str());
        ImGui::Separator();

        for (auto const& [levelOfDetail, LoDname] : WorkspaceLevelOfDetailName)
        {
            if (ImGui::MenuItem(LoDname.c_str()))
            {
                m_levelOfDetail = levelOfDetail;
            }
        }
        ImGui::EndMenu();
    }
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

void WorkspaceNode::drawHeader(util::NodeBuilder& builder)
{
	builder.Header(m_color);

	ImGui::Spring(0);     // 0 - spring will always have zero size - left align the header
	ImGui::TextUnformatted(m_headerLabel.c_str());
	ImGui::Spring(1);     // 1 - power of the current spring = 1, use default spacing .x or .y
	ImGui::Dummy(ImVec2(0, 28));
	ImGui::Spring(0);

	builder.EndHeader();
}


WorkspaceLinkProperties::WorkspaceLinkProperties(const ne::LinkId id) : m_id(id), m_color(ImColor(255, 255, 255))
{}

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

