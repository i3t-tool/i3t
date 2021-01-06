#pragma once

#include "../Windows/WorkspaceWindow.h"

class NormalizeVectorImpl : public Namespace
{
public:
  NormalizeVectorImpl(int id, const char* name)
  {
    Type = ENodeType::NormalizeVector;
    ID = id;
    Name = name;
    Size = ImVec2(0, 0);
    Color = ImColor(138, 201, 60);
  }

  NormalizeVectorImpl() { Type = ENodeType::NormalizeVector; };

  Namespace* SpawnNode(std::vector<Namespace*>* s_nodes);

  void drawBox(util::NodeBuilder& builder);

  void drawOutputs(util::NodeBuilder& builder, Pin* newLinkPin);

  void drawInputs(util::NodeBuilder& builder, Pin* newLinkPin);
};
