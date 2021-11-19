#pragma once
#include "WorkspaceElementsWithCoreData.h"

template <ENodeType T>
class WorkspaceOperator : public WorkspaceNodeWithCoreDataWithPins
{
public:
    WorkspaceOperator()
        :   WorkspaceNodeWithCoreDataWithPins(Core::Builder::createNode<T>())
    {
        setDataItemsWidth();
    }

	virtual bool topContent(DIWNE::Diwne &diwne)
	{
	    diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max,
                             ImGui::ColorConvertFloat4ToU32(I3T::getTheme().getHeader()), 5, ImDrawCornerFlags_Top); /* \todo JH 5 is rounding of corners -> take from Theme?*/

	    return WorkspaceNodeWithCoreData::topContent(diwne);
	}

    virtual bool inline middleContent(DIWNE::Diwne &diwne)
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
