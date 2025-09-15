#include "pch.h"
#include "CNpcTown1_d2.h"
#include <Engine/CFontMgr.h>
#include <Engine/CLevelMgr.h>
#include <Scripts/CNpcTownDialog.h>
#include <Scripts/CPlayerScript.h>
#include <Engine/CSoundMgr.h>


CNpcTown1_d2::CNpcTown1_d2()
    : m_Dialog1(-1)
    , m_Dialog2(-1)
    , m_Next(false)
    , m_MsgBox(nullptr)
{
}

CNpcTown1_d2::~CNpcTown1_d2()
{
}


void CNpcTown1_d2::FinalTick()
{
    if (CFontMgr::GetInst()->IsComplete(m_Dialog1) && m_Next == false)
    {
        m_Next = true;
        CFontMgr::GetInst()->SetFadeActive(m_Dialog2, true);

    }
    if ((KEY_TAP(KEY::F)||KEY_TAP(KEY::A)) && CFontMgr::GetInst()->IsComplete(m_Dialog2))
    {
        GetStateMachine()->ChangeState(L"CNpcTown1_d3");
    }
}

void CNpcTown1_d2::Enter()
{
    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CNpcTownDialog* sDialogScript = (CNpcTownDialog*)vecScripts[0];
    assert(sDialogScript);

    m_MsgBox = sDialogScript->GetMsgBox();
    m_MsgBox->GetRenderComponent()->SetActive(true);

    m_Dialog1 = sDialogScript->GetDialog1();
    CFontMgr::GetInst()->SetFadeActive(m_Dialog1, true);
    m_Dialog2 = sDialogScript->GetDialog2();
    CSoundMgr::GetInst()->PlayFX(L"Talking", 0.2f);

    CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
    CPlayerScript* pPlayerScript= (CPlayerScript*)pPlayer->GetScripts()[0];
    pPlayerScript->GetInfo().IsRollnAttack = true;

}

void CNpcTown1_d2::Exit()
{
    CFontMgr::GetInst()->SetFadeActive(m_Dialog1, false);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog2, false);
    m_Next = false;
    m_MsgBox = nullptr;
}