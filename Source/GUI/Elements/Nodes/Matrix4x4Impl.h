#pragma once


#include "../Windows/WorkspaceWindow.h"


class Matrix4x4Impl : public Namespace
{
public:
    Matrix4x4Impl(int id, const char* name)
    {
        Type = ENodeType::Inversion;
        ID = id;
        Name = name;
        Size = ImVec2(0, 0);
        Color = ImColor(255, 113, 5);
    }

	Matrix4x4Impl(){ 
		Type = ENodeType::Inversion;
	};

	Namespace* SpawnNode(std::vector<Namespace*> *s_nodes);

	void drawBox(util::NodeBuilder& builder);

  void drawOutputs(util::NodeBuilder& builder, Pin* newLinkPin);

  void drawInputs(util::NodeBuilder& builder, Pin* newLinkPin);

};


