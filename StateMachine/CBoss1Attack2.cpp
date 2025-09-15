#include "pch.h"
#include "CBoss1Attack2.h"

#include <Scripts/CCameraScript.h>
#include <Engine/CLevelMgr.h>
#include <Scripts/CBoss1Script.h>
#include <Engine/CSoundMgr.h>

CBoss1Attack2::CBoss1Attack2()
    : m_Bress(nullptr)
    , m_Camera(nullptr)
    , m_BossDirRight(false)
    , m_Start(false)
    , m_IdleChange(false)
    , m_Once(false)
    , m_Time(0.f)
    , m_Random(0)
    , m_GetAtkCount(0)
{
}


CBoss1Attack2::~CBoss1Attack2()
{
}


void CBoss1Attack2::FinalTick()
{
    // Attck 의 5번 sprite Idx만 적용
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 5 && !m_Once && !m_IdleChange)
    {
        m_Bress->GetRenderComponent()->SetActive(true);
        m_Bress->Collider2D()->Activate();

        CBoss1Script* pBoss = (CBoss1Script*)GetStateMachine()->GetOwner()->GetScripts()[0];
        pBoss->IsBressOn();
        CCameraScript* pCamera = (CCameraScript*)m_Camera->GetScripts()[0];
        pCamera->SetCamShake(3.f, 20.f, 0.4f, 1.2f, 15.f);
        pBoss->IncreaseAtkCount();
        m_GetAtkCount = pBoss->GetAttackCount();
        CSoundMgr::GetInst()->PlayFX(L"BossBress", 1.f);
        m_Once = true;
    }

    // Start 와 Attack 5번이어야 힘을 받기 시작
    if (m_Start && m_Once)
    {
        m_Time += DT;
        if (m_BossDirRight)
            m_Bress->RigidBody2D()->AddForce(Vec3(450.f, 0.f, 0.f));
        else
            m_Bress->RigidBody2D()->AddForce(Vec3(-450.f, 0.f, 0.f));

    }
    // 모션끝나고 대기상태로 전환모션이 필요함
    if (GetStateMachine()->FlipbookPlayer()->IsFinish() && !m_IdleChange)
    {
        GetStateMachine()->FlipbookPlayer()->Play(0, 8, true);
        m_IdleChange = true;
    }

    // State 변경
    if (m_Time > 1.5f && m_Start)
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
        m_Start = false;
    }

    
}

void CBoss1Attack2::Enter()
{
    // Flipbook Animation 진행
    GetStateMachine()->FlipbookPlayer()->Play(1, 13, false);

    // 보스가 보는 방향 체크
    if (GetStateMachine()->GetOwner()->Transform()->GetRelativeScale().x > 0)
        m_BossDirRight = true;
    else
        m_BossDirRight = false;

    m_Start = true;
    

    // 브레스 생성해야함.
    m_Bress = CLevelMgr::GetInst()->FindObjectByName(L"Bress");
    m_Bress->RigidBody2D()->SetMaxSpeed(1000.f);
    m_Bress->RigidBody2D()->UseGravity(false);
    m_Bress->RigidBody2D()->SetVelocity(Vec3(0.f,0.f,0.f));

    Vec3 BossPoss = GetStateMachine()->Transform()->GetRelativePos();
    // 위치 설정
    if(m_BossDirRight)
        m_Bress->Transform()->SetRelativePos(Vec3(BossPoss.x + 180.f, BossPoss.y -50.f,-100.f));
    else
        m_Bress->Transform()->SetRelativePos(Vec3(BossPoss.x - 180.f, BossPoss.y - 50.f, -100.f));
    
    srand(time(nullptr));
    m_Random = rand() % 2;

    // 카메라 등록
    m_Camera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera2");

}

void CBoss1Attack2::Exit()
{

    m_Start = false;
    m_Time = 0.f;
    m_IdleChange = false;
    m_Once = false;
}