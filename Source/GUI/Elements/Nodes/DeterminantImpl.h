#pragma once

#include "../Windows/WorkspaceWindow.h"

class DeterminantImpl : public Namespace
{
public:
  DeterminantImpl(int id, const char* name)
  {
    Type = ENodeType::Determinant;
    ID = id;
    Name = name;
    Size = ImVec2(0, 0);
    Color = ImColor(50, 50, 255);
  }

  DeterminantImpl() { Type = ENodeType::Determinant; };

  Namespace* SpawnNode(std::vector<Namespace*>* s_nodes);

  void drawBox(util::NodeBuilder& builder);

  void drawOutputs(util::NodeBuilder& builder, GUIPin* newLinkPin);

  void drawInputs(util::NodeBuilder& builder, GUIPin* newLinkPin);
};
