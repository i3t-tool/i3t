#pragma once


#include "../Windows/WorkspaceWindow.h"


class MatrixImpl : public Namespace
{
public:
  MatrixImpl(int id, const char* name)
    {
        Type = ENodeType::Matrix;
        ID = id;
        Name = name;
        Size = ImVec2(0, 0);
        Color = ImColor(255, 113, 5);
    }

	MatrixImpl() { 
		Type = ENodeType::Matrix;
	};

	Namespace* SpawnNode(std::vector<Namespace*> *s_nodes);

	void drawBox(util::NodeBuilder& builder);

  void drawOutputs(util::NodeBuilder& builder, GUIPin* newLinkPin);

  void drawInputs(util::NodeBuilder& builder, GUIPin* newLinkPin);

};


