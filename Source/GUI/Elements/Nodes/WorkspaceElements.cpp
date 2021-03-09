
#include "WorkspaceElements.h"
#include <string>
#include "spdlog/fmt/fmt.h"

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

/* \todo JH not use constant values here */
WorkspaceNode::WorkspaceNode(const ne::NodeId id, ImTextureID headerBackground, std::string headerLabel, WorkspaceViewScale viewScale)
    :   m_id(id), m_viewScale(viewScale)
{
	/* \todo Some better default values - take from Const.h*/
	m_state = "default WorkspaceNode state";
	m_label = "default WorkspaceNode Label";
	m_color = ImColor(255, 255, 255);
	m_size = ImVec2(100, 100);
	m_touchTime = 1.0;

	m_headerLabel = headerLabel;
	m_headerBackground = headerBackground;
}

/* \todo JH time-functions are taken from example */
void WorkspaceNode::TouchNode(const float constTouchTime)
{
	m_touchTime = constTouchTime;
}

void WorkspaceNode::UpdateTouch(const float constDeltaTime)
{
	if (m_touchTime > 0)
	{
		m_touchTime -= constDeltaTime;
	}
}

float WorkspaceNode::GetTouchProgress(const float constTouchTime)
{
	if (m_touchTime > 0.0f)
		return (constTouchTime - m_touchTime) / constTouchTime;
	else
		return 0.0f;
}

void WorkspaceNode::drawHeader(util::NodeBuilder& builder)
{
	builder.Header(m_color);

	ImGui::Spring(0); /* \todo JH what 0/1 means */
	ImGui::TextUnformatted(m_headerLabel.c_str());
	ImGui::Spring(1);
	ImGui::Dummy(ImVec2(0, 28));
	ImGui::Spring(0);

	builder.EndHeader();
}


WorkspaceLinkProperties::WorkspaceLinkProperties(const ne::LinkId id) : m_id(id), m_color(ImColor(255, 255, 255))
{}


WorkspacePinProperties::WorkspacePinProperties(const ne::PinId id, const char* name, PinKind kind, EValueType type)
		: m_id(id), m_name(name), m_kind(kind), m_type(type), m_iconSize(24), m_connected(false), m_alpha(100) /* \todo JH no konstants here... */
{
}

/* \todo JH this functions are in Core... */
bool WorkspacePinProperties::IsPinConnected()
{
	return m_connected;
}

bool WorkspacePinProperties::CanCreateLink(Core::Pin* b)
{
	return true; /* \todo todo... */
}
