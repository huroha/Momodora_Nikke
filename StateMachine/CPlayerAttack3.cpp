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

    // ��Ÿ��  collider ���߿� �����°ɷ� �ٲ�
    if (GetStateMachine()->FlipbookPlayer()->GetSpriteIdx() == 3)
    {
        if (!(m_Child[4]->Collider2D()->IsActive()) && !m_IsBust)
        {
            m_Child[4]->Collider2D()->Activate();
            // collider Ȱ��ȭ
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





    // �б� ó�� -> �ʱ�� ���ư����ΰ� �޺��� �̾ ���ΰ�
    if (GetStateMachine()->FlipbookPlayer()->IsFinish() && !m_NextAttack)
        GetStateMachine()->ChangeState(L"CPlayerIdle");
    else if (GetStateMachine()->FlipbookPlayer()->IsFinish() && m_NextAttack)
        GetStateMachine()->ChangeState(L"CPlayerAttack1");
}

void CPlayerAttack3::Enter()
{
    // Ŀ��� �ʱ�ȭ
    m_NextAttack = false;



    // �÷��̾� �ٶ󺸴� ���� ����
    GetStateMachine()->FlipbookPlayer()->Play(10, 16.f, false);

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

    m_Child = GetStateMachine()->GetOwner()->GetChild();

    // ���� ����Ʈ �������� ����
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
    // collider ��Ȱ��ȭ
    m_Child[4]->Collider2D()->Deactivate();


}
