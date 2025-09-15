#include "pch.h"
#include "CPlayerIdletoRun.h"
#include <Scripts/CPlayerScript.h>
#include <Engine/CKeyMgr.h>
#include <Engine/define.h>

CPlayerIdletoRun::CPlayerIdletoRun()
    : m_SlopeAngle(0.f)
{
}

CPlayerIdletoRun::~CPlayerIdletoRun()
{
}


void CPlayerIdletoRun::FinalTick()
{
    // Player 움직임 체크해서 Run으로 넘어갈지 or Idle로 넘어갈지 분기 체크
    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
    {
        if (GetStateMachine()->RigidBody2D()->IsGround() && (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT)))
        {
            GetStateMachine()->ChangeState(L"CPlayerRun");
        }
        else
        {
            GetStateMachine()->ChangeState(L"CPlayerIdle");
        }
    }

    CPlayerScript* pPlayer = (CPlayerScript*)GetStateMachine()->GetOwner()->GetScripts()[0];
    m_SlopeAngle = pPlayer->GetSlopeAngle();
    // 경사면을 따라 이동하는 힘 계산
    Vec3 moveForce = {};
    if (KEY_PRESSED(KEY::LEFT))
    {
        // 경사면을 따라 이동하는 벡터 계산
        moveForce.x = -600.f;
        moveForce.y = -100.f * sin(m_SlopeAngle);
        if(GetStateMachine()->RigidBody2D()->GetVelocity().x > 0)
            GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f,0.f,0.f));
        GetStateMachine()->RigidBody2D()->SetVelocitySlope(moveForce.y);
        GetStateMachine()->RigidBody2D()->AddForce(Vec3(moveForce.x, 0.f, 0.f));

    }
    if (KEY_PRESSED(KEY::RIGHT))
    {
        // 경사면을 따라 이동하는 벡터 계산
        moveForce.x = 600.f;
        moveForce.y = 100.f * sin(m_SlopeAngle);
        // 방향을 틀었을때
        if (GetStateMachine()->RigidBody2D()->GetVelocity().x < 0)
            GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        GetStateMachine()->RigidBody2D()->SetVelocitySlope(moveForce.y);
        GetStateMachine()->RigidBody2D()->AddForce(Vec3(moveForce.x, 0.f, 0.f));

    }

    //if (KEY_PRESSED(KEY::LEFT))
    //{
    //    GetStateMachine()->RigidBody2D()->AddForce(Vec3(-600.f, 0.f, 0.f));
    //}
    //if (KEY_PRESSED(KEY::RIGHT))
    //{
    //    GetStateMachine()->RigidBody2D()->AddForce(Vec3(600.f, 0.f, 0.f));
    //}



    // 긴급 구르기
    if (KEY_TAP(KEY::X) && m_SlopeAngle == 0)
    {
        GetStateMachine()->ChangeState(L"CPlayerRoll");
    }
}

void CPlayerIdletoRun::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(2, 10.f, false);
    if (GetStateMachine()->RigidBody2D()->GetVelocity().x > 0)
    {
        GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
    }
    else if (GetStateMachine()->RigidBody2D()->GetVelocity().x < 0)
    {
        GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
    }
}

void CPlayerIdletoRun::Exit()
{
}