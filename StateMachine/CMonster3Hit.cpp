#include "CMonster3Hit.h"
#include "pch.h"
#include "CMonster3Hit.h"
#include <Scripts/CMonster3Script.h>

CMonster3Hit::CMonster3Hit()
    : m_Monster3Script(nullptr)
    , m_HitTime(0.f)
    , m_HitStart(false)
{
}

CMonster3Hit::~CMonster3Hit()
{
}


void CMonster3Hit::FinalTick()
{
    if (m_HitStart)
    {
        m_HitTime += DT;
    }

    if (m_HitTime > 0.5f)
    {
        GetStateMachine()->ChangeState(L"CMonster3Idle");
        m_HitTime = 0.f;
        m_HitStart = false;
    }

    if (m_Monster3Script->GetMonsterHP() <= 0)
    {
        GetStateMachine()->ChangeState(L"CMonster3Dead");
        vector<CGameObject*> m_Body = GetStateMachine()->GetOwner()->GetChild();
        m_Body[0]->Collider2D()->Deactivate();
    }
}

void CMonster3Hit::Enter()
{
    // 떨림 에니메이션 적용
    GetStateMachine()->FlipbookPlayer()->Play(2, 40, true);

    m_HitStart = true;


    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    m_Monster3Script = (CMonster3Script*)vecScripts[0];
}

void CMonster3Hit::Exit()
{
    m_HitTime = 0.f;
    m_HitStart = false;
}