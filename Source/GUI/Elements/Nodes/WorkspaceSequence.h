#pragma once
#include "WorkspaceNodeWithCoreData.h"


struct WorkspaceSequenceArgs
{
	WorkspaceLevelOfDetail levelOfDetail = WorkspaceLevelOfDetail::Full;
	std::string						 headerLabel	 = "default Sequence header";
	std::string						 nodeLabel		 = "Sequence";

	Ptr<Core::NodeBase> nodebase = Core::Builder::createSequence(); /* in Camera is changed to Camera`s sequence */
};

class WorkspaceSequence : public WorkspaceNodeWithCoreData
{
protected:
	ImRect m_dataRect; /* is set in every frame - depend on levelOfDetail of inner Nodes */
	int		 m_position_of_dummy_data = -1;
	int		 m_widthOfDummy						= 20;
	std::vector<Ptr<WorkspaceNodeWithCoreData>> m_workspaceTransformation;

public:
	WorkspaceSequence(ImTextureID headerBackground, WorkspaceSequenceArgs const& args);
	WorkspaceSequence(ImTextureID headerBackground, Ptr<Core::Sequence> nodebase = nullptr,
										std::string headerLabel = "Sequence", std::string nodeLabel = "Sequence");

	void setPostionOfDummyData(int positionOfDummyData = -1);
	void setWidthOfDummy(int width = 0);


	bool	 isSequence();
	int		 getInnerPosition(ImVec2 point);
	int		 getInnerPosition(std::vector<ImVec2> points);
	ImVec2 getDataSize();

	void popNode(Ptr<WorkspaceNodeWithCoreData> node);
	void pushNode(Ptr<WorkspaceNodeWithCoreData> node, int index = 0);


	std::vector<Ptr<WorkspaceNodeWithCoreData>> const& getInnerWorkspaceNodes() const;

	void drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin = nullptr, bool withPins = true);
	void drawDataFull(util::NodeBuilder& builder, int index);
	void drawDataSetValues(util::NodeBuilder& builder);

	int maxLenghtOfData();
};
