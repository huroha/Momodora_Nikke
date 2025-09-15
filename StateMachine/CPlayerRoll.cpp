#include "pch.h"
#include "CPlayerRoll.h"
#include <Scripts/CScriptMgr.h>
#include <Scripts/CPlayerScript.h>

#include <Engine/CSoundMgr.h>
enum class INPUT_TYPE
{
    ATTACK,
    DODGE,
};


CPlayerRoll::CPlayerRoll()
    : m_PlayerDirRight(false)
{
}

CPlayerRoll::~CPlayerRoll()
{
}



void CPlayerRoll::FinalTick()
{
    CPlayerScript* pPlayer = (CPlayerScript*)GetStateMachine()->GetOwner()->GetScripts()[0];
    m_SlopeAngle = pPlayer->GetSlopeAngle();
    // 경사면을 따라 이동하는 힘 계산
    Vec3 moveForce = {};

    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() < 6)
    {
        if (m_PlayerDirRight)
        {
            if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() < 2)
            {
                GetStateMachine()->RigidBody2D()->AddForce(Vec3(4000.f, 0.f, 0.f));
                moveForce.y = 850.f * sin(m_SlopeAngle);
                GetStateMachine()->RigidBody2D()->SetVelocitySlope(moveForce.y);
            }

        }
        else
        {
            if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() < 2)
            {
                GetStateMachine()->RigidBody2D()->AddForce(Vec3(-4000.f, 0.f, 0.f));
                moveForce.y = -850.f * sin(m_SlopeAngle);
                GetStateMachine()->RigidBody2D()->SetVelocitySlope(moveForce.y);
            }

        }

    }
    else if(GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() > 6)
    {

        if (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
        {
            GetStateMachine()->ChangeState(L"CPlayerIdletoRun");
        }
    }
    if (m_SlopeAngle == 0)
    {
        GetStateMachine()->RigidBody2D()->SetVelocitySlope(0.f);
    }
    else if (m_SlopeAngle != 0 && (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() >= 2 && GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() < 6))
    {
        if (m_PlayerDirRight)
        {
            moveForce.y = 250.f * sin(m_SlopeAngle);
            GetStateMachine()->RigidBody2D()->SetVelocitySlope(moveForce.y);
        }
        else
        {
            moveForce.y = -250.f * sin(m_SlopeAngle);
            GetStateMachine()->RigidBody2D()->SetVelocitySlope(moveForce.y);
        }
    }

    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
        GetStateMachine()->ChangeState(L"CPlayerIdle");
}

void CPlayerRoll::Enter()
{

    // 몸체 콜라이더 Off
    vector<CGameObject*> Child = GetStateMachine()->GetOwner()->GetChild();
    Child[1]->Collider2D()->Deactivate();

    
    // 현재 플레이어가 바라보는 방향 체크
    int checkpDir = GetStateMachine()->Transform()->GetRelativeScale().x;
    if (checkpDir > 0)
        m_PlayerDirRight = true;
    else
        m_PlayerDirRight = false;

    
    GetStateMachine()->RigidBody2D()->SetMaxSpeed(650.f);


    GetStateMachine()->FlipbookPlayer()->Play(7, 13.f, false);

    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];

    sPlayerScript->GetInfo().IsRollnAttack = true;

    CSoundMgr::GetInst()->PlayFX(L"PlayerDodge", 0.6f);

}

void CPlayerRoll::Exit()
{
    GetStateMachine()->RigidBody2D()->SetMaxSpeed(300.f);

    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];

    sPlayerScript->GetInfo().IsRollnAttack = false;

    vector<CGameObject*> Child = GetStateMachine()->GetOwner()->GetChild();
    Child[1]->Collider2D()->Activate();

}
