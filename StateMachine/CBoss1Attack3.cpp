#include "pch.h"
#include "CBoss1Attack3.h"

#include <Engine/CSoundMgr.h>
#include <Scripts/CBoss1Script.h>

CBoss1Attack3::CBoss1Attack3()
    : m_Once(false)
    , m_BossDirRight(false)
    , m_JumpOnce(false)
{
}

CBoss1Attack3::~CBoss1Attack3()
{
}

void CBoss1Attack3::FinalTick()
{
    // Idx 9번째에 생성
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() < 9 && GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() >2)
    {
       if(m_BossDirRight)
           GetStateMachine()->RigidBody2D()->AddForce(Vec3(400.f, 0.f, 0.f));
       else
           GetStateMachine()->RigidBody2D()->AddForce(Vec3(-400.f, 0.f, 0.f));
    }
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 3 && m_JumpOnce)
    {
        GetStateMachine()->RigidBody2D()->Jump();
        CSoundMgr::GetInst()->PlayFX(L"BossJump", 0.4f);
        m_JumpOnce = false;
    }



    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 9 && m_Once)
    {
        GetStateMachine()->GetOwner()->GetChild()[6]->StateMachine()->ChangeState(L"CQuakeEffect");
        CSoundMgr::GetInst()->PlayFX(L"BossJumpDown", 0.6f);
        m_Once = false;
    }

    if (GetStateMachine()->RigidBody2D()->IsGround())
    {
        GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }

    if(GetStateMachine()->FlipbookPlayer()->IsFinish())
    {
        GetStateMachine()->ChangeState(L"CBoss1Idle");
    }



}

void CBoss1Attack3::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(2, 5.5, false);
    


    if (GetStateMachine()->Transform()->GetRelativeScale().x > 0)
        m_BossDirRight = true;
    else
        m_BossDirRight = false;

    m_JumpOnce = true;
    m_Once = true;

    CBoss1Script* pBoss = (CBoss1Script*)GetStateMachine()->GetOwner()->GetScripts()[0];
    pBoss->ResetAtkCount();
}

void CBoss1Attack3::Exit()
{

}