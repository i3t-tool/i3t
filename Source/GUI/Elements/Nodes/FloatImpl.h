#pragma once
#include "../Windows/WorkspaceWindow.h"

class FloatImpl : public Namespace {
public:
  FloatImpl(int id, const char* name)
  {
    Type = ENodeType::Float;
    ID = id;
    Name = name;
    Size = ImVec2(0, 0);
    Color = ImColor(50, 50, 255);
  }

  FloatImpl() { Type = ENodeType::Float; };

  Namespace* SpawnNode(std::vector<Namespace*>* s_nodes);

  void drawBox(util::NodeBuilder& builder);

  void drawOutputs(util::NodeBuilder& builder, GUIPin* newLinkPin);

  void drawInputs(util::NodeBuilder& builder, GUIPin* newLinkPin);
};