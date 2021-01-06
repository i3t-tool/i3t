
#include "Matrix4x4Impl.h"

using namespace Builder;

//Will be almost look the same in all nodes
Namespace* Matrix4x4Impl::SpawnNode(std::vector<Namespace*> *s_nodes)
{
  auto mat = new Matrix4x4Impl(GetNextId(), "Matrix 4x4");
  s_nodes->emplace_back(mat);
  auto node = Builder::createNode<ENodeType::Inversion>();
  s_nodes->back()->nodebase = std::move(node);

  auto inputs = s_nodes->back()->nodebase.get()->getInputPins();
  for (auto input : inputs) {
    //s_nodes->back()->Inputs.push_back(new Pin(input.getIndex(), "IN", PinType::Mat4x4));
    s_nodes->back()->Inputs.push_back(new Pin(GetNextId(), "IN", PinType::Mat4x4));
  }
  auto outputs = s_nodes->back()->nodebase.get()->getOutputPins();
  for (auto output : outputs)
  {
    //s_nodes->back()->Outputs.emplace_back(new Pin(output.getIndex(), "OUT", PinType::Mat4x4));
    s_nodes->back()->Outputs.emplace_back(new Pin(GetNextId(), "OUT", PinType::Mat4x4));
  }
  BuildGUINode(s_nodes->back());

  return s_nodes->back();
}

void Matrix4x4Impl::drawOutputs(util::NodeBuilder& builder, Pin* newLinkPin)
{
  for (auto& output : Outputs)
  {
    auto alpha = ImGui::GetStyle().Alpha;

    if (newLinkPin && !CanCreateLink(newLinkPin, output) && output != newLinkPin)
      alpha = alpha * (48.0f / 255.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    builder.Output(output->ID);
    DrawPinIcon(*output, IsPinLinked(output->ID), (int)(alpha * 255));
    ImGui::PopStyleVar();
    builder.EndOutput();
  }
}

void Matrix4x4Impl::drawInputs(util::NodeBuilder& builder, Pin* newLinkPin)
{
      builder.Header(Color);
      ImGui::Spring(0);
      ImGui::TextUnformatted(Name.c_str());
      ImGui::Spring(1);
      ImGui::Dummy(ImVec2(150, 28));
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

void Matrix4x4Impl::drawBox(util::NodeBuilder &builder) {

    float x[4], y[4], z[4], w[4] = {0, 0, 0, 0};
    glm::vec4 vec(0.0f);

    auto data = nodebase->getInternalData().getMat4();

    vec = data[0];
    fromVecToArray4(vec, x);

    vec = data[1];
    fromVecToArray4(vec, y);

    vec = data[2];
    fromVecToArray4(vec, z);

    vec = data[3];
    fromVecToArray4(vec, w);

    ImGui::PushItemWidth(200.0f);
    ImGui::InputFloat4("x", x, 3);
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(200.0f);
    ImGui::InputFloat4("y", y, 3);
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(200.0f);
    ImGui::InputFloat4("z", z, 3);
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(200.0f);
    ImGui::InputFloat4("w", w, 3);
    ImGui::PopItemWidth();


    fromArrayToVec4(vec, x);
    data[0] = vec;
    fromArrayToVec4(vec, y);
    data[1] = vec;
    fromArrayToVec4(vec, z);
    data[2] = vec;
    fromArrayToVec4(vec, w);
    data[3] = vec;

    nodebase->getInternalData().setValue(data);

    nodebase->updateValues(0);

    ImGui::Spring(0);
}

