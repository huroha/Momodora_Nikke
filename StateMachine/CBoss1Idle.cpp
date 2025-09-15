#include "pch.h"
#include "CBoss1Idle.h"



CBoss1Idle::CBoss1Idle()
    : m_IdleStart(false)
    , m_AtkCoolTime(0.f)
{
}

CBoss1Idle::~CBoss1Idle()
{
}

void CBoss1Idle::FinalTick()
{
    if (m_IdleStart)
        m_AtkCoolTime += DT;

    // Idle에서는 Roar로 넘어감.
    if (m_AtkCoolTime > 1.5f)
    {
        GetStateMachine()->ChangeState(L"CBoss1Attack");
    }



}

void CBoss1Idle::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(0, 8, true);
    m_IdleStart = true;
}

void CBoss1Idle::Exit()
{
    m_IdleStart = false;
    m_AtkCoolTime = 0.f;
}
