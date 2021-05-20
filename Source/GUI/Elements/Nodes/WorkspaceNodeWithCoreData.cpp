#include "WorkspaceNodeWithCoreData.h"

#include "spdlog/fmt/fmt.h"
#include <string>
#include "Core/Input/InputManager.h"

std::map<Core::Transform::DataMap const *, std::string> WorkspaceDatamapName = {
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
    {&Core::Transform::g_Perspective, "Perspective"}
};

std::vector<Core::Transform::DataMap const *> WorkspaceDatamapMenuList = {
&Core::Transform::g_Free,
&Core::Transform::g_Scale,
//&Core::Transform::g_UniformScale,
&Core::Transform::g_EulerX,
&Core::Transform::g_EulerY,
&Core::Transform::g_EulerZ,
&Core::Transform::g_Translate,
&Core::Transform::g_AllLocked,
&Core::Transform::g_Ortho,
&Core::Transform::g_Frustum,
&Core::Transform::g_Perspective};


//std::map<Core::ETransformType, std::vector<Transform::DataMap> > WorkspaceTransformTypeDatamaps = {
//    {ETransformType::Free, {    Transform::DataMap::g_Free  }},
//    {ETransformType::Scale, {   Transform::DataMap::g_Free,
//                                Transform::DataMap::g_Scale,
//                                Transform::DataMap::g_UniformScale  }}
//}

WorkspaceNodeWithCoreData::WorkspaceNodeWithCoreData(ImTextureID headerBackground, WorkspaceNodeWithCoreDataArgs const& args) /* \todo JH take default label from Const.h*/
    :   WorkspaceNode(args.nodebase->getId(), headerBackground, {.headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel})
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
				  pin.getId()
                //, fmt::format("##{}", pin.getIndex())
				        , pin.getLabel()
                , pin
                , *this ));

        m_workspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
				pin.getId()));
	}

	for (Core::Pin const &pin : outputPins)
	{
		m_workspaceOutputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
                  pin.getId()
                //, fmt::format("##{}", pin.getIndex())
				        , pin.getLabel()
				        , pin
                , *this ));
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
		if(nodeLabel == "Sequence"){
      m_workspaceInputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
          pin.getId()
          , fmt::format("##{}", pin.getIndex())
          , pin
          , *this ));
		}else{
      m_workspaceInputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
          pin.getId()
          , pin.getLabel()
          , pin
          , *this ));
		}

        m_workspaceLinksProperties.push_back(std::make_unique<WorkspaceLinkProperties>(
				pin.getId()));
	}

	for (Core::Pin const &pin : outputPins)
	{
    if(nodeLabel == "Sequence"){
      m_workspaceOutputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
          pin.getId()
          , fmt::format("##{}", pin.getIndex())
          , pin
          , *this ));
		}else{
      m_workspaceOutputsProperties.push_back(std::make_unique<WorkspaceCorePinProperties>(
          pin.getId()
          , pin.getLabel()
          , pin
          , *this ));
		}
	}
}

Ptr<Core::NodeBase> const WorkspaceNodeWithCoreData::getNodebase() const {return m_nodebase;}

std::vector<Ptr<WorkspaceLinkProperties>> const& WorkspaceNodeWithCoreData::getLinksProperties() const  { return m_workspaceLinksProperties; }
std::vector<Ptr<WorkspaceCorePinProperties>> const& WorkspaceNodeWithCoreData::getInputsProperties() const  { return m_workspaceInputsProperties; }
std::vector<Ptr<WorkspaceCorePinProperties>> const& WorkspaceNodeWithCoreData::getOutputsProperties() const { return m_workspaceOutputsProperties; }

bool WorkspaceNodeWithCoreData::inSequence()
{
    if (isTransformation())
    {
        return m_nodebase->as<Core::Transformation>()->isInSequence();
    }
    return false;
}

bool WorkspaceNodeWithCoreData::isSequence()
{
	  return m_nodebase->getOperation() == NULL;
    //return fw.name == "WorkspaceSequence";
}

bool WorkspaceNodeWithCoreData::isQuatToFloatVec(){
	return false;
}

bool WorkspaceNodeWithCoreData::isQuatToAngleAxis(){
	return false;
}

bool WorkspaceNodeWithCoreData::isCamera()
{
    return false;
}

bool WorkspaceNodeWithCoreData::isCycle()
{
  return false;
}

bool WorkspaceNodeWithCoreData::isTrackball()
{
	return false;
}

bool WorkspaceNodeWithCoreData::isTransformation()
{
    return m_nodebase->as<Core::Transformation>() != nullptr;
}

int WorkspaceNodeWithCoreData::getNumberOfVisibleDecimal()
{
    return m_numberOfVisibleDecimal;
}

int WorkspaceNodeWithCoreData::setNumberOfVisibleDecimal(int value)
{
    m_numberOfVisibleDecimal = (value >= 0 ? value : 0);
    setDataItemsWidth();
    return m_numberOfVisibleDecimal;
}

float WorkspaceNodeWithCoreData::getDataItemsWidth()
{
    return m_dataItemsWidth;
}

float WorkspaceNodeWithCoreData::setDataItemsWidth()
{
	  //ImFont* f = I3T::getTheme().get(EFont::Node);
		float size = ImGui::GetFontSize();
    float oneCharWidth = size / 2, padding = 1;
    m_dataItemsWidth = (float)(maxLenghtOfData())*oneCharWidth + 2*padding;
    return m_dataItemsWidth;
}

Core::Transform::DataMap const * WorkspaceNodeWithCoreData::setDataMap(Core::Transform::DataMap const * mapToSet)
{
    m_nodebase->setDataMap(mapToSet);
    return getDataMap();
}

Core::Transform::DataMap const * WorkspaceNodeWithCoreData::getDataMap()
{
    return m_nodebase->getDataMap();
}

WorkspaceLevelOfDetail WorkspaceNodeWithCoreData::setLevelOfDetail(WorkspaceLevelOfDetail levelOfDetail)
{
    m_levelOfDetail = levelOfDetail;
    return getLevelOfDetail();
}

WorkspaceLevelOfDetail WorkspaceNodeWithCoreData::getLevelOfDetail()
{
    return m_levelOfDetail;
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

  float step = 0.015f;
	auto io = ImGui::GetIO();
	if(InputManager::isKeyPressed(Keys::ctrll)){
    step = 0.0015f;
	}else if(InputManager::isKeyPressed(Keys::altl)){
    step = 0.00015f;
  }else if(InputManager::isKeyPressed(Keys::shiftl)){
    step = 0.15f;
  }else{
    step = 0.015f;
	}

	// make step a configurable constant.
	bool valueChanged = ImGui::DragFloat(label.c_str(), value, step, 0.0f, 0.0f, fmt::format("%.{}f", getNumberOfVisibleDecimal()).c_str(), 1.0f); /* \todo JH what parameter "power" mean? //SS if power >1.0f the number changes logaritmic */

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

void WorkspaceNodeWithCoreData::drawDataSetValues_builder(util::NodeBuilder& builder, std::vector<std::string>const & labels, std::vector<getter_function_pointer>const & getters, std::vector<setter_function_pointer>const & setters, std::vector<unsigned char> datamap_values)
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
        ImGui::TextUnformatted( labels[i].c_str() );
        localData = getters[i]();
        if (drawDragFloatWithMap_Inline(&localData,
                                        datamap_values[i],
                                        fmt::format("##{}:ch{}", idOfNode, i))) //todo check
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

    ImGui::Spring(0);

}

void WorkspaceNodeWithCoreData::drawMenuSetDataMap()
{
    if (ImGui::BeginMenu("Set datamap")) {
        for( Core::Transform::DataMap const * datamap : m_nodebase->getValidDataMaps() )
        {
            if (ImGui::MenuItem(WorkspaceDatamapName[datamap].c_str())) {
                setDataMap(datamap);
            }
        }

        ImGui::EndMenu();
    }

}

void WorkspaceNodeWithCoreData::drawMenuSetPrecision()
{
    if (ImGui::BeginMenu("Precision")) {
        ImGui::TextUnformatted(fmt::format("Actual precision: {}", getNumberOfVisibleDecimal()).c_str());
        ImGui::Separator();
        for(int i = 0; i < 5; i++) /* \todo JH some better setter for precision */
        {
            if (ImGui::MenuItem(fmt::format("{}",i).c_str()))
            {
                setNumberOfVisibleDecimal(i);
            }
        }
        ImGui::EndMenu();
    }
}

void WorkspaceNodeWithCoreData::drawMenuLevelOfDetail()
{
    if (ImGui::BeginMenu("Level of detail")) {
        ImGui::TextUnformatted(fmt::format("Actual level: {}", WorkspaceLevelOfDetailName[m_levelOfDetail]).c_str());
        ImGui::Separator();

        for (auto const& [levelOfDetail, LoDname] : WorkspaceLevelOfDetailName)
        {
            if (ImGui::MenuItem(LoDname.c_str()))
            {
                m_levelOfDetail = setLevelOfDetail(levelOfDetail);
            }
        }
        ImGui::EndMenu();
    }
}

void WorkspaceNodeWithCoreData::drawInputLinks()
{
	for (std::pair<corePinPropIter, linkPropIter> elem( m_workspaceInputsProperties.begin(), m_workspaceLinksProperties.begin() );
	     elem.first != m_workspaceInputsProperties.end() && elem.second != m_workspaceLinksProperties.end();
	     ++elem.first, ++elem.second)
	{
		if (elem.first->get()->isConnected())
        {
            ne::Link(elem.second->get()->getId(), elem.first->get()->getParentPinId(), elem.first->get()->getId(),
                     WorkspacePinColor[elem.first->get()->getType()], elem.second->get()->getThickness());
        }
	}
}

void WorkspaceNodeWithCoreData::drawData(util::NodeBuilder& builder, int index)
{
    switch(m_levelOfDetail)
    {
    case WorkspaceLevelOfDetail::Full:
        drawDataFull(builder, index);
        break;
    case WorkspaceLevelOfDetail::SetValues:
        drawDataSetValues(builder);
        break;
    case WorkspaceLevelOfDetail::Label:
        drawDataLabel(builder);
        break;

    default:
        /* \todo JH log about not supported viewScale - this should not happen since m_levelOfDetail should not allow set some other than implemented levelOfDetail */
        drawDataFull(builder, index);
    }

    /*if (m_inactiveMark != 0)
    {
        ImVec2 start = ne::GetNodePosition(m_id);
        ImVec2 size = ne::GetNodeSize(m_id);
        ImVec2 end = start + size;
        if(m_inactiveMark > 0)
        {
            end.x -= (1-m_inactiveMark)*size.x;
        }
        else
        {
            start.x += m_inactiveMark*size.x;
        }

        //mGui::PushStyleColor(ImGuiColor, ImVec4(164, 171, 190, 1));
        //ImGui::Dummy(const ImVec2& size);
        ImGui::GetWindowDrawList()->AddRectFilled( start, end, ImColor(0,0,0,0.5) );
    }*/
}

void WorkspaceNodeWithCoreData::drawDataLabel(util::NodeBuilder& builder)
{
    ImGui::TextUnformatted(m_label.c_str());
    ImGui::Spring(0);
}


void WorkspaceNodeWithCoreData::drawInputPin(util::NodeBuilder& builder, Ptr<WorkspaceCorePinProperties> const & pinProp, Core::Pin* newLinkPin)
{
	if(!isTransformation()){
    float alpha = ImGui::GetStyle().Alpha;
    //        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
    //          alpha = alpha * (48.0f / 255.0f);


    builder.Input(pinProp->getId(), WorkspacePinColor[pinProp->getType()]);

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

    ImGui::BeginVertical(pinProp->getId().AsPointer());
    ImGui::Spring(1);

    // color.Value.w = alpha / 255.0f;
    ax::Widgets::Icon(pinProp->getIconSize(),
                      WorkspacePinShape[pinProp->getType()],
                      pinProp->isConnected(), // SS add global variable. User test change or not.
                      WorkspacePinColor[pinProp->getType()],
                      pinProp->getColor());
    ImGui::Spring(1);
    ImGui::EndVertical();


    if (pinProp->getShowLabel() && !isSequence() && !isCamera()){
      if(pinProp->getLabel().empty()){ //it's never empty :(

        auto label = pinProp->getCorePin().getLabel();
        if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" || label == "pulse"){
          ImGui::TextUnformatted("");
        }else
        {
          ImGui::Spring(0,I3T::getSize(ESize::Nodes_LabelIndent));
          ImGui::TextUnformatted(label);
        }

      }else{

        auto label = pinProp->getLabel();
        if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" || label == "pulse"){
          ImGui::TextUnformatted("");
        }else{
          ImGui::Spring(0,I3T::getSize(ESize::Nodes_LabelIndent));
          ImGui::TextUnformatted(label.c_str());
        }

      }
    }

    ImGui::PopStyleVar();
    builder.EndInput();
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeWithCoreData::drawInputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	/*for (auto const & pinProp : m_workspaceInputsProperties)
	{
	    if(pinProp->getType() == EValueType::Matrix)
        {
            drawInputPin(builder, pinProp, newLinkPin);
        }
	}
	ImGui::Spring(2);
    for (auto const & pinProp : m_workspaceInputsProperties)
	{
	    if(pinProp->getType() != EValueType::Matrix)
        {
            drawInputPin(builder, pinProp, newLinkPin);
        }
	}*/

	if(!isTransformation() && !isTrackball()){
    ImGui::BeginVertical(m_nodebase->getId());
    ImGui::Spring(1);

    for (auto const & pinProp : m_workspaceInputsProperties)
    {
      drawInputPin(builder, pinProp, newLinkPin);
    }

    ImGui::Spring(1);
    ImGui::EndVertical();
	}
}

void WorkspaceNodeWithCoreData::drawOutputPin(util::NodeBuilder& builder, Ptr<WorkspaceCorePinProperties> const & pinProp, Core::Pin* newLinkPin, int outputIndex)
{
    float alpha = ImGui::GetStyle().Alpha;
    //        if (newLinkPin && !input.CanCreateLink(newLinkPin) && &input != newLinkPin)
    //          alpha = alpha * (48.0f / 255.0f);




		builder.Output(pinProp->getId(), WorkspacePinColor[pinProp->getType()]);

		if(!isTransformation() && !isCamera() && !isSequence()){ //is Operator
      ImGui::BeginVertical(pinProp->getNode().getId().AsPointer());
      drawData(builder, outputIndex);
      ImGui::EndVertical();
		}

    ImGui::Spring(1);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);


		//label
    if (pinProp->getShowLabel() && !isSequence() && !isCamera()){
			if(pinProp->getLabel().empty()){ //it's never empty :(

        auto label = pinProp->getCorePin().getLabel();
        if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" || label == "pulse"){
          ImGui::TextUnformatted("");
        }else
				{
          ImGui::Spring(1,I3T::getSize(ESize::Nodes_LabelIndent));
          ImGui::TextUnformatted(label);
          ImGui::Spring(1,I3T::getSize(ESize::Nodes_LabelIndent));
				}

      }else{

        auto label = pinProp->getLabel();
        if(label == "float" || label == "vec3" || label == "vec4" || label == "matrix" || label == "quat" || label == "pulse"){
          ImGui::TextUnformatted("");
        }else{
          ImGui::Spring(1,I3T::getSize(ESize::Nodes_LabelIndent));
          ImGui::TextUnformatted(label.c_str());
          ImGui::Spring(1,I3T::getSize(ESize::Nodes_LabelIndent));
        }

      }
    }

      ImGui::Spring(1);

			//TODO better way to solve this?
				if(isCycle() && pinProp->getIndex() > 0){
					//float w = pinProp->getIconSize().x;
					ImGuiWindow* window = ImGui::GetCurrentWindowRead();
					ne::PinRect(ImVec2(window->DC.LastItemRect.Min.x+I3T::getSize(ESizeVec2::Nodes_IconSize).x,
														 window->DC.LastItemRect.Min.y),
											ImVec2(window->DC.LastItemRect.Max.x+I3T::getSize(ESizeVec2::Nodes_IconSize).x,
														 window->DC.LastItemRect.Max.y));
				}

		  //Icon
      ax::Widgets::Icon(pinProp->getIconSize(),
                        WorkspacePinShape[pinProp->getType()],
                        false,
                        WorkspacePinColor[pinProp->getType()],
                        pinProp->getColor());




			ImGui::PopStyleVar();

      builder.EndOutput();



			if(isTrackball()){
				ImGui::EndVertical();
			}
}

void WorkspaceNodeWithCoreData::drawMiddle(util::NodeBuilder& builder){
	if(isTransformation() || isCycle() || isTrackball()){
		if(isTransformation()){
      ImGui::Spring(2, 2); //spring from left side. right side in builder.cpp
      ImGui::BeginVertical(m_nodebase->getId());
      drawData(builder, 0);
      ImGui::EndVertical();
		}else if(isTrackball()){
			ImGui::Spring(1,3);
			drawData(builder, -1);  // for trackball
		}else{
      builder.Middle();
			ImGui::Spring(1);
      drawData(builder, -1);  // for cycle
      ImGui::Spring(1);
		}
	}
}

/* \todo use newLinkPin arg*/
void WorkspaceNodeWithCoreData::drawOutputs(util::NodeBuilder& builder, Core::Pin* newLinkPin)
{
	/*for (auto const & pinProp : m_workspaceOutputsProperties)
	{
	    if(pinProp->getType() == EValueType::Matrix)
        {
            drawOutputPin(builder, pinProp, newLinkPin);
        }
	}
	ImGui::Spring(2);
    for (auto const & pinProp : m_workspaceOutputsProperties)
	{
	    if(pinProp->getType() != EValueType::Matrix)
        {
            drawOutputPin(builder, pinProp, newLinkPin);
        }
	}*/

	if(!isTransformation()){
    for (auto const & pinProp : m_workspaceOutputsProperties)
    {
      drawOutputPin(builder, pinProp, newLinkPin, pinProp->getIndex());
    }
	}

}

WorkspaceCorePinProperties::WorkspaceCorePinProperties(ne::PinId const id, std::string label, Core::Pin const &pin, WorkspaceNodeWithCoreData &node)
		: WorkspacePinProperties(id, label)
		, m_pin(pin)
		, m_node(node)
{}

Core::Pin const & WorkspaceCorePinProperties::getCorePin() const
{
    return m_pin;
}

WorkspaceNodeWithCoreData & WorkspaceCorePinProperties::getNode() const
{
    return m_node;
}

int WorkspaceCorePinProperties::getIndex() const
{
    return m_pin.getIndex();
}

ne::PinId const WorkspaceCorePinProperties::getParentPinId() const
{
    return ne::PinId( m_pin.getParentPin()->getId() );
}

PinKind WorkspaceCorePinProperties::getKind() const
{
    return m_pin.isInput() ? PinKind::Input : PinKind::Output;
}

EValueType WorkspaceCorePinProperties::getType() const
{
    return m_pin.getType();
}

/* \todo JH implement this function in Core? */
bool WorkspaceCorePinProperties::isConnected() const
{
	return (m_pin.isPluggedIn() || (m_pin.getOutComponents().size() > 0));
}

