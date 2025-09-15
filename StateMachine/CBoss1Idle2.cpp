#include "pch.h"
#include "CBoss1Idle2.h"




CBoss1Idle2::CBoss1Idle2()
    : m_IdleStart(false)
    , m_AtkCoolTime(0.f)
{
}

CBoss1Idle2::~CBoss1Idle2()
{
}


void CBoss1Idle2::FinalTick()
{
    if (m_IdleStart)
        m_AtkCoolTime += DT;

    // Idle에서는 Roar로 넘어감.
    if (m_AtkCoolTime >2.f)
    {
        GetStateMachine()->ChangeState(L"CBoss1Attack2");
    }

}

void CBoss1Idle2::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(0, 8, true);
    m_IdleStart = true;
}

void CBoss1Idle2::Exit()
{
    m_IdleStart = false;
    m_AtkCoolTime = 0.f;
}