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
    ,   m_numberOfVisibleDecimal(I3T::getTheme().get(ESize::Default_VisiblePrecision))
    ,   m_dataItemsWidth(I3T::getTheme().get(ESize::Nodes_FloatWidth)) /* just for safe if someone not call setDataItemsWidth() in constructor of child class... */
    ,   m_inactiveMark(I3T::getTheme().get(ESize::Default_InactiveMark))
    ,   m_levelOfDetail(WorkspaceLevelOfDetail::Full)
    ,   m_floatPopupMode(Radians)
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
        ImGui::TextUnformatted(m_nodebase->getLabel());
    }

    return false;
}



Ptr<Core::NodeBase> const WorkspaceNodeWithCoreData::getNodebase() const { return m_nodebase; }



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
	float size				 = ImGui::GetFontSize();   // get current font size (= height in pixels) of current font with current scale applied
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
    if(!m_middleLabel.empty())
    {
        ImGui::TextUnformatted(m_middleLabel.c_str());
    }else
    {
        ImGui::TextUnformatted(m_nodebase->getLabel());
    }
    return false;
}

void WorkspaceNodeWithCoreData::drawMenuSetPrecision()
{
	if (ImGui::BeginMenu("Precision"))
	{
		ImGui::TextUnformatted(fmt::format("Actual precision: {}", getNumberOfVisibleDecimal()).c_str());
		ImGui::Separator();
		for (int i = 0; i < 5; i++) /* \todo JH, MH some better setter for precision - allowed values in settings? */
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


WorkspaceCorePin::WorkspaceCorePin( DIWNE::ID const id
                                   ,Core::Pin const& pin
                                   ,WorkspaceNodeWithCoreData& node)
    :   WorkspacePin(id, "")
    ,   m_pin(pin)
    ,   m_node(node)
    ,   m_connectionPoint(ImVec2(0,0))
    ,   m_iconRectDiwne(ImRect(0,0,0,0))
{}

/* DIWNE function */
bool WorkspaceCorePin::pinContent(DIWNE::Diwne &diwne)
{
		float alpha = ImGui::GetStyle().Alpha;
		bool interaction_happen = false;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		/* \todo JH store this in Theme ?*/
        DIWNE::IconType iconTypeBg = WorkspacePinShapeBackground[getType()];
		ImColor iconColorBg = I3T::getColor(WorkspacePinColorBackground[getType()]);
        DIWNE::IconType iconTypeFg = WorkspacePinShapeForeground[getType()];
		ImColor iconColorFg = I3T::getColor(WorkspacePinColorForeground[getType()]);

		ImVec2 iconSize = I3T::getSize(ESizeVec2::Nodes_IconSize)*diwne.getWorkAreaZoomDiwne();

        float padding = 2*diwne.getWorkAreaZoomDiwne(); /* \todo JH padding of inner shape in icon to Theme? */

        DIWNE::putInvisibleButtonUnder(fmt::format("IB_icon:{}",getId()), iconSize);
		diwne.DrawIcon(iconTypeBg, iconColorBg, iconColorBg,
                        iconTypeFg, iconColorFg, iconColorFg,
                        iconSize,
                        ImVec4(padding, padding, padding, padding),
                        isConnected());
        m_iconRectDiwne = ImRect( diwne.screen2diwne(ImGui::GetItemRectMin()), diwne.screen2diwne(ImGui::GetItemRectMax()));

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
		return interaction_happen;
}

bool WorkspaceCorePin::bypassPinHoveredAction(DIWNE::Diwne &diwne)
{
    return m_iconRectDiwne.Contains(diwne.screen2diwne(ImGui::GetMousePos()));
}

Core::Pin const& WorkspaceCorePin::getCorePin() const { return m_pin; }

WorkspaceNodeWithCoreData& WorkspaceCorePin::getNode() const { return m_node; }

int WorkspaceCorePin::getIndex() const { return m_pin.getIndex(); }

PinKind WorkspaceCorePin::getKind() const { return m_pin.isInput() ? PinKind::Input : PinKind::Output; }

EValueType WorkspaceCorePin::getType() const { return m_pin.getType(); }

/* \todo JH implement this function in Core? */
bool WorkspaceCorePin::isConnected() const
{
	return (m_pin.isPluggedIn() || (m_pin.getOutComponents().size() > 0));
}

bool WorkspaceCorePin::processPinNewLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (m_isHeld && diwne.bypassIsMouseDragging0())
    {
        diwne.setDiwneAction(DIWNE::DiwneAction::NewLink);
        diwne.setLastActivePin<WorkspaceCorePin>(this);
        ImVec2 origin =  getLinkConnectionPointDiwne();
        ImVec2 actual = diwne.screen2diwne( ImGui::GetIO().MousePos );
        diwne.getHelperLink().setLinkEndpointsDiwne(origin, actual);
        return true;
    }
    return false;

}

bool WorkspaceCorePin::processPinConnectLink(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (!m_isHeld && bypassPinPreconnectLinkAction(diwne))
    {
        /* here it is when goal pin is hoovered */
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

        switch (Core::GraphManager::isPlugCorrect(coreInput,coreOutput))
        {
            case ENodePlugResult::Ok:
                diwne.showTooltipLabel("Connection possible", I3T::getColor(EColor::Nodes_ConnectionPossible));
                if (bypassPinUnholdAction(diwne))
                {
                    WorkspaceCoreInputPin* in = dynamic_cast<WorkspaceCoreInputPin*>(input);
                    in->plug(dynamic_cast<WorkspaceCoreOutputPin*>(output));
                }
                break;
            /* \todo JH react informatively to other result too */
            default:
                diwne.showTooltipLabel("Connection not possible", I3T::getColor(EColor::Nodes_ConnectionNotPossible));
        }
        return true;
    }
    return false;
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

/* >>>> Pin types <<<< */

bool WorkspaceCoreOutputPinMatrix4x4::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    float valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawData4x4(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(diwne), node.getFloatPopupMode(),
                                    getCorePin().data().getMat4(), node.getNodebase()->getDataMapRef(),
                                    valueChanged, rowOfChange, columnOfChange, valueOfChange );

    ImGui::SameLine();

    interaction_happen |= WorkspaceCoreOutputPinWithData::pinContent(diwne);
    if (valueChanged)
    {
        /* \todo JM MH set values to given (this) pin*/
        node.getNodebase()->setValue(valueOfChange, {columnOfChange, rowOfChange});
        node.setDataItemsWidth();
    }
    return interaction_happen;
}

int WorkspaceCoreOutputPinMatrix4x4::maxLengthOfData()
{
    return maxLenghtOfData4x4(getCorePin().data().getMat4(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinVector4::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    glm::vec4 valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawDataVec4(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(diwne), node.getFloatPopupMode(),
                                    getCorePin().data().getVec4(), node.getNodebase()->getDataMapRef(),
                                    valueChanged, valueOfChange );


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
    return maxLenghtOfDataVec4(getCorePin().data().getVec4(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinVector3::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    glm::vec3 valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawDataVec3(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(diwne), node.getFloatPopupMode(),
                                    getCorePin().data().getVec3(), node.getNodebase()->getDataMapRef(),
                                    valueChanged, valueOfChange );

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
    return maxLenghtOfDataVec3(getCorePin().data().getVec3(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinFloat::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    float valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawDataFloat(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(diwne), node.getFloatPopupMode(),
                                    getCorePin().data().getFloat(), node.getNodebase()->getDataMapRef(),
                                    valueChanged, valueOfChange );

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
    return maxLenghtOfDataFloat(getCorePin().data().getFloat(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinQuaternion::pinContent(DIWNE::Diwne &diwne)
{
    bool valueChanged = false, interaction_happen = false;
    int rowOfChange, columnOfChange;
    glm::quat valueOfChange;
    WorkspaceNodeWithCoreData &node = getNode();

    interaction_happen = drawDataQuaternion(diwne, node.getId(), node.getNumberOfVisibleDecimal(), node.getDataItemsWidth(diwne), node.getFloatPopupMode(),
                                            getCorePin().data().getQuat(), node.getNodebase()->getDataMapRef(),
                                            valueChanged, valueOfChange );

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
    return maxLenghtOfDataQuaternion(getCorePin().data().getQuat(), getNode().getNumberOfVisibleDecimal());
}

bool WorkspaceCoreOutputPinPulse::pinContent(DIWNE::Diwne &diwne)
{
    ImGui::Button(fmt::format("Pulse##n{}:p{}", getNode().getId(), m_idDiwne).c_str());
    return ImGui::IsItemHovered() && ImGui::IsMouseReleased(0);
}
int WorkspaceCoreOutputPinPulse::maxLengthOfData() {return 0;} /* no data with length here*/

WorkspaceCoreOutputPinScreen::WorkspaceCoreOutputPinScreen(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node)
 : WorkspaceCoreOutputPinWithData(id, pin, node)
{
	  // tohle nema smysl tady - muselo by se v camera update, resp. v render()
    //glClearColor(Config::BACKGROUND_COLOR.x, Config::BACKGROUND_COLOR.y, Config::BACKGROUND_COLOR.z, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	  //todo - size dle velikosti krabicky a dle zoomu
    rend = new RenderTexture(&renderTexture, 256, 256);   // create FBO and texture as attachment
    cam = new Camera(60.0f, Application::get().world()->sceneRoot, rend);  // connet textre with camera
    cam->update();
}
bool WorkspaceCoreOutputPinScreen::pinContent(DIWNE::Diwne &diwne)
{
    if(getCorePin().isPluggedIn()){
        glm::mat4 camera = Core::GraphManager::getParent(getNode().getNodebase())->getData(2).getMat4(); /* JH why magic 2? */

        cam->m_perspective = camera;
        cam->update();

        //ImGui::Image((void*)(intptr_t)renderTexture,I3T::getSize(ESizeVec2::Nodes_ScreenTextureSize),ImVec2(0.0f,1.0f), ImVec2(1,0));
    	  ImGui::Image(reinterpret_cast<ImTextureID>(renderTexture), I3T::getSize(ESizeVec2::Nodes_ScreenTextureSize), ImVec2(0.0f,1.0f), ImVec2(1,0)); //vertiocal flip
    }
    return false;
}
int WorkspaceCoreOutputPinScreen::maxLengthOfData() {return 0;} /* no data with length here*/

/* >>>> WorkspaceCoreInputPin <<<< */

WorkspaceCoreInputPin::WorkspaceCoreInputPin(DIWNE::ID const id, Core::Pin const& pin, WorkspaceNodeWithCoreData& node)
    : WorkspaceCorePin(id, pin, node)
    , m_link(id, this)
{}

void WorkspaceCoreInputPin::setConnectedWorkspaceOutput(WorkspaceCoreOutputPin* ou)
{
    m_link.setStartPin(ou);
}

void WorkspaceCoreInputPin::unplug()
{
    Core::GraphManager::unplugInput(getNode().getNodebase(), getIndex());
    m_link.setStartPin(nullptr);
}

void WorkspaceCoreInputPin::plug(WorkspaceCoreOutputPin* ou)
{
    Core::Pin const* coreInput = &(getCorePin());
    Core::Pin const* coreOutput = &(ou->getCorePin());

    if (ENodePlugResult::Ok == Core::GraphManager::plug(coreOutput->getOwner(), coreInput->getOwner(),
                                                        coreOutput->getIndex(), coreInput->getIndex()))
    {
        setConnectedWorkspaceOutput(ou);
        m_link.m_just_pluged = true;
    }
}

bool WorkspaceCoreInputPin::pinContent(DIWNE::Diwne &diwne)
{
    return WorkspaceCorePin::pinContent(diwne);
}

bool WorkspaceCoreInputPin::processPin(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    bool interaction_happen = false;
    if (bypassPinHoveredAction(diwne)){
    if (bypassPinHoveredAction(diwne) && bypassPinUnholdAction(diwne))
    {
        interaction_happen = true;
        WorkspaceWindow& ww = dynamic_cast<WorkspaceWindow&>(diwne);
        ww.m_workspaceWindowAction = WorkspaceWindowAction::CreateAndPlugTypeConstructor;
        ww.m_linkCreatingPin = this;
    }
    }
    return interaction_happen || DIWNE::Pin::processPin(diwne, inner_interaction_happen);
}
/* >>>> WorkspaceCoreLink <<<< */

WorkspaceCoreLink::WorkspaceCoreLink(DIWNE::ID id, WorkspaceCoreInputPin *endPin)
    : DIWNE::Link(id)
    , m_endPin(endPin)
    , m_startPin(nullptr)
{}

void WorkspaceCoreLink::unplug()
{
    m_endPin->unplug();
    m_startPin = nullptr;
}

void WorkspaceCoreLink::linkPopupContent()
{
    if (ImGui::MenuItem("Delete")) {unplug();}
}


void WorkspaceCoreLink::updateEndpoints(){
    ImVec2 start, end;
    WorkspaceCoreOutputPin* startPin = getStartPin();
    WorkspaceCoreInputPin* endPin = getEndPin();
    if(startPin)    start = startPin->getLinkConnectionPointDiwne();
    if(endPin)  end = endPin->getLinkConnectionPointDiwne();
    setLinkEndpointsDiwne(start, end);
}

void WorkspaceCoreLink::updateControlPointsOffsets(){
    float offset = (getEndpoint().x-getStartpoint().x)*I3T::getTheme().get(ESize::Links_ControlpointsPositionFraction);
    if (offset < I3T::getTheme().get(ESize::Links_ControlpointsPositionMin)) offset = I3T::getTheme().get(ESize::Links_ControlpointsPositionMin);
    setLinkControlpointsOffsetDiwne(ImVec2(offset,0), ImVec2(-offset,0));
}


bool WorkspaceCoreLink::processLinkHovered(DIWNE::Diwne &diwne, bool& inner_interaction_happen)
{
    if (bypassLinkHoveredAction(diwne))
    {
        m_color.Value.w = 1; /* \todo JH alpha to settings? */
        return true;
    }else
    {
        m_color.Value.w = 0.4; /* \todo JH alpha to settings? */
    }
    return false;
}

bool WorkspaceCoreLink::linkContent(DIWNE::Diwne &diwne)
{
    ImColor col = ImGui::ColorConvertFloat4ToU32(I3T::getTheme().get(WorkspacePinColorBackground[m_endPin->getType()])); /* \todo JH make precessBeforeLinkContetn function */
    m_color.Value.x = col.Value.x; m_color.Value.y = col.Value.y; m_color.Value.z = col.Value.z;
    if (m_selected)
    {
        m_selectedColor = col;
        m_selectedColor.Value.x += 20;
        m_selectedColor.Value.y += 20;
        m_selectedColor.Value.z += 20;
        m_selectedColor.Value.w = 0.8;
        diwne.AddBezierCurveDiwne(getStartpoint(), getStartControlPoint(), getEndControlPoint(), getEndpoint(), m_selectedColor, I3T::getTheme().get(ESize::Links_Thickness)+I3T::getTheme().get(ESize::Links_ThicknessSelectedBorder) );
    }

    diwne.AddBezierCurveDiwne(getStartpoint(), getStartControlPoint(), getEndControlPoint(), getEndpoint(), m_color, I3T::getTheme().get(ESize::Links_Thickness));
    return false;
}

WorkspaceNodeWithCoreDataWithPins::WorkspaceNodeWithCoreDataWithPins(Ptr<Core::NodeBase> nodebase, bool showDataOnPins/*=true*/)
 : WorkspaceNodeWithCoreData(nodebase)
    , m_showDataOnPins(showDataOnPins)
{
    const auto& inputPins   = m_nodebase->getInputPins();
    const auto& outputPins  = m_nodebase->getOutputPins();

    m_workspaceInputs.reserve(inputPins.size());
    m_workspaceOutputs.reserve(outputPins.size());

    for (Core::Pin const& pin : inputPins)
    {
        switch(pin.getType())
        {
            case EValueType::MatrixMul:
//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinMatrixMul>(    pin.getId()
//                                                                ,   pin
//                                                                ,   *this));
                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPin>(    pin.getId()
                                                                ,   pin
                                                                ,   *this));
            break;
//            case EValueType::Vec4:
//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinVector4>( pin.getId()
//                                                                                            ,   pin
//                                                                                            ,   *this));
//                break;
//            case EValueType::Vec3:
//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinVector3>( pin.getId()
//                                                                                            ,   pin
//                                                                                            ,   *this));
//                break;
//            case EValueType::Float:
//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinFloat>( pin.getId()
//                                                                                            ,   pin
//                                                                                            ,   *this));
//                break;
//            case EValueType::Quat:
//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinQuaternion>( pin.getId()
//                                                                                            ,   pin
//                                                                                            ,   *this));
//                break;
//            case EValueType::Pulse:
//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinPulse>( pin.getId()
//                                                                                            ,   pin
//                                                                                            ,   *this));
//                break;
//            case EValueType::MatrixMul:
//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinMatrixMul>( pin.getId()
//                                                                                            ,   pin
//                                                                                            ,   *this));
//                break;
//            case EValueType::Screen:
//                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPinScreen>( pin.getId()
//                                                                                            ,   pin
//                                                                                            ,   *this));
//                break;
//            case EValueType::Ptr:
//                /* Pin with type Ptr have no graphic representation */
//                break;
            default:
                m_workspaceInputs.push_back(std::make_unique<WorkspaceCoreInputPin>(    pin.getId()
                                                                ,   pin
                                                                ,   *this));
                //Debug::Assert(false , "Unknown Pin type while loading input pins from Core to Workspace");
        }
    }
    if (!m_showDataOnPins) /* for example sequence do not show data in output pins */
    {
        for (Core::Pin const& pin : outputPins)
        {
            m_workspaceOutputs.push_back(std::make_unique<WorkspaceCoreOutputPin>( pin.getId()
                                                                                            ,   pin
                                                                                            ,   *this));
        }
    }else
    {

			auto outs = nodebase->getOutputPins();
			auto outsBegin = outs.begin();
			auto outsEnd   = outs.end();

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
                    Debug::Assert(false , "Unknown Pin type while loading output pins from Core to Workspace");
            }

        }
    }

}

bool WorkspaceNodeWithCoreDataWithPins::processNodeOutsideOfWorkspace(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;
    for (auto const& pin : m_workspaceInputs) {
        if (pin->isConnected())
        {
            Ptr<WorkspaceCoreInputPin> in = std::dynamic_pointer_cast<WorkspaceCoreInputPin>(pin);
            if (in->getLink().isLinkOnWorkArea(diwne))
            {
                inner_interaction_happen |= in->getLink().drawLinkDiwne(diwne);
            }
        }
    }
    return inner_interaction_happen;
}

bool WorkspaceNodeWithCoreDataWithPins::leftContent(DIWNE::Diwne &diwne)
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

bool WorkspaceNodeWithCoreDataWithPins::rightContent(DIWNE::Diwne &diwne)
{
    bool inner_interaction_happen = false;

    for (auto const& pin : m_workspaceOutputs) {
        inner_interaction_happen |= pin->drawPinDiwne(diwne);
    }
    return inner_interaction_happen;
}

/* >>>>> STATIC FUNCTIONS <<<<< */
bool drawDragFloatWithMap_Inline(DIWNE::Diwne &diwne, int const numberOfVisibleDecimals, FloatPopupMode& floatPopupMode, std::string const label, float& value, int const mapValue, bool& valueChanged)
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


//	ImGuiStyle& style = ImGui::GetStyle();
//	style.Colors[ImGuiCol_Text] = I3T::getColor(EColor::Nodes_FloatText);
//	style.Colors[ImGuiCol_FrameBg] = I3T::getColor(EColor::FloatBg);
//	style.Colors[ImGuiCol_FrameBgHovered] = I3T::getColor(EColor::FloatBgHovered);
//	style.Colors[ImGuiCol_FrameBgActive] = I3T::getColor(EColor::FloatBgActive);


	// \todo JH is it done? make step a configurable constant - same or smaller than dragStep - other way drag is fired when step is not fired...
	valueChanged = ImGui::DragFloat(
			label.c_str(), &value, step, 0.0f, 0.0f, fmt::format("%.{}f", numberOfVisibleDecimals).c_str(),
			1.0f); /* if power >1.0f the number changes logarithmic */
    ImGui::SetItemAllowOverlap();

    if (diwne.bypassIsItemActive())
        inner_interaction_happen = true;

    if (!inactive)
	{
		inner_interaction_happen |= diwne.popupDiwneItem(label, &popupFloatContent, floatPopupMode, value, valueChangedByPopup);
		valueChanged |= valueChangedByPopup;
	}

	if (inactive)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

//	style.Colors[ImGuiCol_Text] = I3T::getColor(EColor::Text);

	/* \todo JH this is not very elegant ... */
	/* \todo JH hack for not change value if for example dragging node from position over DragFloat */
    if ( diwne.getDiwneAction() != DIWNE::DiwneAction::None || diwne.getPreviousFrameDiwneAction() != DIWNE::DiwneAction::None){valueChanged = false;}

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

void loadWorkspacePinsFromCorePins(WorkspaceNodeWithCoreData& workspaceNode, Core::Node::PinView coreInputPins, Core::Node::PinView coreOutputPins, std::vector<Ptr<WorkspaceCorePin>> & workspaceInputPins, std::vector<Ptr<WorkspaceCorePin>> & workspaceOutputPins)
{
/* when you create new pin type - add it to both, input and output part */

}
/* \todo JH to docs - valueOfChange will be set to data and than (possibly) changed by user interaction */
bool drawData4x4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::mat4& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, int& rowOfChange, int& columnOfChange, float& valueOfChange )
{
    bool inner_interaction_happen = false;
    bool actualValueChanged = false;
    float localData; /* user can change just one value at the moment */

    ImGui::PushItemWidth(dataWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    valueChanged = false;
    /* Drawing is row-wise */
    ImGui::BeginGroup();
    for (int rows = 0; rows < 4; rows++)
    {
      for (int columns = 0; columns < 4; columns++)
      {

        localData = data[columns][rows]; /* Data are column-wise */
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:r{}c{}", node_id, rows, columns),
                                                                localData, dataMap[columns * 4 + rows], actualValueChanged);
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
    ImGui::EndGroup();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

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

bool drawDataVec4(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec4& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, glm::vec4& valueOfChange)
{
//    const glm::vec4& coreData = m_nodebase->getData(index).getVec4();
//    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

    bool  actualValueChanged = false;
    bool inner_interaction_happen = false;

    ImGui::PushItemWidth(dataWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    valueChanged = false;
    ImGui::BeginGroup();
    for (int columns = 0; columns < 4; columns++)
    {
        valueOfChange[columns] = data[columns]; /* \todo JH copy whole data directly - not in for*/
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:{}", node_id, columns),
                                                                valueOfChange[columns], dataMap[columns], actualValueChanged);
        if(actualValueChanged) valueChanged = true;
    }
    ImGui::EndGroup();
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

bool drawDataVec3(DIWNE::Diwne &diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const glm::vec3& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, glm::vec3& valueOfChange)
{
    bool  actualValueChanged = false;
    bool inner_interaction_happen = false;

    ImGui::PushItemWidth(dataWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    valueChanged = false;
    ImGui::BeginGroup();
    for (int columns = 0; columns < 3; columns++)
    {
        valueOfChange[columns] = data[columns];
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:{}", node_id, columns),
                                                                valueOfChange[columns], dataMap[columns], actualValueChanged);;
        if(actualValueChanged) valueChanged = true;
    }
    ImGui::EndGroup();
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

bool drawDataFloat(DIWNE::Diwne &diwne, DIWNE::ID node_id, int numberOfVisibleDecimals, float dataWidth, FloatPopupMode& floatPopupMode, const float& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, float& valueOfChange)
{
    bool inner_interaction_happen = false;


    ImGui::PushItemWidth(dataWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

    valueChanged = false;
    valueOfChange = data;
    inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:_", node_id),
                                                                valueOfChange, dataMap[0], valueChanged);

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    return inner_interaction_happen;

}
int maxLenghtOfDataFloat(const float& data, int numberOfVisibleDecimal)
{
    return numberOfCharWithDecimalPoint( data, numberOfVisibleDecimal );
}

bool drawDataQuaternion(DIWNE::Diwne &diwne, DIWNE::ID const node_id, int const numberOfVisibleDecimals, float dataWidth, FloatPopupMode floatPopupMode, const glm::quat& data, const Core::Transform::DataMap& dataMap, bool& valueChanged, glm::quat& valueOfChange)
{
    bool inner_interaction_happen = false;
    bool actualValueChanged = false;

    ImGui::PushItemWidth(dataWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, I3T::getSize(ESizeVec2::Nodes_FloatPadding));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, I3T::getSize(ESizeVec2::Nodes_ItemsSpacing));

	valueChanged = false;
	ImGui::BeginGroup();
    for (int columns = 0; columns < 4; columns++)
    {
        valueOfChange[columns] = data[columns];
        inner_interaction_happen |= drawDragFloatWithMap_Inline(diwne, numberOfVisibleDecimals, floatPopupMode, fmt::format("##{}:{}", node_id, columns),
                                                                valueOfChange[columns], dataMap[columns], actualValueChanged);

        if (actualValueChanged) valueChanged = true;
        if (columns < 3) ImGui::SameLine();

    }
    ImGui::EndGroup();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

  return inner_interaction_happen;
}

int maxLenghtOfDataQuaternion(const glm::quat& data, int numberOfVisibleDecimal)
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
