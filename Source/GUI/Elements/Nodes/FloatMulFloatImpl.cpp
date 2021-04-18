//#include "FloatMulFloatImpl.h"
//
// using namespace Builder;
//
// void ImGuiEx_BeginColumn()
//{
//  ImGui::BeginGroup();
//}
//
// void ImGuiEx_NextColumn()
//{
//  ImGui::EndGroup();
//  ImGui::SameLine();
//  ImGui::BeginGroup();
//}
//
// void ImGuiEx_EndColumn()
//{
//  ImGui::EndGroup();
//}
//
//// Will be almost look the same in all nodes
// Namespace* FloatMulFloatImpl::SpawnNode(std::vector<Namespace*>* s_nodes)
//{
//  auto NV = new FloatMulFloatImpl(GetNextId(), "FloatMulFloat");
//  s_nodes->emplace_back(NV);
//  auto node = Core::Builder::createNode<ENodeType::FloatMulFloat>();
//  s_nodes->back()->nodebase = std::move(node);
//
//  auto inputs = s_nodes->back()->nodebase.get()->getInputPins();
//  for (auto input : inputs)
//  {
//
//    // s_nodes->back()->Inputs.push_back(new Pin(input.getIndex(), "IN", PinType::Mat4x4));
//    s_nodes->back()->Inputs.push_back(new GUIPin(GetNextId(), "IN", input.getType()));
//  }
//  auto outputs = s_nodes->back()->nodebase.get()->getOutputPins();
//  for (auto output : outputs)
//  {
//    // s_nodes->back()->Outputs.emplace_back(new Pin(output.getIndex(), "OUT", PinType::Mat4x4));
//    s_nodes->back()->Outputs.emplace_back(new GUIPin(GetNextId(), "OUT", output.getType()));
//  }
//
//  BuildGUINode(s_nodes->back());
//
//  return s_nodes->back();
//}
//
// void FloatMulFloatImpl::drawOutputs(util::NodeBuilder& builder, GUIPin* newLinkPin)
//{
//  for (auto& output : Outputs)
//  {
//    auto alpha = ImGui::GetStyle().Alpha;
//
//    if (newLinkPin && !CanCreateLink(newLinkPin, output) && output != newLinkPin)
//      alpha = alpha * (48.0f / 255.0f);
//
//    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
//    builder.Output(output->ID);
//    if (!output->Name.empty())
//    {
//      ImGui::TextUnformatted(output->Name.c_str());
//      ImGui::Spring(0);
//    }
//    DrawPinIcon(*output, IsPinLinked(output->ID), (int)(alpha * 255));
//    ImGui::PopStyleVar();
//    builder.EndOutput();
//  }
//}
//
// void FloatMulFloatImpl::drawInputs(util::NodeBuilder& builder, GUIPin* newLinkPin)
//{
//  // Make new function drawHeader?
//  // header
//  builder.Header(Color);
//  ImGui::Spring(0);
//  ImGui::TextUnformatted(Name.c_str());
//  ImGui::Spring(1);
//  ImGui::Dummy(ImVec2(50, 28));
//  ImGui::Spring(0);
//  builder.EndHeader();
//  // end header
//
//
//  for (auto& input : Inputs)
//  {
//    auto alpha = ImGui::GetStyle().Alpha;
//    if (newLinkPin && !CanCreateLink(newLinkPin, input) && input != newLinkPin)
//      alpha = alpha * (48.0f / 255.0f);
//
//    builder.Input(input->ID);
//    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
//    DrawPinIcon(*input, IsPinLinked(input->ID), (int)(alpha * 255));
//    ImGui::Spring(0);
//    if (!input->Name.empty())
//    {
//      ImGui::TextUnformatted(input->Name.c_str());
//      ImGui::Spring(0);
//    }
//
//    ImGui::PopStyleVar();
//    builder.EndInput();
//  }
//}
//
// void FloatMulFloatImpl::drawBox(util::NodeBuilder& builder)
//{
//  ImGui::SameLine();
//  float data = nodebase->getInternalData().getFloat();
//
//  ImGui::PushItemWidth(20.0f);
//  ImGui::Text("%f", data);
//  ImGui::PopItemWidth();
//
//  ImGui::Spring(0);
//}
