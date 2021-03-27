#include "WorkspaceNodeWithCoreData.h"

#include "spdlog/fmt/fmt.h"
#include <string>

WorkspaceNodeWithCoreData::WorkspaceNodeWithCoreData(ImTextureID headerBackground, WorkspaceNodeWithCoreDataArgs const& args) /* \todo JH take default label from Const.h*/
    :   WorkspaceNode(args.nodebase->getId(), headerBackground, {.viewScale=args.viewScale, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel})
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
				fmt::format("input #{}", pin.getIndex()).c_str() ));

        m_workspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
				pin.getId()));
	}

	for (Core::Pin const &pin : outputPins)
	{
		m_workspaceOutputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
                pin.getId(),
                pin,
                *this,
				fmt::format("output #{}", pin.getIndex()).c_str() ));
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
				fmt::format("input #{}", pin.getIndex()).c_str() ));

        m_workspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
				pin.getId()));
	}

	for (Core::Pin const &pin : outputPins)
	{
		m_workspaceOutputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
                pin.getId(),
                pin,
                *this,
				fmt::format("output #{}", pin.getIndex()).c_str() ));
	}
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
			         elem.second->get()->m_color, 2.0f); /* elem.second->get() for dereferencing unique_ptr*/
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeWithCoreData::drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
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
                            ImColor(32.0, 32.0, 32.0, alpha)); /* \todo JH not constant here... */

		ImGui::Spring(0);
		if (!elem.second->get()->m_name.empty())
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
	for (std::pair<corePinIter, corePinPropIter> elem(m_nodebase->getOutputPins().begin(), m_workspaceOutputsProperties.begin());
	     elem.first != m_nodebase->getOutputPins().end() && elem.second != m_workspaceOutputsProperties.end();
	     ++elem.first, ++elem.second)
	{
		float alpha = ImGui::GetStyle().Alpha;
		//        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
		//          alpha = alpha * (48.0f / 255.0f);

		builder.Output(elem.first->getId());
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		if (!elem.second->get()->m_name.empty())
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
	}

	ImGui::SameLine();
	bool valueChanged = ImGui::DragFloat(label.c_str(), value);

	if (inactive)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	if (valueChanged)
	{
		inactive = true;
	}

    return valueChanged;
    /* maybe usefull
        ImGui::GetFontSize() or ImGui::GetFrameHeight()

    */
}


void WorkspaceNodeWithCoreData::drawDataSetValues(util::NodeBuilder& builder)
{
    const glm::mat4& coreData = m_nodebase->getData().getMat4();
	const Core::Transform::DataMap& coreMap = m_nodebase->getDataMap();
	int const idOfNode = this->m_id.Get();

	bool valueChanged = false;
	int rowOfChange, columnOfChange = 3;
	float valueOfChange, localData; /* user can change just one value at the moment */

	builder.Middle();

	ImGui::PushItemWidth(100.0f);
    /* Drawing is row-wise */
    for (int rows = 0; rows < 3; rows++)
    {
        localData = coreData[columnOfChange][rows]; /* Data are column-wise */
        if (drawDragFloatWithMap_Inline(&localData,
                                        coreMap[columnOfChange*4+rows],
                                        fmt::format("##{}:r{}c{}", idOfNode, rows, columnOfChange)))
        {
            valueChanged = true;
            rowOfChange = rows;
            valueOfChange = localData;
        }
        ImGui::NewLine();
    }
	ImGui::PopItemWidth();

	if (valueChanged)
	{
		m_nodebase->setValue(valueOfChange, {columnOfChange, rowOfChange});
		okynko1 > fce_set_transleate_x(okynko1)
	}

	ImGui::Spring(0); /* \todo JH what is Spring? */

}

void WorkspaceNodeWithCoreData::drawDataLabel(util::NodeBuilder& builder)
{
    builder.Middle();
    ImGui::Text(this->m_label.c_str());
    ImGui::Spring(0);
}


WorkspaceCorePinProperties::WorkspaceCorePinProperties(ne::PinId const id, Core::Pin const &pin, WorkspaceNodeWithCoreData &node, char const * name)
		: m_id(id), m_pin(pin), m_node(node), m_name(name), m_iconSize(24), m_alpha(100) /* \todo JH no konstants here... */
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
