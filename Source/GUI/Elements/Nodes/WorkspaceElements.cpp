
#include "WorkspaceElements.h"

std::map<EValueType, ImColor> WorkspacePinColor = {
    {EValueType::Float,     ImColor(255, 255, 255)},
    {EValueType::Matrix,    ImColor(220, 48, 48)},
    {EValueType::MatrixMul, ImColor(68, 201, 156)},
    {EValueType::Pulse,     ImColor(147, 226, 74)},
    {EValueType::Quat,      ImColor(124, 21, 153)},
    {EValueType::Screen,    ImColor(51, 150, 215)},
    {EValueType::Vec3,      ImColor(218, 0, 183)},
    {EValueType::Vec4,      ImColor(255, 48, 48)}
};

std::map<EValueType, IconType> WorkspacePinShape = {
    {EValueType::Float,     IconType::Circle},
    {EValueType::Matrix,    IconType::Flow},
    {EValueType::MatrixMul, IconType::Circle},
    {EValueType::Pulse,     IconType::Circle},
    {EValueType::Quat,      IconType::Circle},
    {EValueType::Screen,    IconType::Circle},
    {EValueType::Vec3,      IconType::Circle},
    {EValueType::Vec4,      IconType::Square}
};

WorkspacePin::WorkspacePin(ne::PinId id, const char* name, WorkspaceNode* Node, PinKind Kind, EValueType type)
  : ID(id), Name(name), Node(Node),  Kind(Kind), Type(type), Connected(false), Alpha(2.0)
{}

// \TODO change to core pintypes
void WorkspaceNodeBaseData::drawWorkspaceInputs()
{
    //color.Value.w = alpha / 255.0f;
    ax::Widgets::Icon(ImVec2(IconSize, IconSize), WorkspacePinShape[Type], Connected, WorkspacePinColor[Type], ImColor(32.0, 32.0, 32.0, Alpha));
}


bool WorkspacePin::IsPinConnected()
{
    return Connected;
}

WorkspaceLink::WorkspaceLink(ne::LinkId id, WorkspacePin* StartPin, WorkspacePin* EndPin)
      : ID(id), StartPin(StartPin), EndPin(EndPin), Color(255, 255, 255)
{}

void WorkspaceLink::drawLink()
{
    ne::Link(ID, StartPin->ID, EndPin->ID, Color, 2.0f);
}

WorkspaceNode::WorkspaceNode(ne::NodeId id, std::string state, ENodeType type)
    : ID(id), State(state), Type(type)
{
    Color = ImColor(255, 255, 255);
}

void WorkspaceNode::TouchNode(float touchTime)
{
  TouchTime = touchTime;
}

float WorkspaceNode::GetTouchProgress(const float constTouchTime)
{
  if (TouchTime > 0.0f)
    return (constTouchTime - TouchTime) / constTouchTime;
  else
    return 0.0f;
}

WorkspaceNodeHeader::WorkspaceNodeHeader(std::string label, ImTextureID header_background, ImColor color)
    :Label(label), header_background(header_background), Color(color)
{}


void WorkspaceNodeHeader::drawWorkspaceNodeHeader(util::NodeBuilder& builder)
{
      builder.Header(Color);
      ImGui::Spring(0);
      ImGui::TextUnformatted(Label.c_str());
      ImGui::Spring(1);
      ImGui::Dummy(ImVec2(0, 28));
      //ImGui::Spring(0);

      ImGui::BeginVertical("delegates", ImVec2(0, 28)); /* \todo rewrite /delegates/ to label ? */
      ImGui::Spring(1, 0);
      ImGui::Spring(1, 0);
      ImGui::EndVertical();
      ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
      builder.EndHeader();
}


WorkspaceNodeData::WorkspaceNodeData(std::unique_ptr<Core::NodeBase> const &nodebase)
    : Nodebase(nodebase)
{}



WorkspaceNodeInputs::WorkspaceNodeInputs(const std::vector<Core::Pin>& coreInputs, WorkspaceNode* node)
{
    for(auto corePin:coreInputs){
        Inputs.push_back(
                         WorkspacePin(getNew_s_ID() , "test inpin name", node, PinKind::Input, corePin.getType())
                         );
    }
}

void WorkspaceNodeInputs::drawWorkspaceNodeInputs(util::NodeBuilder& builder, WorkspacePin* newLinkPin)
{
    for (auto& input : Inputs)
        {
            float alpha = ImGui::GetStyle().Alpha;
            if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
              alpha = alpha * (48.0f / 255.0f);

            builder.Input(input.ID);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
            input.drawPin();
            ImGui::Spring(0);
            if (!input.Name.empty())
            {
              ImGui::TextUnformatted(input.Name.c_str());
              ImGui::Spring(0);
            }

            ImGui::PopStyleVar();
            builder.EndInput();
        }
}

WorkspaceNodeOutputs::WorkspaceNodeOutputs(const std::vector<Core::Pin>& coreOutputs, WorkspaceNode* node)
{
    for(auto corePin:coreOutputs){
        Outputs.push_back(
                         WorkspacePin(getNew_s_ID() , "test outpin name", node, PinKind::Output, corePin.getType())
                         );
    }
}

bool WorkspacePin::CanCreateLink(WorkspacePin* b)
{
    if (!this || !b || this == b || this->Kind == b->Kind || this->Type != b->Type || this->Node == b->Node)
        return false;

    return true;
}

void WorkspaceNodeOutputs::drawWorkspaceNodeOutputs(util::NodeBuilder& builder, WorkspacePin* newLinkPin)
{
    for (auto& output : Outputs)
    {
        float alpha = ImGui::GetStyle().Alpha;

        if (newLinkPin && !output.CanCreateLink(newLinkPin) && &output != newLinkPin)
          alpha = alpha * (48.0f / 255.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        builder.Output(output.ID);
        if (!output.Name.empty())
        {
          ImGui::TextUnformatted(output.Name.c_str());
          ImGui::Spring(0);
        }
        output.drawPin();
        ImGui::PopStyleVar();
        builder.EndOutput();
    }
}



