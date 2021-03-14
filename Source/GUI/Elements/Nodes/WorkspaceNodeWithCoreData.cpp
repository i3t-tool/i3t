#include "WorkspaceNodeWithCoreData.h"

#include <string>
#include "spdlog/fmt/fmt.h"

WorkspaceNodeWithCoreData::WorkspaceNodeWithCoreData(ImTextureID headerBackground, WorkspaceNodeWithCoreDataArgs const& args) /* \todo JH take default label from Const.h*/
    :   WorkspaceNode(args.nodebase->getId(), headerBackground, {.viewScale=args.viewScale, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel})
    ,   m_nodebase(args.nodebase)

{
	const std::vector<Core::Pin>& inputPins = m_nodebase->getInputPins();
	const std::vector<Core::Pin>& outputPins = m_nodebase->getOutputPins();

	m_workspaceLinksProperties.reserve(inputPins.size());
	m_workspaceInputsProperties.reserve(inputPins.size());
	m_workspaceOutputsProperties.reserve(outputPins.size());

	for (const Core::Pin& pin : inputPins)
	{
        m_workspaceInputsProperties.push_back(std::make_unique<WorkspacePinProperties>(
				pin.getId(),
				fmt::format("input #{}", pin.getIndex()).c_str(),
				PinKind::Input, pin.getType()));

        m_workspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
				pin.getId()));
	}

	for (const Core::Pin& pin : outputPins)
	{
		m_workspaceOutputsProperties.push_back(std::make_unique<WorkspacePinProperties>(
                pin.getId(),
				fmt::format("output #{}", pin.getIndex()).c_str(),
				PinKind::Output, pin.getType()));
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
	for (std::pair<pinIter, linkPropIter> elem(m_nodebase->getInputPins().begin(), m_workspaceLinksProperties.begin());
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
	for (std::pair<pinIter, pinPropIter> elem(m_nodebase->getInputPins().begin(), m_workspaceInputsProperties.begin());
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
                            WorkspacePinShape[elem.second->get()->m_type],
                            elem.second->get()->m_connected, /* \todo do it better - it is copy from Core*/
                            WorkspacePinColor[elem.second->get()->m_type],
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
	for (std::pair<pinIter, pinPropIter> elem(m_nodebase->getOutputPins().begin(), m_workspaceOutputsProperties.begin());
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
                            WorkspacePinShape[elem.second->get()->m_type],
                            elem.second->get()->m_connected, /* \todo do it better - it is copy from Core*/
                            WorkspacePinColor[elem.second->get()->m_type],
                            ImColor(32.0, 32.0, 32.0, alpha));

		ImGui::Spring(0);

		ImGui::PopStyleVar();
		builder.EndOutput();
	}
}

bool WorkspaceNodeWithCoreData::drawDragFloatWithMap_Inline(float * const value, int const mapValue, std::string const label)
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

    if(valueChanged){
        inactive = true;
    }

    return valueChanged;
}

