#include "pch.h"
#include "CBoomMove.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>

CBoomMove::CBoomMove()
    : m_RotateStart(false)
    , m_IsRight(false)
    , m_Power(0)
{
}

CBoomMove::~CBoomMove()
{
}


void CBoomMove::FinalTick()
{
    if (m_RotateStart)
    {
        Vec3 vOriginRot = GetStateMachine()->Transform()->GetRelativeRotation();

        // 완만한 포물선 움직임을 위함.
        if (m_IsRight && GetStateMachine()->RigidBody2D()->GetGravityVelocity().y > 0)
        {
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(50.f * m_Power, 0.f, 0.f));
            GetStateMachine()->Transform()->SetRelativeRotation(Vec3(vOriginRot.x , vOriginRot.y, vOriginRot.z+DT * 2));
        }
        else if (!m_IsRight && GetStateMachine()->RigidBody2D()->GetGravityVelocity().y > 0)
        {
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(-50.f * m_Power, 0.f, 0.f));
            GetStateMachine()->Transform()->SetRelativeRotation(Vec3(vOriginRot.x, vOriginRot.y, vOriginRot.z - DT * 2));
        }
    }

    // Collider 가 하나라도 충돌한다면 이펙트 터져야함.
    if (GetStateMachine()->Collider2D()->GetOverlapCount())
    {
        GetStateMachine()->ChangeState(L"CBombBoom");
    }

}

void CBoomMove::Enter()
{

    GetStateMachine()->MeshRender()->SetActive(true);
    GetStateMachine()->RigidBody2D()->UseGravity(true);
    GetStateMachine()->RigidBody2D()->SetMaxSpeed(300.f);
    GetStateMachine()->RigidBody2D()->SetGravityAccelScale(500.f);
    m_RotateStart = true;

    float Dir = GetStateMachine()->GetOwner()->GetParent()->Transform()->GetRelativeScale().x;
    if (Dir > 0)
    {
        m_IsRight = true;
        GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(100.f, 0.f, 0.f));
    }
    else
    {
        m_IsRight = false;
        GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(-100.f, 0.f, 0.f));
    }
    CSoundMgr::GetInst()->PlayFX(L"BombThrow", 0.3f);

    // 폭탄 이동 속도 조절.
    CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
    float pDir = pPlayer->Transform()->GetRelativePos().x;
    float bDir = GetStateMachine()->GetOwner()->GetParent()->Transform()->GetRelativePos().x;
    float result = abs(pDir - bDir);
    if (result < 200)
        m_Power = 1;
    else if (result > 200 && result < 350)
        m_Power = 2;
    else
        m_Power = 3;

    GetStateMachine()->RigidBody2D()->JellyJump(m_Power*100 + 100.f);
}

void CBoomMove::Exit()
{
    GetStateMachine()->RigidBody2D()->UseGravity(false);
    GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    m_Power = 0;
}
