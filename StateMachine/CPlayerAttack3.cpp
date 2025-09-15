#include "pch.h"
#include "CPlayerAttack3.h"
#include <Scripts/CPlayerScript.h>
#include <Engine/CSoundMgr.h>

CPlayerAttack3::CPlayerAttack3()
    : m_NextAttack(false)
    , m_PlayerDirRight(false)
{
}

CPlayerAttack3::~CPlayerAttack3()
{
}


void CPlayerAttack3::FinalTick()
{

    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() > 4)
    {
        if (KEY_TAP(KEY::A))
        {
            m_NextAttack = true;
        }
    }

    // 공중에서는 힘을 받으면서 공격하는게 자연스러움
    if (!GetStateMachine()->RigidBody2D()->IsGround())
    {
        if (KEY_PRESSED(KEY::LEFT))
        {
            if (!m_PlayerDirRight)
                GetStateMachine()->RigidBody2D()->AddForce(Vec3(-400.f, 0.f, 0.f));
        }
        if (KEY_PRESSED(KEY::RIGHT))
        {
            if (m_PlayerDirRight)
                GetStateMachine()->RigidBody2D()->AddForce(Vec3(400.f, 0.f, 0.f));
        }

    }

    // 막타만  collider 나중에 나오는걸로 바꿈
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 3)
    {
        if (!(m_Child[4]->Collider2D()->IsActive()) && !m_IsBust)
        {
            m_Child[4]->Collider2D()->Activate();
            // collider 활성화
            if (m_PlayerDirRight)
            {
                GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
                m_Child[4]->Collider2D()->SetOffset(Vec2(50.f, 0.f));
                m_Child[4]->Collider2D()->SetScale(Vec2(114.f, 76.f));

                if (GetStateMachine()->RigidBody2D()->IsGround())
                    GetStateMachine()->RigidBody2D()->AddForce(Vec3(600.f, 0.f, 0.f));
            }
            else
            {


                GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
                m_Child[4]->Collider2D()->SetOffset(Vec2(-50.f, 0.f));
                m_Child[4]->Collider2D()->SetScale(Vec2(114.f, 76.f));
                if (GetStateMachine()->RigidBody2D()->IsGround())
                    GetStateMachine()->RigidBody2D()->AddForce(Vec3(-600.f, 0.f, 0.f));
            }
        }
       
    }





    // 분기 처리 -> 초기로 돌아갈것인가 콤보를 이어갈 것인가
    if (GetStateMachine()->FlipbookPlayer()->IsFinish() && !m_NextAttack)
        GetStateMachine()->ChangeState(L"CPlayerIdle");
    else if (GetStateMachine()->FlipbookPlayer()->IsFinish() && m_NextAttack)
        GetStateMachine()->ChangeState(L"CPlayerAttack1");
}

void CPlayerAttack3::Enter()
{
    // 커멘드 초기화
    m_NextAttack = false;



    // 플레이어 바라보는 방향 보정
    GetStateMachine()->FlipbookPlayer()->Play(10, 16.f, false);

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

    m_Child = GetStateMachine()->GetOwner()->GetChild();

    // 현재 버스트 상태인지 구분
    m_IsBust = sPlayerScript->GetInfo().IsBust;
    if (m_IsBust)
    {
        m_Child[4]->Collider2D()->Activate();
        m_Child[4]->Collider2D()->SetOffset(Vec2(0.f, 300.f));
        m_Child[4]->Collider2D()->SetScale(Vec2(1450.f, 600.f));

        CSoundMgr::GetInst()->PlayFX(L"BustSlash", 0.6f);
    }
    else
    {
        CSoundMgr::GetInst()->PlayFX(L"MonsterHit", 0.6f);
    }
    



}

void CPlayerAttack3::Exit()
{
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];
    sPlayerScript->GetInfo().IsRollnAttack = false;
    // collider 비활성화
    m_Child[4]->Collider2D()->Deactivate();


}
