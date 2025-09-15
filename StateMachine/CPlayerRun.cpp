#include "pch.h"
#include "CPlayerRun.h"
#include <Scripts/CPlayerScript.h>

CPlayerRun::CPlayerRun()
    : m_SlopeAngle(0.f)
{
}

CPlayerRun::~CPlayerRun()
{
}


void CPlayerRun::FinalTick()
{
    // 달리다가 하강하면 JumpDown으로 바뀌어야 함.
    if (!GetStateMachine()->RigidBody2D()->IsGround())
        GetStateMachine()->ChangeState(L"CPlayerJumpDown");

    
    // 서로 다른 방향키를 누르고있다가 한쪽을 때면 그에 따라 플레이어 방향을 바꾸어 주어야 함.
    if(GetStateMachine()->RigidBody2D()->GetVelocity().x > 0)
    {
        GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
    }
    else if (GetStateMachine()->RigidBody2D()->GetVelocity().x < 0)
    {
        GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
    }
    else if (GetStateMachine()->RigidBody2D()->GetVelocity().x == 0)
    {
        if ((KEY_PRESSED(KEY::LEFT)) || (KEY_PRESSED(KEY::RIGHT)))
            GetStateMachine()->ChangeState(L"CPlayerIdle");
        else if(GetStateMachine()->RigidBody2D()->IsGround())
            GetStateMachine()->ChangeState(L"CPlayerBrake");
    }

    CPlayerScript* pPlayer = (CPlayerScript*)GetStateMachine()->GetOwner()->GetScripts()[0];
    m_SlopeAngle = pPlayer->GetSlopeAngle();
    // 경사면을 따라 이동하는 힘 계산
    Vec3 moveForce = {};
    if (KEY_PRESSED(KEY::LEFT))
    {
        // 경사면을 따라 이동하는 벡터 계산
        moveForce.x = -600.f;
        moveForce.y = -300.f * sin(m_SlopeAngle);
        GetStateMachine()->RigidBody2D()->SetVelocitySlope(moveForce.y);
        GetStateMachine()->RigidBody2D()->AddForce(Vec3(moveForce.x,0.f,0.f));

    }
    if (KEY_PRESSED(KEY::RIGHT))
    {
        // 경사면을 따라 이동하는 벡터 계산
        moveForce.x = 600.f;
        moveForce.y = 300.f * sin(m_SlopeAngle);
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

void CPlayerRun::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(3, 10.f, true);
    if (KEY_PRESSED(KEY::LEFT))
    {
        GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
    }
    else if (KEY_PRESSED(KEY::RIGHT))
    {
        GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
    }

}

void CPlayerRun::Exit()
{
}
