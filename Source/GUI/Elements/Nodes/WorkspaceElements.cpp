
#include "WorkspaceElements.h"
#include <sstream>
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

void WorkspaceNode::drawWorkspaceNodeHeader(util::NodeBuilder& builder)
{
	builder.Header(Color);

	ImGui::Spring(0);
	ImGui::TextUnformatted(HeaderLabel.c_str());
	ImGui::Spring(1);
	ImGui::Dummy(ImVec2(0, 28));
	ImGui::Spring(0);

	builder.EndHeader();
}

WorkspaceNode::WorkspaceNode(const ne::NodeId id, ImTextureID headerBackground) : Id(id)
{
	/* \todo Some better default values - take from Const.h*/
	State = "selected";
	Label = "default WorkspaceNode Label";
	Color = ImColor(255, 255, 255);
	Size = ImVec2(100, 100);
	TouchTime = 1.0;

	HeaderLabel = "default WorkspaceNode HeaderLabel";
	HeaderBackground = headerBackground;
}

WorkspaceNode::WorkspaceNode(const ne::NodeId id, std::string headerLabel, ImTextureID headerBackground) : Id(id)
{
	/* \todo Some better default values - take from Const.h*/
	State = "selected";
	Label = "default WorkspaceNode Label";
	Color = ImColor(255, 255, 255);
	Size = ImVec2(100, 100);
	TouchTime = 1.0;

	HeaderLabel = headerLabel;
	HeaderBackground = headerBackground;
}

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

WorkspaceLinkProperties::WorkspaceLinkProperties(const ne::LinkId id) : Id(id), Color(ImColor(255, 255, 255))
{
}

WorkspacePinProperties::WorkspacePinProperties(const ne::PinId& id, const char* name, PinKind kind, EValueType type)
		: Id(id), Name(name), Kind(kind), Type(type), IconSize(24), Connected(false), Alpha(100)
{
}

bool WorkspacePinProperties::IsPinConnected()
{
	return Connected;
}

bool WorkspacePinProperties::CanCreateLink(Core::Pin* b)
{
	return true; /* \todo todo... */
}

/* see:
 * https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function/8114913*/
WorkspaceNodeBaseData::WorkspaceNodeBaseData(Ptr<Core::NodeBase> nodebase) : Nodebase(std::move(nodebase))
{
	std::ostringstream pinName;
	std::string pinNameStr;

	const std::vector<Core::Pin>& InputPins = Nodebase->getInputPins();
	const std::vector<Core::Pin>& OutputPins = Nodebase->getOutputPins();

	WorkspaceLinksProperties.reserve(InputPins.size());

	WorkspaceInputsProperties.reserve(InputPins.size());
	WorkspaceOutputsProperties.reserve(OutputPins.size());

	for (const Core::Pin& pin : InputPins)
	{
		pinName << "input #" << pin.getIndex();
		pinNameStr = pinName.str();
		WorkspaceInputsProperties.push_back(std::make_unique<WorkspacePinProperties>(
				pin.getId(),
				pinNameStr.c_str() //,std::format("input #{}", pin.getIndex()) // format is not part of standard library
				,
				PinKind::Input, pin.getType()));
    WorkspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(pin.getId()));
		pinName.str("");
	}

	for (const Core::Pin& pin : OutputPins)
	{
		pinName << "output #" << pin.getIndex();
		pinNameStr = pinName.str();
		WorkspaceOutputsProperties.push_back(std::make_unique<WorkspacePinProperties>(
				pin.getId(), pinNameStr.c_str() //,std::format("output #{}", pin.getIndex())
				,
				PinKind::Output, pin.getType()));
		pinName.str("");
	}
}

/* \todo test whether it work... */
void WorkspaceNodeBaseData::drawWorkspaceInputLinks()
{
	for (std::pair<pinIter, linkPropIter> elem(Nodebase->getInputPins().begin(), WorkspaceLinksProperties.begin());
	     elem.first != Nodebase->getInputPins().end() /* && elem.second != WorkspaceLinksProperties.end() */
	     ;
	     ++elem.first, ++elem.second)
	{
		if (elem.first->isPluggedIn())
			ne::Link(elem.second->get()->Id, elem.first->getParentPin()->getId(), elem.first->getId(),
			         elem.second->get()->Color, 2.0f); /* elem.second->get() for dereferencing unique_ptr*/
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeBaseData::drawWorkspaceInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	for (std::pair<pinIter, pinPropIter> elem(Nodebase->getInputPins().begin(), WorkspaceInputsProperties.begin());
	     /* elem.first() != Nodebase->getInputPins().end()  &&*/ elem.second != WorkspaceInputsProperties.end();
	     ++elem.first, ++elem.second)
	{
		float alpha = ImGui::GetStyle().Alpha;
		//        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
		//          alpha = alpha * (48.0f / 255.0f);

		builder.Input(elem.first->getId());
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		// color.Value.w = alpha / 255.0f;
		ax::Widgets::Icon(ImVec2(elem.second->get()->IconSize, elem.second->get()->IconSize),
		                  WorkspacePinShape[elem.second->get()->Type],
		                  elem.second->get()->Connected /* \todo do it better - it is copy from Core*/
		                  ,
		                  WorkspacePinColor[elem.second->get()->Type], ImColor(32.0, 32.0, 32.0, alpha));

		ImGui::Spring(0);
		if (!elem.second->get()->Name.empty())
		{
			ImGui::TextUnformatted(elem.second->get()->Name.c_str());
			ImGui::Spring(0);
		}

		ImGui::PopStyleVar();
		builder.EndInput();
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeBaseData::drawWorkspaceOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	for (std::pair<pinIter, pinPropIter> elem(Nodebase->getOutputPins().begin(), WorkspaceOutputsProperties.begin());
	     /* elem.first() != Nodebase->getOutputPins().end()  &&*/ elem.second != WorkspaceOutputsProperties.end();
	     ++elem.first, ++elem.second)
	{
		float alpha = ImGui::GetStyle().Alpha;
		//        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
		//          alpha = alpha * (48.0f / 255.0f);

		builder.Output(elem.first->getId());
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		if (!elem.second->get()->Name.empty())
		{
			ImGui::TextUnformatted(elem.second->get()->Name.c_str());
			ImGui::Spring(0);
		}

		// color.Value.w = alpha / 255.0f;
		ax::Widgets::Icon(ImVec2(elem.second->get()->IconSize, elem.second->get()->IconSize),
		                  WorkspacePinShape[elem.second->get()->Type],
		                  elem.second->get()->Connected /* \todo do it better - it is copy from Core*/
		                  ,
		                  WorkspacePinColor[elem.second->get()->Type], ImColor(32.0, 32.0, 32.0, alpha));

		ImGui::Spring(0);

		ImGui::PopStyleVar();
		builder.EndOutput();
	}
}
