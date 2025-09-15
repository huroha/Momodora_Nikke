#include "pch.h"
#include "CBoss1Dead.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>
#include <Engine/CDataMgr.h>

CBoss1Dead::CBoss1Dead()
    : m_DeadStart(false)
    , m_IsDead(false)
    , m_PaperBurnInst(0.f)
{
}

CBoss1Dead::~CBoss1Dead()
{
}

void CBoss1Dead::FinalTick()
{

    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
    {
        m_DeadStart = true;
    }


    if (m_DeadStart)
    {
        m_PaperBurnInst += DT * 0.3f;

        if (GetStateMachine()->GetOwner()->GetRenderComponent())
        {
            Ptr<CMaterial> pMtrl = GetStateMachine()->GetOwner()->GetRenderComponent()->GetDynamicMaterial();


            if (nullptr != pMtrl)
            {
                pMtrl->SetScalarParam(FLOAT_0, m_PaperBurnInst);
            }
        }
    }

    if (m_PaperBurnInst > 1.5f && !m_IsDead)
    {
        DestroyObject(GetStateMachine()->GetOwner());
        DestroyObject(m_BossBar);
        DestroyObject(m_BossName);
        m_IsDead = true;
    }
}

void CBoss1Dead::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(3, 4, false);
   // 0번과 4번이 몸체
    CGameObject* pBody1 = GetStateMachine()->GetOwner()->GetChild()[0];
    CGameObject* pBody2 = GetStateMachine()->GetOwner()->GetChild()[4];

    pBody1->Collider2D()->Deactivate();
    pBody2->Collider2D()->Deactivate();

    m_BossBar = CLevelMgr::GetInst()->FindObjectByName(L"Boos_Bar");
    m_BossName = CLevelMgr::GetInst()->FindObjectByName(L"Boss_Name");

    CGameObject* pWall =CLevelMgr::GetInst()->FindObjectByName(L"Wall_Particle1");
    CGameObject* pWall2 =CLevelMgr::GetInst()->FindObjectByName(L"Wall_R");
    if (pWall2 != nullptr)
        pWall2->Collider2D()->Deactivate();
    if (pWall != nullptr)
        DestroyObject(pWall);
    // SlayCount 증가
    CDataMgr::GetInst()->SlayCountUp();
    CSoundMgr::GetInst()->PlayFX(L"BossDead", 0.7f);

}

void CBoss1Dead::Exit()
{
}
