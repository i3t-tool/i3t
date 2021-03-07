#include "WorkspaceNodeWithCoreData.h"

#include <string>
#include "spdlog/fmt/fmt.h"

/* see:
 * https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function/8114913*/
WorkspaceNodeWithCoreData::WorkspaceNodeWithCoreData(Ptr<Core::NodeBase> nodebase, ImTextureID headerBackground, std::string headerLabel) /* \todo JH take default label from Const.h*/
    :   WorkspaceNode(nodebase->getId(), headerBackground, headerLabel)
//    ,   Nodebase(std::move(nodebase))
    ,   Nodebase(nodebase)

{
	const std::vector<Core::Pin>& InputPins = Nodebase->getInputPins();
	const std::vector<Core::Pin>& OutputPins = Nodebase->getOutputPins();

	WorkspaceLinksProperties.reserve(InputPins.size());
	WorkspaceInputsProperties.reserve(InputPins.size());
	WorkspaceOutputsProperties.reserve(OutputPins.size());

	for (const Core::Pin& pin : InputPins)
	{
        WorkspaceInputsProperties.push_back(std::make_unique<WorkspacePinProperties>(
				pin.getId(),
				fmt::format("input #{}", pin.getIndex()).c_str(),
				PinKind::Input, pin.getType()));

        WorkspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
				pin.getId()));
	}

	for (const Core::Pin& pin : OutputPins)
	{
		WorkspaceOutputsProperties.push_back(std::make_unique<WorkspacePinProperties>(
                pin.getId(),
				fmt::format("output #{}", pin.getIndex()).c_str(),
				PinKind::Output, pin.getType()));
	}
}

void WorkspaceNodeWithCoreData::drawNode(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	builder.Begin(Id);

	drawHeader(builder);
	drawInputs(builder, newLinkPin);
	drawData(builder);
	drawOutputs(builder, newLinkPin);

	builder.End();
}

void WorkspaceNodeWithCoreData::drawInputLinks()
{
	for (std::pair<pinIter, linkPropIter> elem(Nodebase->getInputPins().begin(), WorkspaceLinksProperties.begin());
	     elem.first != Nodebase->getInputPins().end() && elem.second != WorkspaceLinksProperties.end();
	     ++elem.first, ++elem.second)
	{
		if (elem.first->isPluggedIn())
			ne::Link(elem.second->get()->Id, elem.first->getParentPin()->getId(), elem.first->getId(),
			         elem.second->get()->Color, 2.0f); /* elem.second->get() for dereferencing unique_ptr*/
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeWithCoreData::drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	for (std::pair<pinIter, pinPropIter> elem(Nodebase->getInputPins().begin(), WorkspaceInputsProperties.begin());
            elem.first != Nodebase->getInputPins().end()  && elem.second != WorkspaceInputsProperties.end();
            ++elem.first, ++elem.second)
	{
		float alpha = ImGui::GetStyle().Alpha;
		//        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
		//          alpha = alpha * (48.0f / 255.0f);

		builder.Input(elem.first->getId());
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		// color.Value.w = alpha / 255.0f;
		ax::Widgets::Icon(ImVec2(elem.second->get()->IconSize, elem.second->get()->IconSize),
		                  WorkspacePinShape[elem.second->get()->Type],
		                  elem.second->get()->Connected, /* \todo do it better - it is copy from Core*/
		                  WorkspacePinColor[elem.second->get()->Type], ImColor(32.0, 32.0, 32.0, alpha)); /* \todo JH not constant here... */

		ImGui::Spring(0);
		if (!elem.second->get()->Name.empty())
		{
			ImGui::TextUnformatted(elem.second->get()->Name.c_str());
			ImGui::Spring(0);
		}

		ImGui::PopStyleVar();
		builder.EndInput();
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeWithCoreData::drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	for (std::pair<pinIter, pinPropIter> elem(Nodebase->getOutputPins().begin(), WorkspaceOutputsProperties.begin());
	     /* elem.first() != Nodebase->getOutputPins().end()  &&*/ elem.second != WorkspaceOutputsProperties.end();
	     ++elem.first, ++elem.second)
	{
		float alpha = ImGui::GetStyle().Alpha;
		//        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
		//          alpha = alpha * (48.0f / 255.0f);

		builder.Output(elem.first->getId());
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		if (!elem.second->get()->Name.empty())
		{
			ImGui::TextUnformatted(elem.second->get()->Name.c_str());
			ImGui::Spring(0);
		}

		// color.Value.w = alpha / 255.0f;
		ax::Widgets::Icon(ImVec2(elem.second->get()->IconSize, elem.second->get()->IconSize),
		                  WorkspacePinShape[elem.second->get()->Type],
		                  elem.second->get()->Connected, /* \todo do it better - it is copy from Core*/
		                  WorkspacePinColor[elem.second->get()->Type], ImColor(32.0, 32.0, 32.0, alpha));

		ImGui::Spring(0);

		ImGui::PopStyleVar();
		builder.EndOutput();
	}
}

bool WorkspaceNodeWithCoreData::drawDragFloatWithMap_Inline(float * const value, int const mapValue, std::string const label)
{
    bool inactive = (mapValue == 0 || mapValue == 255) ? true : false; /* \todo JH some other type than just active/inactive will be here - maybe */
    /* \todo JH some mark for "hard-coded" values (diagonal 1 in translation (255 Map value) for example) */

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

