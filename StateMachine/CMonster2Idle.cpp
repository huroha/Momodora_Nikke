#include "pch.h"
#include "CMonster2Idle.h"


#include "Scripts/CMonster2Script.h"

CMonster2Idle::CMonster2Idle()
{
}

CMonster2Idle::~CMonster2Idle()
{
}


void CMonster2Idle::FinalTick()
{
    if(GetStateMachine()->RigidBody2D()->IsGround())
        GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));

    if (m_Monster2Script->GetTrace())
    {
        m_Time += DT;
    }
    if (m_Time > 2.f)
        m_Monster2Script->SetTrace(false);
}

void CMonster2Idle::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(0, 8.f, true);
    //GetStateMachine()->RigidBody2D()->AddForce(Vec3(0.f, 0.f, 0.f));

    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    m_Monster2Script = (CMonster2Script*)vecScripts[0];
}

void CMonster2Idle::Exit()
{
}
