#include "Pin.h"


namespace DIWNE
{

Pin::Pin()
{
    //ctor
}

Pin::~Pin()
{
    //dtor
}

bool Pin::drawPinDiwne()
{
    bool interaction_happen = false;
    bool inner_interaction_happen = false;

    ImGui::BeginGroup();
        inner_interaction_happen |= drawPin();
    ImGui::EndGroup();

//    m_pinRect.Min = m_diwne->screen2diwne( ImGui::GetItemRectMin() );
//    m_pinRect.Max = m_diwne->screen2diwne( ImGui::GetItemRectMax() );

    interaction_happen = ImGui::IsItemHovered();

    if (!inner_interaction_happen && interaction_happen)
    {
            ImGui::Text("Pin is hoovered");
    }

    return inner_interaction_happen || interaction_happen;
}

bool Pin::drawPin()
{
    bool interaction_happen = false;

    // \todo JH ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0),  const ImVec2& uv1 = ImVec2(1,1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0,0,0,0), const ImVec4& tint_col = ImVec4(1,1,1,1));
    if(ImGui::Button("Input1", ImVec2(50,50)))
    {
        interaction_happen = true;
        ImGui::Text("Clicked");
    }
    return interaction_happen;
}

} /* namespace DIWNE */
