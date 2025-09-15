#include "pch.h"
#include "CameraUI.h"

#include <Engine/CCamera.h>
#include "Inspector.h"
#include "CImGuiMgr.h"
#include <Engine/CRenderMgr.h>



CameraUI::CameraUI()
    : ComponentUI("CameraUI", COMPONENT_TYPE::CAMERA)
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::Render_Update()
{
    ComponentTitle("Camera");

    CCamera* pCamera = GetTargetObject()->Camera();
    ImGui::SameLine(350);
    if (ImGui::Button("X"))
    {
        DeleteComponentTask(pCamera);

        Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
        pInspector->DeleteComponentCheck(GetType());
    }

    ImGui::Text("Projection Type");
    ImGui::SameLine(120);

    // PROJ_TYPE
    const char* szProjType[2] = { "Perspective", "Orthographic" };
    int ProjType = pCamera->GetProjType();
    ImGui::Combo("##combo", &ProjType, szProjType, 2);
    pCamera->SetProjType((PROJ_TYPE)ProjType);

    // Far
    ImGui::Text("Far");
    ImGui::SameLine(120);
    float Far = pCamera->GetFar();
    ImGui::InputFloat("##Far", &Far);

    // Far �� Near(1) ���� �۰ų� ������ �ȵȴ�. �þ߹����� Near ���� Far �����̱� ����
    if (Far <= 1.f)
        Far = 1.1f;
    pCamera->SetFar(Far);

    // Width
    float Width = pCamera->GetWidth();
    float AspectRatio = pCamera->GetAspectRatio();
    float FOV = pCamera->GetFOV();

    ImGui::Text("Width");
    ImGui::SameLine(120);
    ImGui::InputFloat("##Width", &Width);
    pCamera->SetWidth(Width);

    ImGui::Text("AspectRatio");
    ImGui::SameLine(120);
    ImGui::DragFloat("##AspectRatio", &AspectRatio, 0.01f);
    pCamera->SetAspectRatio(AspectRatio);

    // ���� ī�޶� ���� �ɼ��� ���������̸� ��Ȱ��ȭ
    ImGui::BeginDisabled(ProjType == (int)PROJ_TYPE::ORTHOGRAPHIC);

    // �������� ����
    ImGui::Text("Field Of View");
    ImGui::SameLine(120);
    FOV = (FOV * 180.f) / XM_PI;
    ImGui::DragFloat("##FOV", &FOV, 0.1f);
    FOV = (FOV * XM_PI) / 180.f;
    pCamera->SetFOV(FOV);

    ImGui::EndDisabled();


    // ���� ī�޶� ���� �ɼ��� ���������̸� ��Ȱ��ȭ
    ImGui::BeginDisabled(ProjType == (int)PROJ_TYPE::PERSPECTIVE);

    // �������� ���� ����
    float Scale = pCamera->GetScale();

    ImGui::Text("Scale");
    ImGui::SameLine(120);
    FOV = (FOV * 180.f) / XM_PI;
    ImGui::DragFloat("##Scale", &Scale, 0.1f);
    FOV = (FOV * XM_PI) / 180.f;

    // �������� ������ 0 ���Ϸ� �������� �ȵȴ�.
    if (Scale <= 0.f)
        Scale = 0.1f;

    pCamera->SetScale(Scale);

    ImGui::EndDisabled();

    // ����    
    UINT        m_LayerCheck;   // ī�޶� ���� ���̾� ��Ʈ üũ
    int         m_Priority;     // ī�޶� �켱����, 0 : MainCamera, -1 : �̵��

    m_Priority = pCamera->GetCameraPriority();
    ImGui::Text("Priority");
    ImGui::SameLine(120);
    if (ImGui::InputInt("##Priority", &m_Priority))
    {
        m_Priority = max(-1, min(m_Priority, 31));

        pCamera->SetPriority(m_Priority);
    }



    ImGui::Text("LayerCheck");
    ImGui::SameLine(120);

    UINT layerFlags = pCamera->GetLayerCheck();


    for (int i = 0; i < 32; ++i)
    {
        if (i > 0 && i % 8 == 0)
        {
            ImGui::NewLine();
            ImGui::SameLine(120);
        }

        if (i % 8 != 0)
            ImGui::SameLine();

        char label[8];
        snprintf(label, sizeof(label), "##%d", i);

        bool layerChecked = (pCamera->GetLayerCheck() & (1 << i)) != 0;
        if (ImGui::Checkbox(label, &layerChecked))
        {
            pCamera->LayerCheck(i);
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(layerNames[i]);
        }
    }



    
}