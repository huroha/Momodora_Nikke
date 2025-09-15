#include "pch.h"
#include "CNpcEnter_d2.h"

#include <Engine/CFontMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CNpcEnterDialog.h>

CNpcEnter_d2::CNpcEnter_d2()
    : m_MsgBox(nullptr)
    , m_Dialog1(-1)
{
}

CNpcEnter_d2::~CNpcEnter_d2()
{
}

void CNpcEnter_d2::FinalTick()
{
    if ((KEY_TAP(KEY::F) || KEY_TAP(KEY::A)) && CFontMgr::GetInst()->IsComplete(m_Dialog1))
    {
        GetStateMachine()->ChangeState(L"CNpcEnter_d3");
    }
}

void CNpcEnter_d2::Enter()
{
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CNpcEnterDialog* sDialogScript = (CNpcEnterDialog*)vecScripts[0];
    assert(sDialogScript);

    m_MsgBox = sDialogScript->GetMsgBox();
    m_MsgBox->GetRenderComponent()->SetActive(true);

    m_Dialog1 = sDialogScript->GetDialog1();
    CFontMgr::GetInst()->SetFadeActive(m_Dialog1, true);
    CSoundMgr::GetInst()->PlayFX(L"Talking", 0.2f);

    CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
    CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->GetScripts()[0];
    pPlayerScript->GetInfo().IsRollnAttack = true;
}

void CNpcEnter_d2::Exit()
{
    CFontMgr::GetInst()->SetFadeActive(m_Dialog1, false);
    m_MsgBox = nullptr;
}
