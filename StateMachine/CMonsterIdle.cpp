#include "pch.h"
#include "CMonsterIdle.h"



CMonsterIdle::CMonsterIdle()
    : m_IsRight(true)
    , m_Start(false)
    , m_CoolTime(0.f)
{
}

CMonsterIdle::~CMonsterIdle()
{
}


void CMonsterIdle::FinalTick()
{
    if (m_vecChild[1] != nullptr && m_CoolTime > 0.4f)
    {
         int OverlapCount = m_vecChild[1]->Collider2D()->GetOverlapCount();

         if (OverlapCount > 0)
         {
             GetStateMachine()->ChangeState(L"CMonsterRun");
         }
    }

    if (m_Start)
        m_CoolTime += DT;
}

void CMonsterIdle::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(0, 6.f, true);
    m_vecChild = GetStateMachine()->GetOwner()->GetChild();
    if (GetStateMachine()->Transform()->GetRelativeScale().x > 0)
        m_IsRight = true;
    else
        m_IsRight = false;

    if (m_IsRight)
    {
        m_vecChild[1]->Collider2D()->SetOffset(Vec2(250.f, 0.f));
        m_vecChild[2]->Collider2D()->SetOffset(Vec2(30.f, 0.f));
    }
    else
    {
        m_vecChild[1]->Collider2D()->SetOffset(Vec2(-250.f, 0.f));
        m_vecChild[2]->Collider2D()->SetOffset(Vec2(-30.f, 0.f));
    }

    m_Start = true;
}

void CMonsterIdle::Exit()
{

    m_Start = false;
    m_CoolTime = 0.f;
}
