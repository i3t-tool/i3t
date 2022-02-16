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

	virtual bool beforeContent()
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

class WorkspaceAngleAxisToQuat : public WorkspaceOperator<ENodeType::AngleAxisToQuat>
{
public:
    bool m_halfAngle;
    WorkspaceAngleAxisToQuat(DIWNE::Diwne& diwne)
        :   WorkspaceOperator<ENodeType::AngleAxisToQuat>(diwne)
        ,   m_halfAngle(false) /* true == pin index 1, false == pin index 0 */
    {}

    bool leftContent()
    {
        bool interaction_happen = false;

        interaction_happen |= ImGui::Checkbox("/2", &m_halfAngle);

        if (interaction_happen) /* mode changed */
        {
            if(m_workspaceInputs.at(m_halfAngle?0:1)->isConnected()) /* previous mode pin is connected */
            {
                /* connect visible pin and unplug hidden one */
                m_workspaceInputs.at(m_halfAngle?1:0).get()->plug(m_workspaceInputs.at(m_halfAngle?0:1)->getLink().getStartPin());
                m_workspaceInputs.at(m_halfAngle?0:1)->unplug();
            }
        }

        interaction_happen |= m_workspaceInputs.at(m_halfAngle?1:0)->drawDiwne();
        interaction_happen |= m_workspaceInputs.at(2)->drawDiwne();

        return interaction_happen;
    }
};
