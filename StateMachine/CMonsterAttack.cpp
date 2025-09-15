#include "pch.h"
#include "CMonsterAttack.h"

#include <Scripts/CMonster1Script.h>

#include <Engine/CSoundMgr.h>
CMonsterAttack::CMonsterAttack()
    : m_IsRight(true)
{
}

CMonsterAttack::~CMonsterAttack()
{
}

void CMonsterAttack::FinalTick()
{
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 4)
    {
        if (!(m_vecChild[3]->Collider2D()->IsActive()))
        {
            m_vecChild[3]->Collider2D()->Activate();
            CSoundMgr::GetInst()->PlayFX(L"Monster1Attack", 0.4f);
        }
    }
    else if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 7)
    {

        if (m_vecChild[3]->Collider2D()->IsActive())
        {
            m_vecChild[3]->Collider2D()->Deactivate();
        }
    }



    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
        GetStateMachine()->ChangeState(L"CMonsterIdle");
}

void CMonsterAttack::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(3, 12, false);
    m_vecChild = GetStateMachine()->GetOwner()->GetChild();

    if (GetStateMachine()->Transform()->GetRelativeScale().x > 0)
        m_IsRight = true;
    else
        m_IsRight = false;

    if (m_IsRight)
    {
        m_vecChild[1]->Collider2D()->SetOffset(Vec2(250.f, 0.f));
        m_vecChild[2]->Collider2D()->SetOffset(Vec2(30.f, 0.f));
        m_vecChild[3]->Collider2D()->SetOffset(Vec2(42.f, 0.f));
    }
    else
    {
        m_vecChild[1]->Collider2D()->SetOffset(Vec2(-250.f, 0.f));
        m_vecChild[2]->Collider2D()->SetOffset(Vec2(-30.f, 0.f));
        m_vecChild[3]->Collider2D()->SetOffset(Vec2(-42.f, 0.f));
    }

}

void CMonsterAttack::Exit()
{
    GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
}
