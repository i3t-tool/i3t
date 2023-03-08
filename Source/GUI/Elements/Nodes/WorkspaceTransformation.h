#pragma once
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceTransformation : public WorkspaceNodeWithCoreData
{
public:
	WorkspaceTransformation(DIWNE::Diwne& diwne, Ptr<Core::NodeBase> nodebase);

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceTransformation>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	std::vector<ImVec2> getInteractionPointsWithSequence();

	bool m_removeFromSequence;
	bool getRemoveFromSequence() const { return m_removeFromSequence; };
	void setRemoveFromSequence(bool value) { m_removeFromSequence = value; };
	bool isInSequence();
	DIWNE::ID aboveSequence;
	float m_topOversizeSpace;

	Ptr<Core::NodeBase> getNodebaseSequence();

	/**
	 * \brief helper function used for decision about showing the corrupted
	 * transform flag in topContent(). Overriden in WorkspaceTransformation_s.h
	 *
	 * \return Core::ETransformState:: Valid, Invalid, or Unknown
	 */
	virtual bool isMatrixValid() = 0; ///{return true;}; // todo = 0

	virtual void drawMenuSetDataMap();

	virtual bool drawDataFull();
	virtual bool inline drawDataSetValues() { return drawDataFull(); };

	virtual int maxLenghtOfData();

	bool beforeBegin();
	bool beforeContent();
	bool topContent();
	bool middleContent();
	bool afterContent();
	virtual void deleteAction();
	virtual bool allowDrawing();

	void popupContent();
	virtual void drawMenuLevelOfDetail() = 0;
	void drawMenuDelete();
	void drawMenuStorevalues();

	void updateSizes();

	virtual bool drawDataSetValues_InsideTablebuilder(std::vector<std::string> const& labels,
	                                                  std::vector<float*> const& local_data, bool& value_changed);
	virtual bool drawDataSetValuesTable_builder(std::string const cornerLabel,
	                                            std::vector<std::string> const& columnLabels,
	                                            std::vector<std::string> const& rowLabels,
	                                            std::vector<float*> const& local_data, bool& value_changed,
	                                            int& index_of_change);
};
