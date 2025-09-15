#include "pch.h"
#include "AddComponentUI.h"

#include <Engine/CComponent.h>

#include "ComponentUI.h"
#include <Engine/components.h>
#include "CImGuiMgr.h"
#include "Inspector.h"

#include <Scripts/CScriptMgr.h>
#include "ListUI.h"

#include <Engine/CScript.h>

AddComponentUI::AddComponentUI()
    : EditorUI("##AddComponentUI")
    , m_TargetObject(nullptr)
    , m_ComponentIdx(0)
{

}

AddComponentUI::~AddComponentUI()
{
}





void AddComponentUI::CheckComponent()
{
    m_vComponent.clear();
    for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        CComponent* pComponent = m_TargetObject->GetComponent((COMPONENT_TYPE)i);
        if(pComponent == nullptr)
            m_vComponent.push_back(i);
    }


    m_StringStorage.clear();
    m_ComponentPointers.clear();
    for (int c = 0; c < m_vComponent.size(); ++c)
    {
        COMPONENT_TYPE eCom = (COMPONENT_TYPE)m_vComponent[c];
        switch (eCom)
        {
        case COMPONENT_TYPE::COLLIDER2D:
        {
            m_StringStorage.push_back("Collider2D");
        }
            break;
        case COMPONENT_TYPE::COLLIDER3D:
        {
            m_StringStorage.push_back("Collider3D");
        }
            break;
        case COMPONENT_TYPE::FLIPBOOKPLAYER:
        {
            m_StringStorage.push_back("FlipbookPlayer");
        }
            break;
        case COMPONENT_TYPE::ANIMATOR3D:
        {
            m_StringStorage.push_back("Animator3D");
        }
            break;
        case COMPONENT_TYPE::CAMERA:
        {
            m_StringStorage.push_back("Camera");
        }
            break;
        case COMPONENT_TYPE::LIGHT2D:
        {
            m_StringStorage.push_back("Light2D");
        }
            break;
        case COMPONENT_TYPE::LIGHT3D:
        {
            m_StringStorage.push_back("Light3D");
        }
            break;
        case COMPONENT_TYPE::STATEMACINE:
        {
            m_StringStorage.push_back("StateMachine");
        }
            break;
        case COMPONENT_TYPE::MESHRENDER:
        {
            m_StringStorage.push_back("MeshRender");
        }
            break;
        case COMPONENT_TYPE::TILEMAP:
        {
            m_StringStorage.push_back("TileMap");
        }
            break;
        case COMPONENT_TYPE::PARTICLE_SYSTEM:
        {
            m_StringStorage.push_back("ParticleSystem");
        }
            break;
        case COMPONENT_TYPE::SKYBOX:
        {
            m_StringStorage.push_back("SkyBox");
        }
            break;
        case COMPONENT_TYPE::DECAL:
        {
            m_StringStorage.push_back("Decal");
        }
            break;
        case COMPONENT_TYPE::LANDSCALE:
        {
            m_StringStorage.push_back("LandScale");
        }
            break;
        case COMPONENT_TYPE::RIGIDBODY2D:
        {
            m_StringStorage.push_back("RigidBody2D");
        }
            break;
        }
    }
    m_ComponentPointers.reserve(m_StringStorage.size()); // �޸� ���Ҵ� ����
    for (const auto& str : m_StringStorage)
    {
        m_ComponentPointers.push_back(str.c_str());
    }
   
}

void AddComponentUI::Render_Update()
{
    if (m_TargetObject == nullptr)
        return;


    if (ImGui::Button("Add"))
    {
        COMPONENT_TYPE eComType = (COMPONENT_TYPE)m_vComponent[m_ComponentIdx];
        switch (eComType)
        {
        case COMPONENT_TYPE::COLLIDER2D:
        {
            m_TargetObject->AddComponent(new CCollider2D);
        }
            break;
        case COMPONENT_TYPE::COLLIDER3D:
            break;
        case COMPONENT_TYPE::FLIPBOOKPLAYER:
        {
            m_TargetObject->AddComponent(new CFlipbookPlayer);
        }
            break;
        case COMPONENT_TYPE::ANIMATOR3D:
            break;
        case COMPONENT_TYPE::CAMERA:
        {
            m_TargetObject->AddComponent(new CCamera);
        }
            break;
        case COMPONENT_TYPE::LIGHT2D:
        {
            m_TargetObject->AddComponent(new CLight2D);
        }
            break;
        case COMPONENT_TYPE::LIGHT3D:
            break;
        case COMPONENT_TYPE::STATEMACINE:
        {
            m_TargetObject->AddComponent(new CStateMachine);
        }
            break;
        case COMPONENT_TYPE::MESHRENDER:
        {
            if (nullptr != m_TargetObject->GetRenderComponent())
            {
                m_TargetObject->DeleteComponent(COMPONENT_TYPE::TILEMAP);
                m_TargetObject->DeleteComponent(COMPONENT_TYPE::PARTICLE_SYSTEM);
            }
            m_TargetObject->AddComponent(new CMeshRender);
        }
            break;
        case COMPONENT_TYPE::TILEMAP:
        {
            if (nullptr != m_TargetObject->GetRenderComponent())
            {
                m_TargetObject->DeleteComponent(COMPONENT_TYPE::MESHRENDER);
                m_TargetObject->DeleteComponent(COMPONENT_TYPE::PARTICLE_SYSTEM);
            }

            m_TargetObject->AddComponent(new CTileMap);
        }
            break;
        case COMPONENT_TYPE::PARTICLE_SYSTEM:
        {
            if (nullptr != m_TargetObject->GetRenderComponent())
            {
                m_TargetObject->DeleteComponent(COMPONENT_TYPE::MESHRENDER);
                m_TargetObject->DeleteComponent(COMPONENT_TYPE::TILEMAP);
            }
            m_TargetObject->AddComponent(new CParticleSystem);
        }
            break;
        case COMPONENT_TYPE::SKYBOX:
            break;
        case COMPONENT_TYPE::DECAL:
            break;
        case COMPONENT_TYPE::LANDSCALE:
            break;
        case COMPONENT_TYPE::RIGIDBODY2D:
        {
            m_TargetObject->AddComponent(new CRigidBody2D);
        }
            break;
        }

        
        Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
        pInspector->ReLoad();

    }


    ImGui::SameLine(100);
    ImGui::Combo("##ComponentCombo", &m_ComponentIdx,
        m_ComponentPointers.data(), (int)m_ComponentPointers.size());

    if(ImGui::Button("AddScript"))
    {
        // ListUI �� Ȱ��ȭ ��Ű��
        ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
        pListUI->SetName("Choose Script");
        pListUI->SetActive(true);

        // ListUI �� �־��� ���ڿ� ���� ��������
        pListUI->AddItem("None");

        vector<wstring> vecScriptNames;

        CScriptMgr::GetScriptInfo(vecScriptNames);

        for (int i = 0; i < vecScriptNames.size(); ++i)
        {
            pListUI->AddItem(vecScriptNames[i]);
        }
        // ���� Ŭ�� �� ȣ���ų �Լ� ���
        pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&AddComponentUI::AddScript);

    }

    ImGui::SameLine(100);
    if (ImGui::Button("Begin Script"))
    {
        vector<CScript*> vecScript= m_TargetObject->GetScripts();
        for (int i = 0; i < vecScript.size(); ++i)
        {
            vecScript[i]->Begin();
        }
    }

    //if (ImGui::Button("DeleteScript"))
    //{
    //    m_TargetObject->DeleteScript();
    //}
}

void AddComponentUI::AddScript(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{

    // ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
    ListUI* pListUI = (ListUI*)_ListUI;
    string* pStr = (string*)_SelectString;

    if (*pStr == "None")
        return;

    // �ش� Script�̸��� ã�Ƽ�, ���� Ÿ�� ������Ʈ�� ��ũ��Ʈ �߰�

    wstring ScriptName = wstring(pStr->begin(), pStr->end());

    m_TargetObject->AddComponent(CScriptMgr::GetScript(ScriptName));

}