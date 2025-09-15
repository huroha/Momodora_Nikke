#include "pch.h"
#include "CBoss1Attack.h"

#include <Scripts/CBoss1Script.h>
#include <Engine/CLevelMgr.h>
#include <Scripts/CCameraScript.h>
#include <Engine/CSoundMgr.h>

CBoss1Attack::CBoss1Attack()
    : m_AtkObj(nullptr)
    , m_Camera(nullptr)
    , m_CreatePref(false)
    , m_BossDirRight(false)
    , m_GetAtkCount(0)
{
}

CBoss1Attack::~CBoss1Attack()
{
}

void CBoss1Attack::FinalTick()
{
    // 5번 sprite Idx임
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 5 && m_CreatePref)
    {
        CSoundMgr::GetInst()->PlayFX(L"BossRoar", 0.8f);
        CBoss1Script* pBoss = (CBoss1Script*)GetStateMachine()->GetOwner()->GetScripts()[0];
        if (m_BossDirRight)
            m_AtkObj->Collider2D()->SetOffset(Vec2(166.f, -50.f));
        else
            m_AtkObj->Collider2D()->SetOffset(Vec2(-166.f, -50.f));

        m_AtkObj->Collider2D()->Activate();
        pBoss->RoarAtkOn();
        pBoss->IncreaseAtkCount();
        m_GetAtkCount = pBoss->GetAttackCount();

        CCameraScript* pCamera = (CCameraScript*)m_Camera->GetScripts()[0];
        pCamera->SetCamShake(3.f, 20.f, 0.4f, 1.2f, 15.f);
        m_CreatePref = false;
    }


    
    // 브레스냐 로어냐로 갈림
    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
    {
        // 패턴 3번 하면 점공으로 하게끔해야겠다.
        if (m_GetAtkCount == 3)
            GetStateMachine()->ChangeState(L"CBoss1Attack3");
        else
        {
            if (m_Random != 0)
                GetStateMachine()->ChangeState(L"CBoss1Idle");
            else
                GetStateMachine()->ChangeState(L"CBoss1Idle2");
        }

    }

    

}

void CBoss1Attack::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(1, 13, true);
    // 3번이 Atk Collider
    m_AtkObj = GetStateMachine()->GetOwner()->GetChild()[3];
    m_CreatePref = true;

    if (GetStateMachine()->Transform()->GetRelativeScale().x > 0)
        m_BossDirRight = true;
    else
        m_BossDirRight = false;

    srand(time(nullptr));
    m_Random = rand() % 2;

    // 카메라 등록
    m_Camera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera2");

}

void CBoss1Attack::Exit()
{

    m_AtkObj->Collider2D()->Deactivate();
}
