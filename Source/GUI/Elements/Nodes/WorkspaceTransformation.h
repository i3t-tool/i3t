#pragma once
#include "WorkspaceElementsWithCoreData.h"

class WorkspaceTransformation : public WorkspaceNodeWithCoreData
{
public:
	WorkspaceTransformation(DIWNE::Diwne& diwne, Ptr<Core::NodeBase> nodebase);

	//===-- Double dispatch ---------------------------------------------------===//
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

	Ptr<Core::NodeBase> getNodebaseSequence();


	virtual bool dataAreValid() { return true; }; /* \todo JH query to core */

	virtual void drawMenuSetDataMap();

	virtual bool drawDataFull();
	virtual bool inline drawDataSetValues() { return drawDataFull(); };

	virtual int maxLenghtOfData();

	bool beforeBegin();
    bool beforeContent();
	bool topContent();
	bool middleContent();
	bool afterContent();

	void popupContent();
	void drawMenuLevelOfDetail();
	void drawMenuDelete();

//
//	virtual bool drawDataSetValues_builder( std::vector<std::string> const& labels,
//                                            std::vector<getter_function_pointer> const& getters,
//                                            std::vector<setter_function_pointer> const& setters
//                                             /*,   std::vector<unsigned char> const& datamap_values*/);

    virtual bool drawDataSetValues_builder( std::vector<std::string> const& labels,
                                            std::vector<float*> const& local_data,
                                            bool &value_changed);
};
