#include <string>

#include "spdlog/fmt/fmt.h"

#include "WorkspaceElements.h"

// #include <format> // not as standard library yet

/// \todo Remove these.
std::map<EValueType, EColor> WorkspacePinColor = {
		{EValueType::Float, EColor::FloatPin},				 {EValueType::Matrix, EColor::MatrixPin},
		{EValueType::MatrixMul, EColor::MatrixMulPin}, {EValueType::Pulse, EColor::PulsePin},
		{EValueType::Quat, EColor::QuatPin},					 {EValueType::Screen, EColor::ScreenPin},
		{EValueType::Vec3, EColor::Vec3Pin},					 {EValueType::Vec4, EColor::Vec4Pin}};

std::map<EValueType, IconType> WorkspacePinShape = {
		{EValueType::Float, IconType::Arrow},			{EValueType::Matrix, IconType::Arrow},
		{EValueType::MatrixMul, IconType::Arrow}, {EValueType::Pulse, IconType::Arrow},
		{EValueType::Quat, IconType::Arrow},			{EValueType::Screen, IconType::Arrow},
		{EValueType::Vec3, IconType::Arrow},			{EValueType::Vec4, IconType::Arrow}};

std::map<EValueType, EColor> WorkspaceInnerPinColor = {
		{EValueType::Float, EColor::InnerFloatPin},					{EValueType::Matrix, EColor::InnerMatrixPin},
		{EValueType::MatrixMul, EColor::InnerMatrixMulPin}, {EValueType::Pulse, EColor::InnerPulsePin},
		{EValueType::Quat, EColor::InnerQuatPin},						{EValueType::Screen, EColor::InnerScreenPin},
		{EValueType::Vec3, EColor::InnerVec3Pin},						{EValueType::Vec4, EColor::InnerVec4Pin}};

std::map<WorkspaceLevelOfDetail, std::string> WorkspaceLevelOfDetailName = {
		{WorkspaceLevelOfDetail::Full, "Full"},
		{WorkspaceLevelOfDetail::SetValues, "Set values"},
		{WorkspaceLevelOfDetail::Label, "Label"}};
// endtodo

/* \todo JH not use constant values here */
WorkspaceNode::WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, WorkspaceNodeArgs const& args) :
		m_id(id), m_headerBackground(headerBackground), m_headerLabel(args.headerLabel), m_label(args.nodeLabel)
{
	/* \todo Some better default values - take from Const.h*/
	m_color			= I3T::getColor(EColor::NodeBgOperator);
	m_size			= ImVec2(1, 1);
	m_touchTime = 1.0;
}

/* \todo JH not use constant values here */
WorkspaceNode::WorkspaceNode(ne::NodeId const id, ImTextureID headerBackground, std::string headerLabel,
														 std::string nodeLabel) :
		m_id(id),
		m_headerBackground(headerBackground), m_headerLabel(headerLabel), m_label(nodeLabel)
{
	/* \todo Some better default values - take from Const.h*/

	m_color			= I3T::getColor(EColor::NodeBgOperator);
	m_size			= ImVec2(1, 1);
	m_touchTime = 1.0;
}

ne::NodeId const WorkspaceNode::getId() const { return m_id; }

std::string WorkspaceNode::getHeaderLabel() { return m_headerLabel; }

ImTextureID WorkspaceNode::getHeaderBackground() { return m_headerBackground; }

bool WorkspaceNode::dataAreValid() { return false; /* \todo this function will return true on default  */ }
void WorkspaceNode::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin, bool withPins)
{

	builder.Begin(m_id);

	drawHeader(builder);


	if (withPins) { drawInputs(builder, newLinkPin); }

	drawMiddle(builder);

	if (withPins) { drawOutputs(builder, newLinkPin); }


	builder.End();
}

void WorkspaceNode::drawHeader(util::NodeBuilder& builder)
{

	Theme& t = I3T::getTheme();
	m_color	 = t.getHeader();
	builder.Header(m_color);
	ImGui::Spring(
			0, I3T::getSize(ESize::Nodes_HeaderLabelIndent)); // 0 - spring will always have zero size - left align the header
	ImGui::TextUnformatted(m_headerLabel.c_str());
	ImGui::Spring(10);	 // 1 - power of the current spring = 1, use default spacing .x or .y
	if (!dataAreValid()) /* \todo JH function for check validity of data here */
	{
		/*ax::Widgets::Icon(ImVec2(20, 20), // \todo JH size based on header size
                            IconType::Square,
                            true,
                            ImColor(255,0,0),
                            ImColor(0,0,0));  //\todo JH not constant here...
																						 */
	}
	ImGui::Spring(0);

	builder.EndHeader();
}

/* \todo JH time-functions are taken from example */
void WorkspaceNode::TouchNode(float const constTouchTime) { m_touchTime = constTouchTime; }

void WorkspaceNode::UpdateTouch(float const constDeltaTime)
{
	if (m_touchTime > 0) { m_touchTime -= constDeltaTime; }
}

float WorkspaceNode::GetTouchProgress(float const constTouchTime)
{
	if (m_touchTime > 0.0f) return (constTouchTime - m_touchTime) / constTouchTime;
	else
		return 0.0f;
}


WorkspaceLinkProperties::WorkspaceLinkProperties(const ne::LinkId id) :
		m_id(id), m_color(I3T::getColor(EColor::MatrixPin)), m_thickness(I3T::getSize(ESize::Nodes_LinkThickness))
{}

ne::LinkId const WorkspaceLinkProperties::getId() const { return m_id; }
ImColor const		 WorkspaceLinkProperties::getColor() const { return m_color; }
float const			 WorkspaceLinkProperties::getThickness() const { return m_thickness; }

WorkspacePinProperties::WorkspacePinProperties(ne::PinId const id, std::string label) :
		m_id(id), m_label(label), m_showLabel(true), m_iconSize(I3T::getSize(ESizeVec2::Nodes_IconSize)),
		m_color(I3T::getColor(EColor::MatrixPin))
{}


ne::PinId const WorkspacePinProperties::getId() const { return m_id; }
ImVec2 const		WorkspacePinProperties::getIconSize() const { return m_iconSize; }
ImColor const		WorkspacePinProperties::getColor() const { return m_color; }

bool							WorkspacePinProperties::getShowLabel() const { return m_showLabel; }
std::string const WorkspacePinProperties::getLabel() const { return m_label; }




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
