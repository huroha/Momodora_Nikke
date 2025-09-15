#include "pch.h"
#include "CPlayerLand.h"

#include <Engine/CSoundMgr.h>
#include <Scripts/CPlayerScript.h>
CPlayerLand::CPlayerLand()
{
}

CPlayerLand::~CPlayerLand()
{
}


void CPlayerLand::FinalTick()
{
    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
    {
        GetStateMachine()->ChangeState(L"CPlayerIdle");
    }

    // 긴급 구르기
    if (KEY_TAP(KEY::X))
    {
        CPlayerScript* pPlayer = (CPlayerScript*)GetStateMachine()->GetOwner()->GetScripts()[0];
        float SlopeAngle = pPlayer->GetSlopeAngle();
        if (SlopeAngle == 0)
            GetStateMachine()->ChangeState(L"CPlayerRoll");
    }
}

void CPlayerLand::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(4, 12.f, false);
    CSoundMgr::GetInst()->PlayFX(L"PlayerLand", 0.3f);

    CPlayerScript* pScript = (CPlayerScript*)GetStateMachine()->GetOwner()->GetScripts()[0];
    pScript->GetInfo().JumpCount = 0;
}

void CPlayerLand::Exit()
{
}
