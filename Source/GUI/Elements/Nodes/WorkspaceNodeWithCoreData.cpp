#include "WorkspaceNodeWithCoreData.h"

#include "spdlog/fmt/fmt.h"
#include <string>

WorkspaceNodeWithCoreData::WorkspaceNodeWithCoreData(ImTextureID headerBackground, WorkspaceNodeWithCoreDataArgs const& args) /* \todo JH take default label from Const.h*/
    :   WorkspaceNode(args.nodebase->getId(), headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel})
    ,   m_nodebase(args.nodebase)
{
	const std::vector<Core::Pin>& inputPins = m_nodebase->getInputPins();
	const std::vector<Core::Pin>& outputPins = m_nodebase->getOutputPins();

	m_workspaceLinksProperties.reserve(inputPins.size());
	m_workspaceInputsProperties.reserve(inputPins.size());
	m_workspaceOutputsProperties.reserve(outputPins.size());

	for (Core::Pin const &pin : inputPins)
	{
        m_workspaceInputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
				pin.getId(),
                pin,
                *this,
				fmt::format("##{}", pin.getIndex()).c_str() ));

        m_workspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
				pin.getId()));
	}

	for (Core::Pin const &pin : outputPins)
	{
		m_workspaceOutputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
                pin.getId(),
                pin,
                *this,
				fmt::format("##{}", pin.getIndex()).c_str() ));
	}

}

WorkspaceNodeWithCoreData::WorkspaceNodeWithCoreData(ImTextureID headerBackground, Ptr<Core::NodeBase> nodebase, std::string headerLabel, std::string nodeLabel) /* \todo JH take default label from Const.h*/
    :   WorkspaceNode(nodebase->getId(), headerBackground, headerLabel, nodeLabel)
    ,   m_nodebase(nodebase)
{
	const std::vector<Core::Pin>& inputPins = m_nodebase->getInputPins();
	const std::vector<Core::Pin>& outputPins = m_nodebase->getOutputPins();

	m_workspaceLinksProperties.reserve(inputPins.size());
	m_workspaceInputsProperties.reserve(inputPins.size());
	m_workspaceOutputsProperties.reserve(outputPins.size());

	for (Core::Pin const &pin : inputPins)
	{
        m_workspaceInputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
				pin.getId(),
                pin,
                *this,
				fmt::format("##{}", pin.getIndex()).c_str() ));

        m_workspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
				pin.getId()));
	}

	for (Core::Pin const &pin : outputPins)
	{
		m_workspaceOutputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
                pin.getId(),
                pin,
                *this,
				fmt::format("##{}", pin.getIndex()).c_str() ));
	}

}

int WorkspaceNodeWithCoreData::getNumberOfVisibleDecimal()
{
    return m_numberOfVisibleDecimal;
}

int WorkspaceNodeWithCoreData::setNumberOfVisibleDecimal(int value)
{
    value >= 0 ? m_numberOfVisibleDecimal = value : m_numberOfVisibleDecimal = 0;
    setDataItemsWidth();
    return m_numberOfVisibleDecimal;
}

float WorkspaceNodeWithCoreData::getDataItemsWidth()
{
    return m_dataItemsWidth;
}

float WorkspaceNodeWithCoreData::setDataItemsWidth()
{
    float oneCharWidth = 20, padding = 10; /* \todo JH take from some font setting */
    m_dataItemsWidth = (float)(maxLenghtOfData())*oneCharWidth + 2*padding;
    return m_dataItemsWidth;
}


void WorkspaceNodeWithCoreData::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	builder.Begin(m_id);

	drawHeader(builder);
	drawInputs(builder, newLinkPin);
	drawData(builder);
	drawOutputs(builder, newLinkPin);

	builder.End();
}

void WorkspaceNodeWithCoreData::drawInputLinks()
{
	for (std::pair<corePinIter, linkPropIter> elem(m_nodebase->getInputPins().begin(), m_workspaceLinksProperties.begin());
	     elem.first != m_nodebase->getInputPins().end() && elem.second != m_workspaceLinksProperties.end();
	     ++elem.first, ++elem.second)
	{
		if (elem.first->isPluggedIn())
			ne::Link(elem.second->get()->m_id, elem.first->getParentPin()->getId(), elem.first->getId(),
			         elem.second->get()->m_color, 2.0f);
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeWithCoreData::drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
    bool showlabel = false;
	for (std::pair<corePinIter, corePinPropIter> elem(m_nodebase->getInputPins().begin(), m_workspaceInputsProperties.begin());
            elem.first != m_nodebase->getInputPins().end()  && elem.second != m_workspaceInputsProperties.end();
            ++elem.first, ++elem.second)
	{
		float alpha = ImGui::GetStyle().Alpha;
		//        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
		//          alpha = alpha * (48.0f / 255.0f);

		builder.Input(elem.first->getId());
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		// color.Value.w = alpha / 255.0f;
		ax::Widgets::Icon(ImVec2(elem.second->get()->m_iconSize, elem.second->get()->m_iconSize),
                            WorkspacePinShape[elem.second->get()->getType()],
                            elem.second->get()->getIsConnected(), /* \todo do it better - it is copy from Core*/
                            WorkspacePinColor[elem.second->get()->getType()],
                            ImColor(100.0, 200.0, 10.0, 1.0f)); /* \todo JH not constant here... */ //SS what is this?
		
		


		ImGui::Spring(0);

        /* \todo JH enable drawing of pin name? - editable by user? -> move showLabel to class variable */
		if (showlabel && !elem.second->get()->m_name.empty())
		{
			ImGui::TextUnformatted(elem.second->get()->m_name.c_str());
			ImGui::Spring(0);
		}

		ImGui::PopStyleVar();
		builder.EndInput();
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeWithCoreData::drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
    bool showlabel = false;
	for (std::pair<corePinIter, corePinPropIter> elem(m_nodebase->getOutputPins().begin(), m_workspaceOutputsProperties.begin());
	     elem.first != m_nodebase->getOutputPins().end() && elem.second != m_workspaceOutputsProperties.end();
	     ++elem.first, ++elem.second)
	{
		float alpha = ImGui::GetStyle().Alpha;
		//        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
		//          alpha = alpha * (48.0f / 255.0f);

		builder.Output(elem.first->getId());
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

        /* \todo JH enable drawing of pin name? - editable by user? -> move showLabel to class variable */
		if (showlabel && !elem.second->get()->m_name.empty())
		{
			ImGui::TextUnformatted(elem.second->get()->m_name.c_str());
			ImGui::Spring(0);
		}

		// color.Value.w = alpha / 255.0f;
		ax::Widgets::Icon(ImVec2(elem.second->get()->m_iconSize, elem.second->get()->m_iconSize),
                            WorkspacePinShape[elem.second->get()->getType()],
                            elem.second->get()->getIsConnected(), /* \todo do it better - it is copy from Core*/
                            WorkspacePinColor[elem.second->get()->getType()],
                            ImColor(32.0, 32.0, 32.0, alpha));

		ImGui::Spring(0);

		ImGui::PopStyleVar();
		builder.EndOutput();
	}
}

bool WorkspaceNodeWithCoreData::drawDragFloatWithMap_Inline(float* const value, int const mapValue,
                                                            std::string const label)
{
    bool inactive = (mapValue == 0 || mapValue == 255) ? true : false; /* \todo JH some other type than just active/inactive will be here - maybe */
    /* \todo JH some graphical mark for "hard-coded" values (diagonal 1 in translation (255 Map value) for example) ? */

	if (inactive)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		//ImGui::PushStyleVar();
	}

	ImGui::SameLine();
	bool valueChanged = ImGui::DragFloat(label.c_str(), value, 1.0f, 0.0f, 0.0f, fmt::format("% .{}f", getNumberOfVisibleDecimal()).c_str(), 1.0f); /* \todo JH what parameter "power" mean? //SS if power >1.0f the number changes logaritmic */

	if (inactive)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

    return valueChanged;
    /* maybe usefull
        ImGui::GetFontSize() or ImGui::GetFrameHeight()

    */
}


void WorkspaceNodeWithCoreData::drawData(util::NodeBuilder& builder)
{
    builder.Middle();

    switch(m_levelOfDetail)
    {
    case WorkspaceLevelOfDetail::Full:
        drawDataFull(builder); /* \todo JH here will be switch between different scale of view */
        break;
    case WorkspaceLevelOfDetail::SetValues:
        drawDataSetValues(builder);
        break;
    case WorkspaceLevelOfDetail::Label:
        drawDataLabel(builder);
        break;

    default:
        /* \todo JH log about not supported viewScale - this should not happen since m_levelOfDetail should not allow set some other than implemented levelOfDetail */
        drawDataFull(builder);
    }

}

void WorkspaceNodeWithCoreData::drawDataSetValues_builder(util::NodeBuilder& builder, std::vector<std::string>const & labels, std::vector<getter_function_pointer>const & getters, std::vector<setter_function_pointer>const & setters)
{
    /* \todo JH assert different length of vectors in argument */
    int number_of_values = labels.size();
    int const idOfNode = this->m_id.Get();

    bool valueChanged = false;
    int index_of_change;
    float valueOfChange, localData; /* user can change just one value at the moment */

    ImGui::PushItemWidth(m_dataItemsWidth);
    for (int i = 0; i < number_of_values; i++)
    {
        ImGui::Text( labels[i].c_str() );
        localData = getters[i]();
        if (drawDragFloatWithMap_Inline(&localData,
                                        1, /* \todo JH will be always changeable? */
                                        fmt::format("##{}:ch{}", idOfNode, i)))
        {
            valueChanged = true;
            index_of_change = i;
            valueOfChange = localData;
        }

    }
    ImGui::PopItemWidth();

    if (valueChanged)
	{
		setters[index_of_change](valueOfChange); /* \todo JH react to different returned value*/
		setDataItemsWidth();
	}

    ImGui::Spring(0); /* \todo JH what is Spring? */

}

void WorkspaceNodeWithCoreData::drawDataLabel(util::NodeBuilder& builder)
{
    ImGui::Text(this->m_label.c_str());
    ImGui::Spring(0);
}


WorkspaceCorePinProperties::WorkspaceCorePinProperties(ne::PinId const id, Core::Pin const &pin, WorkspaceNodeWithCoreData &node, char const * name)
		: m_id(id), m_pin(pin), m_node(node), m_name(name), m_iconSize(24), m_alpha(100) /* \todo JH no constants here... */
{}

PinKind WorkspaceCorePinProperties::getKind()
{
    return m_pin.isInput() ? PinKind::Input : PinKind::Output;
}

EValueType WorkspaceCorePinProperties::getType()
{
    return m_pin.getType();
}

/* \todo JH implement this function in Core? */
bool WorkspaceCorePinProperties::getIsConnected()
{
	return (m_pin.isPluggedIn() || (m_pin.getOutComponents().size() > 0));
}
