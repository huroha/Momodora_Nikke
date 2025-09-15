#include "pch.h"
#include "CNpcTemD2.h"

#include <Engine/CFontMgr.h>
#include <Engine/CLevelMgr.h>
#include <Scripts/CNpcTempleDialog.h>
#include <Engine/CSoundMgr.h>

CNpcTemD2::CNpcTemD2()
    : m_DialogBox(nullptr)
    , m_Dialog1(-1)
    , m_Dialog2(-1)
    , m_Next(false)
{
}

CNpcTemD2::~CNpcTemD2()
{
}


void CNpcTemD2::FinalTick()
{
    if (CFontMgr::GetInst()->IsComplete(m_Dialog1) && m_Next == false)
    {
        m_Next = true;
        CFontMgr::GetInst()->SetFadeActive(m_Dialog2, true);

    }
    if ((KEY_TAP(KEY::F) || KEY_TAP(KEY::A)) && CFontMgr::GetInst()->IsComplete(m_Dialog2))
    {
        GetStateMachine()->ChangeState(L"CNpcTemD3");
    }

}

void CNpcTemD2::Enter()
{

    const vector<CScript*>& vecScripts = CLevelMgr::GetInst()->FindObjectByName(L"KeyDialog")->GetScripts();
    CNpcTempleDialog* sDialogScript = dynamic_cast<CNpcTempleDialog*>(vecScripts[0]);
    assert(sDialogScript);
    m_Dialog1 = sDialogScript->GetDialog1();
    m_Dialog2 = sDialogScript->GetDialog2();


    m_DialogBox = CLevelMgr::GetInst()->FindObjectByName(L"Dialog_Box1");
    m_DialogBox->GetRenderComponent()->SetActive(true);
    CSoundMgr::GetInst()->PlayFX(L"Talking", 0.2f);
}

void CNpcTemD2::Exit()
{
    CFontMgr::GetInst()->SetFadeActive(m_Dialog1, false);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog2, false);
    m_Next = false;
}