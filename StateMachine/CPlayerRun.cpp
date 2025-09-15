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
    // �޸��ٰ� �ϰ��ϸ� JumpDown���� �ٲ��� ��.
    if (!GetStateMachine()->RigidBody2D()->IsGround())
        GetStateMachine()->ChangeState(L"CPlayerJumpDown");

    
    // ���� �ٸ� ����Ű�� �������ִٰ� ������ ���� �׿� ���� �÷��̾� ������ �ٲپ� �־�� ��.
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
    // ������ ���� �̵��ϴ� �� ���
    Vec3 moveForce = {};
    if (KEY_PRESSED(KEY::LEFT))
    {
        // ������ ���� �̵��ϴ� ���� ���
        moveForce.x = -600.f;
        moveForce.y = -300.f * sin(m_SlopeAngle);
        GetStateMachine()->RigidBody2D()->SetVelocitySlope(moveForce.y);
        GetStateMachine()->RigidBody2D()->AddForce(Vec3(moveForce.x,0.f,0.f));

    }
    if (KEY_PRESSED(KEY::RIGHT))
    {
        // ������ ���� �̵��ϴ� ���� ���
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


    // ��� ������
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
