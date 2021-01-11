
#include "MatrixImpl.h"


using namespace Builder;

//Will be almost look the same in all nodes
Namespace* MatrixImpl::SpawnNode(std::vector<Namespace*>* s_nodes)
{
  auto mat = new MatrixImpl(GetNextId(), "Matrix");
  s_nodes->emplace_back(mat);
  auto node = Builder::createNode<ENodeType::Matrix>();
  s_nodes->back()->nodebase = std::move(node);

  auto inputs = s_nodes->back()->nodebase.get()->getInputPins();
  for (auto input : inputs) {
    //s_nodes->back()->Inputs.push_back(new Pin(input.getIndex(), "IN", PinType::Mat4x4));
    s_nodes->back()->Inputs.push_back(new GUIPin(GetNextId(), "IN", input.getType()));
  }
  auto outputs = s_nodes->back()->nodebase.get()->getOutputPins();
  for (auto output : outputs)
  {
    //s_nodes->back()->Outputs.emplace_back(new Pin(output.getIndex(), "OUT", PinType::Mat4x4));
    s_nodes->back()->Outputs.emplace_back(new GUIPin(GetNextId(), "OUT", output.getType()));
  }
  BuildGUINode(s_nodes->back());

  return s_nodes->back();
}

void MatrixImpl::drawOutputs(util::NodeBuilder& builder, GUIPin* newLinkPin)
{
  for (auto& output : Outputs)
  {
    auto alpha = ImGui::GetStyle().Alpha;

    if (newLinkPin && !CanCreateLink(newLinkPin, output) && output != newLinkPin)
      alpha = alpha * (48.0f / 255.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    builder.Output(output->ID);
    if (!output->Name.empty())
    {
      ImGui::TextUnformatted(output->Name.c_str());
      ImGui::Spring(0);
    }
    DrawPinIcon(*output, IsPinLinked(output->ID), (int)(alpha * 255));
    ImGui::PopStyleVar();
    builder.EndOutput();
  }
}

void MatrixImpl::drawInputs(util::NodeBuilder& builder, GUIPin* newLinkPin)
{
      builder.Header(Color);
      ImGui::Spring(0);
      ImGui::TextUnformatted(Name.c_str());
      ImGui::Spring(1);
      ImGui::Dummy(ImVec2(0, 28));
      //ImGui::Spring(0);

      ImGui::BeginVertical("delegates", ImVec2(0, 28));
      ImGui::Spring(1, 0);
      ImGui::Spring(1, 0);
      ImGui::EndVertical();
      ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
      builder.EndHeader();
      
    for (auto& input : Inputs)
  {
    auto alpha = ImGui::GetStyle().Alpha;
    if (newLinkPin && !CanCreateLink(newLinkPin, input) && input != newLinkPin)
      alpha = alpha * (48.0f / 255.0f);

    builder.Input(input->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    DrawPinIcon(*input, IsPinLinked(input->ID), (int)(alpha * 255));
    ImGui::Spring(0);
    if (!input->Name.empty())
    {
      ImGui::TextUnformatted(input->Name.c_str());
      ImGui::Spring(0);
    }
    
    ImGui::PopStyleVar();
    builder.EndInput();
  }
}

void MatrixImpl::drawBox(util::NodeBuilder& builder)
{

    glm::mat4& data = nodebase->getInternalData().getMat4();
    bool valueCH = false;
    std::string s = "";
    const char* c = "";

    glm::mat4 f;
    for (int rows = 0; rows < 4; rows++) {
        for (int columns = 0; columns < 4; columns++) {
          f[rows][columns] = data[rows][columns];
      }
    }

    ImGui::PushItemWidth(50.0f);
    for (int rows = 0; rows < 4; rows++)
    {
      for (int columns = 0; columns < 4; columns++)
      {
          s = "##";
          s += std::to_string(rows);
          s += std::to_string(columns);
          c = s.c_str();
          valueCH |= ImGui::InputFloat(c, &f[rows][columns]);
          if (columns < 3)
          {
            ImGui::SameLine();
          }
        }
      }
    ImGui::PopItemWidth();

    if (valueCH)
    {
      nodebase->getInternalData().setValue(f);
      nodebase->updateValues(0);
    }
   
    ImGui::Spring(0);
}

