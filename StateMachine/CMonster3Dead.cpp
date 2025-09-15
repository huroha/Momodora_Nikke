#include "pch.h"
#include "CMonster3Dead.h"
#include <Scripts/CCameraScript.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CDataMgr.h>

CMonster3Dead::CMonster3Dead()
    : m_DeadStart(false)
    , m_PaperBurnInst(0.f)
    , m_IsDead(false)
{
}

CMonster3Dead::~CMonster3Dead()
{
}

void CMonster3Dead::FinalTick()
{
    if (m_DeadStart)
    {
        m_PaperBurnInst += DT * 0.8f;

        if (GetStateMachine()->GetOwner()->GetRenderComponent())
        {
            Ptr<CMaterial> pMtrl = GetStateMachine()->GetOwner()->GetRenderComponent()->GetDynamicMaterial();

            if (nullptr != pMtrl)
            {
                pMtrl->SetScalarParam(FLOAT_0, m_PaperBurnInst);
            }
        }
    }

    if (m_PaperBurnInst > 1.4f && !m_IsDead)
    {
        DestroyObject(GetStateMachine()->GetOwner());
        m_IsDead = true;
    }
}

void CMonster3Dead::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(3, 1, false);

    // Monster가 들고있는 collider의 비활성화 처리가 필요함.
    vector<CGameObject*> Child = GetStateMachine()->GetOwner()->GetChild();
    Child[0]->Collider2D()->Deactivate();
    Child[2]->Collider2D()->Deactivate();
    m_DeadStart = true;

    
    // 이거 MainCamera2 가 있는 경우도 있어야 되는데?
    CGameObject* pCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
    if (pCamera == nullptr)
        pCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera2");
    CCameraScript* pCameraScript = (CCameraScript*)pCamera->GetScripts()[0];
    pCameraScript->SetCamShake(0.4f, 15.f, 0.3f, 0.f, 15.f);


    CDataMgr::GetInst()->SlayCountUp();
}

void CMonster3Dead::Exit()
{
    m_DeadStart = false;
}
