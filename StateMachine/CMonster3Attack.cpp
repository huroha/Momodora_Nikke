#include "pch.h"
#include "CMonster3Attack.h"



CMonster3Attack::CMonster3Attack()
    : m_IsRight(false)
{
}

CMonster3Attack::~CMonster3Attack()
{
}


void CMonster3Attack::FinalTick()
{
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 5)
    {
        // Boom 던지는 타이밍
        if(m_vecChild[2]->StateMachine())
            m_vecChild[2]->StateMachine()->ChangeState(L"CBoomMove");
    }

    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
        GetStateMachine()->ChangeState(L"CMonster3Idle");
}

void CMonster3Attack::Enter()
{
    // Boom 쪽 State도 활성화 해야함.
    GetStateMachine()->FlipbookPlayer()->Play(1, 12, false);
    m_vecChild = GetStateMachine()->GetOwner()->GetChild();

}

void CMonster3Attack::Exit()
{
   
}
