#include "pch.h"
#include "CMonsterRun.h"



CMonsterRun::CMonsterRun()
{
}

CMonsterRun::~CMonsterRun()
{
}


void CMonsterRun::FinalTick()
{
    if (m_vecChild[1] != nullptr)
    {
        int OverlapCount = m_vecChild[1]->Collider2D()->GetOverlapCount();
        float Dir = GetStateMachine()->Transform()->GetRelativeScale().x;

        // 추적
        if (OverlapCount  == 0)
        {
            GetStateMachine()->ChangeState(L"CMonsterIdle");
        }
        else if(OverlapCount > 0)
        {
            // 오른쪽으로 이동
            if (Dir > 0)
                GetStateMachine()->RigidBody2D()->AddForce(Vec3(200.f, 0.f, 0.f));
            else
                GetStateMachine()->RigidBody2D()->AddForce(Vec3(-200.f, 0.f, 0.f));
        }
        // 공격 가능범위에 들어오면
        if (m_vecChild[2]->Collider2D()->GetOverlapCount() > 0)
        {
            GetStateMachine()->ChangeState(L"CMonsterAttack");
        }

    }


}

void CMonsterRun::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(2, 12, true);
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
}

void CMonsterRun::Exit()
{
    GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
}