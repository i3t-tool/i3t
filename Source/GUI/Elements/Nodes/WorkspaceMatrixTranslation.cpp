#include "WorkspaceMatrixTranslation.h"

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, WorkspaceMatrixTranslationArgs const& args, DIWNE::Diwne * diwne)
    : WorkspaceMatrix4x4(headerBackground, {.levelOfDetail=args.levelOfDetail, .headerLabel=args.headerLabel, .nodeLabel=args.nodeLabel, .nodebase=Core::Builder::createTransform<Core::Translation>() })
    , DIWNE::Node(diwne)
{}

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation(ImTextureID headerBackground, DIWNE::Diwne * diwne, std::string headerLabel, std::string nodeLabel)
    : WorkspaceMatrix4x4(headerBackground, Core::Builder::createTransform<Core::Translation>(), headerLabel, nodeLabel)
    , DIWNE::Node(diwne)
{}

void WorkspaceMatrixTranslation::drawDataSetValues(util::NodeBuilder& builder)
{
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
    drawDataSetValues_builder(builder,
                                {   "x",
                                    "y",
                                    "z" },
                                {   [this](){return m_nodebase->as<Core::Translation>()->getX();},
                                    [this](){return m_nodebase->as<Core::Translation>()->getY();},
                                    [this](){return m_nodebase->as<Core::Translation>()->getZ();} },
                                {   [this](float v){return m_nodebase->as<Core::Translation>()->setX(v);},
                                    [this](float v){return m_nodebase->as<Core::Translation>()->setY(v);},
                                    [this](float v){return m_nodebase->as<Core::Translation>()->setZ(v);} },
                                {   coreMap[3*4+0], /* \todo JH some better way how determine what element from DataMap should be used? */
                                    coreMap[3*4+1],
                                    coreMap[3*4+2] }
                            );
}

/* DIWNE functions */

bool WorkspaceMatrixTranslation::drawTop()
{
    bool interaction_happen = false;

    Theme& t = I3T::getTheme();
	m_color	 = t.getHeader();

    ImGui::GetWindowDrawList()->AddRectFilled(
          m_diwne->diwne2screen(m_topRectDiwne.Min)
        , m_diwne->diwne2screen(m_topRectDiwne.Max)
        , m_color, 5, ImDrawCornerFlags_Top);


	ImGui::Spring(
			0, I3T::getSize(ESize::Nodes_HeaderLabelIndent)); // 0 - spring will always have zero size - left align the header
	ImGui::TextUnformatted(m_headerLabel.c_str());
	ImGui::Spring(10);	 // 1 - power of the current spring = 1, use default spacing .x or .y
	if (!dataAreValid()) /* \todo JH function for check validity of data here */
	{
		/*ax::Widgets::Icon(ImVec2(20, 20), // \todo JH size based on header size
                            IconType::Square,
                            true,
                            ImColor(255,0,0),
                            ImColor(0,0,0));  //\todo JH not constant here...
																						 */
	}
	ImGui::Spring(0);

    return interaction_happen;
}

/* DIWNE ^ */
