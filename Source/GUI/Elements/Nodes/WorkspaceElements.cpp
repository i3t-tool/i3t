
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
WorkspaceNode::WorkspaceNode(const ne::NodeId id, ImTextureID headerBackground, std::string headerLabel = "default WorkspaceNode HeaderLabel")
    :   Id(id)
{
	/* \todo Some better default values - take from Const.h*/
	State = "default WorkspaceNode state";
	Label = "default WorkspaceNode Label";
	Color = ImColor(255, 255, 255);
	Size = ImVec2(100, 100);
	TouchTime = 1.0;

	HeaderLabel = headerLabel;
	HeaderBackground = headerBackground;
}

/* \todo JH time-functions are taken from example */
void WorkspaceNode::TouchNode(const float constTouchTime)
{
	TouchTime = constTouchTime;
}

void WorkspaceNode::UpdateTouch(const float constDeltaTime)
{
	if (TouchTime > 0)
	{
		TouchTime -= constDeltaTime;
	}
}

float WorkspaceNode::GetTouchProgress(const float constTouchTime)
{
	if (TouchTime > 0.0f)
		return (constTouchTime - TouchTime) / constTouchTime;
	else
		return 0.0f;
}

void WorkspaceNode::drawHeader(util::NodeBuilder& builder)
{
	builder.Header(Color);

	ImGui::Spring(0); /* \todo JH what 0/1 means */
	ImGui::TextUnformatted(HeaderLabel.c_str());
	ImGui::Spring(1);
	ImGui::Dummy(ImVec2(0, 28));
	ImGui::Spring(0);

	builder.EndHeader();
}


WorkspaceLinkProperties::WorkspaceLinkProperties(const ne::LinkId id) : Id(id), Color(ImColor(255, 255, 255))
{}
//
///* see:
// * https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function/8114913*/
//WorkspaceNodeBaseData::WorkspaceNodeBaseData(Ptr<Core::NodeBase> nodebase) : Nodebase(nodebase)
//{
//	std::ostringstream pinName;
//	std::string pinNameStr;
//
//	const std::vector<Core::Pin>& InputPins = Nodebase->getInputPins();
//	const std::vector<Core::Pin>& OutputPins = Nodebase->getOutputPins();
//
//	WorkspaceLinksProperties.reserve(InputPins.size());
//
//	WorkspaceInputsProperties.reserve(InputPins.size());
//	WorkspaceOutputsProperties.reserve(OutputPins.size());
//
//	for (const Core::Pin& pin : InputPins)
//	{
//		pinName << "input #" << pin.getIndex();
//		pinNameStr = pinName.str();
//		WorkspaceInputsProperties.push_back(std::make_unique<WorkspacePinProperties>(
//				pin.getId(),
//				pinNameStr.c_str(), //,std::format("input #{}", pin.getIndex()) // format is not part of standard library
//				PinKind::Input,
//				pin.getType()));
//
//		WorkspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
//                pin.getId()));
//		pinName.str("");
//	}
//
//	for (const Core::Pin& pin : OutputPins)
//	{
//		pinName << "output #" << pin.getIndex();
//		pinNameStr = pinName.str();
//		WorkspaceOutputsProperties.push_back(std::make_unique<WorkspacePinProperties>(
//				pin.getId(),
//                pinNameStr.c_str(), //,std::format("output #{}", pin.getIndex())
//				PinKind::Output,
//				pin.getType()));
//		pinName.str("");
//	}
//
//}

WorkspacePinProperties::WorkspacePinProperties(const ne::PinId id, const char* name, PinKind kind, EValueType type)
		: Id(id), Name(name), Kind(kind), Type(type), IconSize(24), Connected(false), Alpha(100) /* \todo JH no konstants here... */
{
}

/* \todo JH this functions are in Core... */
bool WorkspacePinProperties::IsPinConnected()
{
	return Connected;
}

bool WorkspacePinProperties::CanCreateLink(Core::Pin* b)
{
	return true; /* \todo todo... */
}
