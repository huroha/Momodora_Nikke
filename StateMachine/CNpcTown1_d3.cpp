#include "pch.h"
#include "CNpcTown1_d3.h"
#include <Engine/CFontMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CNpcTownDialog.h>

CNpcTown1_d3::CNpcTown1_d3()
{
}

CNpcTown1_d3::~CNpcTown1_d3()
{
}


void CNpcTown1_d3::FinalTick()
{
    if (CFontMgr::GetInst()->IsComplete(m_Dialog3) && (KEY_TAP(KEY::F) || KEY_TAP(KEY::A)))
    {
        GetStateMachine()->ChangeState(L"CNpcTown1_d1");
    }
}

void CNpcTown1_d3::Enter()
{
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CNpcTownDialog* sDialogScript = (CNpcTownDialog*)vecScripts[0];
    assert(sDialogScript);

    m_MsgBox = sDialogScript->GetMsgBox();
    CSoundMgr::GetInst()->PlayFX(L"Talking", 0.2f);

    m_Dialog3 = sDialogScript->GetDialog3();
    CFontMgr::GetInst()->SetFadeActive(m_Dialog3, true);
}

void CNpcTown1_d3::Exit()
{
    CFontMgr::GetInst()->SetFadeActive(m_Dialog3, false);
    m_MsgBox->GetRenderComponent()->SetActive(false);

    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CNpcTownDialog* sDialogScript = (CNpcTownDialog*)vecScripts[0];
    sDialogScript->TalkReset();

    m_MsgBox = nullptr;

    CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
    CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->GetScripts()[0];
    pPlayerScript->GetInfo().IsRollnAttack = false;
}
