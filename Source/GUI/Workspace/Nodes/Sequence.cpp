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

#include "GUI/Elements/Windows/ViewportWindow.h"
#include "GUI/Fonts/Bindings/BindingFontAwesome.h"
#include "GUI/Fonts/Bindings/IconsFontAwesome6.h"
#include "GUI/I3TGui.h"
#include "GUI/Toolkit.h"
#include "GUI/Viewport/ViewportModule.h"
#include "GUI/Workspace/WorkspaceDiwne.h"
#include "GUI/Workspace/WorkspaceModule.h"
#include "TransformationBase.h"
#include "Viewport/Viewport.h"

using namespace Workspace;

Sequence::Sequence(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase, bool isCameraSequence /*=false*/)
    : CoreNodeWithPins(diwne, nodebase, false), m_isCameraSequence(isCameraSequence)
{
	setStyleOverride(&I3T::getTheme().m_transformationStyle);
	// m_headerMinWidth = 120;
	// We want drop zone background to be flush with the header, handled in left/right panels.
	m_topBottomSpacingDefault = false;
	m_contentSpacing = 0;
	m_drawContextMenuButton = true;

	// Setup sequence tooltips
	m_workspaceOutputs.at(Core::I3T_SEQ_OUT_MAT)->setTooltip(_tbd("Matrix in this sequence"));
	m_workspaceOutputs.at(Core::I3T_SEQ_OUT_MAT)->setTooltipEnabled(true);

	m_workspaceOutputs.at(Core::I3T_SEQ_OUT_MOD)->setTooltip(_tbd("Composite matrix of all matrices to the left"));
	m_workspaceOutputs.at(Core::I3T_SEQ_OUT_MOD)->setTooltipEnabled(true);

	m_workspaceInputs.at(Core::I3T_SEQ_IN_MAT)->setTooltip(_tbd("Matrix replacing this sequence content"));
	m_workspaceInputs.at(Core::I3T_SEQ_IN_MAT)->setTooltipEnabled(true);
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
	return isCameraSequence() || Super::allowDrawing();
}

void Sequence::begin(DIWNE::DrawInfo& context)
{
	Super::begin(context);
	const auto matrixInput = getInputs().at(Core::I3T_SEQ_IN_MAT);
	if (matrixInput->isConnected())
	{
		if (matrixInput->connectionChanged())
		{
			// Ensure that width is recalculated the first time data is shown prompted by a new input connection
			queueUpdateDataItemsWidth();
		}
	}
}

void Sequence::centerContent(DIWNE::DrawInfo& context)
{
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
		queueUpdateDataItemsWidth();
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
		return GUI::maxLengthOfData4x4(m_nodebase->data(0).getMat4(), m_numberOfVisibleDecimal);
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
	DIWNE::DGui::NewLine();
	ImGui::Dummy(ImGui::GetCurrentContext()->LastItemData.Rect.GetSize()); // Second dummy pin to keep matrix mul align
	DIWNE::DGui::NewLine();

	if (pins[0]->allowDrawing())
	{
		m_left.vspring(0.15f);
		pins[0]->drawDiwne(context);
		DIWNE::DGui::NewLine();
	}
}

void Sequence::drawOutputPins(DIWNE::DrawInfo& context)
{
	auto& pins = m_rightPins;
	assert(pins.size() == 3); // Sequences have special pin handling, expecting matrix mul at 0; matrix data at 1 and 2

	ImGui::Spacing(); // Manually add vertical spacing

	updatePinStyle(*pins[1]);
	updatePinStyle(*pins[2]);
	pins[2]->m_pinStyle = static_cast<PinStyle>(WorkspaceModule::g_pinStyleModelMatrix);

	m_outputPinsVstack.begin();
	for (auto pin : {pins[1], pins[2]})
	{
		if (pin->allowDrawing())
		{
			DIWNE::DiwnePanel* row = m_outputPinsVstack.beginRow();
			row->spring(1.0f);
			pin->drawDiwne(context);
			m_outputPinsVstack.endRow();
		}
	}

	auto& pin = pins[0];
	updatePinStyle(*pin);
	if (pin->allowDrawing())
	{
		m_outputPinsVstack.spring(0.15f);
		DIWNE::DiwnePanel* row = m_outputPinsVstack.beginRow();
		row->spring(1.0f);
		pin->drawDiwne(context);
		m_outputPinsVstack.endRow();
	}
	m_outputPinsVstack.end();
}

void Sequence::popupContent(DIWNE::DrawInfo& context)
{
	drawMenuSetEditable();

	ImGui::Separator();

	drawMenuLevelOfDetail();

	ImGui::Separator();

	popupContentTracking();

	if (!isCameraSequence())
	{
		popupContentReferenceSpace();
	}

	ImGui::Separator();

	drawMenuSetPrecision();

	ImGui::Separator();

	drawMenuDuplicate(context);

	ImGui::Separator();

	Node::popupContent(context);
}

void Sequence::popupContentTracking()
{
	auto& workspaceDiwne = static_cast<WorkspaceDiwne&>(diwne);
	if (workspaceDiwne.isTracking() && workspaceDiwne.getTracker()->getSequenceID() == this->getNodebase()->getId())
	{
		if (I3TGui::MenuItemWithLog(_t("Stop tracking"), ""))
		{
			workspaceDiwne.stopTracking();
		}
		if (I3TGui::MenuItemWithLog(_t("Smooth tracking"), "", workspaceDiwne.smoothTracking, true))
		{
			workspaceDiwne.trackingModeSwitch();
		}
	}
	else
	{
		if (I3TGui::BeginMenuWithLog(ICON_T(ICON_FA_CROSSHAIRS " ", "Tracking")))
		{
			if (I3TGui::MenuItemWithLog(ICON_T(ICON_FA_ARROW_LEFT " ", "Start tracking from right"), ""))
			{
				workspaceDiwne.startTracking(this, false);
			}
			if (!isCameraSequence())
			{
				if (I3TGui::MenuItemWithLog(ICON_T(ICON_FA_ARROW_RIGHT " ", "Start tracking from left"), ""))
				{
					workspaceDiwne.startTracking(this, true);
				}
			}
			ImGui::EndMenu();
		}
	}
}

void Sequence::popupContentReferenceSpace()
{
	auto& viewportModule = I3T::getViewportModule();
	if (viewportModule.getWindowCount() == 1)
	{
		auto viewportWindow = viewportModule.getWindow(0);
		if (!viewportWindow->m_space.customSource)
		{
			if (I3TGui::MenuItemWithLog(ICON_TBD(ICON_FA_SOLAR_PANEL " ", "Set as reference space")))
			{
				viewportWindow->m_space.customSource = true;
				viewportWindow->m_space.sourceNode = this->sharedPtr<Sequence>();
			}
		}
		else
		{
			if (I3TGui::MenuItemWithLog(ICON_TBD(ICON_FA_ARROW_ROTATE_LEFT " ", "Reset reference space")))
			{
				viewportWindow->m_space.customSource = false;
				viewportWindow->m_space.sourceNode.reset();
			}
		}
	}
	else
	{
		if (I3TGui::BeginMenuWithLog(ICON_TBD(ICON_FA_SOLAR_PANEL " ", "Reference space")))
		{
			ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
			ImGui::TextDisabled(_tbd("Set as reference space in scene view"));
			for (int i = 0; i < viewportModule.getWindowCount(); i++)
			{
				auto viewportWindow = viewportModule.getWindow(i);
				if (!viewportWindow->m_space.customSource)
				{
					if (I3TGui::MenuItemWithLog(viewportWindow->getTitle().c_str()))
					{
						viewportWindow->m_space.customSource = true;
						viewportWindow->m_space.sourceNode = this->sharedPtr<Sequence>();
					}
				}
				else
				{
					if (I3TGui::MenuItemWithLog((_ts("Reset ") + _ts(viewportWindow->getTitle())).c_str()))
					{
						viewportWindow->m_space.customSource = false;
						viewportWindow->m_space.sourceNode.reset();
					}
				}
			}
			ImGui::PopItemFlag();
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
	auto* coreSeq = this->getNodebase()->asRaw<Core::Sequence>();
	const Core::TrackedNodeData* t = coreSeq->getTrackingData();
	if (t)
	{
		if (t->isSequenceTransform() || t->getChildCount() == 0)
		{
			const ImRect& center = m_center.getRect();
			ImVec2 dropZoneMargin = diwne.style().size(DIWNE::Style::DROP_ZONE_MARGIN);
			// dropZoneMargin += {0, diwne.canvas().screen2diwneSize(ImGui::GetStyle().ItemSpacing.y)};
			drawTrackingCursor(ImRect(center.Min + dropZoneMargin, center.Max - dropZoneMargin), t,
			                   coreSeq->getMatrices().size() != 0);
		}
		if (t->chain || t->modelSubtree)
			drawTrackingBorder(t->active, t->interpolating, t->progress);
	}

	Super::afterDraw(context);

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
	Super::onDestroy(logEvent);
}

bool Sequence::isCameraSequence() const
{
	return m_isCameraSequence;
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
