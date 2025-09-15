#include "pch.h"
#include "CPlayerAttack1.h"
#include "Scripts/CPlayerScript.h"
#include <Engine/CSoundMgr.h>


CPlayerAttack1::CPlayerAttack1()
    : m_NextAttack(false)
    , m_PlayerDirRight(false)
    , m_AttackSucces(false)
{
}

CPlayerAttack1::~CPlayerAttack1()
{
}


void CPlayerAttack1::FinalTick()
{
    
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() < 2)
    {
        if (m_PlayerDirRight && !m_AttackSucces)
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(300.f, 0.f, 0.f));
        else if(!m_PlayerDirRight && !m_AttackSucces)
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(-300.f, 0.f, 0.f));
    }

    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() > 2)
    {
        if (KEY_TAP(KEY::A))
            m_NextAttack = true;
    }

    // 공중에서는 힘을 받으면서 공격하는게 자연스러움
    if (!GetStateMachine()->RigidBody2D()->IsGround())
    {
        if (KEY_PRESSED(KEY::LEFT))
        {
            if(!m_PlayerDirRight)
                GetStateMachine()->RigidBody2D()->AddForce(Vec3(-400.f, 0.f, 0.f));
        }
        if (KEY_PRESSED(KEY::RIGHT))
        {
            if (m_PlayerDirRight)
                GetStateMachine()->RigidBody2D()->AddForce(Vec3(400.f, 0.f, 0.f));
        }

    }


    // 분기 처리 -> 초기로 돌아갈것인가 콤보를 이어갈 것인가
    if (GetStateMachine()->FlipbookPlayer()->IsFinish() && !m_NextAttack)
        GetStateMachine()->ChangeState(L"CPlayerIdle");
    else if(GetStateMachine()->FlipbookPlayer()->IsFinish() && m_NextAttack)
        GetStateMachine()->ChangeState(L"CPlayerAttack2");

}

void CPlayerAttack1::Enter()
{
    // 커멘드 초기화
    m_NextAttack = false;

    // Flipbook plays
    GetStateMachine()->FlipbookPlayer()->Play(8, 16.f, false);

    // 현재 플레이어가 바라보는 방향 체크
    if (KEY_PRESSED(KEY::LEFT))
        m_PlayerDirRight = false;
    else if (KEY_PRESSED(KEY::RIGHT))
        m_PlayerDirRight = true;
    else
    {
        int checkpDir = GetStateMachine()->Transform()->GetRelativeScale().x;
        if (checkpDir > 0)
            m_PlayerDirRight = true;
        else
            m_PlayerDirRight = false;
    }

    


    // 다른 키 입력 방지
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];
    sPlayerScript->GetInfo().IsRollnAttack = true;
    // collider 활성화
    vector<CGameObject*> Child = GetStateMachine()->GetOwner()->GetChild();
    Child[2]->Collider2D()->Activate();
    if (m_PlayerDirRight)
    {
        GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
        Child[2]->Collider2D()->SetOffset(Vec2(40.f, 0.f));
        if (GetStateMachine()->RigidBody2D()->IsGround())
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(600.f, 0.f, 0.f));
    }
    else
    {
        GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
        Child[2]->Collider2D()->SetOffset(Vec2(-40.f, 0.f));
        if (GetStateMachine()->RigidBody2D()->IsGround())
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(-600.f, 0.f, 0.f));
    }
    CSoundMgr::GetInst()->PlayFX(L"PlayerAttack1", 0.6f);

}

void CPlayerAttack1::Exit()
{

    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];
    sPlayerScript->GetInfo().IsRollnAttack = false;
    // collider 비활성화
    vector<CGameObject*> Child = GetStateMachine()->GetOwner()->GetChild();
    Child[2]->Collider2D()->Deactivate();

    m_AttackSucces = false;
}