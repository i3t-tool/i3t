#pragma once
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceTransformation : public WorkspaceNodeWithCoreData
{
    public:
    WorkspaceTransformation(Ptr<Core::NodeBase> nodebase);

	std::vector<ImVec2> getInteractionPointsWithSequence();

	bool m_removeFromSequence;
	bool getRemoveFromSequence() const {return m_removeFromSequence;};
	void setRemoveFromSequence(bool value) {m_removeFromSequence=value;};
	bool isInSequence();

    virtual bool dataAreValid() {return true;}; /* \todo JH query to core */

	virtual void drawMenuSetDataMap();

    virtual bool drawDataFull(DIWNE::Diwne &diwne);
	virtual bool inline drawDataSetValues(DIWNE::Diwne &diwne){ return drawDataFull(diwne); };

	int maxLenghtOfData();

	bool topContent(DIWNE::Diwne &diwne);
	bool middleContent(DIWNE::Diwne &diwne);
	void nodePopupContent();
	void drawMenuLevelOfDetail();
	void drawMenuDelete();


virtual bool drawDataSetValues_builder(  DIWNE::Diwne &diwne
                                    ,   std::vector<std::string> const& labels
                                    ,   std::vector<getter_function_pointer> const& getters
                                    ,   std::vector<setter_function_pointer> const& setters
                                    /*,   std::vector<unsigned char> const& datamap_values*/);
};
