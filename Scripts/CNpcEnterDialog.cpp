#include "pch.h"
#include "CNpcEnterDialog.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CFontMgr.h>


CNpcEnterDialog::CNpcEnterDialog()
    : CScript((UINT)SCRIPT_TYPE::NPCENTERDIALOG)
    , m_DialogBox(nullptr)
    , m_MsgBox(nullptr)
    , m_PlayerDirCheck(false)
    , m_TalkStart(false)
    , m_Dialog_1(-1)
    , m_Dialog_2(-1)
    , m_Dialog_3(-1)
{
}

CNpcEnterDialog::~CNpcEnterDialog()
{
}


void CNpcEnterDialog::SaveComponent(FILE* _File)
{
}

void CNpcEnterDialog::LoadComponent(FILE* _File)
{
}

void CNpcEnterDialog::Begin()
{
    m_DialogBox = CLevelMgr::GetInst()->FindObjectByName(L"DialogBox");
    m_DialogBox->GetRenderComponent()->SetActive(false);

    m_MsgBox = CLevelMgr::GetInst()->FindObjectByName(L"MsgBox");
    m_MsgBox->GetRenderComponent()->SetActive(false);


    wstring Dialog = L"아래로 내려가면 될거야,";
    wstring Dialog2 = L"위험하면 언제든지 돌아오고";

    wstring Dialog3 = L"대화";

    m_Dialog_1 = CFontMgr::GetInst()->RegisterFadeText(Dialog, 20.f, 504.f, 20.f, FONT_RGBA(0, 0, 0, 255), 10.f);
    m_Dialog_2 = CFontMgr::GetInst()->RegisterFadeText(Dialog2, 20.f, 504.f, 20.f, FONT_RGBA(0, 0, 0, 255), 10.f);
  
    m_Dialog_3 = CFontMgr::GetInst()->RegisterText(Dialog3, 116.f, 506.f, 20.f, FONT_RGBA(255, 255, 255, 255));

    CFontMgr::GetInst()->SetFadeActive(m_Dialog_1, false);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog_2, false);
    CFontMgr::GetInst()->SetActive(m_Dialog_3, false);
}

void CNpcEnterDialog::Tick()
{
    if (m_PlayerDirCheck)
    {
        if (m_DialogBox != nullptr && !m_TalkStart)
        {

            m_DialogBox->GetRenderComponent()->SetActive(true);
            CFontMgr::GetInst()->SetActive(m_Dialog_3, true);
        }

    }
    else
    {
        if (m_DialogBox != nullptr)
        {
            m_DialogBox->GetRenderComponent()->SetActive(false);
            CFontMgr::GetInst()->SetActive(m_Dialog_3, false);
        }
    }


    if (KEY_TAP(KEY::F) && !m_TalkStart && m_PlayerDirCheck == true)
    {
        m_TalkStart = true;
        StateMachine()->ChangeState(L"CNpcEnter_d2");
        CFontMgr::GetInst()->SetActive(m_Dialog_3, false);
    }
}

void CNpcEnterDialog::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CNpcEnterDialog::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    // 플레이어 방향에 따라 대화창 띄우고 지우는거 확인해야됨.
    if (_OtherObject->Transform()->GetRelativeScale().x < 0 && _OtherObject->GetLayerIdx() == 1 && _OtherObject->RigidBody2D()->IsGround())
        m_PlayerDirCheck = true;
    else if (_OtherObject->Transform()->GetRelativeScale().x > 0 && _OtherObject->GetLayerIdx() == 1)
        m_PlayerDirCheck = false;
}

void CNpcEnterDialog::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    m_PlayerDirCheck = false;
}