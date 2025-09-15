#include "pch.h"
#include "ComponentUI.h"

ComponentUI::ComponentUI(const string& _ID, COMPONENT_TYPE _Type)
    : EditorUI(_ID)
    , m_TargetObject(nullptr)
    , m_Type(_Type)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::SetTargetObject(CGameObject* _Target)
{
    m_TargetObject = _Target;

    if (nullptr == m_TargetObject)
    {
        SetActive(false);
    }
    else
    {
        CComponent* pComponent = m_TargetObject->GetComponent(m_Type);
        if (nullptr == pComponent)
        {
            SetActive(false);
        }
        else
        {
            SetActive(true);
        }
    }
}

void ComponentUI::ComponentTitle(const string& _title, Vec3 _Color)
{
    ImGui::PushID(0);

    float r = _Color.x;
    float g = _Color.y;
    float b = _Color.z;

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(r, g, b));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(r, g, b));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(r, g, b));

    ImGui::Button(_title.c_str());

    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void ComponentUI::ComponentSubtitle(const string& _title, Vec3 _Color)
{
    ImGui::PushID(0);


    float r = _Color.x;
    float g = _Color.y;
    float b = _Color.z;

    ImGui::TextColored(ImVec4(r, g, b, 1.0f), _title.c_str());

    ImGui::PopID();

}
