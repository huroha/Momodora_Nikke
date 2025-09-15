#include "pch.h"
#include "CMonster2Hit.h"

#include <Scripts/CMonster2Script.h>

CMonster2Hit::CMonster2Hit()
    : m_Monster2Script(nullptr)
    , m_HitTime(0.f)
    , m_HitStart(false)
{
}

CMonster2Hit::~CMonster2Hit()
{
}


void CMonster2Hit::FinalTick()
{
    if (m_HitStart)
    {
        m_HitTime += DT;
    }

    if (m_HitTime > 0.5f)
    {
        GetStateMachine()->ChangeState(L"CMonster2Idle");
        m_HitTime = 0.f;
        m_HitStart = false;
    }

    if (m_Monster2Script->GetMonsterHP() <= 0)
    {
        GetStateMachine()->ChangeState(L"CMonster2Dead");
        vector<CGameObject*> m_Body = GetStateMachine()->GetOwner()->GetChild();
        m_Body[0]->Collider2D()->Deactivate();
        m_Body[1]->Collider2D()->Deactivate();
    }
}

void CMonster2Hit::Enter()
{
    // 떨림 에니메이션 적용
    GetStateMachine()->FlipbookPlayer()->Play(2, 40, true);

    m_HitStart = true;

    GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    m_Monster2Script = (CMonster2Script*)vecScripts[0];

    GetStateMachine()->RigidBody2D()->UseGravity(false);
}

void CMonster2Hit::Exit()
{
    GetStateMachine()->RigidBody2D()->UseGravity(true);
    m_HitStart = false;
    m_HitTime = 0.f;
}