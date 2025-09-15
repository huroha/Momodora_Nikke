#include "pch.h"
#include "CMonster2Dead.h"

#include <Scripts/CMonster2Script.h>

#include <Scripts/CCameraScript.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CDataMgr.h>


CMonster2Dead::CMonster2Dead()
    : m_DeadStart(false)
    , m_PaperBurnInst(0.f)
    , m_IsDead(false)
{
}

CMonster2Dead::~CMonster2Dead()
{
}


void CMonster2Dead::FinalTick()
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

void CMonster2Dead::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(3, 1, false);
    m_DeadStart = true;
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CMonster2Script* Script = (CMonster2Script*)vecScripts[0];
    Script->SetDead();

    // 이거 MainCamera2 가 있는 경우도 있어야 되는데?
    CGameObject* pCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
    if (pCamera == nullptr)
        pCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera2");
    CCameraScript* pCameraScript = (CCameraScript*)pCamera->GetScripts()[0];
    pCameraScript->SetCamShake(0.4f, 15.f, 0.3f, 0.f, 15.f);

    CDataMgr::GetInst()->SlayCountUp();
}

void CMonster2Dead::Exit()
{
    m_DeadStart = false;
}

