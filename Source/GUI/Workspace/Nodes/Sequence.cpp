/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Sequence.h"

#include "GUI/I3TGui.h"
#include "GUI/Workspace/WorkspaceDiwne.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"

using namespace Workspace;

Sequence::Sequence(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase, bool isCameraSequence /*=false*/)
    : CoreNodeWithPins(diwne, nodebase, false), m_isCameraSequence(isCameraSequence)
{
	updateDataItemsWidth();
	setStyleOverride(&I3T::getTheme().m_transformationStyle);
	m_headerMinWidth = 120;
	m_headerSpacing = false; // We want drop zone background to be flush with the header, handled in left/right panels.
	m_bottomSpacing = false;
	m_contentSpacing = 4;
	m_drawContextMenuButton = true;
}

void Sequence::moveNodeToSequence(Ptr<CoreNode> dragedNode, int index)
{
	m_dropZone->addNodeAt(dragedNode, index);
}

void Sequence::moveNodeToWorkspace(Ptr<CoreNode> node)
{
	m_dropZone->removeNode(node);
}

DIWNE::NodeRange<> Sequence::getNodes() const
{
	return m_dropZone->getNodes();
}
DIWNE::NodeList& Sequence::getNodeList()
{
	return m_dropZone->getNodeList();
}
// TODO: Rename to get core nodes or get workspace nodes
DIWNE::NodeRange<CoreNode> Sequence::getInnerWorkspaceNodes()
{
	return DIWNE::NodeRange<CoreNode>(&getNodeList());
}
std::optional<Ptr<CoreNode>> Sequence::getTransform(int index) const
{
	if (index >= m_dropZone->getNodeList().size())
		return std::nullopt;

	return std::static_pointer_cast<CoreNode>(m_dropZone->getNodeList()[index]);
}

bool Sequence::allowDrawing()
{
	// TODO: Why do we care if we're a Camera sequence? What's the reason?
	return m_isCameraSequence || CoreNode::allowDrawing();
}

void Sequence::begin(DIWNE::DrawInfo& context)
{
	BasicNode::begin(context);
	const auto matrixInput = getInputs().at(Core::I3T_SEQ_IN_MAT);
	if (matrixInput->isConnected())
	{
		if (matrixInput->connectionChanged())
		{
			// Ensure that width is recalculated the first time data is shown prompted by a new input connection
			updateDataItemsWidth();
		}
	}
}

void Sequence::centerContent(DIWNE::DrawInfo& context)
{
	int position_of_draged_node_in_sequence = -1; /* -1 means not in Sequence */
	Ptr<TransformationBase> dragedNode;

	if (m_levelOfDetail == LevelOfDetail::Label)
	{
		return;
	}

	const auto matrixInput = getInputs().at(Core::I3T_SEQ_IN_MAT);
	if (matrixInput->isConnected())
	{
		// We want to retain the drop zone's background
		diwne.canvas().AddRectFilledDiwne(m_center.getMin(), m_center.getMax(),
		                                  diwne.style().color(DIWNE::Style::DROP_ZONE_BG));

		ImGui::Spacing();

		ImVec2 margin = diwne.style().size(DIWNE::Style::DROP_ZONE_MARGIN) * diwne.getZoom();
		DIWNE::DGui::DummyXY(margin);

		bool valueChanged = false;
		int rowOfChange, columnOfChange;
		float valueOfChange;
		const auto inputMatrix = m_nodebase->getInput(Core::I3T_SEQ_IN_MAT).data().getMat4();

		// TODO: Pass context to draw data
		if (DataRenderer::drawData4x4(diwne, context, getId(), m_labelDiwne, m_numberOfVisibleDecimal,
		                              getDataItemsWidth(), m_floatPopupMode, inputMatrix,
		                              {Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                               Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                               Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                               Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                               Core::EValueState::Locked, Core::EValueState::Locked, Core::EValueState::Locked,
		                               Core::EValueState::Locked},
		                              valueChanged, rowOfChange, columnOfChange, valueOfChange))
		{
			context.update(true, true, true);
		}
		ImGui::Spacing();
		DIWNE::DGui::DummyMax(margin);
	}
	else
	{
		m_dropZone->drawDiwne(context, m_drawMode);
	}
}

void Sequence::setNumberOfVisibleDecimal(int value)
{
	if (getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected())
	{
		m_numberOfVisibleDecimal = value;
		updateDataItemsWidth();
	}
	else
	{
		for (auto& transformation : getInnerWorkspaceNodes())
		{
			transformation.setNumberOfVisibleDecimal(value);
		}
	}
}

int Sequence::maxLengthOfData()
{
	if (getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected())
	{
		/*\todo JM HM better selection (index) of data*/
		return DataRenderer::maxLengthOfData4x4(m_nodebase->data(0).getMat4(), m_numberOfVisibleDecimal);
	}
	return 0;
}

void Sequence::drawInputPins(DIWNE::DrawInfo& context)
{
	const std::vector<Ptr<CorePin>>& pins = m_workspaceInputs;
	assert(pins.size() == 2); // Sequences have special pin handling, expecting matrix mul at 0; matrix data in at 1

	ImGui::Spacing(); // Manually add vertical spacing

	updatePinStyle(*pins[0]);
	updatePinStyle(*pins[1]);

	pins[1]->drawDiwne(context);
	ImGui::Dummy(ImGui::GetCurrentContext()->LastItemData.Rect.GetSize()); // Second dummy pin to keep matrix mul align

	if (pins[0]->allowDrawing())
	{
		m_left.vspring(0.15f);
		pins[0]->drawDiwne(context);
	}
}

void Sequence::drawOutputPins(DIWNE::DrawInfo& context)
{
	std::vector<Ptr<CorePin>> pins = getOutputsToShow();
	assert(pins.size() == 3); // Sequences have special pin handling, expecting matrix mul at 0; matrix data at 1 and 2

	ImGui::Spacing(); // Manually add vertical spacing


	updatePinStyle(*pins[1]);
	updatePinStyle(*pins[2]);
	pins[2]->m_pinStyle = static_cast<PinStyle>(WorkspaceModule::g_pinStyleModelMatrix);

	outputPinsVstack.begin();
	for (auto pin : {pins[1], pins[2]})
	{
		if (pin->allowDrawing())
		{
			DIWNE::DiwnePanel* row = outputPinsVstack.beginRow();
			row->spring(1.0f);
			pin->drawDiwne(context);
			outputPinsVstack.endRow();
		}
	}

	auto& pin = pins[0];
	updatePinStyle(*pin);
	if (pin->allowDrawing())
	{
		outputPinsVstack.spring(0.15f);
		DIWNE::DiwnePanel* row = outputPinsVstack.beginRow();
		row->spring(1.0f);
		pin->drawDiwne(context);
		outputPinsVstack.endRow();
	}
	outputPinsVstack.end();
}

void Sequence::popupContent(DIWNE::DrawInfo& context)
{
	CoreNodeWithPins::drawMenuSetEditable();

	ImGui::Separator();

	popupContentTracking();

	ImGui::Separator();

	drawMenuSetPrecision();

	ImGui::Separator();

	drawMenuLevelOfDetail();

	ImGui::Separator();

	CoreNode::drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}

void Sequence::popupContentTracking()
{
	auto& workspaceDiwne = static_cast<WorkspaceDiwne&>(diwne);
	if (Core::GraphManager::isTrackingEnabled() &&
	    workspaceDiwne.tracking->getSequence()->getId() == this->getNodebase()->getId())
	{
		if (I3TGui::MenuItemWithLog(_t("Stop tracking"), ""))
		{
			workspaceDiwne.trackingSwitchOff();
		}
		if (I3TGui::MenuItemWithLog(_t("Smooth tracking"), "", workspaceDiwne.smoothTracking, true))
		{
			workspaceDiwne.trackingModeSwitch();
		}
	}
	else
	{
		if (I3TGui::BeginMenuWithLog(_t("Tracking")))
		{
			if (I3TGui::MenuItemWithLog(_t("Start tracking from right"), ""))
			{
				if (Core::GraphManager::isTrackingEnabled())
				{
					workspaceDiwne.trackingSwitchOff();
				}

				workspaceDiwne.trackingSwitchOn(std::static_pointer_cast<Sequence>(shared_from_this()), true);
			}
			if (I3TGui::MenuItemWithLog(_t("Start tracking from left"), ""))
			{
				if (Core::GraphManager::isTrackingEnabled())
				{
					workspaceDiwne.trackingSwitchOff();
				}

				workspaceDiwne.trackingSwitchOn(std::static_pointer_cast<Sequence>(shared_from_this()), false);
			}
			ImGui::EndMenu();
		}
	}
}

void Sequence::drawMenuLevelOfDetail()
{
	drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
	                              {LevelOfDetail::Full, LevelOfDetail::Label});
}

void Sequence::afterDraw(DIWNE::DrawInfo& context)
{
	DiwneObject::afterDraw(context);
	DIWNE_DEBUG_OBJECTS((diwne), {
		ImRect rect = getRect();
		ImVec2 originPos = ImVec2(rect.Min.x, rect.Min.y);
		ImGui::GetForegroundDrawList()->AddText(diwne.canvas().diwne2screen(originPos) -
		                                            ImVec2(0, ImGui::GetTextLineHeightWithSpacing()),
		                                        m_destroy ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE,
		                                        fmt::format("Nodes: {}", m_dropZone->getNodeList().size()).c_str());
	});
}

void Sequence::onDestroy(bool logEvent)
{
	m_dropZone->destroy(logEvent);
	CoreNodeWithPins::onDestroy(logEvent);
}

Sequence::SequenceDropZone::SequenceDropZone(DIWNE::NodeEditor& diwne, Sequence* sequence)
    : m_sequence(sequence), NodeDropZone(diwne, sequence)
{}
void Sequence::SequenceDropZone::onNodeAdd(DIWNE::Node* node, int index)
{
	NodeDropZone::onNodeAdd(node, index);
	auto transformation = static_cast<TransformationBase*>(node);
	auto coreTransformation = transformation->getNodebase()->as<Core::Transform>();
	m_sequence->m_nodebase->as<Core::Sequence>()->pushMatrix(coreTransformation, index);
	transformation->m_parentSequence = std::static_pointer_cast<Sequence>(m_sequence->shared_from_this());
	assert(m_sequence->m_nodebase->as<Core::Sequence>()->getMatrices().size() == this->m_nodes.size());
	LOG_EVENT_NODE_ADDED_AT_INDEX(std::to_string(index), node->m_labelDiwne, m_sequence->m_labelDiwne);
}
void Sequence::SequenceDropZone::onNodeRemove(std::shared_ptr<DIWNE::Node> node, int index)
{
	NodeDropZone::onNodeRemove(node, index);
	auto transformation = static_cast<TransformationBase*>(node.get());
	transformation->m_parentSequence.reset();
	m_sequence->m_nodebase->as<Core::Sequence>()->popMatrix(index);
	assert(m_sequence->m_nodebase->as<Core::Sequence>()->getMatrices().size() == this->m_nodes.size());
	LOG_EVENT_NODE_REMOVED(node->m_labelDiwne, m_sequence->m_labelDiwne);
}
bool Sequence::SequenceDropZone::acceptNode(DIWNE::Node* node)
{
	// TODO: Remove dynamic_cast
	return dynamic_cast<TransformationBase*>(node) != nullptr;
}
void Sequence::SequenceDropZone::drawEmptyContent(DIWNE::DrawInfo& context)
{
	float zoom = diwne.getZoom();
	const ImVec2 defaultSize = ImVec2(10.0f * ImGui::GetFontSize(), 8.3f * ImGui::GetFontSize());
	ImVec2 origin = ImGui::GetCursorScreenPos();
	const char* emptyLabel = "    Drag and drop\ntransformations here";
	ImGui::SetCursorScreenPos(origin + (defaultSize / 2.0f) - (ImGui::CalcTextSize(emptyLabel) / 2.0f) -
	                          ImVec2(0, ImGui::GetStyle().ItemSpacing.y / 2.0f));
	ImGui::TextDisabled("%s", emptyLabel);
	ImGui::SetCursorScreenPos(origin);
	ImGui::Dummy(defaultSize);
}
