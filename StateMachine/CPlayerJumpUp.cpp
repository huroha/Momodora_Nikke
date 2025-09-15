#include "pch.h"
#include "CPlayerJumpUp.h"


#include <Engine/CSoundMgr.h>

CPlayerJumpUp::CPlayerJumpUp()
{
}

CPlayerJumpUp::~CPlayerJumpUp()
{
}


void CPlayerJumpUp::FinalTick()
{
    if (GetStateMachine()->RigidBody2D()->GetGravityVelocity().y < 0)
    {
        GetStateMachine()->ChangeState(L"CPlayerJumpDown");
    }

    if (KEY_PRESSED(KEY::LEFT))
    {
        GetStateMachine()->RigidBody2D()->AddForce(Vec3(-600.f, 0.f, 0.f));
    }
    if (KEY_PRESSED(KEY::RIGHT))
    {
        GetStateMachine()->RigidBody2D()->AddForce(Vec3(600.f, 0.f, 0.f));
    }
    if (GetStateMachine()->RigidBody2D()->GetVelocity().x > 0)
        GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
    else if (GetStateMachine()->RigidBody2D()->GetVelocity().x < 0)
        GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);

}

void CPlayerJumpUp::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(6, 10.f, false);
    CSoundMgr::GetInst()->PlayFX(L"PlayerJump", 0.2f);
}

void CPlayerJumpUp::Exit()
{

}