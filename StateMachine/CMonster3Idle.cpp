#include "pch.h"
#include "CMonster3Idle.h"



CMonster3Idle::CMonster3Idle()
    : m_IsRight(true)
    , m_Start(false)
    , m_CoolTime(0.f)
{
}

CMonster3Idle::~CMonster3Idle()
{
}


void CMonster3Idle::FinalTick()
{
    if (m_vecChild[1] != nullptr && m_CoolTime > 1.5f)
    {
        if (m_vecChild[2]->StateMachine()->GetCurStateName() != L"CBombBoom")
        {
            int OverlapCount = m_vecChild[1]->Collider2D()->GetOverlapCount();

            if (OverlapCount > 0)
            {
                GetStateMachine()->ChangeState(L"CMonster3Attack");
            }
        }

    }

    if (m_Start)
        m_CoolTime += DT;
}

void CMonster3Idle::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(0, 6.f, false);
    m_vecChild = GetStateMachine()->GetOwner()->GetChild();
    if (GetStateMachine()->Transform()->GetRelativeScale().x > 0)
        m_IsRight = true;
    else
        m_IsRight = false;

    if (m_IsRight)
    {
        m_vecChild[1]->Collider2D()->SetOffset(Vec2(180.f, 0.f));
    }
    else
    {
        m_vecChild[1]->Collider2D()->SetOffset(Vec2(-180.f, 0.f));
    }

    m_Start = true;
}

void CMonster3Idle::Exit()
{
    m_Start = false;
    m_CoolTime = 0.f;
}
