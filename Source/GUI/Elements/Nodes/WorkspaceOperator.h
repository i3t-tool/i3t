#pragma once
#include "WorkspaceElementsWithCoreData.h"

template <ENodeType T>
class WorkspaceOperator : public WorkspaceNodeWithCoreData
{
public:
    WorkspaceOperator()
        :   WorkspaceNodeWithCoreData(Core::Builder::createNode<T>())
    {

        const auto& inputPins   = m_nodebase->getInputPins();
        const auto& outputPins  = m_nodebase->getOutputPins();

        m_workspaceInputs.reserve(inputPins.size());
        m_workspaceOutputs.reserve(outputPins.size());

        for (Core::Pin const& pin : inputPins)
        {
            if(pin.getType() == EValueType::MatrixMul)
            {
                m_workspaceInputs.push_back(
                    std::make_unique<WorkspaceCoreInputPinMatrixMul>(    pin.getId()
                                                                    ,   pin
                                                                    ,   *this));
            }else
            {
                m_workspaceInputs.push_back(
                    std::make_unique<WorkspaceCoreInputPin>(    pin.getId()
                                                            ,   pin
                                                            ,   *this));
            }

        }

        for (Core::Pin const& pin : outputPins)
        {
            switch (pin.getType())
            {
                case EValueType::Matrix:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinMatrix4x4>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Vec4:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinVector4>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Vec3:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinVector3>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Float:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinFloat>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Quat:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinQuaternion>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Pulse:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinPulse>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::MatrixMul:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinMatrixMul>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Screen:
                    m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPinScreen>( pin.getId()
                                                                                                ,   pin
                                                                                                ,   *this));
                    break;
                case EValueType::Ptr:
                    /* Pin with type Ptr have no graphic representation */
                    break;
                default:
                    Debug::Assert(false , "Unknown Pin type in Operator constructor");
            }

        }

        setDataItemsWidth();
    }

	std::vector<Ptr<WorkspaceCorePin>>    m_workspaceInputs;
	std::vector<Ptr<WorkspaceCorePin>>    m_workspaceOutputs;
    std::vector<Ptr<WorkspaceCorePin>> const& getInputs() const {return m_workspaceInputs;};
	std::vector<Ptr<WorkspaceCorePin>> const& getOutputs() const {return m_workspaceOutputs;};

    virtual bool leftContent(DIWNE::Diwne &diwne)
    {
        bool inner_interaction_happen = false;
        for (auto const& pin : m_workspaceInputs) {
            inner_interaction_happen |= pin->drawPinDiwne(diwne);
            if (pin->isConnected())
            {
                Ptr<WorkspaceCoreInputPin> in = std::dynamic_pointer_cast<WorkspaceCoreInputPin>(pin);
                WorkspaceCoreLink * lin = &(in->getLink());
                inner_interaction_happen |= lin->drawLinkDiwne(diwne);
            }
        }
        return inner_interaction_happen;
    }

    virtual bool rightContent(DIWNE::Diwne &diwne)
    {
        bool inner_interaction_happen = false;
        for (auto const& pin : m_workspaceOutputs) {
            inner_interaction_happen |= pin->drawPinDiwne(diwne);
        }
        return inner_interaction_happen;
    }

    virtual bool inline middleContent(DIWNE::Diwne &diwne)
    {
        return false;
    }

    void drawMenuLevelOfDetail()
    {
        drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<WorkspaceNodeWithCoreData>(shared_from_this()), {WorkspaceLevelOfDetail::Full, WorkspaceLevelOfDetail::Label});
    }

    void nodePopupContent()
    {

        drawMenuLevelOfDetail();

        WorkspaceNodeWithCoreData::nodePopupContent();
    }

    int maxLenghtOfData()
    {
        int maxLen = 0;
        for (auto const& pin : m_workspaceOutputs) {
            maxLen = std::max(maxLen, std::dynamic_pointer_cast<WorkspaceCoreOutputPinWithData>(pin)->maxLengthOfData());
        }
        return maxLen;
    }
};
