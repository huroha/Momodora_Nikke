#include "pch.h"
#include "CMonster2Attack.h"
#include <Engine/CSoundMgr.h>

CMonster2Attack::CMonster2Attack()
    : m_DirRight(false)
    , m_Jump(false)
{
}

CMonster2Attack::~CMonster2Attack()
{
}


void CMonster2Attack::FinalTick()
{
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() > 5)
    {
        if (GetStateMachine()->FlipbookPlayer()->IsFinish() && GetStateMachine()->RigidBody2D()->IsGround())
            GetStateMachine()->ChangeState(L"CMonster2Idle");
    }
    else
    {
        if(m_DirRight)
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(750.f, 0.f, 0.f));
        else
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(-750.f, 0.f, 0.f));
    }
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 3)
        if (m_Jump)
        {
            GetStateMachine()->RigidBody2D()->Jump();
            CSoundMgr::GetInst()->PlayFX(L"Monster2Jump", 0.4f);
            m_Jump = false;
        }
}

void CMonster2Attack::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(1, 12, false);
    if (GetStateMachine()->Transform()->GetRelativeScale().x > 0)
        m_DirRight = true;
    else
        m_DirRight = false;
    m_Jump = true;
}

void CMonster2Attack::Exit()
{
}

