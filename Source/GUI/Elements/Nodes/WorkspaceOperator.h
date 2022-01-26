#pragma once
#include "WorkspaceElementsWithCoreData.h"

template <ENodeType T>
class WorkspaceOperator : public WorkspaceNodeWithCoreDataWithPins
{
public:
    WorkspaceOperator(DIWNE::Diwne& diwne)
        :   WorkspaceNodeWithCoreDataWithPins(diwne, Core::Builder::createNode<T>())
    {
        setDataItemsWidth();
    }

	//===-- Double dispatch ---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	virtual bool processBeforeContent()
    {
        /* whole node background */
        diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max,
                                 ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::NodeBgOperator)), 5, ImDrawCornerFlags_Top); /* \todo JH 5 is rounding of corners -> take from Theme?*/
        return false;
    }

	virtual bool topContent()
	{
	    diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max,
                             ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(EColor::NodeHeaderOperator)), 5, ImDrawCornerFlags_Top); /* \todo JH 5 is rounding of corners -> take from Theme?*/

	    return WorkspaceNodeWithCoreData::topContent();
	}

    virtual bool inline middleContent()
    {
        return false;
    }

    void drawMenuLevelOfDetail()
    {
        drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
    }
//
//    void nodePopupContent()
//    {
//
//        drawMenuLevelOfDetail();
//
//        WorkspaceNodeWithCoreData::nodePopupContent();
//    }

    int maxLenghtOfData()
    {
        int maxLen = 0;
        for (auto const& pin : m_workspaceOutputs) {
            maxLen = std::max(maxLen, std::dynamic_pointer_cast<WorkspaceCoreOutputPinWithData>(pin)->maxLengthOfData());
        }
        return maxLen;
    }
};
