#include "WorkspaceElementsWithCoreData.h"

#include "../Windows/WorkspaceWindow.h"



/// \todo Will be removed.
std::map<Core::Transform::DataMap const*, std::string> WorkspaceDatamapName = {
		{&Core::Transform::g_Free, "Free"},
		{&Core::Transform::g_Scale, "Scale"},
		//{&Core::Transform::g_UniformScale, "Uniform scale"},
		{&Core::Transform::g_EulerX, "EulerX"},
		{&Core::Transform::g_EulerY, "EulerY"},
		{&Core::Transform::g_EulerZ, "EulerZ"},
		{&Core::Transform::g_Translate, "Translate"},
		{&Core::Transform::g_AllLocked, "Locked"},
		{&Core::Transform::g_Ortho, "Ortho"},
		{&Core::Transform::g_Frustum, "Frustum"},
		{&Core::Transform::g_Perspective, "Perspective"}};

std::vector<Core::Transform::DataMap const*> WorkspaceDatamapMenuList = {
		&Core::Transform::g_Free, &Core::Transform::g_Scale,
		//&Core::Transform::g_UniformScale,
		&Core::Transform::g_EulerX, &Core::Transform::g_EulerY, &Core::Transform::g_EulerZ, &Core::Transform::g_Translate,
		&Core::Transform::g_AllLocked, &Core::Transform::g_Ortho, &Core::Transform::g_Frustum,
		&Core::Transform::g_Perspective};
// endtodo

WorkspaceNodeWithCoreData::WorkspaceNodeWithCoreData(Ptr<Core::NodeBase> nodebase)
    :   WorkspaceNode(nodebase->getId(), nodebase->getOperation()->keyWord, nodebase->getOperation()->defaultLabel )
    ,   m_nodebase(nodebase)
    ,   m_numberOfVisibleDecimal(1) /* \todo JH default number from some setting */
    ,   m_dataItemsWidth(25.0f) /* \todo JH default number from some setting - just for safe if someone not call setDataItemsWidth() in construktor of child class... */
    ,   m_inactiveMark(0.7f) /* \todo JH default 0 ; 0.7 for testing */
    ,   m_levelOfDetail(WorkspaceLevelOfDetail::Full) /* \todo JH default value from some setting */
    ,   m_floatPopupMode(Radians) /* \todo JH default value from some setting */
{

}

WorkspaceNodeWithCoreData::~WorkspaceNodeWithCoreData()
{
	m_nodebase->finalize();
}

bool WorkspaceNodeWithCoreData::topContent(DIWNE::Diwne &diwne)
{
    if(!m_topLabel.empty())
    {
        ImGui::TextUnformatted(m_topLabel.c_str());
    }else
    {
        ImGui::TextUnformatted("Empty topLabel -> read it from Core"); /* \todo JH */
    }

    return false;
}



Ptr<Core::NodeBase> const WorkspaceNodeWithCoreData::getNodebase() const { return m_nodebase; }

//std::vector<Ptr<WorkspaceLinkProperties>> const& WorkspaceNodeWithCoreData::getLinksProperties() const
//{
//	return m_workspaceLinksProperties;
//}


bool WorkspaceNodeWithCoreData::isSequence() { return m_nodebase->getOperation() == NULL; }

bool WorkspaceNodeWithCoreData::isQuatToFloatVec() { return false; }

bool WorkspaceNodeWithCoreData::isQuatToAngleAxis() { return false; }

bool WorkspaceNodeWithCoreData::isCamera() { return false; }

bool WorkspaceNodeWithCoreData::isCycle() { return false; }

bool WorkspaceNodeWithCoreData::isTrackball() { return false; }

bool WorkspaceNodeWithCoreData::isTransformation()
{
	//SS remove if when the problem with dragged node in camera will be solved
	if (this != NULL) { return m_nodebase->as<Core::Transformation>() != nullptr; }
	return false;
}

int WorkspaceNodeWithCoreData::getNumberOfVisibleDecimal() { return m_numberOfVisibleDecimal; }

int WorkspaceNodeWithCoreData::setNumberOfVisibleDecimal(int value)
{
	m_numberOfVisibleDecimal = (value >= 0 ? value : 0);
	setDataItemsWidth();
	return m_numberOfVisibleDecimal;
}

float WorkspaceNodeWithCoreData::getDataItemsWidth(DIWNE::Diwne &diwne) { return m_dataItemsWidth * diwne.getWorkAreaZoomDiwne(); }

float WorkspaceNodeWithCoreData::setDataItemsWidth()
{
	float size				 = ImGui::GetFontSize();
	float oneCharWidth = size / 2, padding = I3T::getSize(ESize::Nodes_FloatInnerPadding);
	m_dataItemsWidth = (float) (maxLenghtOfData()) * oneCharWidth + 2 * padding;
	return m_dataItemsWidth;
}

Core::Transform::DataMap const* WorkspaceNodeWithCoreData::setDataMap(Core::Transform::DataMap const* mapToSet)
{
	m_nodebase->setDataMap(mapToSet);
	return getDataMap();
}

Core::Transform::DataMap const* WorkspaceNodeWithCoreData::getDataMap() { return m_nodebase->getDataMap(); }

WorkspaceLevelOfDetail WorkspaceNodeWithCoreData::setLevelOfDetail(WorkspaceLevelOfDetail levelOfDetail)
{
	m_levelOfDetail = levelOfDetail;
	return getLevelOfDetail();
}

WorkspaceLevelOfDetail WorkspaceNodeWithCoreData::getLevelOfDetail() { return m_levelOfDetail; }


bool WorkspaceNodeWithCoreData::drawDataLabel(DIWNE::Diwne &diwne)
{
    ImGui::Text(m_middleLabel.c_str()); /* \todo JH label from core or from user */
}

void WorkspaceNodeWithCoreData::drawMenuSetPrecision()
{
	if (ImGui::BeginMenu("Precision"))
	{
		ImGui::TextUnformatted(fmt::format("Actual precision: {}", getNumberOfVisibleDecimal()).c_str());
		ImGui::Separator();
		for (int i = 0; i < 5; i++) /* \todo JH some better setter for precision */
		{
			if (ImGui::MenuItem(fmt::format("{}", i).c_str())) { setNumberOfVisibleDecimal(i); }
		}
		ImGui::EndMenu();
	}
}


void WorkspaceNodeWithCoreData::nodePopupContent()
{

    drawMenuSetPrecision();
    drawMenuLevelOfDetail();

    WorkspaceNode::nodePopupContent();
}



//void WorkspaceNodeWithCoreData::drawInputLinks()
//{
//	for (std::pair<corePinPropIter, linkPropIter> elem(m_workspaceInputsProperties.begin(),
//																										 m_workspaceLinksProperties.begin());
//			 elem.first != m_workspaceInputsProperties.end() && elem.second != m_workspaceLinksProperties.end();
//			 ++elem.first, ++elem.second)
//	{
//		if (elem.first->get()->isConnected())
//		{
//			ne::Link(elem.second->get()->getId(), elem.first->get()->getParentPinId(), elem.first->get()->getId(),
//							 I3T::getColor(WorkspacePinColor[elem.first->get()->getType()]), elem.second->get()->getThickness());
//		}
//	}
//}


//void WorkspaceNodeWithCoreData::drawInputPin(util::NodeBuilder& builder, Ptr<WorkspaceCorePinProperties> const& pinProp,
//																						 Core::Pin* newLinkPin)
//{
//	if (!isTransformation())
//	{
//		float alpha = ImGui::GetStyle().Alpha;
//
//		builder.Input(pinProp->getId(), I3T::getColor(WorkspacePinColor[pinProp->getType()]));
//
//		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
//
//		ImGui::BeginVertical(pinProp->getId().AsPointer());
//		// ImGui::Spring(1);
//
//		EColor type			 = WorkspacePinColor[pinProp->getType()];
//		EColor innerType = WorkspaceInnerPinColor[pinProp->getType()];
//
//		ax::Widgets::Icon(I3T::getSize(ESizeVec2::Nodes_IconSize), WorkspacePinShape[pinProp->getType()],
//											pinProp->isConnected(), // SS User test change or not.
//											I3T::getColor(type), I3T::getColor(innerType));
//		// ImGui::Spring(1);
//		ImGui::EndVertical();
//
//
//		if (pinProp->getShowLabel() && !isSequence() && !isCamera())
//		{
//			if (pinProp->getLabel().empty())
//			{ //it's never empty :(
//
//				auto label = pinProp->getCorePin().getLabel();
//				if (label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ||
//						label == "pulse")
//				{
//					ImGui::TextUnformatted("");
//				}
//				else
//				{
//					// ImGui::Spring(0, I3T::getSize(ESize::Nodes_LabelIndent));
//					ImGui::TextUnformatted(label);
//				}
//			}
//			else
//			{
//
//				auto label = pinProp->getLabel();
//				if (label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ||
//						label == "pulse")
//				{
//					ImGui::TextUnformatted("");
//				}
//				else
//				{
//					// ImGui::Spring(0, I3T::getSize(ESize::Nodes_LabelIndent));
//					ImGui::TextUnformatted(label.c_str());
//				}
//			}
//		}
//
//		ImGui::PopStyleVar();
//		builder.EndInput();
//	}
//}




//
///* \todo use newLinkPin arg*/
//void WorkspaceNodeWithCoreData::drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
//{
//	/*for (auto const & pinProp : m_workspaceInputsProperties)
//	{
//	    if(pinProp->getType() == EValueType::Matrix)
//        {
//            drawInputPin(builder, pinProp, newLinkPin);
//        }
//	}
//	// ImGui::Spring(2);
//    for (auto const & pinProp : m_workspaceInputsProperties)
//	{
//	    if(pinProp->getType() != EValueType::Matrix)
//        {
//            drawInputPin(builder, pinProp, newLinkPin);
//        }
//	}*/
//
//	if (!isTransformation() && !isTrackball())
//	{
//		ImGui::BeginVertical(m_nodebase->getId());
//		// ImGui::Spring(1);
//
//		for (auto const& pinProp : m_workspaceInputsProperties) { drawInputPin(builder, pinProp, newLinkPin); }
//
//		// ImGui::Spring(1);
//		ImGui::EndVertical();
//	}
//}

//void WorkspaceNodeWithCoreData::drawOutputPin(util::NodeBuilder&										 builder,
//																							Ptr<WorkspaceCorePinProperties> const& pinProp, Core::Pin* newLinkPin,
//																							int outputIndex)
//{
//	float alpha = ImGui::GetStyle().Alpha;
//
//
//	builder.Output(pinProp->getId(), I3T::getColor(WorkspacePinColor[pinProp->getType()]));
//
//	if (!isTransformation() && !isCamera() && !isSequence())
//	{ //is Operator
//		ImGui::BeginVertical(pinProp->getNode().getId().AsPointer());
//		drawData(builder, outputIndex);
//		ImGui::EndVertical();
//	}
//
//	// ImGui::Spring(1);
//	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
//
//
//	//label
//	if (pinProp->getShowLabel() && !isSequence() && !isCamera())
//	{
//		if (pinProp->getLabel().empty())
//		{ //it's never empty :(
//
//			auto label = pinProp->getCorePin().getLabel();
//			if (label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ||
//					label == "pulse")
//			{
//				ImGui::TextUnformatted("");
//			}
//			else
//			{
//				// ImGui::Spring(1, I3T::getSize(ESize::Nodes_LabelIndent));
//				ImGui::TextUnformatted(label);
//				// ImGui::Spring(1, I3T::getSize(ESize::Nodes_LabelIndent));
//			}
//		}
//		else
//		{
//
//			auto label = pinProp->getLabel();
//			if (label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ||
//					label == "pulse")
//			{
//				ImGui::TextUnformatted("");
//			}
//			else
//			{
//				// ImGui::Spring(1, I3T::getSize(ESize::Nodes_LabelIndent));
//				ImGui::TextUnformatted(label.c_str());
//				// ImGui::Spring(1, I3T::getSize(ESize::Nodes_LabelIndent));
//			}
//		}
//	}
//
//	// ImGui::Spring(1);
//
//	//TODO better way to solve this?
//	if (isCycle() && pinProp->getIndex() > 0)
//	{
//		ImGuiWindow* window = ImGui::GetCurrentWindowRead();
//		ne::PinRect(ImVec2(window->DC.LastItemRect.Min.x + I3T::getSize(ESizeVec2::Nodes_IconSize).x,
//											 window->DC.LastItemRect.Min.y),
//								ImVec2(window->DC.LastItemRect.Max.x + I3T::getSize(ESizeVec2::Nodes_IconSize).x,
//											 window->DC.LastItemRect.Max.y));
//	}
//
//	//Icon
//	EColor type			 = WorkspacePinColor[pinProp->getType()];
//	EColor innerType = WorkspaceInnerPinColor[pinProp->getType()];
//
//	ax::Widgets::Icon(I3T::getSize(ESizeVec2::Nodes_IconSize), WorkspacePinShape[pinProp->getType()], false,
//										I3T::getColor(type), I3T::getColor(innerType));
//
//	ImGui::PopStyleVar();
//	builder.EndOutput();
//
//	if (isTrackball()) { ImGui::EndVertical(); }
//}
//
//void WorkspaceNodeWithCoreData::drawMiddle(util::NodeBuilder& builder)
//{
//	if (isTransformation() || isCycle() || isTrackball())
//	{
//		if (isTransformation())
//		{
//			// ImGui::Spring(2, I3T::getSize(ESize::Nodes_leftSideSpacing)); //spring from left side. right side in builder.cpp
//			ImGui::BeginVertical(m_nodebase->getId());
//			drawData(builder, 0);
//			ImGui::EndVertical();
//		}
//		else if (isTrackball())
//		{
//			// ImGui::Spring(1, I3T::getSize(ESize::Nodes_leftSideSpacing));
//			drawData(builder, -1); // for trackball
//		}
//		else
//		{
//			builder.Middle();
//			// ImGui::Spring(1);
//			drawData(builder, -1); // for cycle
//			// ImGui::Spring(1);
//		}
//	}
//}




///* \todo use newLinkPin arg*/
//void WorkspaceNodeWithCoreData::drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
//{
//	/*for (auto const & pinProp : m_workspaceOutputsProperties)
//	{
//	    if(pinProp->getType() == EValueType::Matrix)
//        {
//            drawOutputPin(builder, pinProp, newLinkPin);
//        }
//	}
//	// ImGui::Spring(2);
//    for (auto const & pinProp : m_workspaceOutputsProperties)
//	{
//	    if(pinProp->getType() != EValueType::Matrix)
//        {
//            drawOutputPin(builder, pinProp, newLinkPin);
//        }
//	}*/
//
//	if (!isTransformation())
//	{
//		for (auto const& pinProp : m_workspaceOutputsProperties)
//		{
//			drawOutputPin(builder, pinProp, newLinkPin, pinProp->getIndex());
//		}
//	}
//}



//WorkspaceCorePinProperties::WorkspaceCorePinProperties(ne::PinId const id, std::string label, Core::Pin const& pin,
//																											 WorkspaceNodeWithCoreData& node) :
//		WorkspacePinProperties(id, label),
//		m_pin(pin), m_node(node)
//{}


WorkspaceCorePin::WorkspaceCorePin(     DIWNE::ID const id
                                                       ,    Core::Pin const& pin
                                                       ,    WorkspaceNodeWithCoreData& node)
    :   WorkspacePin(id, "")
    ,   m_pin(pin)
    ,   m_node(node)
{}

/* DIWNE function */
bool WorkspaceCorePin::pinContent(DIWNE::Diwne &diwne)
{
		float alpha = ImGui::GetStyle().Alpha;

		//builder.Input(pinProp->getId(), I3T::getColor(WorkspacePinColor[pinProp->getType()]));

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

//		ImGui::BeginVertical(pinProp->getId().AsPointer());
//		// ImGui::Spring(1);

		/* \todo JH store this in Theme ?*/
        DIWNE::IconType iconTypeBg = WorkspacePinShapeBackground[getType()];
		ImColor iconColorBg = I3T::getColor(WorkspacePinColorBackground[getType()]);
        DIWNE::IconType iconTypeFg = WorkspacePinShapeForeground[getType()];
		ImColor iconColorFg = I3T::getColor(WorkspacePinColorForeground[getType()]);

		diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg,
                        iconTypeFg, iconColorFg, iconColorFg,
                        I3T::getSize(ESizeVec2::Nodes_IconSize),
                        ImVec4(2,2,2,2), /* \todo JH padding of inner shape in icon to Theme? */
                        isConnected());


//		// ImGui::Spring(1);
//		ImGui::EndVertical();


		if (getShowLabel())
		{
			if (getLabel().empty())
			{ //it's never empty :(

				auto label = getCorePin().getLabel();
				if (label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ||
						label == "pulse")
				{
					ImGui::TextUnformatted("");
				}
				else
				{
					// ImGui::Spring(0, I3T::getSize(ESize::Nodes_LabelIndent));
					ImGui::TextUnformatted(label);
				}
			}
			else
			{

				auto label = getLabel();
				if (label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" ||
						label == "pulse")
				{
					ImGui::TextUnformatted("");
				}
				else
				{
					// ImGui::Spring(0, I3T::getSize(ESize::Nodes_LabelIndent));
					ImGui::TextUnformatted(label.c_str());
				}
			}
		}

		ImGui::PopStyleVar();
		return false;
}


Core::Pin const& WorkspaceCorePin::getCorePin() const { return m_pin; }

WorkspaceNodeWithCoreData& WorkspaceCorePin::getNode() const { return m_node; }

int WorkspaceCorePin::getIndex() const { return m_pin.getIndex(); }

//ne::PinId const WorkspaceCorePinProperties::getParentPinId() const { return ne::PinId(m_pin.getParentPin()->getId()); }

PinKind WorkspaceCorePin::getKind() const { return m_pin.isInput() ? PinKind::Input : PinKind::Output; }

EValueType WorkspaceCorePin::getType() const { return m_pin.getType(); }

/* \todo JH implement this function in Core? */
bool WorkspaceCorePin::isConnected() const
{
	return (m_pin.isPluggedIn() || (m_pin.getOutComponents().size() > 0));
}

void WorkspaceCorePin::pinActiveProcess(DIWNE::Diwne &diwne)
{
    DIWNE::Pin::pinActiveProcess(diwne);
    diwne.setLastActivePin<WorkspaceCorePin>(this);
    ImVec2 origin =  getLinkConnectionPoint();
    ImVec2 actual = diwne.screen2diwne_noZoom( ImGui::GetIO().MousePos );
    diwne.getHelperLink().setLinkEndpointsDiwne(origin, actual);
}



void WorkspaceCorePin::pinConnectLinkProcess(DIWNE::Diwne &diwne)
{
    WorkspaceCorePin *input, *output;
    if (getKind() == PinKind::Input)
    {
        input = this;
        output = diwne.getLastActivePin<WorkspaceCorePin>();
    }
    else
    {
        input = diwne.getLastActivePin<WorkspaceCorePin>();
        output = this;
    }

    Core::Pin const* coreInput = &(input->getCorePin());
    Core::Pin const* coreOutput = &(output->getCorePin());
//    if (! coreInput->isInput() ) /* todo JH move this to Core::isPlugCorrect and Core::Plug? */
//    {
//        Core::Pin const* tmp = coreInput;
//        coreInput = coreOutput;
//        coreOutput = tmp;
//    }

    switch (Core::GraphManager::isPlugCorrect(coreInput,coreOutput))
    {
    case ENodePlugResult::Ok:
        diwne.showPopUpLabel("Connection possible", I3T::getColor(EColor::Nodes_ConnectionPossible));
        if (!ImGui::GetIO().MouseDown[0])
        {
            //getLink().setStartPin((WorkspaceCoreOutputPin*)ww.m_linkCreatingPin);
            if (ENodePlugResult::Ok == Core::GraphManager::plug(coreOutput->getOwner(), coreInput->getOwner(),
                                                                coreOutput->getIndex(), coreInput->getIndex()))
            {
                WorkspaceCoreInputPin* in = dynamic_cast<WorkspaceCoreInputPin*>(input);
                WorkspaceCoreLink *lin = &(in->getLink());
                WorkspaceCoreOutputPin* ou = dynamic_cast<WorkspaceCoreOutputPin*>(output);
                lin->setStartPin(ou);
            }
        }
        break;
    /* \todo JH react informatively to other result too */
    default:
        diwne.showPopUpLabel("Connection not possible", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
    }
}

WorkspaceCoreOutputPin::WorkspaceCoreOutputPin(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node)
    : WorkspaceCorePin(id, pin, node)
{}

WorkspaceCoreOutputPinWithData::WorkspaceCoreOutputPinWithData(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node)
    : WorkspaceCoreOutputPin(id, pin, node)
{}

bool WorkspaceCoreOutputPinWithData::pinContent(DIWNE::Diwne &diwne)
{
    return WorkspaceCorePin::pinContent(diwne);
}

int WorkspaceCoreOutputPinMatrix4x4::maxLengthOfData()
{
    WorkspaceNodeWithCoreData &node = getNode();
    return maxLenghtOfData4x4(node.getNodebase()->getData().getMat4(), node.getNumberOfVisibleDecimal());
}

/* DIWNE function */
bool WorkspaceCoreOutputPinMatrix4x4::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    float valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawData4x4(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getFloatPopupMode(),
                                    node.getNodebase()->getData().getMat4(), node.getNodebase()->getDataMapRef(),
                                    node.getDataItemsWidth(diwne), valueChanged, rowOfChange, columnOfChange, valueOfChange );

    ImGui::SameLine();

    interaction_happen |= WorkspaceCoreOutputPinWithData::pinContent(diwne);
    if (valueChanged)
    {
        node.getNodebase()->setValue(valueOfChange, {columnOfChange, rowOfChange});
        node.setDataItemsWidth();
    }
    return interaction_happen;
}

bool WorkspaceCoreOutputPinVector4::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    glm::vec4 valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawDataVec4(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getFloatPopupMode(),
                                    node.getNodebase()->getData().getVec4(), node.getNodebase()->getDataMapRef(),
                                    node.getDataItemsWidth(diwne), valueChanged, valueOfChange );

    ImGui::SameLine();

    interaction_happen |= WorkspaceCoreOutputPinWithData::pinContent(diwne);
    if (valueChanged)
    {
        node.getNodebase()->setValue(valueOfChange);
        node.setDataItemsWidth();
    }
    return interaction_happen;
}
int WorkspaceCoreOutputPinVector4::maxLengthOfData()
{
    WorkspaceNodeWithCoreData &node = getNode();
    return maxLenghtOfDataVec4(node.getNodebase()->getData().getVec4(), node.getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinVector3::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    glm::vec3 valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawDataVec3(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getFloatPopupMode(),
                                    node.getNodebase()->getData().getVec3(), node.getNodebase()->getDataMapRef(),
                                    node.getDataItemsWidth(diwne), valueChanged, valueOfChange );

    ImGui::SameLine();

    interaction_happen |= WorkspaceCoreOutputPinWithData::pinContent(diwne);
    if (valueChanged)
    {
        node.getNodebase()->setValue(valueOfChange);
        node.setDataItemsWidth();
    }
    return interaction_happen;
}
int WorkspaceCoreOutputPinVector3::maxLengthOfData()
{
    WorkspaceNodeWithCoreData &node = getNode();
    return maxLenghtOfDataVec3(node.getNodebase()->getData().getVec3(), node.getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinFloat::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    float valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawDataFloat(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getFloatPopupMode(),
                                    node.getNodebase()->getData().getFloat(), node.getNodebase()->getDataMapRef(),
                                    node.getDataItemsWidth(diwne), valueChanged, valueOfChange );

    ImGui::SameLine();

    interaction_happen |= WorkspaceCoreOutputPinWithData::pinContent(diwne);
    if (valueChanged)
    {
        node.getNodebase()->setValue(valueOfChange);
        node.setDataItemsWidth();
    }
    return interaction_happen;
}
int WorkspaceCoreOutputPinFloat::maxLengthOfData()
{
    WorkspaceNodeWithCoreData &node = getNode();
    return maxLenghtOfDataFloat(node.getNodebase()->getData().getFloat(), node.getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinQuaternion::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    glm::quat valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawDataQuaternion(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getFloatPopupMode(),
                                    node.getNodebase()->getData().getQuat(), node.getNodebase()->getDataMapRef(),
                                    node.getDataItemsWidth(diwne), valueChanged, valueOfChange );

    ImGui::SameLine();

    interaction_happen |= WorkspaceCoreOutputPinWithData::pinContent(diwne);
    if (valueChanged)
    {
        node.getNodebase()->setValue(valueOfChange);
        node.setDataItemsWidth();
    }
    return interaction_happen;
}
int WorkspaceCoreOutputPinQuaternion::maxLengthOfData()
{
    WorkspaceNodeWithCoreData &node = getNode();
    return maxLenghtOfDataQuaternion(node.getNodebase()->getData().getQuat(), node.getNumberOfVisibleDecimal());
}

//void WorkspaceCoreOutputPin::pinActiveProcess(DIWNE::Diwne &diwne)
//{
//    WorkspaceCorePin::pinActiveProcess(diwne);
//    diwne.setActivePin<WorkspaceCoreOutputPin>(this);
//}

///* run if pinConnectLinkCheck() return true */
//void WorkspaceCoreOutputPin::pinConnectLinkProcess(DIWNE::Diwne &diwne)
//{
//    WorkspaceCorePin::pinConnectLinkProcess(diwne, *(diwne.getActivePin<WorkspaceCoreInputPin>()), *this);
//}

/* >>>> WorkspaceCoreInputPin <<<< */

WorkspaceCoreInputPin::WorkspaceCoreInputPin(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node)
    : WorkspaceCorePin(id, pin, node)
    , m_link(id, this)
{}

//void WorkspaceCoreInputPin::pinActiveProcess(DIWNE::Diwne &diwne)
//{
//    WorkspaceCorePin::pinActiveProcess(diwne);
//    diwne.setActivePin<WorkspaceCoreInputPin>(this);
//}

///* run if pinConnectLinkCheck() return true */
//void WorkspaceCoreInputPin::pinConnectLinkProcess(DIWNE::Diwne &diwne)
//{
//    WorkspaceWindow* ww = (WorkspaceWindow*)diwne.m_customData;
//    WorkspaceCorePin::pinConnectLinkProcess(diwne, *this, *(diwne.getActivePin<WorkspaceCoreOutputPin>()));
//}


WorkspaceCoreLink::WorkspaceCoreLink(DIWNE::ID id, WorkspaceCoreInputPin *endPin)
    : DIWNE::Link(id)
    , m_endPin(endPin)
    , m_startPin(nullptr)
{}

void WorkspaceCoreLink::updateEndpoints(){
    ImVec2 start, end;
    WorkspaceCoreOutputPin* startPin = getStartPin();
    WorkspaceCoreInputPin* endPin = getEndPin();
    if(startPin)    start = startPin->getLinkConnectionPoint();
    if(endPin)  end = endPin->getLinkConnectionPoint();
    setLinkEndpointsDiwne(start, end);
}

void WorkspaceCoreLink::updateControlPointsOffsets(){
    float offset = (getEndpoint().x-getStartpoint().x)*0.2;  /* \todo minimum control point offset and fraction (0.2) from settings */
    if (offset < 50) offset = 50;
    setLinkControlpointsOffsetDiwne(ImVec2(offset,0), ImVec2(-offset,0));
}

/* >>>>> STATIC FUNCTIONS <<<<< */
/* \todo JH correct order of arguments to make sense */

bool drawDragFloatWithMap_Inline(DIWNE::Diwne &diwne, int numberOfVisibleDecimals, FloatPopupMode floatPopupMode, bool& valueChanged, float& value, int const mapValue, std::string const label)
{
	bool inactive = (mapValue == 0 || mapValue == 255)
			? true
			: false; /* \todo JH some other type than just active/inactive will be here - maybe */
	/* \todo JH some graphical mark for "hard-coded" values (diagonal 1 in translation (255 Map value) for example) ? */
    bool inner_interaction_happen = false, valueChangedByPopup = false;

	if (inactive)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	float step = I3T::getSize(ESize::Nodes_dragSpeedDefaulrRatio);
	auto	io	 = ImGui::GetIO();
	/// \todo No need for this code. DragFloat step is handled by ImGui by default.
	/*
	if(InputManager::isKeyPressed(Keys::ctrll) || InputManager::isKeyPressed(Keys::ctrlr)){
    step = I3T::getSize(ESize::Nodes_CtrlMultiplicator) * step;
	}else if(InputManager::isKeyPressed(Keys::altl) || InputManager::isKeyPressed(Keys::altr)){
		step = I3T::getSize(ESize::Nodes_ALTMultiplicator) * step;
  }else if(InputManager::isKeyPressed(Keys::shiftl) || InputManager::isKeyPressed(Keys::shiftr)){
		step = I3T::getSize(ESize::Nodes_SHIFTMultiplicator) * step;
  }
	 */

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = I3T::getColor(EColor::Nodes_FloatText);

	// \todo JH is it done? make step a configurable constant - same or smaller than dragStep - other way drag is fired when step is not fired...
	valueChanged = ImGui::DragFloat(
			label.c_str(), &value, step, 0.0f, 0.0f, fmt::format("%.{}f", numberOfVisibleDecimals).c_str(),
			1.0f); /* if power >1.0f the number changes logarithmic */
    ImGui::SetItemAllowOverlap();

    if (ImGui::IsItemClicked(0)) inner_interaction_happen = true;

    if (!inactive)
	{
		//inner_interaction_happen |= diwne.popupFloatDiwne(diwne, label, value, valueChangedByPopup);
		inner_interaction_happen |= diwne.popupDiwneItem(label, &popupFloatContent, floatPopupMode, value, valueChangedByPopup);
		valueChanged |= valueChangedByPopup;
	}

	if (inactive)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	style.Colors[ImGuiCol_Text] = I3T::getColor(EColor::Text);

	return inner_interaction_happen || valueChanged;
}

void popupFloatContent(FloatPopupMode &popupMode, float& selectedValue, bool& valueSelected)
{
    ImGui::Text("Set value...                ");
    ImGui::Separator();

    if (ImGui::RadioButton("Radians", popupMode == FloatPopupMode::Radians)){popupMode = FloatPopupMode::Radians;} ImGui::SameLine();
    if (ImGui::RadioButton("Degrees", popupMode == FloatPopupMode::Degree)){popupMode = FloatPopupMode::Degree;}

    if (popupMode == FloatPopupMode::Radians)
    {
        ImGui::Columns(2, "floatPopupColumnsRadians", false); // 2-ways, no border

        if (ImGui::Selectable("-PI/6"))
        {
            selectedValue		= -M_PI / 6;
            valueSelected = true;
        }
        if (ImGui::Selectable("-PI/4"))
        {
            selectedValue		= -M_PI / 4;
            valueSelected = true;
        }
        if (ImGui::Selectable("-PI/3"))
        {
            selectedValue		= -M_PI / 3;
            valueSelected = true;
        }
        if (ImGui::Selectable("-PI/2"))
        {
            selectedValue		= -M_PI / 2;
            valueSelected = true;
        }
        if (ImGui::Selectable("-PI"))
        {
            selectedValue		= -M_PI;
            valueSelected = true;
        }
        if (ImGui::Selectable("-3PI/2"))
        {
            selectedValue		= -3 * M_PI / 2;
            valueSelected = true;
        }
        if (ImGui::Selectable("-1"))
        {
            selectedValue		= -1.0f;
            valueSelected = true;
        }
        ImGui::NextColumn();

        if (ImGui::Selectable("PI/6"))
        {
            selectedValue		= M_PI / 6;
            valueSelected = true;
        }
        if (ImGui::Selectable("PI/4"))
        {
            selectedValue		= M_PI / 4;
            valueSelected = true;
        }
        if (ImGui::Selectable("PI/3"))
        {
            selectedValue		= M_PI / 3;
            valueSelected = true;
        }
        if (ImGui::Selectable("PI/2"))
        {
            selectedValue		= M_PI / 2;
            valueSelected = true;
        }
        if (ImGui::Selectable("PI"))
        {
            selectedValue		= M_PI;
            valueSelected = true;
        }
        if (ImGui::Selectable("3PI/2"))
        {
            selectedValue		= -3 * M_PI / 2;
            valueSelected = true;
        }
        if (ImGui::Selectable("1"))
        {
            selectedValue		= 1.0f;
            valueSelected = true;
        }

        ImGui::Columns(1);

        if (ImGui::Selectable("0"))
        {
            selectedValue		= 0.0f;
            valueSelected = true;
        }
    }
    else if (popupMode == FloatPopupMode::Degree)
    {
        ImGui::Columns(2, "floatPopupColumnsDegrees", false);
        if (ImGui::Selectable("-1/2"))
        {
            selectedValue		= -1.0f / 2.0f;
            valueSelected = true;
        }
        if (ImGui::Selectable("-sqrt(2)/2"))
        {
            selectedValue		= -sqrt(2) / 2;
            valueSelected = true;
        }
        if (ImGui::Selectable("-sqrt(3)/2"))
        {
            selectedValue		= -sqrt(3) / 2;
            valueSelected = true;
        }
        if (ImGui::Selectable("-2"))
        {
            selectedValue		= -2.0f;
            valueSelected = true;
        }
        if (ImGui::Selectable("-1"))
        {
            selectedValue		= -1.0f;
            valueSelected = true;
        }

        ImGui::NextColumn();

        if (ImGui::Selectable("1/2"))
        {
            selectedValue = 1.0f / 2.0f;
            valueSelected = true;
        }
        if (ImGui::Selectable("sqrt(3)/2"))
        {
            selectedValue		= sqrt(3) / 2;
            valueSelected = true;
        }
        if (ImGui::Selectable("sqrt(2)/2"))
        {
            selectedValue		= sqrt(2) / 2;
            valueSelected = true;
        }
        if (ImGui::Selectable("2"))
        {
            selectedValue		= 2.0f;
            valueSelected = true;
        }
        if (ImGui::Selectable("1"))
        {
            selectedValue		= 1.0f;
            valueSelected = true;
        }
        ImGui::Columns(1);
        if (ImGui::Selectable("0"))
        {
            selectedValue		= 0.0f;
            valueSelected = true;
        }

    }
}

bool drawData4x4(DIWNE::Diwne &diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, FloatPopupMode floatPopupMode, const glm::mat4& data, const Core::Transform::DataMap& dataMap, float dataWidth, bool& valueChanged, int& rowOfChange, int& columnOfChange, float& valueOfChange )
{
    bool inner_interaction_happen = false;
    bool actualValueChanged = false;
    float localData; /* user can change just one value at the moment */
    //const glm::mat4& coreData = m_nodebase->getData(index).getMat4();
    //const Core::Transform::DataMap& dataMap = m_nodebase->getDataMapRef();

    //ImGui::PushItemWidth(getDataItemsWidth(diwne));
    ImGui::PushItemWidth(dataWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    valueChanged = false;
    /* Drawing is row-wise */
    for (int rows = 0; rows < 4; rows++)
    {
      for (int columns = 0; columns < 4; columns++)
      {

        localData = data[columns][rows]; /* Data are column-wise */
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, actualValueChanged, localData, dataMap[columns * 4 + rows],
                                        fmt::format("##{}:r{}c{}", node_id, rows, columns));
        if (actualValueChanged)
        {
            valueChanged = true;
            columnOfChange = columns;
            rowOfChange = rows;
            valueOfChange = localData;
        }
        if(columns != 3)
        {
            ImGui::SameLine();
        }
      }
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

//    if (valueChanged)
//    {
//      m_nodebase->setValue(valueOfChange, {columnOfChange, rowOfChange});
//      setDataItemsWidth(); /* \todo JH maybe somehow wrap setValue to Core and set Items Width */
//    }

    return inner_interaction_happen;
}

int maxLenghtOfData4x4(const glm::mat4& data, int numberOfVisibleDecimal)
{
    int act, maximal = 0;
    for(int column = 0; column < 4; column++)
    {
        for(int row = 0; row < 4; row++)
        {
            act = numberOfCharWithDecimalPoint( data[column][row], numberOfVisibleDecimal);
            if(act > maximal)
            {
                maximal = act;
            }
        }
    }
    return maximal;
}

bool drawDataVec4(DIWNE::Diwne &diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, FloatPopupMode floatPopupMode, const glm::vec4& data, const Core::Transform::DataMap& dataMap, float dataWidth, bool& valueChanged, glm::vec4& valueOfChange)
{
//    const glm::vec4& coreData = m_nodebase->getData(index).getVec4();
//    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

    bool  actualValueChanged = false;
    bool inner_interaction_happen = false;
    glm::vec4 localData;

    ImGui::PushItemWidth(dataWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    valueChanged = false;
    for (int columns = 0; columns < 4; columns++)
    {
        localData[columns] = data[columns];
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, actualValueChanged, localData[columns], dataMap[columns], fmt::format("##{}:{}", node_id, columns));
        if(actualValueChanged) valueChanged = true;
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    return inner_interaction_happen;
}

int maxLenghtOfDataVec4(const glm::vec4& data, int numberOfVisibleDecimal)
{
    int act, maximal = 0;

    for(int column=0; column < 4; column++)
    {
        act = numberOfCharWithDecimalPoint( data[column], numberOfVisibleDecimal );
        if(act > maximal)
        {
            maximal = act;
        }
    }

    return maximal;
}

bool drawDataVec3(DIWNE::Diwne &diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, FloatPopupMode floatPopupMode, const glm::vec3& data, const Core::Transform::DataMap& dataMap, float dataWidth, bool& valueChanged, glm::vec3& valueOfChange)
{
    bool  actualValueChanged = false;
    bool inner_interaction_happen = false;
    glm::vec3 localData;

    ImGui::PushItemWidth(dataWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    valueChanged = false;
    for (int columns = 0; columns < 3; columns++)
    {
        localData[columns] = data[columns];
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, actualValueChanged, localData[columns], dataMap[columns], fmt::format("##{}:{}", node_id, columns));
        if(actualValueChanged) valueChanged = true;
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    return inner_interaction_happen;
}
int maxLenghtOfDataVec3(const glm::vec3& data, int numberOfVisibleDecimal)
{
    int act, maximal = 0;

    for(int column=0; column < 3; column++)
    {
        act = numberOfCharWithDecimalPoint( data[column], numberOfVisibleDecimal );
        if(act > maximal)
        {
            maximal = act;
        }
    }

    return maximal;
}

bool drawDataFloat(DIWNE::Diwne &diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, FloatPopupMode floatPopupMode, const float& data, const Core::Transform::DataMap& dataMap, float dataWidth, bool& valueChanged, float& valueOfChange)
{
    bool inner_interaction_happen = false;
    glm::vec3 localData;

    ImGui::PushItemWidth(dataWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    valueChanged = false;

    inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, valueChanged, valueOfChange, dataMap[0], fmt::format("##{}:_", node_id));

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    return inner_interaction_happen;

}
int maxLenghtOfDataFloat(const float& data, int numberOfVisibleDecimal)
{
    return numberOfCharWithDecimalPoint( data, numberOfVisibleDecimal );
}


bool drawDataQuaternion(DIWNE::Diwne &diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, FloatPopupMode floatPopupMode, const glm::quat& data, const Core::Transform::DataMap& dataMap, float dataWidth, bool& valueChanged, glm::quat& valueOfChange)
{
	// SS WIP
//  const glm::quat& coreData = m_nodebase->getData(index).getQuat();
//  const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

    bool inner_interaction_happen = false;
    bool actualValueChanged = false;

    ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

  for (int columns = 0; columns < 4; columns++)
  {
    valueOfChange[columns] = data[columns];
    inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, actualValueChanged, valueOfChange[columns], dataMap[columns], fmt::format("##{}:{}", node_id, columns));
    if (actualValueChanged) valueChanged = true;
    if (columns < 3) ImGui::SameLine();

  }
  ImGui::PopStyleVar();
  ImGui::PopStyleVar();
  ImGui::PopItemWidth();

//  if (valueChanged)
//  {
//    m_nodebase->setValue(localData);
//    setDataItemsWidth();
//  }

  // ImGui::Spring(0);
  return inner_interaction_happen;
}

int maxLenghtOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal)
{
	// SS WIP
  int act, maximal = 0;

//  const glm::quat& coreData = m_nodebase->getData().getQuat();

  for(int column=0; column < 4; column++)
  {
    act = numberOfCharWithDecimalPoint( data[column], numberOfVisibleDecimal );
    if(act > maximal)
    {
      maximal = act;
    }
  }

  return maximal;
}

void drawMenuLevelOfDetail_builder(Ptr<WorkspaceNodeWithCoreData> node, std::vector<WorkspaceLevelOfDetail> const & levels_of_detail)
{
    if (ImGui::BeginMenu("Level of detail"))
	{
		ImGui::TextUnformatted(fmt::format("Actual level: {}", WorkspaceLevelOfDetailName[node->getLevelOfDetail()]).c_str());
		ImGui::Separator();

		for (auto const& levelOfDetail : levels_of_detail)
		{
            if (ImGui::MenuItem(WorkspaceLevelOfDetailName[levelOfDetail].c_str())) { node->setLevelOfDetail(levelOfDetail); }
		}
		ImGui::EndMenu();
	}
}
