#include "pch.h"
#include "CMonsterHit.h"

#include <Scripts/CMonster1Script.h>

CMonsterHit::CMonsterHit()
    : m_Monster1Script(nullptr)
    , m_HitTime(0.f)
    , m_HitStart(false)
{
}

CMonsterHit::~CMonsterHit()
{
}



void CMonsterHit::FinalTick()
{
    if (m_HitStart)
    {
        m_HitTime += DT;
    }

    if (m_HitTime > 0.5f)
    {
        GetStateMachine()->ChangeState(L"CMonsterIdle");
        m_HitTime = 0.f;
        m_HitStart = false;
    }

    if (m_Monster1Script->GetMonsterHP() <= 0)
    {
        GetStateMachine()->ChangeState(L"CMonster1Dead");
        vector<CGameObject*> m_Body = GetStateMachine()->GetOwner()->GetChild();
        m_Body[0]->Collider2D()->Deactivate();
    }

}

void CMonsterHit::Enter()
{
    // 떨림 에니메이션 적용
    GetStateMachine()->FlipbookPlayer()->Play(1, 40, true);

    m_HitStart = true;


    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    m_Monster1Script = (CMonster1Script*)vecScripts[0];
}

void CMonsterHit::Exit()
{
    m_HitStart = false;
    m_HitTime = 0.f;
}