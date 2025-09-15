#include "pch.h"
#include "CPlayerHitState.h"

#include "Scripts/CPlayerScript.h"
#include "Scripts/CPlayerHit.h"

CPlayerHitState::CPlayerHitState()
    : m_HitDirRight(false)
    , m_StartHit(false)
{
}

CPlayerHitState::~CPlayerHitState()
{
}



void CPlayerHitState::FinalTick()
{
    if (!(GetStateMachine()->FlipbookPlayer()->IsFinish()) && m_StartHit)
    {
        if (m_HitDirRight)
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(-300.f, 0.f, 0.f));
        else
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(300.f, 0.f, 0.f));
    }
    else
    {
        GetStateMachine()->ChangeState(L"CPlayerIdle");
    }

}


void CPlayerHitState::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(11, 40.f, true);
    
    
    // �ٸ� Ű �Է� ����
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];
    sPlayerScript->GetInfo().IsRollnAttack = true;

    const vector<CScript*>& vecScripts2 = (GetStateMachine()->GetOwner()->GetChild()[1])->GetScripts();
    CPlayerHit* sPlayerHit = (CPlayerHit*)vecScripts2[0];

     bool chekDir = sPlayerHit->GetHitDir();
    // �÷��̾� ���� üũ
  
    if (chekDir)
        m_HitDirRight = true;
    else
        m_HitDirRight = false;

    m_StartHit = true;
    GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
}



void CPlayerHitState::Exit()
{
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];

    sPlayerScript->GetInfo().IsRollnAttack = false;

}