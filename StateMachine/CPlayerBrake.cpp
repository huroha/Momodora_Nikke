#include "pch.h"
#include "CPlayerBrake.h"

#include <Scripts/CPlayerScript.h>

CPlayerBrake::CPlayerBrake()
    : m_SlopeAngle(0.f)
{
}

CPlayerBrake::~CPlayerBrake()
{
}


void CPlayerBrake::FinalTick()
{
    GetStateMachine()->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
        GetStateMachine()->ChangeState(L"CPlayerIdle");


    // ���� ����Ű�� ������ �ִٰ� ���� ����
    if (KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT))
    {
        GetStateMachine()->ChangeState(L"CPlayerRun");
    }

    // ��� ������
    if (KEY_TAP(KEY::X))
    {
        CPlayerScript* pPlayer = (CPlayerScript*)GetStateMachine()->GetOwner()->GetScripts()[0];
        m_SlopeAngle = pPlayer->GetSlopeAngle();
        if(m_SlopeAngle == 0)
            GetStateMachine()->ChangeState(L"CPlayerRoll");
    }
}

void CPlayerBrake::Enter()
{
    // �÷��̾� �ٶ󺸴� ���� ����
    GetStateMachine()->FlipbookPlayer()->Play(1, 12.f, false);
    if (KEY_PRESSED(KEY::LEFT))
    {
        GetStateMachine()->Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
    }
    else if (KEY_PRESSED(KEY::RIGHT))
    {
        GetStateMachine()->Transform()->SetRelativeScale(230.f, 230.f, 1.f);
    }
}

void CPlayerBrake::Exit()
{
}