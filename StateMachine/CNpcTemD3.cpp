#include "pch.h"
#include "CNpcTemD3.h"
#include <Engine/CFontMgr.h>
#include <Engine/CLevelMgr.h>
#include <Scripts/CNpcTempleDialog.h>
#include <Scripts/CNpcTemple.h>
#include <Engine/CSoundMgr.h>
CNpcTemD3::CNpcTemD3()
    : m_DialogBox(nullptr)
    , m_Dialog3(-1)
{
}

CNpcTemD3::~CNpcTemD3()
{
}


void CNpcTemD3::FinalTick()
{
    if (CFontMgr::GetInst()->IsComplete(m_Dialog3) && (KEY_TAP(KEY::F) || KEY_TAP(KEY::A)))
    {
        GetStateMachine()->ChangeState(L"CNpcTemD1");
    }

}

void CNpcTemD3::Enter()
{
    const vector<CScript*>& vecScripts = CLevelMgr::GetInst()->FindObjectByName(L"KeyDialog")->GetScripts();
    CNpcTempleDialog* sDialogScript = dynamic_cast<CNpcTempleDialog*>(vecScripts[0]);
    assert(sDialogScript);
    m_Dialog3 = sDialogScript->GetDialog3();
    m_DialogBox = CLevelMgr::GetInst()->FindObjectByName(L"Dialog_Box1");


    CFontMgr::GetInst()->SetFadeActive(m_Dialog3, true);
    CSoundMgr::GetInst()->PlayFX(L"Talking", 0.2f);

}

void CNpcTemD3::Exit()
{
    m_DialogBox->GetRenderComponent()->SetActive(false);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog3, false);

    vector<CGameObject*> Child = GetStateMachine()->GetOwner()->GetChild();
    Child[0]->Collider2D()->Deactivate();

    const vector<CScript*>& vecScripts = GetStateMachine()->GetOwner()->GetScripts();
    CNpcTemple* sDialogScript = dynamic_cast<CNpcTemple*>(vecScripts[0]);
    assert(sDialogScript);
    sDialogScript->ResetNext();

}
