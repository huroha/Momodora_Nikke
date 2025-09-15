#include "pch.h"
#include "CPlayerBrake.h"

#include <Scripts/CPlayerScript.h>

CPlayerBrake::CPlayerBrake()
    : m_SlopeAngle(0.f)
{
}

CPlayerBrake::~CPlayerBrake()
{
}


void CPlayerBrake::FinalTick()
{
    GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
        GetStateMachine()->ChangeState(L"CPlayerIdle");


    // 양쪽 방향키를 누르고 있다가 때면 반응
    if (KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT))
    {
        GetStateMachine()->ChangeState(L"CPlayerRun");
    }

    // 긴급 구르기
    if (KEY_TAP(KEY::X))
    {
        CPlayerScript* pPlayer = (CPlayerScript*)GetStateMachine()->GetOwner()->GetScripts()[0];
        m_SlopeAngle = pPlayer->GetSlopeAngle();
        if(m_SlopeAngle == 0)
            GetStateMachine()->ChangeState(L"CPlayerRoll");
    }
}

void CPlayerBrake::Enter()
{
    // 플레이어 바라보는 방향 보정
    GetStateMachine()->FlipbookPlayer()->Play(1, 12.f, false);
    if (KEY_PRESSED(KEY::LEFT))
    {
        GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
    }
    else if (KEY_PRESSED(KEY::RIGHT))
    {
        GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
    }
}

void CPlayerBrake::Exit()
{
}