#include "pch.h"
#include "CNpcTownDialog.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CFontMgr.h>

CNpcTownDialog::CNpcTownDialog()
    :CScript(NPCTOWNDIALOG)
    , m_DialogBox(nullptr)
    , m_MsgBox(nullptr)
    , m_PlayerDirCheck(false)
    , m_TalkStart(false)
    , m_Dialog_1(-1)
    , m_Dialog_2(-1)
    , m_Dialog_3(-1)
    , m_Dialog_4(-1)
{
}

CNpcTownDialog::~CNpcTownDialog()
{
}

void CNpcTownDialog::SaveComponent(FILE* _File)
{
}

void CNpcTownDialog::LoadComponent(FILE* _File)
{
}

void CNpcTownDialog::Begin()
{
    m_DialogBox = CLevelMgr::GetInst()->FindObjectByName(L"DialogBox");
    m_DialogBox->GetRenderComponent()->SetActive(false);

    m_MsgBox = CLevelMgr::GetInst()->FindObjectByName(L"MsgBox");
    m_MsgBox->GetRenderComponent()->SetActive(false);


    wstring Dialog = L"이따금 불길한 소리가 들리더니,";
    wstring Dialog2 = L"이젠 갑자기 악마들이 나타나다니...";
    wstring Dialog3 = L"난 여기 남아 마을을 지켜야겠어..";
    wstring Dialog4 = L"대화";

    m_Dialog_1 = CFontMgr::GetInst()->RegisterFadeText(Dialog, 180.f, 386.f, 20.f, FONT_RGBA(0, 0, 0, 255), 10.f);
    m_Dialog_2 = CFontMgr::GetInst()->RegisterFadeText(Dialog2, 180.f, 416.f, 20.f, FONT_RGBA(0, 0, 0, 255), 10.f);
    m_Dialog_3 = CFontMgr::GetInst()->RegisterFadeText(Dialog3, 180.f, 401.f, 20.f, FONT_RGBA(0, 0, 0, 255), 10.f);
    m_Dialog_4 = CFontMgr::GetInst()->RegisterText(Dialog4, 302.f, 428.f, 20.f, FONT_RGBA(255, 255, 255, 255));

    CFontMgr::GetInst()->SetFadeActive(m_Dialog_1, false);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog_2, false);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog_3, false);
    CFontMgr::GetInst()->SetActive(m_Dialog_4, false);

}

void CNpcTownDialog::Tick()
{
    if (m_PlayerDirCheck)
    {
        if (m_DialogBox != nullptr && !m_TalkStart)
        {
            
            m_DialogBox->GetRenderComponent()->SetActive(true);
            CFontMgr::GetInst()->SetActive(m_Dialog_4, true);
        }

    }
    else
    {
        if (m_DialogBox != nullptr)
        {
            m_DialogBox->GetRenderComponent()->SetActive(false);
            CFontMgr::GetInst()->SetActive(m_Dialog_4, false);
        }
    }


    if (KEY_TAP(KEY::F) && !m_TalkStart && m_PlayerDirCheck ==true)
    {
        m_TalkStart = true;
        StateMachine()->ChangeState(L"CNpcTown1_d2");
        CFontMgr::GetInst()->SetActive(m_Dialog_4, false);
    }

}

void CNpcTownDialog::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{


}

void CNpcTownDialog::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    // 플레이어 방향에 따라 대화창 띄우고 지우는거 확인해야됨.
    if (_OtherObject->Transform()->GetRelativeScale().x < 0 && _OtherObject->GetLayerIdx() == 1)
        m_PlayerDirCheck = true;
    else if(_OtherObject->Transform()->GetRelativeScale().x > 0 && _OtherObject->GetLayerIdx() == 1)
        m_PlayerDirCheck = false;

}

void CNpcTownDialog::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    m_PlayerDirCheck = false;

}