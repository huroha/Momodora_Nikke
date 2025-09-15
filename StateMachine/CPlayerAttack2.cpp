#include "pch.h"
#include "CPlayerAttack2.h"
#include "Scripts/CPlayerScript.h"
#include <Engine/CSoundMgr.h>

CPlayerAttack2::CPlayerAttack2()
    : m_NextAttack(false)
    , m_PlayerDirRight(false)
{
}

CPlayerAttack2::~CPlayerAttack2()
{
}


void CPlayerAttack2::FinalTick()
{
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() < 2)
    {
        if (m_PlayerDirRight && !m_AttackSucces)
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(300.f, 0.f, 0.f));
        else if (!m_PlayerDirRight && !m_AttackSucces)
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(-300.f, 0.f, 0.f));
    }

    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() > 2)
    {
        if (KEY_TAP(KEY::A))
        {
            m_NextAttack = true;
        }
    }

    // ���߿����� ���� �����鼭 �����ϴ°� �ڿ�������
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




    // �б� ó�� -> �ʱ�� ���ư����ΰ� �޺��� �̾ ���ΰ�
    if (GetStateMachine()->FlipbookPlayer()->IsFinish() && !m_NextAttack)
        GetStateMachine()->ChangeState(L"CPlayerIdle");
    else if (GetStateMachine()->FlipbookPlayer()->IsFinish() && m_NextAttack)
        GetStateMachine()->ChangeState(L"CPlayerAttack3");


}

void CPlayerAttack2::Enter()
{
    // Ŀ��� �ʱ�ȭ
    m_NextAttack = false;

    // Flipbook ���
    GetStateMachine()->FlipbookPlayer()->Play(9, 16.f, false);


    // ���� �÷��̾ �ٶ󺸴� ���� üũ
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
    
    // �ٸ� Ű �Է� ����
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];
    sPlayerScript->GetInfo().IsRollnAttack = true;
    // collider Ȱ��ȭ
    vector<CGameObject*> Child = GetStateMachine()->GetOwner()->GetChild();
    Child[3]->Collider2D()->Activate();
    if (m_PlayerDirRight)
    {
        GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
        Child[3]->Collider2D()->SetOffset(Vec2(40.f, 0.f));
        if(GetStateMachine()->RigidBody2D()->IsGround())
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(100.f, 0.f, 0.f));
    }
    else
    {
        GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
        Child[3]->Collider2D()->SetOffset(Vec2(-40.f, 0.f));
        if (GetStateMachine()->RigidBody2D()->IsGround())
            GetStateMachine()->RigidBody2D()->AddForce(Vec3(-100.f, 0.f, 0.f));
    }
    CSoundMgr::GetInst()->PlayFX(L"PlayerAttack2", 0.6f);

}

void CPlayerAttack2::Exit()
{

    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];

    sPlayerScript->GetInfo().IsRollnAttack = false;

    // collider ��Ȱ��ȭ
    vector<CGameObject*> Child = GetStateMachine()->GetOwner()->GetChild();
    Child[3]->Collider2D()->Deactivate();

    m_AttackSucces = false;
}