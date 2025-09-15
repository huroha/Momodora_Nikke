#include "pch.h"
#include "CQuakeEffect.h"

#include <Engine/CLevelMgr.h>
#include <Scripts/CCameraScript.h>

#include <Engine/CSoundMgr.h>

CQuakeEffect::CQuakeEffect()
    : m_RepeatCount(0)
    , m_CycleStart(false)
    , m_EndCheck(false)
    , m_BossDirRight(false)
{
}

CQuakeEffect::~CQuakeEffect()
{
}


void CQuakeEffect::FinalTick()
{
    if (m_CycleStart && m_RepeatCount < 3)
    {
        if (GetStateMachine()->FlipbookPlayer()->IsFinish())
        {
            GetStateMachine()->FlipbookPlayer()->Play(0, 20.f, false);
            CSoundMgr::GetInst()->PlayFX(L"BossQuake", 0.3f);
            // 이동거리 셋팅
            float Dir = 72.f * (m_RepeatCount+1);
   
            if(m_BossDirRight)
                GetStateMachine()->Transform()->SetRelativePos(Vec3(168.f + Dir, -27.f, 0.f));
            else
                GetStateMachine()->Transform()->SetRelativePos(Vec3(-168.f - Dir, -27.f, 0.f));

            ++m_RepeatCount;
            if (m_RepeatCount == 2)
            {
                m_EndCheck = true;
                m_CycleStart = false;
            }

        }

    }

    if (GetStateMachine()->FlipbookPlayer()->IsFinish() && m_EndCheck)
    {
        GetStateMachine()->Collider2D()->Deactivate();
        GetStateMachine()->GetOwner()->GetRenderComponent()->SetActive(false);
        m_EndCheck = false;
        GetStateMachine()->ChangeState(L"CQuakeEffectNone");
    }

}

void CQuakeEffect::Enter()
{
    m_CycleStart = true;
    m_RepeatCount = 0;
    GetStateMachine()->FlipbookPlayer()->Play(0, 20.f, false);

    GetStateMachine()->GetOwner()->GetRenderComponent()->SetActive(true);
    GetStateMachine()->Collider2D()->Activate();

    if (GetStateMachine()->GetOwner()->GetParent()->Transform()->GetRelativeScale().x > 0)
        m_BossDirRight = true;
    else
        m_BossDirRight = false;


    if (m_BossDirRight)
        GetStateMachine()->Transform()->SetRelativePos(Vec3(168.f, -27.f, 0.f));
    else
        GetStateMachine()->Transform()->SetRelativePos(Vec3(-168.f, -27.f, 0.f));

    // 카메라 등록
    CGameObject* Camera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera2");
    CCameraScript* pCamera = (CCameraScript*)Camera->GetScripts()[0];
    pCamera->SetCamShake(4.f, 25.f, 1.2f, 1.6f, 15.f);

}

void CQuakeEffect::Exit()
{
    m_CycleStart = false; 
    m_RepeatCount = 0;



}

